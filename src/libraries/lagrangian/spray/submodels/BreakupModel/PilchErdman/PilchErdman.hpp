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
    CML::PilchErdman

Description
    Particle secondary breakup model, based on the reference:

    @verbatim
    Pilch, M. and Erdman, C.A.
    "Use of breakup time data and velocity history data
     to predict the maximum size of stable fragments for acceleration
     induced breakup of a liquid drop."
    Int. J. Multiphase Flows 13 (1987), 741-757
    @endverbatim

    The droplet fragment velocity is described by the equation:

    \f[
        V_d = V sqrt(epsilon)(B1 T + B2 T^2)
    \f]

    Where:
        V_d     : fragment velocity
        V       : magnitude of the relative velocity
        epsilon : density ratio (rho_carrier/rho_droplet)
        T       : characteristic break-up time
        B1, B2  : model input coefficients

    The authors suggest that:
        compressible flow   : B1 = 0.75*1.0; B2 = 3*0.116
        incompressible flow : B1 = 0.75*0.5; B2 = 3*0.0758


\*---------------------------------------------------------------------------*/

#ifndef PilchErdman_H
#define PilchErdman_H

#include "BreakupModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                        Class PilchErdman Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class PilchErdman
:
    public BreakupModel<CloudType>
{
private:

    // Private data

        scalar B1_;
        scalar B2_;


public:

    //- Runtime type information
    TypeName("PilchErdman");


    // Constructors

        //- Construct from dictionary
        PilchErdman(const dictionary&, CloudType&);

        //- Construct copy
        PilchErdman(const PilchErdman<CloudType>& bum);

        //- Construct and return a clone
        virtual autoPtr<BreakupModel<CloudType> > clone() const
        {
            return autoPtr<BreakupModel<CloudType> >
            (
                new PilchErdman<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~PilchErdman();


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
CML::PilchErdman<CloudType>::PilchErdman
(
    const dictionary& dict,
    CloudType& owner
)
:
    BreakupModel<CloudType>(dict, owner, typeName),
    B1_(0.375),
    B2_(0.2274)
{
    if (!this->defaultCoeffs(true))
    {
        this->coeffDict().lookup("B1") >> B1_;
        this->coeffDict().lookup("B2") >> B2_;
    }
}


template<class CloudType>
CML::PilchErdman<CloudType>::PilchErdman(const PilchErdman<CloudType>& bum)
:
    BreakupModel<CloudType>(bum),
    B1_(bum.B1_),
    B2_(bum.B2_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::PilchErdman<CloudType>::~PilchErdman()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
bool CML::PilchErdman<CloudType>::update
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
    // Weber number - eq (1)
    scalar We = rhoc*sqr(Urmag)*d/sigma;

    // Ohnesorge number - eq (2)
    scalar Oh = mu/sqrt(rho*d*sigma);

    // Critical Weber number - eq (5)
    scalar Wec = 12.0*(1.0 + 1.077*pow(Oh, 1.6));

    if (We > Wec)
    {
        // We > 2670, wave crest stripping - eq (12)
        scalar taubBar = 5.5;

        if (We < 2670)
        {
            if (We > 351)
            {
                // sheet stripping - eq (11)
                taubBar = 0.766*pow(We - 12.0, 0.25);
            }
            else if (We > 45)
            {
                // bag-and-stamen breakup  - eq (10)
                taubBar = 14.1*pow(We - 12.0, 0.25);
            }
            else if (We > 18)
            {
                // bag breakup - eq (9)
                taubBar = 2.45*pow(We - 12.0, 0.25);
            }
            else if (We > 12)
            {
                // vibrational breakup - eq (8)
                taubBar = 6.0*pow(We - 12.0, -0.25);
            }
            else
            {
                // no break-up
                taubBar = GREAT;
            }
        }

        scalar rho12 = sqrt(rhoc/rho);

        // velocity of fragmenting drop - eq (20)
        scalar Vd = Urmag*rho12*(B1_*taubBar + B2_*sqr(taubBar));

        // maximum stable diameter - eq (33)
        scalar Vd1 = sqr(1.0 - Vd/Urmag);
        Vd1 = max(Vd1, SMALL);
        scalar dStable = Wec*sigma/(Vd1*rhoc*sqr(Urmag));

        if (d < dStable)
        {
            // droplet diameter already stable = no break-up
            // - do not update d and nParticle
            return false;
        }
        else
        {
            scalar semiMass = nParticle*pow3(d);

            // invert eq (3) to create a dimensional break-up time
            scalar taub = taubBar*d/(Urmag*rho12);

            // update droplet diameter according to the rate eq (implicitly)
            scalar frac = dt/taub;
            d = (d + frac*dStable)/(1.0 + frac);

            // correct the number of particles to conserve mass
            nParticle = semiMass/pow3(d);
        }
    }

    return false;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
