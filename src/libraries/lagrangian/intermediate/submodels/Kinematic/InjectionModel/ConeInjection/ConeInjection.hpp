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
    CML::ConeInjection

Description
    This injector injects particles in a number of cones. The user specifies a
    position and a direction to inject at, and two angles to inject between.
    Optionally, this injector can introduce particles over a disc, instead of
    at a point, in which case inner and outer diameters of the disc are also
    specified.

    The velocity is specified either as constant, or it is calculated from an
    injection pressure, or it is calculated from the injector mass flow rate
    and a discharge coefficient; i.e.:

    Constant velocity:
    \f[
    U = U_{constant}
    \f]

    Pressure driven velocity:
    \f[
    U = \sqrt{2(p_{injection} - p)/\rho}
    \f]

    Flow rate and discharge:
    \f[
    U = \dot{m}/(\rho A C_{discharge})
    \f]

Usage
    \table
    Property        | Description                    | Required | Default
    position        | The injection position         | yes      |
    direction       | The injection direction        | yes      |
    thetaInner      | The inner cone angle           | yes      |
    thetaOuter      | The outer cone angle           | yes      |
    injectionMethod | Inject at a point or on a disc | no       | point
    dInner          | The inner disc diameter        |\\
                                               if disc or flowRateAndDischarge |
    dInner          | The outer disc diameter        |\\
                                               if disc or flowRateAndDischarge |
    flowType        | Inject with constantVelocity, pressureDrivenVelocity \\
                                 or flowRateAndDischarge | no | constantVelocity
    Umag            | The injection velocity         | if constantVelocity |
    Pinj            | The injection pressure         |\\
                                                     if pressureDrivenVelocity |
    Cd              | The discharge coefficient      | if flowRateAndDischarge |
    \endtable

    Example specification:

    \verbatim
    injectionModels
    {
        model1
        {
            type            coneInjection;

            // Times
            SOI             0;
            duration        1;

            // Quantities
            massTotal       0; // <-- not used with these settings
            parcelBasisType fixed;
            parcelsPerSecond 1000000;
            flowRateProfile constant 1;
            nParticle       1;

            // Sizes
            sizeDistribution
            {
                type        fixedValue;
                fixedValueDistribution
                {
                    value   0.0025;
                }
            }

            // Geometry
            position        (-0.15 -0.1 0);
            direction       (1 0 0);
            thetaInner      0;
            thetaOuter      45;

            // - Inject at a point
            injectionMethod point;

            //// - Or, inject over a disc:
            //injectionMethod disc;
            //dInner          0;
            //dOuter          0.05;

            // Velocity

            // - Inject with constant velocity
            flowType        constantVelocity;
            Umag            1;

            //// - Or, inject with flow rate and discharge coefficient
            ////   This also requires massTotal, dInner and dOuter
            //flowType        flowRateAndDischarge;
            //Cd              0.9;

            //// - Or, inject at a pressure
            //flowType        pressureDrivenVelocity;
            //Pinj            10e5;
        }
    }
    \endverbatim

SourceFiles
    ConeInjection.C

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
public:

    //- Injection method enumeration
    enum injectionMethod
    {
        imPoint,
        imDisc
    };

    //- Flow type enumeration
    enum flowType
    {
        ftConstantVelocity,
        ftPressureDrivenVelocity,
        ftFlowRateAndDischarge
    };


private:

    // Private data

        //- Point/disc injection method
        injectionMethod injectionMethod_;

        //- Flow type
        flowType flowType_;

        //- Position of the injector
        const TimeDataEntry<vector> position_;

        //- Is the position constant?
        const bool positionIsConstant_;

        //- Centreline direction in which to inject
        const TimeDataEntry<vector> direction_;

        //- Cell label corresponding to the injector position
        label injectorCell_;

        //- Tet-face label corresponding to the injector position
        label injectorTetFace_;

        //- Tet-point label corresponding to the injector position
        label injectorTetPt_;

        //- Injection duration [s]
        scalar duration_;

        //- Number of parcels to introduce per second
        const label parcelsPerSecond_;

        //- Flow rate profile relative to SOI []
        const TimeDataEntry<scalar> flowRateProfile_;

        //- Inner half-cone angle relative to SOI [deg]
        const TimeDataEntry<scalar> thetaInner_;

        //- Outer half-cone angle relative to SOI [deg]
        const TimeDataEntry<scalar> thetaOuter_;

        //- Parcel size distribution model
        const autoPtr<distributionModel> sizeDistribution_;


        // Disc geometry

            //- The inner disc diameter [m]
            scalar dInner_;

            //- The outer disc diameter [m]
            scalar dOuter_;


        // Velocity model coefficients

            //- Parcel velocity [m/s]
            TimeDataEntry<scalar> Umag_;

            //- Discharge coefficient []
            TimeDataEntry<scalar> Cd_;

            //- Injection pressure [Pa]
            TimeDataEntry<scalar> Pinj_;


    // Private Member Functions

        //- Set the injection type
        void setInjectionMethod();

        //- Set the injection flow type
        void setFlowType();


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
                label& tetFacei,
                label& tetPti
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
// * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::ConeInjection<CloudType>::setInjectionMethod()
{
    const word injectionMethod =
        this->coeffDict().template lookupOrDefault<word>
        (
            "injectionMethod",
            word::null
        );

    if (injectionMethod == "point" || injectionMethod == word::null)
    {
        injectionMethod_ = imPoint;

        updateMesh();
    }
    else if (injectionMethod == "disc")
    {
        injectionMethod_ = imDisc;

        this->coeffDict().lookup("dInner") >> dInner_;
        this->coeffDict().lookup("dOuter") >> dOuter_;
    }
    else
    {
        FatalErrorInFunction
            << "injectionMethod must be either 'point' or 'disc'"
            << exit(FatalError);
    }
}


template<class CloudType>
void CML::ConeInjection<CloudType>::setFlowType()
{
    const word flowType =
        this->coeffDict().template lookupOrDefault<word>
        (
            "flowType",
            word::null
        );

    if (flowType == "constantVelocity" || flowType == word::null)
    {
        flowType_ = ftConstantVelocity;

        Umag_.reset(this->coeffDict());
    }
    else if (flowType == "pressureDrivenVelocity")
    {
        flowType_ = ftPressureDrivenVelocity;

        Pinj_.reset(this->coeffDict());
    }
    else if (flowType == "flowRateAndDischarge")
    {
        flowType_ = ftFlowRateAndDischarge;

        this->coeffDict().lookup("dInner") >> dInner_;
        this->coeffDict().lookup("dOuter") >> dOuter_;

        Cd_.reset(this->coeffDict());
    }
    else
    {
        FatalErrorInFunction
            << "flowType must be either 'constantVelocity', "
            << "'pressureDrivenVelocity' or 'flowRateAndDischarge'"
            << exit(FatalError);
    }
}


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
    injectionMethod_(imPoint),
    flowType_(ftConstantVelocity),
    position_
    (
        TimeDataEntry<vector>
        (
            owner.db().time(),
            "position",
            this->coeffDict()
        )
    ),
    positionIsConstant_(isA<DataEntryTypes::Constant<vector>>(position_)),
    direction_
    (
        TimeDataEntry<vector>
        (
            owner.db().time(),
            "direction",
            this->coeffDict()
        )
    ),
    injectorCell_(-1),
    injectorTetFace_(-1),
    injectorTetPt_(-1),
    duration_(readScalar(this->coeffDict().lookup("duration"))),
    parcelsPerSecond_
    (
        readScalar(this->coeffDict().lookup("parcelsPerSecond"))
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
        distributionModel::New
        (
            this->coeffDict().subDict("sizeDistribution"), owner.rndGen()
        )
    ),
    dInner_(VGREAT),
    dOuter_(VGREAT),
    Umag_(owner.db().time(), "Umag"),
    Cd_(owner.db().time(), "Cd"),
    Pinj_(owner.db().time(), "Pinj")
{
    duration_ = owner.db().time().userTimeToTime(duration_);

    setInjectionMethod();

    setFlowType();

    // Set total volume to inject
    this->volumeTotal_ = flowRateProfile_.integrate(0, duration_);

    updateMesh();
}


template<class CloudType>
CML::ConeInjection<CloudType>::ConeInjection
(
    const ConeInjection<CloudType>& im
)
:
    InjectionModel<CloudType>(im),
    injectionMethod_(im.injectionMethod_),
    flowType_(im.flowType_),
    position_(im.position_),
    positionIsConstant_(im.positionIsConstant_),
    direction_(im.direction_),
    injectorCell_(im.injectorCell_),
    injectorTetFace_(im.injectorTetFace_),
    injectorTetPt_(im.injectorTetPt_),
    duration_(im.duration_),
    parcelsPerSecond_(im.parcelsPerSecond_),
    flowRateProfile_(im.flowRateProfile_),
    thetaInner_(im.thetaInner_),
    thetaOuter_(im.thetaOuter_),
    sizeDistribution_(im.sizeDistribution_().clone().ptr()),
    dInner_(im.dInner_),
    dOuter_(im.dOuter_),
    Umag_(im.Umag_),
    Cd_(im.Cd_),
    Pinj_(im.Pinj_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::ConeInjection<CloudType>::~ConeInjection()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::ConeInjection<CloudType>::updateMesh()
{
    if (injectionMethod_ == imPoint && positionIsConstant_)
    {
        vector position = position_.value(0);
        this->findCellAtPosition
        (
            injectorCell_,
            injectorTetFace_,
            injectorTetPt_,
            position
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
    if (time0 >= 0 && time0 < duration_)
    {
        //// Standard calculation
        //return floor(parcelsPerSecond_*(time1 - time0));

        // Modified calculation to make numbers exact
        return floor(parcelsPerSecond_*time1 - this->parcelsAddedTotal());
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
    if (time0 >= 0 && time0 < duration_)
    {
        return flowRateProfile_.integrate(time0, time1);
    }
    else
    {
        return 0;
    }
}


template<class CloudType>
void CML::ConeInjection<CloudType>::setPositionAndCell
(
    const label parcelI,
    const label,
    const scalar time,
    vector& position,
    label& cellOwner,
    label& tetFacei,
    label& tetPti
)
{
    Random& rndGen = this->owner().rndGen();

    const scalar t = time - this->SOI_;

    switch (injectionMethod_)
    {
        case imPoint:
        {
            position = position_.value(t);
            if (positionIsConstant_)
            {
                cellOwner = injectorCell_;
                tetFacei = injectorTetFace_;
                tetPti = injectorTetPt_;
            }
            else
            {
                this->findCellAtPosition
                (
                    cellOwner,
                    tetFacei,
                    tetPti,
                    position,
                    false
                );
            }
            break;
        }
        case imDisc:
        {
            const scalar beta = twoPi*rndGen.globalScalar01();
            const scalar frac = rndGen.globalScalar01();
            const vector n = normalised(direction_.value(t));
            const vector t1 = normalised(perpendicular(n));
            const vector t2 = normalised(n ^ t1);
            const vector tanVec = t1*cos(beta) + t2*sin(beta);
            const scalar d = sqrt((1 - frac)*sqr(dInner_) + frac*sqr(dOuter_));
            position = position_.value(t) + d/2*tanVec;
            this->findCellAtPosition
            (
                cellOwner,
                tetFacei,
                tetPti,
                position,
                false
            );
            break;
        }
        default:
        {
            break;
        }
    }
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
    Random& rndGen = this->owner().rndGen();

    const scalar t = time - this->SOI_;

    // Get the angle from the axis and the vector perpendicular from the axis.
    // If injecting at a point, then these are calculated from two new random
    // numbers. If a disc, then these calculations have already been done in
    // setPositionAndCell, so the angle and vector can be reverse engineered
    // from the position.
    scalar theta = VGREAT;
    vector tanVec = vector::max;
    switch (injectionMethod_)
    {
        case imPoint:
        {
            const scalar beta = twoPi*rndGen.scalar01();
            const scalar frac = rndGen.scalar01();
            const vector n = normalised(direction_.value(t));
            const vector t1 = normalised(perpendicular(n));
            const vector t2 = normalised(n ^ t1);
            tanVec = t1*cos(beta) + t2*sin(beta);
            theta =
                degToRad
                (
                    sqrt
                    (
                        (1 - frac)*sqr(thetaInner_.value(t))
                        + frac*sqr(thetaOuter_.value(t))
                    )
                );
            break;
        }
        case imDisc:
        {
            const scalar r = mag(parcel.position() - position_.value(t));
            const scalar frac = (2*r - dInner_)/(dOuter_ - dInner_);
            tanVec = normalised(parcel.position() - position_.value(t));
            theta =
                degToRad
                (
                    (1 - frac)*thetaInner_.value(t)
                    + frac*thetaOuter_.value(t)
                );
            break;
        }
        default:
        {
            break;
        }
    }

    // The direction of injection
    const vector dirVec =
        normalised
        (
            cos(theta)*normalised(direction_.value(t))
          + sin(theta)*tanVec
        );

    // Set the velocity
    switch (flowType_)
    {
        case ftConstantVelocity:
        {
            parcel.U() = Umag_.value(t)*dirVec;
            break;
        }
        case ftPressureDrivenVelocity:
        {
            const scalar pAmbient = this->owner().pAmbient();
            const scalar rho = parcel.rho();
            const scalar Umag = ::sqrt(2*(Pinj_.value(t) - pAmbient)/rho);
            parcel.U() = Umag*dirVec;
            break;
        }
        case ftFlowRateAndDischarge:
        {
            const scalar A = 0.25*pi*(sqr(dOuter_) - sqr(dInner_));
            const scalar massFlowRate =
                this->massTotal()*flowRateProfile_.value(t)/this->volumeTotal();
            const scalar Umag =
                massFlowRate/(parcel.rho()*Cd_.value(t)*A);
            parcel.U() = Umag*dirVec;
            break;
        }
        default:
        {
            break;
        }
    }

    // Set the particle diameter
    parcel.d() = sizeDistribution_->sample();
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
