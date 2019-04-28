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
    CML::PatchFlowRateInjection

Description
    Patch injection, by using patch flow rate to determine concentration and
    velocity.

    User specifies:
      - Total mass to inject
      - Name of patch
      - Injection duration
      - Injection target concentration/carrier volume flow rate

    Properties:
      - Initial parcel velocity given by local flow velocity
      - Parcel diameters obtained by distribution model
      - Parcels injected randomly across the patch


\*---------------------------------------------------------------------------*/

#ifndef PatchFlowRateInjection_H
#define PatchFlowRateInjection_H

#include "InjectionModel.hpp"
#include "TimeDataEntry.hpp"
#include "distributionModel.hpp"
#include "mathematicalConstants.hpp"
#include "surfaceFields.hpp"

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
    public InjectionModel<CloudType>,
    public patchInjectionBase
{
    // Private data

        //- Name of carrier (mass or volume) flux field
        const word phiName_;

        //- Name of carrier density field
        const word rhoName_;

        //- Injection duration [s]
        scalar duration_;

        //- Concentration profile of particle volume to carrier volume [-]
        const TimeDataEntry<scalar> concentration_;

        //- Parcels to introduce per unit volume flow rate m3 [n/m3]
        const scalar parcelConcentration_;

        //- Parcel size distribution model
        const autoPtr<distributionModel> sizeDistribution_;


public:

    //- Runtime type information
    TypeName("patchFlowRateInjection");


    // Constructors

        //- Construct from dictionary
        PatchFlowRateInjection
        (
            const dictionary& dict,
            CloudType& owner,
            const word& modelName
        );

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

        //- Inherit updateMesh from patchInjectionBase
        using patchInjectionBase::updateMesh;

        //- Set injector locations when mesh is updated
        virtual void updateMesh();

        //- Return the end-of-injection time
        scalar timeEnd() const;

        //- Return the total volumetric flow rate across the patch [m3/s]
        virtual scalar flowRate() const;

        //- Number of parcels to introduce relative to SOI
        virtual label parcelsToInject(const scalar time0, const scalar time1);

        //- Volume of parcels to introduce relative to SOI
        virtual scalar volumeToInject(const scalar time0, const scalar time1);


        // Injection geometry

            //- Inherit setPositionAndCell from patchInjectionBase
            using patchInjectionBase::setPositionAndCell;

            //- Set the injection position and owner cell, tetFace and tetPt
            virtual void setPositionAndCell
            (
                const label parcelI,
                const label nParcels,
                const scalar time,
                vector& position,
                label& cellOwner,
                label& tetFacei,
                label& tetPti
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
    CloudType& owner,
    const word& modelName
)
:
    InjectionModel<CloudType>(dict, owner, modelName,typeName),
    patchInjectionBase(owner.mesh(), this->coeffDict().lookup("patchName")),
    phiName_(this->coeffDict().template lookupOrDefault<word>("phi", "phi")),
    rhoName_(this->coeffDict().template lookupOrDefault<word>("rho", "rho")),
    duration_(readScalar(this->coeffDict().lookup("duration"))),
    concentration_
    (
        TimeDataEntry<scalar>
        (
            owner.db().time(),
            "concentration",
            this->coeffDict()
        )
    ),
    parcelConcentration_
    (
        readScalar(this->coeffDict().lookup("parcelConcentration"))
    ),
    sizeDistribution_
    (
        distributionModel::New
        (
            this->coeffDict().subDict("sizeDistribution"),
            owner.rndGen()
        )
    )
{
    duration_ = owner.db().time().userTimeToTime(duration_);

    patchInjectionBase::updateMesh(owner.mesh());

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
    patchInjectionBase(im),
    phiName_(im.phiName_),
    rhoName_(im.rhoName_),
    duration_(im.duration_),
    concentration_(im.concentration_),
    parcelConcentration_(im.parcelConcentration_),
    sizeDistribution_(im.sizeDistribution_().clone().ptr())
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::PatchFlowRateInjection<CloudType>::~PatchFlowRateInjection()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::PatchFlowRateInjection<CloudType>::updateMesh()
{
    patchInjectionBase::updateMesh(this->owner().mesh());
}


template<class CloudType>
CML::scalar CML::PatchFlowRateInjection<CloudType>::timeEnd() const
{
    return this->SOI_ + duration_;
}


template<class CloudType>
CML::scalar CML::PatchFlowRateInjection<CloudType>::flowRate() const
{
   const polyMesh& mesh = this->owner().mesh();

    const surfaceScalarField& phi =
        mesh.lookupObject<surfaceScalarField>(phiName_);

    const scalarField& phip = phi.boundaryField()[patchId_];

    scalar flowRateIn = 0.0;
    if (phi.dimensions() == dimVelocity*dimArea)
    {
        flowRateIn = max(0.0, -sum(phip));
    }
    else
    {
        const volScalarField& rho =
            mesh.lookupObject<volScalarField>(rhoName_);
        const scalarField& rhop = rho.boundaryField()[patchId_];

        flowRateIn = max(0.0, -sum(phip/rhop));
    }

    reduce(flowRateIn, sumOp<scalar>());

    return flowRateIn;
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
        scalar dt = time1 - time0;

        scalar c = concentration_.value(0.5*(time0 + time1));

        scalar nParcels = parcelConcentration_*c*flowRate()*dt;

        Random& rnd = this->owner().rndGen();

        label nParcelsToInject = floor(nParcels);

        // Inject an additional parcel with a probability based on the
        // remainder after the floor function
        if
        (
            nParcelsToInject > 0
         && nParcels - scalar(nParcelsToInject) > rnd.globalScalar01()
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
        scalar c = concentration_.value(0.5*(time0 + time1));

        volume = c*(time1 - time0)*flowRate();
    }

    this->volumeTotal_ = volume;
    this->massTotal_ = volume*this->owner().constProps().rho0();

    return volume;
}


template<class CloudType>
void CML::PatchFlowRateInjection<CloudType>::setPositionAndCell
(
    const label,
    const label,
    const scalar,
    vector& position,
    label& cellOwner,
    label& tetFacei,
    label& tetPti
)
{
    patchInjectionBase::setPositionAndCell
    (
        this->owner().mesh(),
        this->owner().rndGen(),
        position,
        cellOwner,
        tetFacei,
        tetPti
    );
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
    // Set particle velocity to carrier velocity
    parcel.U() = this->owner().U()[parcel.cell()];

    // Set particle diameter
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
