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
    CML::PatchInjection

Description
    Patch injection
    - User specifies
      - Total mass to inject
      - Name of patch
      - Injection duration
      - Initial parcel velocity
      - Injection volume flow rate
    - Parcel diameters obtained by distribution model
    - Parcels injected at cell centres adjacent to patch

\*---------------------------------------------------------------------------*/

#ifndef PatchInjection_H
#define PatchInjection_H

#include "InjectionModel.hpp"
#include "TimeDataEntry.hpp"
#include "distributionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class Type>
class TimeDataEntry;

class distributionModel;

/*---------------------------------------------------------------------------*\
                       Class PatchInjection Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class PatchInjection
:
    public InjectionModel<CloudType>
{
    // Private data

        //- Name of patch
        const word patchName_;

        //- Id of patch
        const label patchId_;

        //- Injection duration [s]
        scalar duration_;

        //- Number of parcels to introduce per second []
        const label parcelsPerSecond_;

        //- Initial parcel velocity [m/s]
        const vector U0_;

        //- Flow rate profile relative to SOI []
        const TimeDataEntry<scalar> flowRateProfile_;

        //- Parcel size distribution model
        const autoPtr<distributionModels::distributionModel> sizeDistribution_;

        //- List of cell labels corresponding to injector positions
        labelList cellOwners_;

        //- Fraction of injection controlled by this processor
        scalar fraction_;


public:

    //- Runtime type information
    TypeName("patchInjection");


    // Constructors

        //- Construct from dictionary
        PatchInjection
        (
            const dictionary& dict,
            CloudType& owner,
            const word& modelName
        );

        //- Construct copy
        PatchInjection(const PatchInjection<CloudType>& im);

        //- Construct and return a clone
        virtual autoPtr<InjectionModel<CloudType> > clone() const
        {
            return autoPtr<InjectionModel<CloudType> >
            (
                new PatchInjection<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~PatchInjection();


    // Member Functions

        //- Set injector locations when mesh is updated
        virtual void updateMesh();

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
CML::PatchInjection<CloudType>::PatchInjection
(
    const dictionary& dict,
    CloudType& owner,
    const word& modelName
)
:
    InjectionModel<CloudType>(dict, owner, modelName, typeName),
    patchName_(this->coeffDict().lookup("patchName")),
    patchId_(owner.mesh().boundaryMesh().findPatchID(patchName_)),
    duration_(readScalar(this->coeffDict().lookup("duration"))),
    parcelsPerSecond_
    (
        readScalar(this->coeffDict().lookup("parcelsPerSecond"))
    ),
    U0_(this->coeffDict().lookup("U0")),
    flowRateProfile_
    (
        TimeDataEntry<scalar>
        (
            owner.db().time(),
            "flowRateProfile",
            this->coeffDict()
        )
    ),
    sizeDistribution_
    (
        distributionModels::distributionModel::New
        (
            this->coeffDict().subDict("sizeDistribution"),
            owner.rndGen()
        )
    ),
    cellOwners_(),
    fraction_(1.0)
{
    if (patchId_ < 0)
    {
        FatalErrorIn
        (
            "PatchInjection<CloudType>::PatchInjection"
            "("
                "const dictionary&, "
                "CloudType&"
            ")"
        )   << "Requested patch " << patchName_ << " not found" << nl
            << "Available patches are: " << owner.mesh().boundaryMesh().names()
            << nl << exit(FatalError);
    }

    duration_ = owner.db().time().userTimeToTime(duration_);

    updateMesh();

    label patchSize = cellOwners_.size();
    label totalPatchSize = patchSize;
    reduce(totalPatchSize, sumOp<label>());
    fraction_ = scalar(patchSize)/totalPatchSize;

    // Set total volume/mass to inject
    this->volumeTotal_ = fraction_*flowRateProfile_.integrate(0.0, duration_);
    this->massTotal_ *= fraction_;
}


template<class CloudType>
CML::PatchInjection<CloudType>::PatchInjection
(
    const PatchInjection<CloudType>& im
)
:
    InjectionModel<CloudType>(im),
    patchName_(im.patchName_),
    patchId_(im.patchId_),
    duration_(im.duration_),
    parcelsPerSecond_(im.parcelsPerSecond_),
    U0_(im.U0_),
    flowRateProfile_(im.flowRateProfile_),
    sizeDistribution_(im.sizeDistribution_().clone().ptr()),
    cellOwners_(im.cellOwners_),
    fraction_(im.fraction_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::PatchInjection<CloudType>::~PatchInjection()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::PatchInjection<CloudType>::updateMesh()
{
    // Set/cache the injector cells
    const polyPatch& patch = this->owner().mesh().boundaryMesh()[patchId_];
    cellOwners_ = patch.faceCells();
}


template<class CloudType>
CML::scalar CML::PatchInjection<CloudType>::timeEnd() const
{
    return this->SOI_ + duration_;
}


template<class CloudType>
CML::label CML::PatchInjection<CloudType>::parcelsToInject
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
CML::scalar CML::PatchInjection<CloudType>::volumeToInject
(
    const scalar time0,
    const scalar time1
)
{
    if ((time0 >= 0.0) && (time0 < duration_))
    {
        return fraction_*flowRateProfile_.integrate(time0, time1);
    }
    else
    {
        return 0.0;
    }
}


template<class CloudType>
void CML::PatchInjection<CloudType>::setPositionAndCell
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
void CML::PatchInjection<CloudType>::setProperties
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
bool CML::PatchInjection<CloudType>::fullyDescribed() const
{
    return false;
}


template<class CloudType>
bool CML::PatchInjection<CloudType>::validInjection(const label)
{
    return true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
