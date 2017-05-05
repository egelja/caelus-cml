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
    CML::PatchFlowRateInjection

Description
    Patch injection
    - uses patch flow rate to determine concentration and velociy

    - User specifies
      - Total mass to inject
      - Name of patch
      - Injection duration
      - Initial parcel velocity
      - Injection target concentration/carrier volume flow rate
    - Parcel diameters obtained by distribution model
    - Parcels injected at cell centres adjacent to patch


\*---------------------------------------------------------------------------*/

#ifndef PatchFlowRateInjection_H
#define PatchFlowRateInjection_H

#include "InjectionModel.hpp"
#include "TimeDataEntry.hpp"
#include "distributionModel.hpp"
#include "mathematicalConstants.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class Type>
class TimeDataEntry;

class distributionModel;

/*---------------------------------------------------------------------------*\
                   Class PatchFlowRateInjection Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class PatchFlowRateInjection
:
    public InjectionModel<CloudType>
{
    // Private data

        //- Name of patch
        const word patchName_;

        //- Id of patch
        const label patchId_;

        //- Patch area
        scalar patchArea_;

        //- Patch normal direction
        vector patchNormal_;

        //- Name of carrier (mass or volume) flux field
        const word phiName_;

        //- Name of carrier density field
        const word rhoName_;

        //- Injection duration [s]
        scalar duration_;

        //- Concentration of particles to carrier [] (particles/m3)
        const scalar concentration_;

        //- Number of parcels to introduce per second []
        const label parcelsPerSecond_;

        //- Initial parcel velocity [m/s]
        vector U0_;

        //- Parcel size distribution model
        const autoPtr<distributionModels::distributionModel> sizeDistribution_;

        //- List of cell labels corresponding to injector positions
        labelList cellOwners_;

        //- Fraction of injection controlled by this processor
        scalar fraction_;

        //- Mean particle volume TODO: temporary measure - return from PDF
        scalar pMeanVolume_;


public:

    //- Runtime type information
    TypeName("patchFlowRateInjection");


    // Constructors

        //- Construct from dictionary
        PatchFlowRateInjection(const dictionary& dict, CloudType& owner);

        //- Construct copy
        PatchFlowRateInjection(const PatchFlowRateInjection<CloudType>& im);

        //- Construct and return a clone
        virtual autoPtr<InjectionModel<CloudType> > clone() const
        {
            return autoPtr<InjectionModel<CloudType> >
            (
                new PatchFlowRateInjection<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~PatchFlowRateInjection();


    // Member Functions

        //- Return the end-of-injection time
        scalar timeEnd() const;

        //- Number of parcels to introduce relative to SOI
        virtual label parcelsToInject(const scalar time0, const scalar time1);

        //- Volume of parcels to introduce relative to SOI
        virtual scalar volumeToInject(const scalar time0, const scalar time1);


        // Injection geometry

            //- Set the injection position and owner cell, tetFace and tetPt
            virtual void setPositionAndCell
            (
                const label parcelI,
                const label nParcels,
                const scalar time,
                vector& position,
                label& cellOwner,
                label& tetFaceI,
                label& tetPtI
            );

            virtual void setProperties
            (
                const label parcelI,
                const label nParcels,
                const scalar time,
                typename CloudType::parcelType& parcel
            );

            //- Flag to identify whether model fully describes the parcel
            virtual bool fullyDescribed() const;

            //- Return flag to identify whether or not injection of parcelI is
            //  permitted
            virtual bool validInjection(const label parcelI);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::PatchFlowRateInjection<CloudType>::PatchFlowRateInjection
(
    const dictionary& dict,
    CloudType& owner
)
:
    InjectionModel<CloudType>(dict, owner, typeName),
    patchName_(this->coeffDict().lookup("patchName")),
    patchId_(owner.mesh().boundaryMesh().findPatchID(patchName_)),
    patchArea_(0.0),
    patchNormal_(vector::zero),
    phiName_(this->coeffDict().template lookupOrDefault<word>("phi", "phi")),
    rhoName_(this->coeffDict().template lookupOrDefault<word>("rho", "rho")),
    duration_(readScalar(this->coeffDict().lookup("duration"))),
    concentration_(readScalar(this->coeffDict().lookup("concentration"))),
    parcelsPerSecond_
    (
        readScalar(this->coeffDict().lookup("parcelsPerSecond"))
    ),
    U0_(vector::zero),
    sizeDistribution_
    (
        distributionModels::distributionModel::New
        (
            this->coeffDict().subDict("sizeDistribution"),
            owner.rndGen()
        )
    ),
    cellOwners_(),
    fraction_(1.0),
    pMeanVolume_(0.0)
{
    if (patchId_ < 0)
    {
        FatalErrorIn
        (
            "PatchFlowRateInjection<CloudType>::PatchFlowRateInjection"
            "("
                "const dictionary&, "
                "CloudType&"
            ")"
        )   << "Requested patch " << patchName_ << " not found" << nl
            << "Available patches are: " << owner.mesh().boundaryMesh().names()
            << nl << exit(FatalError);
    }

    const polyPatch& patch = owner.mesh().boundaryMesh()[patchId_];

    duration_ = owner.db().time().userTimeToTime(duration_);

    cellOwners_ = patch.faceCells();

    // TODO: retrieve mean diameter from distrution model
    scalar pMeanDiameter =
        readScalar(this->coeffDict().lookup("meanParticleDiameter"));
    pMeanVolume_ = constant::mathematical::pi*pow3(pMeanDiameter)/6.0;

    // patch geometry
    label patchSize = cellOwners_.size();
    label totalPatchSize = patchSize;
    reduce(totalPatchSize, sumOp<label>());
    fraction_ = scalar(patchSize)/totalPatchSize;

    patchArea_ = gSum(mag(patch.faceAreas()));
    patchNormal_ = gSum(patch.faceNormals())/totalPatchSize;
    patchNormal_ /= mag(patchNormal_);

    // Re-initialise total mass/volume to inject to zero
    // - will be reset during each injection
    this->volumeTotal_ = 0.0;
    this->massTotal_ = 0.0;
}


template<class CloudType>
CML::PatchFlowRateInjection<CloudType>::PatchFlowRateInjection
(
    const PatchFlowRateInjection<CloudType>& im
)
:
    InjectionModel<CloudType>(im),
    patchName_(im.patchName_),
    patchId_(im.patchId_),
    patchArea_(im.patchArea_),
    patchNormal_(im.patchNormal_),
    phiName_(im.phiName_),
    rhoName_(im.rhoName_),
    duration_(im.duration_),
    concentration_(im.concentration_),
    parcelsPerSecond_(im.parcelsPerSecond_),
    U0_(im.U0_),
    sizeDistribution_(im.sizeDistribution_().clone().ptr()),
    cellOwners_(im.cellOwners_),
    fraction_(im.fraction_),
    pMeanVolume_(im.pMeanVolume_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::PatchFlowRateInjection<CloudType>::~PatchFlowRateInjection()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::scalar CML::PatchFlowRateInjection<CloudType>::timeEnd() const
{
    return this->SOI_ + duration_;
}


template<class CloudType>
CML::label CML::PatchFlowRateInjection<CloudType>::parcelsToInject
(
    const scalar time0,
    const scalar time1
)
{
    if ((time0 >= 0.0) && (time0 < duration_))
    {
        scalar nParcels = fraction_*(time1 - time0)*parcelsPerSecond_;

        cachedRandom& rnd = this->owner().rndGen();

        label nParcelsToInject = floor(nParcels);

        // Inject an additional parcel with a probability based on the
        // remainder after the floor function
        if
        (
            nParcelsToInject > 0
         && (
               nParcels - scalar(nParcelsToInject)
             > rnd.position(scalar(0), scalar(1))
            )
        )
        {
            ++nParcelsToInject;
        }

        return nParcelsToInject;
    }
    else
    {
        return 0;
    }
}


template<class CloudType>
CML::scalar CML::PatchFlowRateInjection<CloudType>::volumeToInject
(
    const scalar time0,
    const scalar time1
)
{
    scalar volume = 0.0;

    if ((time0 >= 0.0) && (time0 < duration_))
    {
        const polyMesh& mesh = this->owner().mesh();

        const surfaceScalarField& phi =
            mesh.lookupObject<surfaceScalarField>(phiName_);

        const scalarField& phip = phi.boundaryField()[patchId_];

        scalar carrierVolume = 0.0;
        if (phi.dimensions() == dimVelocity*dimArea)
        {
            const scalar flowRateIn = max(0.0, -sum(phip));
            U0_ = -patchNormal_*flowRateIn/patchArea_;
            carrierVolume = (time1 - time0)*flowRateIn;
        }
        else
        {
            const volScalarField& rho =
                mesh.lookupObject<volScalarField>(rhoName_);
            const scalarField& rhop = rho.boundaryField()[patchId_];

            const scalar flowRateIn = max(0.0, -sum(phip/rhop));
            U0_ = -patchNormal_*flowRateIn/patchArea_;
            carrierVolume = (time1 - time0)*flowRateIn;
        }

        const scalar newParticles = concentration_*carrierVolume;

        volume = pMeanVolume_*newParticles;
    }

    reduce(volume, sumOp<scalar>());

    this->volumeTotal_ = volume;
    this->massTotal_ = volume*this->owner().constProps().rho0();

    return fraction_*volume;
}


template<class CloudType>
void CML::PatchFlowRateInjection<CloudType>::setPositionAndCell
(
    const label,
    const label,
    const scalar,
    vector& position,
    label& cellOwner,
    label& tetFaceI,
    label& tetPtI
)
{
    if (cellOwners_.size() > 0)
    {
        cachedRandom& rnd = this->owner().rndGen();

        label cellI = rnd.position<label>(0, cellOwners_.size() - 1);

        cellOwner = cellOwners_[cellI];

        // The position is between the face and cell centre, which could be
        // in any tet of the decomposed cell, so arbitrarily choose the first
        // face of the cell as the tetFace and the first point after the base
        // point on the face as the tetPt.  The tracking will pick the cell
        // consistent with the motion in the firsttracking step.
        tetFaceI = this->owner().mesh().cells()[cellOwner][0];
        tetPtI = 1;

        // position perturbed between cell and patch face centres
        const vector& pc = this->owner().mesh().C()[cellOwner];
        const vector& pf =
            this->owner().mesh().Cf().boundaryField()[patchId_][cellI];

        const scalar a = rnd.sample01<scalar>();
        const vector d = pf - pc;
        position = pc + 0.5*a*d;
    }
    else
    {
        cellOwner = -1;
        tetFaceI = -1;
        tetPtI = -1;
        // dummy position
        position = pTraits<vector>::max;
    }
}


template<class CloudType>
void CML::PatchFlowRateInjection<CloudType>::setProperties
(
    const label,
    const label,
    const scalar,
    typename CloudType::parcelType& parcel
)
{
    // set particle velocity
    parcel.U() = U0_;

    // set particle diameter
    parcel.d() = sizeDistribution_->sample();
}


template<class CloudType>
bool CML::PatchFlowRateInjection<CloudType>::fullyDescribed() const
{
    return false;
}


template<class CloudType>
bool CML::PatchFlowRateInjection<CloudType>::validInjection(const label)
{
    return true;
}


// ************************************************************************* //

#endif

// ************************************************************************* //
