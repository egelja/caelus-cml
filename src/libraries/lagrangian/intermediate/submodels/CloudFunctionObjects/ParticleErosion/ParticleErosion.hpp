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
    CML::ParticleErosion

Description
    Function object to create a field of eroded volume, Q, on a specified list
    of patches. The volume is calculated by the model of Finnie et al. The
    implementation follows the description given by the review of Yadav et al.

    References:
    \verbatim
        I Finnie, A Levy, D H McFadden
        "Fundamental Mechanisms of the Erosive Wear of Ductile Metals by Solid
        Particles"
        ASTM STP664, 1979, pages 36-58
    \endverbatim

    \verbatim
        I Finnie and D H McFadden
        "On the Velocity Dependence of the Erosion of Ductile Metal by Solid
        Particle at Low Angles of Incidence"
        Wear, 1978, volume 48, pages 181-190
    \endverbatim

    \verbatim
        G Yadav, S Tiwari, A Rajput, R Jatola, M L Jain
        "A Review: Erosion Wear Models"
        Emerging Trends in Mechanical Engineering, 2016, volume 1, pages 150-154
    \endverbatim

Usage
    \table
        Property    | Description                          | Req'd? | Default
        patches     | The patches on which to calculate Q  | yes    |
        p           | Plastic flow stress                  | yes    |
        psi         | Ratio between depth of contact and length of cut | no | 2
        K           | Ratio of normal and tangential force | no | 2
    \endtable

    Example:
    \verbatim
    <functionName>
    {
        type    particleErosion;
        patches (wall1 wall2);
        p       2.7e9;
    }
    \endverbatim

SourceFiles
    ParticleErosion.C

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


        //- Particle eroded volume field
        autoPtr<volScalarField> QPtr_;

        //- List of patch indices to post-process
        labelList patchIDs_;

        //- Plastic flow stress
        scalar p_;

        //- Ratio between depth of contact and length of cut. Default 2.
        scalar psi_;

        //- Ratio of normal and tangential forces. Default 2.
        scalar K_;


protected:

    // Protected Member Functions

        //- Returns local patchi if patch is in patchIds_ list
        label applyToPatch(const label globalPatchi) const;

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

            //- Post-patch hook
            virtual void postPatch
            (
                const parcelType& p,
                const polyPatch& pp,
                bool& keepParticle
            );
};


} // End namespace CML


// * * * * * * * * * * * * * Protected Member Functions * * * * * * * * * * * //

template<class CloudType>
CML::label CML::ParticleErosion<CloudType>::applyToPatch
(
    const label globalPatchi
) const
{
    forAll(patchIDs_, i)
    {
        if (patchIDs_[i] == globalPatchi)
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
        FatalErrorInFunction
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
    QPtr_(nullptr),
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
            WarningInFunction
                << "Cannot find any patch names matching " << patchName[i]
                << endl;
        }

        uniquePatchIDs.insert(patchIDs);
    }

    patchIDs_ = uniquePatchIDs.toc();

    // Trigger creation of the Q field
    preEvolve();
}


template<class CloudType>
CML::ParticleErosion<CloudType>::ParticleErosion
(
    const ParticleErosion<CloudType>& pe
)
:
    CloudFunctionObject<CloudType>(pe),
    QPtr_(nullptr),
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
                    this->owner().name() + ":Q",
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
void CML::ParticleErosion<CloudType>::postPatch
(
    const parcelType& p,
    const polyPatch& pp,
    bool&
)
{
    const label patchi = pp.index();
    const label localPatchi = applyToPatch(patchi);

    if (localPatchi != -1)
    {
        // Get patch data
        vector nw, Up;
        this->owner().patchData(p, pp, nw, Up);

        // Particle velocity relative to patch
        const vector& U = p.U() - Up;

        // Quick rejection if the particle is travelling away from the patch
        if ((nw & U) < 0)
        {
            return;
        }

        const scalar magU = mag(U);
        const vector UHat = U/magU;

        // Impact angle
        const scalar alpha = mathematical::pi/2 - acos(nw & UHat);

        // Get the face value to accumulate into
        const label patchFacei = pp.whichFace(p.face());
        scalar& Q = QPtr_->boundaryField()[patchi][patchFacei];

        // Finnie's model
        const scalar coeff = p.nParticle()*p.mass()*sqr(magU)/(p_*psi_*K_);
        if (tan(alpha) < K_/6)
        {
            Q += coeff*(sin(2*alpha) - 6/K_*sqr(sin(alpha)));
        }
        else
        {
            Q += coeff*(K_*sqr(cos(alpha))/6);
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
