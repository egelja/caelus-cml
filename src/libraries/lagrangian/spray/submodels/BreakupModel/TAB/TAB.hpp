/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::TAB

Description
    The TAB Method for Numerical Calculation of Spray Droplet Breakup.

    @verbatim
        O'Rourke, P.J. and Amsden, A.A.,
        "The TAB Method for Numerical Calculation of Spray Droplet Breakup,"
        1987 SAE International Fuels and Lubricants Meeting and Exposition,
        Toronto, Ontario, November 2-5, 1987,
        Los Alamos National Laboratory document LA-UR-87-2105;
        SAE Technical Paper Series, Paper 872089.
    @endverbatim

    This implementation follows the kiva version.

See Also
    The Enhanced %TAB model - ETAB

\*---------------------------------------------------------------------------*/

#ifndef TAB_H
#define TAB_H

#include "BreakupModel.hpp"
#include "FixedList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                           Class TAB Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class TAB
:
    public BreakupModel<CloudType>
{
public:

    //- Enumeration for the SMD brekup calculation
    enum SMDMethods
    {
        method1,
        method2
    };


private:

    // Private data

        // Inverse function approximation of the Rossin-Rammler Distribution
        // used when calculating the droplet size after breakup
        FixedList<scalar, 100> rrd_;


        // Model constants

            word SMDCalcMethod_;
            SMDMethods SMDMethod_;


public:

    //- Runtime type information
    TypeName("TAB");


    // Constructors

        //- Construct from dictionary
        TAB(const dictionary& dict, CloudType& owner);

        //- Construct copy
        TAB(const TAB<CloudType>& im);

        //- Construct and return a clone
        virtual autoPtr<BreakupModel<CloudType> > clone() const
        {
            return autoPtr<BreakupModel<CloudType> >
            (
                new TAB<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~TAB();


    // Member Functions

        //- update the parcel diameter
        virtual bool update
        (
            const scalar dt,
            const vector& g,
            scalar& d,
            scalar& tc,
            scalar& ms,
            scalar& nParticle,
            scalar& KHindex,
            scalar& y,
            scalar& yDot,
            const scalar d0,
            const scalar rho,
            const scalar mu,
            const scalar sigma,
            const vector& U,
            const scalar rhoc,
            const scalar muc,
            const vector& Urel,
            const scalar Urmag,
            const scalar tMom,
            scalar& dChild,
            scalar& massChild
    );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template <class CloudType>
CML::TAB<CloudType>::TAB
(
    const dictionary& dict,
    CloudType& owner
)
:
    BreakupModel<CloudType>(dict, owner, typeName, true),
    SMDCalcMethod_(this->coeffDict().lookup("SMDCalculationMethod"))
{
    // calculate the inverse function of the Rossin-Rammler Distribution
    const scalar xx0 = 12.0;
    const scalar rrd100 =
        1.0/(1.0 - exp(-xx0)*(1.0 + xx0 + sqr(xx0)/2.0 + pow3(xx0)/6.0));

    forAll(rrd_, n)
    {
        scalar xx = 0.12*(n + 1);
        rrd_[n] =
            (1.0 - exp(-xx)*(1.0 + xx + sqr(xx)/2.0 + pow3(xx)/6.0))*rrd100;
    }

    if (SMDCalcMethod_ == "method1")
    {
        SMDMethod_ = method1;
    }
    else if (SMDCalcMethod_ == "method2")
    {
        SMDMethod_ = method2;
    }
    else
    {
        SMDMethod_ = method2;
        WarningIn("CML::TAB<CloudType>::TAB(const dictionary&, CloudType&)")
            << "Unknown SMDCalculationMethod. Valid options are "
            << "(method1 | method2). Using method2" << endl;
    }
}


template<class CloudType>
CML::TAB<CloudType>::TAB(const TAB<CloudType>& bum)
:
    BreakupModel<CloudType>(bum),
    SMDCalcMethod_(bum.SMDCalcMethod_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::TAB<CloudType>::~TAB()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
bool CML::TAB<CloudType>::update
(
    const scalar dt,
    const vector& g,
    scalar& d,
    scalar& tc,
    scalar& ms,
    scalar& nParticle,
    scalar& KHindex,
    scalar& y,
    scalar& yDot,
    const scalar d0,
    const scalar rho,
    const scalar mu,
    const scalar sigma,
    const vector& U,
    const scalar rhoc,
    const scalar muc,
    const vector& Urel,
    const scalar Urmag,
    const scalar tMom,
    scalar& dChild,
    scalar& massChild
)
{
    cachedRandom& rndGen = this->owner().rndGen();

    scalar r = 0.5*d;
    scalar r2 = r*r;
    scalar r3 = r*r2;

    scalar semiMass = nParticle*pow3(d);

    // inverse of characteristic viscous damping time
    scalar rtd = 0.5*this->TABCmu_*mu/(rho*r2);

    // oscillation frequency (squared)
    scalar omega2 = this->TABComega_*sigma/(rho*r3) - rtd*rtd;

    if (omega2 > 0)
    {
        scalar omega = sqrt(omega2);
        scalar We = rhoc*sqr(Urmag)*r/sigma;
        scalar Wetmp = We/this->TABWeCrit_;

        scalar y1 = y - Wetmp;
        scalar y2 = yDot/omega;

        scalar a = sqrt(y1*y1 + y2*y2);

        // scotty we may have break-up
        if (a+Wetmp > 1.0)
        {
            scalar phic = y1/a;

            // constrain phic within -1 to 1
            phic = max(min(phic, 1), -1);

            scalar phit = acos(phic);
            scalar phi = phit;
            scalar quad = -y2/a;
            if (quad < 0)
            {
                phi = constant::mathematical::twoPi - phit;
            }

            scalar tb = 0;

            if (mag(y) < 1.0)
            {
                scalar coste = 1.0;
                if ((Wetmp - a < -1) && (yDot < 0))
                {
                    coste = -1.0;
                }

                scalar theta = acos((coste-Wetmp)/a);

                if (theta < phi)
                {
                    if (constant::mathematical::twoPi - theta >= phi)
                    {
                        theta = -theta;
                    }
                    theta += constant::mathematical::twoPi;
                }
                tb = (theta-phi)/omega;

                // breakup occurs
                if (dt > tb)
                {
                    y = 1.0;
                    yDot = -a*omega*sin(omega*tb + phi);
                }

            }

            // update droplet size
            if (dt > tb)
            {
                scalar rs =
                    r/(1.0 + (4.0/3.0)*sqr(y) + rho*r3/(8*sigma)*sqr(yDot));

                label n = 0;
                scalar rNew = 0.0;
                switch (SMDMethod_)
                {
                    case method1:
                    {
                        #include "TABSMDCalcMethod1.hpp"
                        break;
                    }
                    case method2:
                    {
                        #include "TABSMDCalcMethod2.hpp"
                        break;
                    }
                }

                if (rNew < r)
                {
                    d = 2*rNew;
                    y = 0;
                    yDot = 0;
                }
            }
        }
    }
    else
    {
        // reset droplet distortion parameters
        y = 0;
        yDot = 0;
    }

    // update the nParticle count to conserve mass
    nParticle = semiMass/pow3(d);

    // Do not add child parcel
    return false;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
