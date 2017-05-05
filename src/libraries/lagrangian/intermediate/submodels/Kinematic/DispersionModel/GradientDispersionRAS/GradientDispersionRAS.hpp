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
    CML::GradientDispersionRAS

Description
    The velocity is perturbed in the direction of -grad(k), with a
    Gaussian random number distribution with variance sigma.
    where sigma is defined below

\*---------------------------------------------------------------------------*/

#ifndef GradientDispersionRAS_H
#define GradientDispersionRAS_H

#include "DispersionRASModel.hpp"
#include "demandDrivenData.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class GradientDispersionRAS Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class GradientDispersionRAS
:
    public DispersionRASModel<CloudType>
{
protected:

    // Protected data

        // Locally cached turbulence fields

            //- Gradient of k
            const volVectorField* gradkPtr_;

            //- Take ownership of the grad(k)
            bool ownGradK_;


public:

    //- Runtime type information
    TypeName("gradientDispersionRAS");


    // Constructors

        //- Construct from components
        GradientDispersionRAS(const dictionary& dict, CloudType& owner);

        //- Construct copy
        GradientDispersionRAS(GradientDispersionRAS<CloudType>& dm);

        //- Construct and return a clone
        virtual autoPtr<DispersionModel<CloudType> > clone()
        {
            return autoPtr<DispersionModel<CloudType> >
            (
                new GradientDispersionRAS<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~GradientDispersionRAS();


    // Member Functions

        //- Cache carrier fields
        virtual void cacheFields(const bool store);

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


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::GradientDispersionRAS<CloudType>::GradientDispersionRAS
(
    const dictionary& dict,
    CloudType& owner
)
:
    DispersionRASModel<CloudType>(dict, owner),
    gradkPtr_(NULL),
    ownGradK_(false)
{}


template<class CloudType>
CML::GradientDispersionRAS<CloudType>::GradientDispersionRAS
(
    GradientDispersionRAS<CloudType>& dm
)
:
    DispersionRASModel<CloudType>(dm),
    gradkPtr_(dm.gradkPtr_),
    ownGradK_(dm.ownGradK_)
{
    dm.ownGradK_ = false;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::GradientDispersionRAS<CloudType>::~GradientDispersionRAS()
{
    cacheFields(false);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::GradientDispersionRAS<CloudType>::cacheFields(const bool store)
{
    DispersionRASModel<CloudType>::cacheFields(store);

    if (store)
    {
        gradkPtr_ = fvc::grad(*this->kPtr_).ptr();
        ownGradK_ = true;
    }
    else
    {
        if (ownGradK_)
        {
            deleteDemandDrivenData(gradkPtr_);
            gradkPtr_ = NULL;
            ownGradK_ = false;
        }
    }
}


template<class CloudType>
CML::vector CML::GradientDispersionRAS<CloudType>::update
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
    const vector& gradk = this->gradkPtr_->internalField()[cellI];

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
            vector dir = -gradk/(mag(gradk) + SMALL);

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

            // In 2D calculations the -grad(k) is always
            // away from the axis of symmetry
            // This creates a 'hole' in the spray and to
            // prevent this we let x1 be both negative/positive
            if (this->owner().mesh().nSolutionD() == 2)
            {
                fac *= x1;
            }
            else
            {
                fac *= mag(x1);
            }

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
