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
    CML::ConeInjection

Description
    Multi-point cone injection model
    - User specifies
      - time of start of injection
      - list of injector positions and directions (along injection axes)
      - number of parcels to inject per injector
      - parcel velocities
      - inner and outer cone angles
    - Parcel diameters obtained by distribution model


\*---------------------------------------------------------------------------*/

#ifndef ConeInjection_H
#define ConeInjection_H

#include "InjectionModel.hpp"
#include "distributionModel.hpp"
#include "vectorList.hpp"
#include "TimeDataEntry.hpp"
#include "TimeDataEntry.hpp"
#include "mathematicalConstants.hpp"
#include "unitConversion.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class ConeInjection Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ConeInjection
:
    public InjectionModel<CloudType>
{
    // Private data

        //- List of position and axis for each injector
        List<Tuple2<vector, vector> > positionAxis_;

        //- List of cell labels corresponding to injector positions
        labelList injectorCells_;

        //- List of tetFace labels corresponding to injector positions
        labelList injectorTetFaces_;

        //- List of tetPt labels corresponding to injector positions
        labelList injectorTetPts_;

        //- Injection duration [s]
        scalar duration_;

        //- Number of parcels to introduce per injector
        const label parcelsPerInjector_;

        //- Flow rate profile relative to SOI []
        const TimeDataEntry<scalar> flowRateProfile_;

        //- Parcel velocity magnitude relative to SOI [m/s]
        const TimeDataEntry<scalar> Umag_;

        //- Inner half-cone angle relative to SOI [deg]
        const TimeDataEntry<scalar> thetaInner_;

        //- Outer half-cone angle relative to SOI [deg]
        const TimeDataEntry<scalar> thetaOuter_;

        //- Parcel size distribution model
        const autoPtr<distributionModels::distributionModel> sizeDistribution_;

        //- Number of parcels per injector already injected
        mutable label nInjected_;


        // Tangential vectors to the direction vector

            //- First tangential vector
            vectorList tanVec1_;

            //- Second tangential vector
            vectorList tanVec2_;


public:

    //- Runtime type information
    TypeName("coneInjection");


    // Constructors

        //- Construct from dictionary
        ConeInjection
        (
            const dictionary& dict,
            CloudType& owner,
            const word& modelName
        );

        //- Construct copy
        ConeInjection(const ConeInjection<CloudType>& im);

        //- Construct and return a clone
        virtual autoPtr<InjectionModel<CloudType> > clone() const
        {
            return autoPtr<InjectionModel<CloudType> >
            (
                new ConeInjection<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~ConeInjection();


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

            //- Set the parcel properties
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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


using namespace CML::constant::mathematical;

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ConeInjection<CloudType>::ConeInjection
(
    const dictionary& dict,
    CloudType& owner,
    const word& modelName
)
:
    InjectionModel<CloudType>(dict, owner, modelName, typeName),
    positionAxis_(this->coeffDict().lookup("positionAxis")),
    injectorCells_(positionAxis_.size()),
    injectorTetFaces_(positionAxis_.size()),
    injectorTetPts_(positionAxis_.size()),
    duration_(readScalar(this->coeffDict().lookup("duration"))),
    parcelsPerInjector_
    (
        readScalar(this->coeffDict().lookup("parcelsPerInjector"))
    ),
    flowRateProfile_
    (
        TimeDataEntry<scalar>
        (
            owner.db().time(),
            "flowRateProfile",
            this->coeffDict()
        )
    ),
    Umag_
    (
        TimeDataEntry<scalar>
        (
            owner.db().time(),
            "Umag",
            this->coeffDict()
        )
    ),
    thetaInner_
    (
        TimeDataEntry<scalar>
        (
            owner.db().time(),
            "thetaInner",
            this->coeffDict()
        )
    ),
    thetaOuter_
    (
        TimeDataEntry<scalar>
        (
            owner.db().time(),
            "thetaOuter",
            this->coeffDict()
        )
    ),
    sizeDistribution_
    (
        distributionModels::distributionModel::New
        (
            this->coeffDict().subDict("sizeDistribution"), owner.rndGen()
        )
    ),
    nInjected_(this->parcelsAddedTotal()),
    tanVec1_(positionAxis_.size()),
    tanVec2_(positionAxis_.size())
{
    duration_ = owner.db().time().userTimeToTime(duration_);

    // Normalise direction vector and determine direction vectors
    // tangential to injector axis direction
    forAll(positionAxis_, i)
    {
        vector& axis = positionAxis_[i].second();

        axis /= mag(axis);

        vector tangent = vector::zero;
        scalar magTangent = 0.0;

        cachedRandom& rnd = this->owner().rndGen();
        while (magTangent < SMALL)
        {
            vector v = rnd.sample01<vector>();

            tangent = v - (v & axis)*axis;
            magTangent = mag(tangent);
        }

        tanVec1_[i] = tangent/magTangent;
        tanVec2_[i] = axis^tanVec1_[i];
    }

    // Set total volume to inject
    this->volumeTotal_ = flowRateProfile_.integrate(0.0, duration_);

    updateMesh();
}


template<class CloudType>
CML::ConeInjection<CloudType>::ConeInjection
(
    const ConeInjection<CloudType>& im
)
:
    InjectionModel<CloudType>(im),
    positionAxis_(im.positionAxis_),
    injectorCells_(im.injectorCells_),
    injectorTetFaces_(im.injectorTetFaces_),
    injectorTetPts_(im.injectorTetPts_),
    duration_(im.duration_),
    parcelsPerInjector_(im.parcelsPerInjector_),
    flowRateProfile_(im.flowRateProfile_),
    Umag_(im.Umag_),
    thetaInner_(im.thetaInner_),
    thetaOuter_(im.thetaOuter_),
    sizeDistribution_(im.sizeDistribution_().clone().ptr()),
    nInjected_(im.nInjected_),
    tanVec1_(im.tanVec1_),
    tanVec2_(im.tanVec2_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::ConeInjection<CloudType>::~ConeInjection()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::ConeInjection<CloudType>::updateMesh()
{
    // Set/cache the injector cells
    forAll(positionAxis_, i)
    {
        this->findCellAtPosition
        (
            injectorCells_[i],
            injectorTetFaces_[i],
            injectorTetPts_[i],
            positionAxis_[i].first()
        );
    }
}


template<class CloudType>
CML::scalar CML::ConeInjection<CloudType>::timeEnd() const
{
    return this->SOI_ + duration_;
}


template<class CloudType>
CML::label CML::ConeInjection<CloudType>::parcelsToInject
(
    const scalar time0,
    const scalar time1
)
{
    if ((time0 >= 0.0) && (time0 < duration_))
    {
        const scalar targetVolume = flowRateProfile_.integrate(0, time1);

        const label targetParcels =
            parcelsPerInjector_*targetVolume/this->volumeTotal_;

        const label nToInject = targetParcels - nInjected_;

        nInjected_ += nToInject;

        return positionAxis_.size()*nToInject;
    }
    else
    {
        return 0;
    }
}


template<class CloudType>
CML::scalar CML::ConeInjection<CloudType>::volumeToInject
(
    const scalar time0,
    const scalar time1
)
{
    if ((time0 >= 0.0) && (time0 < duration_))
    {
        return flowRateProfile_.integrate(time0, time1);
    }
    else
    {
        return 0.0;
    }
}


template<class CloudType>
void CML::ConeInjection<CloudType>::setPositionAndCell
(
    const label parcelI,
    const label,
    const scalar,
    vector& position,
    label& cellOwner,
    label& tetFaceI,
    label& tetPtI
)
{
    const label i = parcelI % positionAxis_.size();

    position = positionAxis_[i].first();
    cellOwner = injectorCells_[i];
    tetFaceI = injectorTetFaces_[i];
    tetPtI = injectorTetPts_[i];
}


template<class CloudType>
void CML::ConeInjection<CloudType>::setProperties
(
    const label parcelI,
    const label,
    const scalar time,
    typename CloudType::parcelType& parcel
)
{
    cachedRandom& rnd = this->owner().rndGen();

    // set particle velocity
    const label i = parcelI % positionAxis_.size();

    scalar t = time - this->SOI_;
    scalar ti = thetaInner_.value(t);
    scalar to = thetaOuter_.value(t);
    scalar coneAngle = degToRad(rnd.position<scalar>(ti, to));

    scalar alpha = sin(coneAngle);
    scalar dcorr = cos(coneAngle);
    scalar beta = twoPi*rnd.sample01<scalar>();

    vector normal = alpha*(tanVec1_[i]*cos(beta) + tanVec2_[i]*sin(beta));
    vector dirVec = dcorr*positionAxis_[i].second();
    dirVec += normal;
    dirVec /= mag(dirVec);

    parcel.U() = Umag_.value(t)*dirVec;

    // set particle diameter
    parcel.d() = sizeDistribution_().sample();
}


template<class CloudType>
bool CML::ConeInjection<CloudType>::fullyDescribed() const
{
    return false;
}


template<class CloudType>
bool CML::ConeInjection<CloudType>::validInjection(const label)
{
    return true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
