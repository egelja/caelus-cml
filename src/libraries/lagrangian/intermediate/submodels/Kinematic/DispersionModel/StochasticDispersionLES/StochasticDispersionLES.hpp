/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2015 Applied CCM
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
    CML::StochasticDispersionLES

Description
    The velocity is perturbed in random direction, with a
    Gaussian random number distribution with variance sigma.
    where sigma is defined below

\*---------------------------------------------------------------------------*/

#ifndef StochasticDispersionLES_H
#define StochasticDispersionLES_H

#include "DispersionLESModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                   Class StochasticDispersionLES Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class StochasticDispersionLES
:
    public DispersionLESModel<CloudType>
{
public:

    //- Runtime type information
    TypeName("stochasticDispersionLES");


    // Constructors

        //- Construct from components
        StochasticDispersionLES(const dictionary& dict, CloudType& owner);

        //- Construct copy
        StochasticDispersionLES(StochasticDispersionLES<CloudType>& dm);

        //- Construct and return a clone
        virtual autoPtr<DispersionModel<CloudType> > clone()
        {
            return autoPtr<DispersionModel<CloudType> >
            (
                new StochasticDispersionLES<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~StochasticDispersionLES();


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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::StochasticDispersionLES<CloudType>::StochasticDispersionLES
(
    const dictionary& dict,
    CloudType& owner
)
:
    DispersionLESModel<CloudType>(dict, owner)
{}


template<class CloudType>
CML::StochasticDispersionLES<CloudType>::StochasticDispersionLES
(
    StochasticDispersionLES<CloudType>& dm
)
:
    DispersionLESModel<CloudType>(dm)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::StochasticDispersionLES<CloudType>::~StochasticDispersionLES()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::vector CML::StochasticDispersionLES<CloudType>::update
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

    const scalar k = this->kPtr_->internalField()[cellI];
    const scalar epsilon =
        this->epsilonPtr_->internalField()[cellI] + ROOTVSMALL;

    const scalar tTurbLoc = scalar(2)*scalar(4)*k/(min(epsilon, SMALL)*scalar(3)*scalar(3));


    // Parcel is perturbed by the turbulence
    if (dt < tTurbLoc)
    {
        tTurb += dt;

        if (tTurb > tTurbLoc)
        {
            tTurb = 0.0;

            const scalar sigma = sqrt(2.0*k/3.0);
            
            // Set to give equal components in each direction
            vector dir = vector::one;
 //           dir /= mag(dir) + SMALL;

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
