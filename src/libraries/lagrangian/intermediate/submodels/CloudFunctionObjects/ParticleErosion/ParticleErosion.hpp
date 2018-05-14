/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011-2012 OpenFOAM Foundation
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
    CML::ParticleErosion

Description
    Creates particle erosion field, Q


\*---------------------------------------------------------------------------*/

#ifndef ParticleErosion_H
#define ParticleErosion_H

#include "CloudFunctionObject.hpp"
#include "volFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class ParticleErosion Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ParticleErosion
:
    public CloudFunctionObject<CloudType>
{
    // Private Data

        // Typedefs

            //- Convenience typedef for parcel type
            typedef typename CloudType::parcelType parcelType;


        //- Particle erosion field
        autoPtr<volScalarField> QPtr_;

        //- List of patch indices to post-process
        labelList patchIDs_;

        //- Plastic flow stress - typical metal value = 2.7 GPa
        scalar p_;

        //- Ratio between depth of contact and length of cut - default=2
        scalar psi_;

        //- Ratio of normal and tangential forces - default=2
        scalar K_;


protected:

    // Protected Member Functions

        //- Returns local patchI if patch is in patchIds_ list
        label applyToPatch(const label globalPatchI) const;

        //- Write post-processing info
        virtual void write();


public:

    //- Runtime type information
    TypeName("particleErosion");


    // Constructors

        //- Construct from dictionary
        ParticleErosion
        (
            const dictionary& dict,
            CloudType& owner,
            const word& modelName
        );

        //- Construct copy
        ParticleErosion(const ParticleErosion<CloudType>& pe);

        //- Construct and return a clone
        virtual autoPtr<CloudFunctionObject<CloudType> > clone() const
        {
            return autoPtr<CloudFunctionObject<CloudType> >
            (
                new ParticleErosion<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~ParticleErosion();


    // Member Functions

        // Evaluation

            //- Pre-evolve hook
            virtual void preEvolve();

            //- Post-evolve hook
            virtual void postEvolve();

            //- Post-move hook
            virtual void postMove
            (
                typename CloudType::parcelType& p,
                const label cellI,
                const scalar dt,
                const point& position0,
                bool& keepParticle
            );

            //- Post-patch hook
            virtual void postPatch
            (
                const typename CloudType::parcelType& p,
                const polyPatch& pp,
                const scalar trackFraction,
                const tetIndices& tetIs,
                bool& keepParticle
            );

            //- Post-face hook
            virtual void postFace
            (
                const typename CloudType::parcelType& p,
                const label faceI,
                bool& keepParticle
            );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * Protected Member Functions * * * * * * * * * * * //

template<class CloudType>
CML::label CML::ParticleErosion<CloudType>::applyToPatch
(
    const label globalPatchI
) const
{
    forAll(patchIDs_, i)
    {
        if (patchIDs_[i] == globalPatchI)
        {
            return i;
        }
    }

    return -1;
}


template<class CloudType>
void CML::ParticleErosion<CloudType>::write()
{
    if (QPtr_.valid())
    {
        QPtr_->write();
    }
    else
    {
        FatalErrorIn("void CML::ParticleErosion<CloudType>::write()")
            << "QPtr not valid" << abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ParticleErosion<CloudType>::ParticleErosion
(
    const dictionary& dict,
    CloudType& owner,
    const word& modelName
)
:
    CloudFunctionObject<CloudType>(dict, owner, modelName, typeName),
    QPtr_(NULL),
    patchIDs_(),
    p_(readScalar(this->coeffDict().lookup("p"))),
    psi_(this->coeffDict().template lookupOrDefault<scalar>("psi", 2.0)),
    K_(this->coeffDict().template lookupOrDefault<scalar>("K", 2.0))
{
    const wordList allPatchNames = owner.mesh().boundaryMesh().names();
    wordList patchName(this->coeffDict().lookup("patches"));

    labelHashSet uniquePatchIDs;
    forAllReverse(patchName, i)
    {
        labelList patchIDs = findStrings(patchName[i], allPatchNames);

        if (patchIDs.empty())
        {
            WarningIn
            (
                "CML::ParticleErosion<CloudType>::ParticleErosion"
                "("
                    "const dictionary&, "
                    "CloudType& "
                ")"
            )   << "Cannot find any patch names matching " << patchName[i]
                << endl;
        }

        uniquePatchIDs.insert(patchIDs);
    }

    patchIDs_ = uniquePatchIDs.toc();

    // trigger the creation of the Q field
    preEvolve();
}


template<class CloudType>
CML::ParticleErosion<CloudType>::ParticleErosion
(
    const ParticleErosion<CloudType>& pe
)
:
    CloudFunctionObject<CloudType>(pe),
    QPtr_(NULL),
    patchIDs_(pe.patchIDs_),
    p_(pe.p_),
    psi_(pe.psi_),
    K_(pe.K_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::ParticleErosion<CloudType>::~ParticleErosion()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::ParticleErosion<CloudType>::preEvolve()
{
    if (QPtr_.valid())
    {
        QPtr_->internalField() = 0.0;
    }
    else
    {
        const fvMesh& mesh = this->owner().mesh();

        QPtr_.reset
        (
            new volScalarField
            (
                IOobject
                (
                    this->owner().name() + "Q",
                    mesh.time().timeName(),
                    mesh,
                    IOobject::READ_IF_PRESENT,
                    IOobject::NO_WRITE
                ),
                mesh,
                dimensionedScalar("zero", dimVolume, 0.0)
            )
        );
    }
}


template<class CloudType>
void CML::ParticleErosion<CloudType>::postEvolve()
{
    CloudFunctionObject<CloudType>::postEvolve();
}


template<class CloudType>
void CML::ParticleErosion<CloudType>::postMove
(
    typename CloudType::parcelType& p,
    const label cellI,
    const scalar dt,
    const point& position0,
    bool& keepParticle
)
{
    // Do nothing
}

template<class CloudType>
void CML::ParticleErosion<CloudType>::postPatch
(
    const typename CloudType::parcelType& p,
    const polyPatch& pp,
    const scalar trackFraction,
    const tetIndices& tetIs,
    bool&
)
{
    const label patchI = pp.index();

    const label localPatchI = applyToPatch(patchI);

    if (localPatchI != -1)
    {
        vector nw;
        vector Up;

        // patch-normal direction
        this->owner().patchData(p, pp, trackFraction, tetIs, nw, Up);

        // particle velocity relative to patch
        const vector& U = p.U() - Up;

        // quick reject if particle travelling away from the patch
        if ((nw & U) < 0)
        {
            return;
        }

        const scalar magU = mag(U);
        const vector Udir = U/magU;

        // determine impact angle, alpha
        const scalar alpha = mathematical::pi/2.0 - acos(nw & Udir);

        const scalar coeff = p.nParticle()*p.mass()*sqr(magU)/(p_*psi_*K_);

        const label patchFaceI = pp.whichFace(p.face());
        scalar& Q = QPtr_->boundaryField()[patchI][patchFaceI];

        if (tan(alpha) < K_/6.0)
        {
            Q += coeff*(sin(2.0*alpha) - 6.0/K_*sqr(sin(alpha)));
        }
        else
        {
            Q += coeff*(K_*sqr(cos(alpha))/6.0);
        }
    }
}


template<class CloudType>
void CML::ParticleErosion<CloudType>::postFace
(
    const typename CloudType::parcelType& p,
    const label faceI,
    bool& keepParticle
)
{
    // Do nothing
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
