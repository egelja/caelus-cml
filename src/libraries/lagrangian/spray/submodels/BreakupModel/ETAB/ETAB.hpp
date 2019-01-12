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
    CML::ETAB

Description
    The Enhanced TAB model.

    Described in the papers below.
    @verbatim
    F.X. Tanner
        "Liquid Jet Atomization and Droplet Breakup Modeling of
        Non-Evaporating Diesel Fuel Sprays"
        SAE 970050,
        SAE Transactions: Journal of Engines, Vol 106, Sec 3 pp 127-140

    F.X. Tanner and G. Weisser
        "Simulation of Liquid Jet Atomization for
        Fuel Sprays by Means of Cascade Drop Breakup Model"
        SAE 980808
        SAE Technical Paper Series
    @endverbatim

See also
    The TAB model

\*---------------------------------------------------------------------------*/

#ifndef ETAB_H
#define ETAB_H

#include "BreakupModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                           Class ETAB Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ETAB
:
    public BreakupModel<CloudType>
{
private:

    // Private data

        // Model constants

            scalar k1_;
            scalar k2_;
            scalar WeTransition_;
            scalar AWe_;


public:

    //- Runtime type information
    TypeName("ETAB");


    // Constructors

        //- Construct from dictionary
        ETAB(const dictionary&, CloudType&);

        //- Construct copy
        ETAB(const ETAB<CloudType>& bum);

        //- Construct and return a clone
        virtual autoPtr<BreakupModel<CloudType> > clone() const
        {
            return autoPtr<BreakupModel<CloudType> >
            (
                new ETAB<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~ETAB();


    // Member Functions

        //- Update the parcel properties
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

template<class CloudType>
CML::ETAB<CloudType>::ETAB
(
    const dictionary& dict,
    CloudType& owner
)
:
    BreakupModel<CloudType>(dict, owner, typeName, true),
    k1_(0.2),
    k2_(0.2),
    WeTransition_(100.0),
    AWe_(0.0)
{
    if (!this->defaultCoeffs(true))
    {
        this->coeffDict().lookup("k1") >> k1_;
        this->coeffDict().lookup("k2") >> k2_;
        this->coeffDict().lookup("WeTransition") >> WeTransition_;
    }

    scalar k21 = k2_/k1_;
    AWe_ = (k21*sqrt(WeTransition_) - 1.0)/pow4(WeTransition_);
}


template<class CloudType>
CML::ETAB<CloudType>::ETAB(const ETAB<CloudType>& bum)
:
    BreakupModel<CloudType>(bum),
    k1_(bum.k1_),
    k2_(bum.k2_),
    WeTransition_(bum.WeTransition_),
    AWe_(bum.AWe_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::ETAB<CloudType>::~ETAB()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
bool CML::ETAB<CloudType>::update
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
    scalar r  = 0.5*d;
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
        scalar romega = 1.0/omega;

        scalar We = rhoc*sqr(Urmag)*r/sigma;
        scalar Wetmp = We/this->TABtwoWeCrit_;

        scalar y1 = y - Wetmp;
        scalar y2 = yDot*romega;

        scalar a = sqrt(y1*y1 + y2*y2);

        // scotty we may have break-up
        if (a + Wetmp > 1.0)
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
                scalar theta = acos((1.0 - Wetmp)/a);

                if (theta < phi)
                {
                    if (constant::mathematical::twoPi - theta >= phi)
                    {
                        theta = -theta;
                    }
                    theta += constant::mathematical::twoPi;
                }
                tb = (theta - phi)*romega;

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
                scalar sqrtWe = AWe_*pow4(We) + 1.0;
                scalar Kbr = k1_*omega*sqrtWe;

                if (We > WeTransition_)
                {
                    sqrtWe = sqrt(We);
                    Kbr =k2_*omega*sqrtWe;
                }

                scalar rWetmp = 1.0/Wetmp;
                scalar cosdtbu = max(-1.0, min(1.0, 1.0 - rWetmp));
                scalar dtbu = romega*acos(cosdtbu);
                scalar decay = exp(-Kbr*dtbu);

                scalar rNew = decay*r;
                if (rNew < r)
                {
                    d = 2.0*rNew;
                    y = 0.0;
                    yDot = 0.0;
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
