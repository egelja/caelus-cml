/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011-2018 OpenFOAM Foundation
Copyright (C) 2015-2018 OpenCFD Ltd.
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
    CML::ConeNozzleInjection

Description
    Cone injection.

    User specifies:
      - time of start of injection
      - injector position
      - direction (along injection axis)
      - parcel flow rate
      - inner and outer half-cone angles

    Properties:
      - Parcel diameters obtained by size distribution model.

      - Parcel velocity is calculated as:
        - Constant velocity:
          \verbatim
          U = \<specified by user\>
          \endverbatim

        - Pressure driven velocity:
          \verbatim
          U = sqrt(2*(Pinj - Pamb)/rho)
          \endverbatim

        - Flow rate and discharge:
          \verbatim
          U = V_dot/(A*Cd)
          \endverbatim


\*---------------------------------------------------------------------------*/

#ifndef ConeNozzleInjection_H
#define ConeNozzleInjection_H

#include "InjectionModel.hpp"
#include "Enum.hpp"
#include "TimeDataEntry.hpp"
#include "mathematicalConstants.hpp"
#include "distributionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes

template<class Type>
class TimeDataEntry;

class distributionModel;

/*---------------------------------------------------------------------------*\
                     Class ConeNozzleInjection Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ConeNozzleInjection
:
    public InjectionModel<CloudType>
{
public:

    //- Injection method enumeration
    enum class injectionMethod
    {
        imPoint,
        imDisc,
        imMovingPoint
    };

    static const Enum<injectionMethod> injectionMethodNames;

    //- Flow type enumeration
    enum class flowType
    {
        ftConstantVelocity,
        ftPressureDrivenVelocity,
        ftFlowRateAndDischarge
    };

    static const Enum<flowType> flowTypeNames;


private:

    // Private data

        //- Point/disc injection method
        injectionMethod injectionMethod_;

        //- Flow type
        flowType flowType_;

        //- Outer nozzle diameter [m]
        const scalar outerDiameter_;

        //- Inner nozzle diameter [m]
        const scalar innerDiameter_;

        //- Injection duration [s]
        scalar duration_;

        //- Position relative to SOI []
        TimeDataEntry<vector> positionVsTime_;

        //- Injector position [m]
        vector position_;

        //- Cell containing injector position []
        label injectorCell_;

        //- Index of tet face for injector cell
        label tetFacei_;

        //- Index of tet point for injector cell
        label tetPti_;

        //- Injector direction []
        vector direction_;

        //- Number of parcels to introduce per second []
        const label parcelsPerSecond_;

        //- Flow rate profile relative to SOI []
        const TimeDataEntry<scalar> flowRateProfile_;

        //- Inner half-cone angle relative to SOI [deg]
        const TimeDataEntry<scalar> thetaInner_;

        //- Outer half-cone angle relative to SOI [deg]
        const TimeDataEntry<scalar> thetaOuter_;

        //- Parcel size PDF model
        const autoPtr<distributionModel> sizeDistribution_;


        // Tangential vectors to the direction vector

            //- First tangential vector
            vector tanVec1_;

            //- Second tangential vector
            vector tanVec2_;

            //- Injection vector orthogonal to direction
            vector normal_;


        // Velocity model coefficients

            //- Constant velocity [m/s]
            scalar UMag_;

            //- Discharge coefficient, relative to SOI [m/s]
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
    TypeName("coneNozzleInjection");


    // Constructors

        //- Construct from dictionary
        ConeNozzleInjection
        (
            const dictionary& dict,
            CloudType& owner,
            const word& modelName
        );

        //- Construct copy
        ConeNozzleInjection(const ConeNozzleInjection<CloudType>& im);

        //- Construct and return a clone
        virtual autoPtr<InjectionModel<CloudType> > clone() const
        {
            return autoPtr<InjectionModel<CloudType> >
            (
                new ConeNozzleInjection<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~ConeNozzleInjection();


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

            //- Set the injection position and owner cell
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

using namespace CML::constant;


template<class CloudType>
const CML::Enum
<
    typename CML::ConeNozzleInjection<CloudType>::injectionMethod
>
CML::ConeNozzleInjection<CloudType>::injectionMethodNames
{
    { injectionMethod::imPoint, "point" },
    { injectionMethod::imDisc, "disc" },
    { injectionMethod::imMovingPoint, "movingPoint" }
};

template<class CloudType>
const CML::Enum
<
    typename CML::ConeNozzleInjection<CloudType>::flowType
>
CML::ConeNozzleInjection<CloudType>::flowTypeNames
{
    { flowType::ftConstantVelocity, "constantVelocity" },
    { flowType::ftPressureDrivenVelocity, "pressureDrivenVelocity" },
    { flowType::ftFlowRateAndDischarge, "flowRateAndDischarge" }
};


// * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::ConeNozzleInjection<CloudType>::setInjectionMethod()
{
    switch (injectionMethod_)
    {
        case injectionMethod::imPoint:
        case injectionMethod::imDisc:
        {
            position_ = this->coeffDict().lookup("position");
            break;
        }
        case injectionMethod::imMovingPoint:
        {
            positionVsTime_.reset(this->coeffDict());
            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unhandled injection method "
                << injectionMethodNames[injectionMethod_]
                << exit(FatalError);
        }
    }
}


template<class CloudType>
void CML::ConeNozzleInjection<CloudType>::setFlowType()
{
    switch (flowType_)
    {
        case flowType::ftConstantVelocity:
        {
            this->coeffDict().lookup("UMag") >> UMag_;
            break;
        }
        case flowType::ftPressureDrivenVelocity:
        {
            Pinj_.reset(this->coeffDict());
            break;
        }
        case flowType::ftFlowRateAndDischarge:
        {
            Cd_.reset(this->coeffDict());
            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unhandled flow type "
                << flowTypeNames[flowType_]
                << exit(FatalError);
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ConeNozzleInjection<CloudType>::ConeNozzleInjection
(
    const dictionary& dict,
    CloudType& owner,
    const word& modelName
)
:
    InjectionModel<CloudType>(dict, owner, modelName, typeName),
    injectionMethod_
    (
        injectionMethodNames.lookup("injectionMethod", this->coeffDict())
    ),
    flowType_(flowTypeNames.lookup("flowType", this->coeffDict())),
    outerDiameter_(readScalar(this->coeffDict().lookup("outerDiameter"))),
    innerDiameter_(readScalar(this->coeffDict().lookup("innerDiameter"))),
    duration_(readScalar(this->coeffDict().lookup("duration"))),
    positionVsTime_(owner.db().time(), "position"),
    position_(vector::zero),
    injectorCell_(-1),
    tetFacei_(-1),
    tetPti_(-1),
    direction_(this->coeffDict().lookup("direction")),
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
            this->coeffDict().subDict("sizeDistribution"),
            owner.rndGen()
        )
    ),
    tanVec1_(Zero),
    tanVec2_(Zero),
    normal_(Zero),

    UMag_(0.0),
    Cd_(owner.db().time(), "Cd"),
    Pinj_(owner.db().time(), "Pinj")
{
    if (innerDiameter_ >= outerDiameter_)
    {
        FatalErrorInFunction
            << "Inner diameter must be less than the outer diameter:" << nl
            << "    innerDiameter: " << innerDiameter_ << nl
            << "    outerDiameter: " << outerDiameter_
            << exit(FatalError);
    }

    duration_ = owner.db().time().userTimeToTime(duration_);

    setInjectionMethod();

    setFlowType();

    // Normalise direction vector
    direction_ /= mag(direction_);

    // Determine direction vectors tangential to direction
    tanVec1_ = normalised(perpendicular(direction_));
    tanVec2_ = normalised(direction_ ^ tanVec1_);

    // Set total volume to inject
    this->volumeTotal_ = flowRateProfile_.integrate(0.0, duration_);

    updateMesh();
}


template<class CloudType>
CML::ConeNozzleInjection<CloudType>::ConeNozzleInjection
(
    const ConeNozzleInjection<CloudType>& im
)
:
    InjectionModel<CloudType>(im),
    injectionMethod_(im.injectionMethod_),
    flowType_(im.flowType_),
    outerDiameter_(im.outerDiameter_),
    innerDiameter_(im.innerDiameter_),
    duration_(im.duration_),
    positionVsTime_(im.positionVsTime_),
    position_(im.position_),
    injectorCell_(im.injectorCell_),
    tetFacei_(im.tetFacei_),
    tetPti_(im.tetPti_),
    direction_(im.direction_),
    parcelsPerSecond_(im.parcelsPerSecond_),
    flowRateProfile_(im.flowRateProfile_),
    thetaInner_(im.thetaInner_),
    thetaOuter_(im.thetaOuter_),
    sizeDistribution_(im.sizeDistribution_().clone().ptr()),
    tanVec1_(im.tanVec1_),
    tanVec2_(im.tanVec2_),
    normal_(im.normal_),
    UMag_(im.UMag_),
    Cd_(im.Cd_),
    Pinj_(im.Pinj_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::ConeNozzleInjection<CloudType>::~ConeNozzleInjection()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::ConeNozzleInjection<CloudType>::updateMesh()
{
    // Set/cache the injector cell info for static methods

    switch (injectionMethod_)
    {
        case injectionMethod::imPoint:
        {
            this->findCellAtPosition
            (
                injectorCell_,
                tetFacei_,
                tetPti_,
                position_
            );
            break;
        }
        default:
        {
            // Do nothing for the other methods
        }
    }
}


template<class CloudType>
CML::scalar CML::ConeNozzleInjection<CloudType>::timeEnd() const
{
    return this->SOI_ + duration_;
}


template<class CloudType>
CML::label CML::ConeNozzleInjection<CloudType>::parcelsToInject
(
    const scalar time0,
    const scalar time1
)
{
    if ((time0 >= 0.0) && (time0 < duration_))
    {
        return floor((time1 - time0)*parcelsPerSecond_);
    }
    else
    {
        return 0;
    }
}


template<class CloudType>
CML::scalar CML::ConeNozzleInjection<CloudType>::volumeToInject
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
void CML::ConeNozzleInjection<CloudType>::setPositionAndCell
(
    const label,
    const label,
    const scalar time,
    vector& position,
    label& cellOwner,
    label& tetFacei,
    label& tetPti
)
{
    Random& rndGen = this->owner().rndGen();

    scalar beta = mathematical::twoPi*rndGen.globalScalar01();
    normal_ = tanVec1_*cos(beta) + tanVec2_*sin(beta);

    switch (injectionMethod_)
    {
        case injectionMethod::imPoint:
        {
            position = position_;
            cellOwner = injectorCell_;
            tetFacei = tetFacei_;
            tetPti = tetPti_;

            break;
        }
        case injectionMethod::imMovingPoint:
        {
            position = positionVsTime_.value(time - this->SOI_);

            this->findCellAtPosition
            (
                cellOwner,
                tetFacei,
                tetPti,
                position
            );

            break;
        }
        case injectionMethod::imDisc:
        {
            scalar frac = rndGen.globalScalar01();
            scalar dr = outerDiameter_ - innerDiameter_;
            scalar r = 0.5*(innerDiameter_ + frac*dr);
            position = position_ + r*normal_;

            this->findCellAtPosition
            (
                cellOwner,
                tetFacei,
                tetPti,
                position
            );
            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unhandled injection method "
                << injectionMethodNames[injectionMethod_]
                << exit(FatalError);
        }
    }
}


template<class CloudType>
void CML::ConeNozzleInjection<CloudType>::setProperties
(
    const label parcelI,
    const label,
    const scalar time,
    typename CloudType::parcelType& parcel
)
{
    Random& rndGen = this->owner().rndGen();

    // Set particle velocity
    const scalar deg2Rad = mathematical::pi/180.0;

    scalar t = time - this->SOI_;
    scalar ti = thetaInner_.value(t);
    scalar to = thetaOuter_.value(t);
    scalar coneAngle = rndGen.sample01<scalar>()*(to - ti) + ti;

    coneAngle *= deg2Rad;
    scalar alpha = sin(coneAngle);
    scalar dcorr = cos(coneAngle);

    vector normal = alpha*normal_;
    vector dirVec = dcorr*direction_;
    dirVec += normal;
    dirVec /= mag(dirVec);

    switch (flowType_)
    {
        case flowType::ftConstantVelocity:
        {
            parcel.U() = UMag_*dirVec;
            break;
        }
        case flowType::ftPressureDrivenVelocity:
        {
            scalar pAmbient = this->owner().pAmbient();
            scalar rho = parcel.rho();
            scalar UMag = ::sqrt(2.0*(Pinj_.value(t) - pAmbient)/rho);
            parcel.U() = UMag*dirVec;
            break;
        }
        case flowType::ftFlowRateAndDischarge:
        {
            scalar Ao = 0.25*mathematical::pi*outerDiameter_*outerDiameter_;
            scalar Ai = 0.25*mathematical::pi*innerDiameter_*innerDiameter_;
            scalar massFlowRate =
                this->massTotal()
               *flowRateProfile_.value(t)
               /this->volumeTotal();

            scalar Umag = massFlowRate/(parcel.rho()*Cd_.value(t)*(Ao - Ai));
            parcel.U() = Umag*dirVec;
            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unhandled injection method "
                << flowTypeNames[flowType_]
                << exit(FatalError);
        }
    }

    // Set particle diameter
    parcel.d() = sizeDistribution_->sample();
}


template<class CloudType>
bool CML::ConeNozzleInjection<CloudType>::fullyDescribed() const
{
    return false;
}


template<class CloudType>
bool CML::ConeNozzleInjection<CloudType>::validInjection(const label)
{
    return true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
