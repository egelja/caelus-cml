/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::StochasticDispersionRAS

Description
    The velocity is perturbed in random direction, with a
    Gaussian random number distribution with variance sigma.
    where sigma is defined below

\*---------------------------------------------------------------------------*/

#ifndef StochasticDispersionRAS_H
#define StochasticDispersionRAS_H

#include "DispersionRASModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                   Class StochasticDispersionRAS Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class StochasticDispersionRAS
:
    public DispersionRASModel<CloudType>
{
public:

    //- Runtime type information
    TypeName("stochasticDispersionRAS");


    // Constructors

        //- Construct from components
        StochasticDispersionRAS(const dictionary& dict, CloudType& owner);

        //- Construct copy
        StochasticDispersionRAS(const StochasticDispersionRAS<CloudType>& dm);

        //- Construct and return a clone
        virtual autoPtr<DispersionModel<CloudType> > clone() const
        {
            return autoPtr<DispersionModel<CloudType> >
            (
                new StochasticDispersionRAS<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~StochasticDispersionRAS();


    // Member Functions

        //- Update (disperse particles)
        virtual vector update
        (
            const scalar dt,
            const label celli,
            const vector& U,
            const vector& Uc,
            vector& UTurb,
            scalar& tTurb
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::StochasticDispersionRAS<CloudType>::StochasticDispersionRAS
(
    const dictionary& dict,
    CloudType& owner
)
:
    DispersionRASModel<CloudType>(dict, owner)
{}


template<class CloudType>
CML::StochasticDispersionRAS<CloudType>::StochasticDispersionRAS
(
    const StochasticDispersionRAS<CloudType>& dm
)
:
    DispersionRASModel<CloudType>(dm)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::StochasticDispersionRAS<CloudType>::~StochasticDispersionRAS()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::vector CML::StochasticDispersionRAS<CloudType>::update
(
    const scalar dt,
    const label celli,
    const vector& U,
    const vector& Uc,
    vector& UTurb,
    scalar& tTurb
)
{
    Random& rnd = this->owner().rndGen();

    const scalar cps = 0.16432;

    const scalar k = this->kPtr_->internalField()[celli];
    const scalar epsilon =
        this->epsilonPtr_->internalField()[celli] + ROOTVSMALL;

    const scalar UrelMag = mag(U - Uc - UTurb);

    const scalar tTurbLoc =
        min(k/epsilon, cps*pow(k, 1.5)/epsilon/(UrelMag + SMALL));


    // Parcel is perturbed by the turbulence
    if (dt < tTurbLoc)
    {
        tTurb += dt;

        if (tTurb > tTurbLoc)
        {
            tTurb = 0;

            const scalar sigma = sqrt(2*k/3.0);

            // Calculate a random direction dir distributed uniformly
            // in spherical coordinates

            const scalar theta = rnd.scalar01()*twoPi;
            const scalar u = 2*rnd.scalar01() - 1;

            const scalar a = sqrt(1 - sqr(u));
            const vector dir(a*cos(theta), a*sin(theta), u);

            UTurb = sigma*mag(rnd.scalarNormal())*dir;
        }
    }
    else
    {
        tTurb = GREAT;
        UTurb = Zero;
    }

    return Uc + UTurb;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
