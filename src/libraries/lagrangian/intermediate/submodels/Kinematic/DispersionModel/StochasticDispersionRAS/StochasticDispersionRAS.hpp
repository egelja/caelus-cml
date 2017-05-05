/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
        StochasticDispersionRAS(StochasticDispersionRAS<CloudType>& dm);

        //- Construct and return a clone
        virtual autoPtr<DispersionModel<CloudType> > clone()
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
            const label cellI,
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
    StochasticDispersionRAS<CloudType>& dm
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
    const label cellI,
    const vector& U,
    const vector& Uc,
    vector& UTurb,
    scalar& tTurb
)
{
    cachedRandom& rnd = this->owner().rndGen();

    const scalar cps = 0.16432;

    const scalar k = this->kPtr_->internalField()[cellI];
    const scalar epsilon =
        this->epsilonPtr_->internalField()[cellI] + ROOTVSMALL;

    const scalar UrelMag = mag(U - Uc - UTurb);

    const scalar tTurbLoc =
        min(k/epsilon, cps*pow(k, 1.5)/epsilon/(UrelMag + SMALL));


    // Parcel is perturbed by the turbulence
    if (dt < tTurbLoc)
    {
        tTurb += dt;

        if (tTurb > tTurbLoc)
        {
            tTurb = 0.0;

            scalar sigma = sqrt(2.0*k/3.0);
            vector dir = 2.0*rnd.sample01<vector>() - vector::one;
            dir /= mag(dir) + SMALL;

            // Numerical Recipes... Ch. 7. Random Numbers...
            scalar x1 = 0.0;
            scalar x2 = 0.0;
            scalar rsq = 10.0;
            while ((rsq > 1.0) || (rsq == 0.0))
            {
                x1 = 2.0*rnd.sample01<scalar>() - 1.0;
                x2 = 2.0*rnd.sample01<scalar>() - 1.0;
                rsq = x1*x1 + x2*x2;
            }

            scalar fac = sqrt(-2.0*log(rsq)/rsq);

            fac *= mag(x1);

            UTurb = sigma*fac*dir;

        }
    }
    else
    {
        tTurb = GREAT;
        UTurb = vector::zero;
    }

    return Uc + UTurb;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
