/*---------------------------------------------------------------------------*\
Copyright (C) 2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::PatchCollisionDensity

Description
    Function object which generates fields of the number and rate of collisions
    per unit area on all patches. Can optionally take a minimum speed below
    which a collision is not counted.

    Example usage:
    \verbatim
    patchCollisionDensity1
    {
        type        patchCollisionDensity;
        minSpeed    1e-3;
    }
    \endverbatim

SourceFiles
    PatchCollisionDensity.cpp

\*---------------------------------------------------------------------------*/

#ifndef PatchCollisionDensity_HPP
#define PatchCollisionDensity_HPP

#include "CloudFunctionObject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class PatchCollisionDensity Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class PatchCollisionDensity
:
    public CloudFunctionObject<CloudType>
{
    // Private data

        typedef typename CloudType::particleType parcelType;

        //- The threshold for a collision
        const scalar minSpeed_;

        //- The field of the number of collisions per unit area
        volScalarField::GeometricBoundaryField collisionDensity_;

        //- The field of the number of collisions per unit area at the last
        //  output
        volScalarField::GeometricBoundaryField collisionDensity0_;

        //- The time at the last output
        scalar time0_;


protected:

    // Protected Member Functions

        //- Write post-processing info
        void write();


public:

    //- Runtime type information
    TypeName("patchCollisionDensity");


    // Constructors

        //- Construct from dictionary
        PatchCollisionDensity
        (
            const dictionary& dict,
            CloudType& owner,
            const word& modelName
        );

        //- Construct copy
        PatchCollisionDensity(const PatchCollisionDensity<CloudType>& ppm);

        //- Construct and return a clone
        virtual autoPtr<CloudFunctionObject<CloudType>> clone() const
        {
            return autoPtr<CloudFunctionObject<CloudType>>
            (
                new PatchCollisionDensity<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~PatchCollisionDensity();


    // Member Functions

        // Evaluation

            //- Post-patch hook
            virtual void postPatch
            (
                const parcelType& p,
                const polyPatch& pp,
                bool& keepParticle
            );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


#include "Pstream.hpp"
#include "stringListOps.hpp"
#include "ListOps.hpp"
#include "ListListOps.hpp"

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

template<class CloudType>
void CML::PatchCollisionDensity<CloudType>::write()
{
    const scalarField z(this->owner().mesh().nCells(), 0);

    volScalarField
    (
        IOobject
        (
            this->owner().name() + ":collisionDensity",
            this->owner().mesh().time().timeName(),
            this->owner().mesh()
        ),
        this->owner().mesh(),
        dimless/dimArea,
        z,
        collisionDensity_
    )
   .write();

    volScalarField
    (
        IOobject
        (
            this->owner().name() + ":collisionDensityRate",
            this->owner().mesh().time().timeName(),
            this->owner().mesh()
        ),
        this->owner().mesh(),
        dimless/dimArea/dimTime,
        z,
        (collisionDensity_ - collisionDensity0_)
       /(this->owner().mesh().time().value() - time0_)
    )
   .write();

    collisionDensity0_ == collisionDensity_;
    time0_ = this->owner().mesh().time().value();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::PatchCollisionDensity<CloudType>::PatchCollisionDensity
(
    const dictionary& dict,
    CloudType& owner,
    const word& modelName
)
:
    CloudFunctionObject<CloudType>(dict, owner, modelName, typeName),
    minSpeed_(dict.lookupOrDefault<scalar>("minSpeed", -1)),
    collisionDensity_
    (
        this->owner().mesh().boundary(),
        DimensionedField<scalar, volMesh>::null(),
        calculatedFvPatchField<scalar>::typeName
    ),
    collisionDensity0_
    (
        this->owner().mesh().boundary(),
        DimensionedField<scalar, volMesh>::null(),
        calculatedFvPatchField<scalar>::typeName
    ),
    time0_(this->owner().mesh().time().value())
{
    collisionDensity_ == 0;
    collisionDensity0_ == 0;

    IOobject io
    (
        this->owner().name() + ":collisionDensity",
        this->owner().mesh().time().timeName(),
        this->owner().mesh(),
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    );

    if (io.headerOk())
    {
        const volScalarField collisionDensity(io, this->owner().mesh());
        collisionDensity_ == collisionDensity.boundaryField();
        collisionDensity0_ == collisionDensity.boundaryField();
    }
}


template<class CloudType>
CML::PatchCollisionDensity<CloudType>::PatchCollisionDensity
(
    const PatchCollisionDensity<CloudType>& ppm
)
:
    CloudFunctionObject<CloudType>(ppm),
    minSpeed_(ppm.minSpeed_),
    collisionDensity_(ppm.collisionDensity_),
    collisionDensity0_(ppm.collisionDensity0_),
    time0_(ppm.time0_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::PatchCollisionDensity<CloudType>::~PatchCollisionDensity()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::PatchCollisionDensity<CloudType>::postPatch
(
    const parcelType& p,
    const polyPatch& pp,
    bool&
)
{
    const label patchi = pp.index();
    const label patchFacei = p.face() - pp.start();

    vector nw, Up;
    this->owner().patchData(p, pp, nw, Up);

    const scalar speed = (p.U() - Up) & nw;
    if (speed > minSpeed_)
    {
        collisionDensity_[patchi][patchFacei] +=
            1/this->owner().mesh().magSf().boundaryField()[patchi][patchFacei];
    }
}


#endif
