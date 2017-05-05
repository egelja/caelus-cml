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
    CML::InjectionModel

Description
    Templated injection model class.

    The injection model nominally describes the parcel:
    - position
    - diameter
    - velocity
    In this case, the fullyDescribed() flag should be set to 0 (false). When
    the parcel is then added to the cloud, the remaining properties are
    populated using values supplied in the constant properties.

    If, however, all of a parcel's properties are described in the model, the
    fullDescribed() flag should be set to 1 (true).


\*---------------------------------------------------------------------------*/

#ifndef InjectionModel_H
#define InjectionModel_H

#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "SubModelBase.hpp"
#include "vector.hpp"
#include "mathematicalConstants.hpp"
#include "meshTools.hpp"
#include "volFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class InjectionModel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class InjectionModel
:
    public SubModelBase<CloudType>
{
public:

    //- Convenience typedef for parcelType
    typedef typename CloudType::parcelType parcelType;


    // Enumerations

        //- Parcel basis representation options
        //  i.e constant number of particles OR constant mass per parcel
        enum parcelBasis
        {
            pbNumber,
            pbMass,
            pbFixed
        };


protected:

    // Protected data

        // Global injection properties

            //- Start of injection [s]
            scalar SOI_;

            //- Total volume of particles introduced by this injector [m^3]
            //  - scaled to ensure massTotal is achieved
            scalar volumeTotal_;

            //- Total mass to inject [kg]
            scalar massTotal_;

            //- Total mass injected to date [kg]
            scalar massInjected_;


        // Counters

            //- Number of injections counter
            label nInjections_;

            //- Running counter of total number of parcels added
            label parcelsAddedTotal_;


        // Injection properties per Lagrangian time step

            //- Parcel basis enumeration
            parcelBasis parcelBasis_;

            //- nParticle to assign to parcels when the 'fixed' basis
            //  is selected
            scalar nParticleFixed_;

            //- Continuous phase time at start of injection time step [s]
            scalar time0_;

            //- Time at start of injection time step [s]
            scalar timeStep0_;

            //- Volume that should have been injected, but would lead to
            //  less than 1 particle per parcel
            scalar delayedVolume_;


    // Protected Member Functions

        //- Additional flag to identify whether or not injection of parcelI is
        //  permitted
        virtual bool validInjection(const label parcelI);

        //- Determine properties for next time step/injection interval
        //  Return true if leads to valid injection
        virtual bool prepareForNextTimeStep
        (
            const scalar time,
            label& newParcels,
            scalar& newVolume
        );

        //- Find the cell that contains the supplied position
        //  Will modify position slightly towards the owner cell centroid to
        //  ensure that it lies in a cell and not edge/face
        virtual bool findCellAtPosition
        (
            label& cellI,
            label& tetFaceI,
            label& tetPtI,
            vector& position,
            bool errorOnNotFound = true
        );

        //- Set number of particles to inject given parcel properties
        virtual scalar setNumberOfParticles
        (
            const label parcels,
            const scalar volume,
            const scalar diameter,
            const scalar rho
        );

        //- Post injection checks
        virtual void postInjectCheck
        (
            const label parcelsAdded,
            const scalar massAdded
        );


public:

    //- Runtime type information
    TypeName("injectionModel");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        InjectionModel,
        dictionary,
        (
            const dictionary& dict,
            CloudType& owner
        ),
        (dict, owner)
    );


    // Constructors

        //- Construct null from owner
        InjectionModel(CloudType& owner);

        //- Construct from dictionary
        InjectionModel
        (
            const dictionary& dict,
            CloudType& owner,
            const word& type
        );

        //- Construct copy
        InjectionModel(const InjectionModel<CloudType>& im);

        //- Construct and return a clone
        virtual autoPtr<InjectionModel<CloudType> > clone() const
        {
            return autoPtr<InjectionModel<CloudType> >
            (
                new InjectionModel<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~InjectionModel();


    //- Selector
    static autoPtr<InjectionModel<CloudType> > New
    (
        const dictionary& dict,
        CloudType& owner
    );


    // Member Functions

        // Global information

            //- Return the start-of-injection time
            inline scalar timeStart() const;

            //- Return the total volume to be injected across the event
            inline scalar volumeTotal() const;

            //- Return mass of particles to introduce
            inline scalar massTotal() const;

            //- Return mass of particles injected (cumulative)
            inline scalar massInjected() const;

            //- Return the end-of-injection time
            virtual scalar timeEnd() const;

            //- Number of parcels to introduce relative to SOI
            virtual label parcelsToInject
            (
                const scalar time0,
                const scalar time1
            );

            //- Volume of parcels to introduce relative to SOI
            virtual scalar volumeToInject
            (
                const scalar time0,
                const scalar time1
            );

            //- Return the average parcel mass over the injection period
            scalar averageParcelMass();


            // Counters

                //- Return the number of injections
                inline label nInjections() const;

                //- Return the total number parcels added
                inline label parcelsAddedTotal() const;


        // Per-injection event functions

            //- Main injection loop
            template<class TrackData>
            void inject(TrackData& td);

            //- Main injection loop - steady-state
            template<class TrackData>
            void injectSteadyState(TrackData& td, const scalar trackTime);


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
                parcelType& parcel
            );

            //- Flag to identify whether model fully describes the parcel
            virtual bool fullyDescribed() const;


        // I-O

            //- Write injection info to stream
            virtual void info(Ostream& os);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeInjectionModel(CloudType)                                         \
                                                                              \
    typedef CloudType::kinematicCloudType kinematicCloudType;                 \
    defineNamedTemplateTypeNameAndDebug                                       \
    (                                                                         \
        InjectionModel<kinematicCloudType>,                                   \
        0                                                                     \
    );                                                                        \
    defineTemplateRunTimeSelectionTable                                       \
    (                                                                         \
        InjectionModel<kinematicCloudType>,                                   \
        dictionary                                                            \
    );


#define makeInjectionModelType(SS, CloudType)                                 \
                                                                              \
    typedef CloudType::kinematicCloudType kinematicCloudType;                 \
    defineNamedTemplateTypeNameAndDebug(SS<kinematicCloudType>, 0);           \
                                                                              \
    InjectionModel<kinematicCloudType>::                                      \
        adddictionaryConstructorToTable<SS<kinematicCloudType> >              \
        add##SS##CloudType##kinematicCloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::scalar CML::InjectionModel<CloudType>::timeStart() const
{
    return SOI_;
}


template<class CloudType>
CML::scalar CML::InjectionModel<CloudType>::volumeTotal() const
{
    return volumeTotal_;
}


template<class CloudType>
CML::scalar CML::InjectionModel<CloudType>::massTotal() const
{
    return massTotal_;
}


template<class CloudType>
CML::scalar CML::InjectionModel<CloudType>::massInjected() const
{
    return massInjected_;
}


template<class CloudType>
CML::label CML::InjectionModel<CloudType>::nInjections() const
{
    return nInjections_;
}


template<class CloudType>
CML::label CML::InjectionModel<CloudType>::parcelsAddedTotal() const
{
    return parcelsAddedTotal_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

using namespace CML::constant::mathematical;

// * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * * //

template<class CloudType>
bool CML::InjectionModel<CloudType>::validInjection(const label parcelI)
{
    notImplemented
    (
        "bool CML::InjectionModel<CloudType>::validInjection(const label)"
    );

    return false;
}


template<class CloudType>
bool CML::InjectionModel<CloudType>::prepareForNextTimeStep
(
    const scalar time,
    label& newParcels,
    scalar& newVolume
)
{
    // Initialise values
    newParcels = 0;
    newVolume = 0.0;
    bool validInjection = false;

    // Return if not started injection event
    if (time < SOI_)
    {
        timeStep0_ = time;
        return validInjection;
    }

    // Make times relative to SOI
    scalar t0 = timeStep0_ - SOI_;
    scalar t1 = time - SOI_;

    // Number of parcels to inject
    newParcels = this->parcelsToInject(t0, t1);

    // Volume of parcels to inject
    newVolume = this->volumeToInject(t0, t1);

    if (newVolume > 0)
    {
        if (newParcels > 0)
        {
            timeStep0_ = time;
            validInjection = true;
        }
        else
        {
            // injection should have started, but not sufficient volume to
            // produce (at least) 1 parcel - hold value of timeStep0_
            validInjection = false;
        }
    }
    else
    {
        timeStep0_ = time;
        validInjection = false;
    }

    return validInjection;
}


template<class CloudType>
bool CML::InjectionModel<CloudType>::findCellAtPosition
(
    label& cellI,
    label& tetFaceI,
    label& tetPtI,
    vector& position,
    bool errorOnNotFound
)
{
    const volVectorField& cellCentres = this->owner().mesh().C();

    const vector p0 = position;

    this->owner().mesh().findCellFacePt
    (
        position,
        cellI,
        tetFaceI,
        tetPtI
    );

    label procI = -1;

    if (cellI >= 0)
    {
        procI = Pstream::myProcNo();
    }

    reduce(procI, maxOp<label>());

    // Ensure that only one processor attempts to insert this Parcel

    if (procI != Pstream::myProcNo())
    {
        cellI = -1;
        tetFaceI = -1;
        tetPtI = -1;
    }

    // Last chance - find nearest cell and try that one - the point is
    // probably on an edge
    if (procI == -1)
    {
        cellI = this->owner().mesh().findNearestCell(position);

        if (cellI >= 0)
        {
            position += SMALL*(cellCentres[cellI] - position);

            if (this->owner().mesh().pointInCell(position, cellI))
            {
                procI = Pstream::myProcNo();
            }
        }

        reduce(procI, maxOp<label>());

        if (procI != Pstream::myProcNo())
        {
            cellI = -1;
            tetFaceI = -1;
            tetPtI = -1;
        }
    }

    if (procI == -1)
    {
        if (errorOnNotFound)
        {
            FatalErrorIn
            (
                "CML::InjectionModel<CloudType>::findCellAtPosition"
                "("
                    "label&, "
                    "label&, "
                    "label&, "
                    "vector&, "
                    "bool"
                ")"
            )   << "Cannot find parcel injection cell. "
                << "Parcel position = " << p0 << nl
                << abort(FatalError);
        }
        else
        {
            return false;
        }
    }

    return true;
}


template<class CloudType>
CML::scalar CML::InjectionModel<CloudType>::setNumberOfParticles
(
    const label parcels,
    const scalar volume,
    const scalar diameter,
    const scalar rho
)
{
    scalar nP = 0.0;
    switch (parcelBasis_)
    {
        case pbMass:
        {
            nP =
                volume/volumeTotal_
               *massTotal_/rho
               /(parcels*pi/6.0*pow3(diameter));
            break;
        }
        case pbNumber:
        {
            nP = massTotal_/(rho*volumeTotal_);
            break;
        }
        case pbFixed:
        {
            nP = nParticleFixed_;
            break;
        }
        default:
        {
            nP = 0.0;
            FatalErrorIn
            (
                "CML::scalar "
                "CML::InjectionModel<CloudType>::setNumberOfParticles"
                "("
                    "const label, "
                    "const scalar, "
                    "const scalar, "
                    "const scalar"
                ")"
            )<< "Unknown parcelBasis type" << nl
             << exit(FatalError);
        }
    }

    return nP;
}


template<class CloudType>
void CML::InjectionModel<CloudType>::postInjectCheck
(
    const label parcelsAdded,
    const scalar massAdded
)
{
    const label allParcelsAdded = returnReduce(parcelsAdded, sumOp<label>());

    if (allParcelsAdded > 0)
    {
        Info<< nl
            << "--> Cloud: " << this->owner().name() << nl
            << "    Added " << allParcelsAdded << " new parcels" << nl << endl;
    }

    // Increment total number of parcels added
    parcelsAddedTotal_ += allParcelsAdded;

    // Increment total mass injected
    massInjected_ += returnReduce(massAdded, sumOp<scalar>());

    // Update time for start of next injection
    time0_ = this->owner().db().time().value();

    // Increment number of injections
    nInjections_++;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::InjectionModel<CloudType>::InjectionModel(CloudType& owner)
:
    SubModelBase<CloudType>(owner),
    SOI_(0.0),
    volumeTotal_(0.0),
    massTotal_(0.0),
    massInjected_(this->template getBaseProperty<scalar>("massInjected")),
    nInjections_(this->template getBaseProperty<scalar>("nInjections")),
    parcelsAddedTotal_
    (
        this->template getBaseProperty<scalar>("parcelsAddedTotal")
    ),
    parcelBasis_(pbNumber),
    nParticleFixed_(0.0),
    time0_(0.0),
    timeStep0_(this->template getBaseProperty<scalar>("timeStep0")),
    delayedVolume_(0.0)
{}


template<class CloudType>
CML::InjectionModel<CloudType>::InjectionModel
(
    const dictionary& dict,
    CloudType& owner,
    const word& type
)
:
    SubModelBase<CloudType>(owner, dict, typeName, type),
    SOI_(0.0),
    volumeTotal_(0.0),
    massTotal_(0.0),
    massInjected_(this->template getBaseProperty<scalar>("massInjected")),
    nInjections_(this->template getBaseProperty<scalar>("nInjections")),
    parcelsAddedTotal_
    (
        this->template getBaseProperty<scalar>("parcelsAddedTotal")
    ),
    parcelBasis_(pbNumber),
    nParticleFixed_(0.0),
    time0_(owner.db().time().value()),
    timeStep0_(this->template getBaseProperty<scalar>("timeStep0")),
    delayedVolume_(0.0)
{
    // Provide some info
    // - also serves to initialise mesh dimensions - needed for parallel runs
    //   due to lazy evaluation of valid mesh dimensions
    Info<< "    Constructing " << owner.mesh().nGeometricD() << "-D injection"
        << endl;

    if (owner.solution().transient())
    {
        this->coeffDict().lookup("massTotal") >> massTotal_;
        this->coeffDict().lookup("SOI") >> SOI_;
        SOI_ = owner.db().time().userTimeToTime(SOI_);
    }
    else
    {
        this->coeffDict().lookup("massFlowRate") >> massTotal_;
    }

    const word parcelBasisType = this->coeffDict().lookup("parcelBasisType");

    if (parcelBasisType == "mass")
    {
        parcelBasis_ = pbMass;
    }
    else if (parcelBasisType == "number")
    {
        parcelBasis_ = pbNumber;
    }
    else if (parcelBasisType == "fixed")
    {
        parcelBasis_ = pbFixed;

        Info<< "    Choosing nParticle to be a fixed value, massTotal "
            << "variable now does not determine anything."
            << endl;

        nParticleFixed_ = readScalar(this->coeffDict().lookup("nParticle"));
    }
    else
    {
        FatalErrorIn
        (
            "CML::InjectionModel<CloudType>::InjectionModel"
            "("
                "const dictionary&, "
                "CloudType&, "
                "const word&"
            ")"
        )<< "parcelBasisType must be either 'number', 'mass' or 'fixed'" << nl
         << exit(FatalError);
    }
}


template<class CloudType>
CML::InjectionModel<CloudType>::InjectionModel
(
    const InjectionModel<CloudType>& im
)
:
    SubModelBase<CloudType>(im),
    SOI_(im.SOI_),
    volumeTotal_(im.volumeTotal_),
    massTotal_(im.massTotal_),
    massInjected_(im.massInjected_),
    nInjections_(im.nInjections_),
    parcelsAddedTotal_(im.parcelsAddedTotal_),
    parcelBasis_(im.parcelBasis_),
    nParticleFixed_(im.nParticleFixed_),
    time0_(im.time0_),
    timeStep0_(im.timeStep0_),
    delayedVolume_(im.delayedVolume_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::InjectionModel<CloudType>::~InjectionModel()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::scalar CML::InjectionModel<CloudType>::timeEnd() const
{
    notImplemented
    (
        "CML::scalar CML::InjectionModel<CloudType>::timeEnd() const"
    );

    return 0.0;
}


template<class CloudType>
CML::label CML::InjectionModel<CloudType>::parcelsToInject
(
    const scalar time0,
    const scalar time1
)
{
    notImplemented
    (
        "CML::label CML::InjectionModel<CloudType>::parcelsToInject"
        "("
            "const scalar, "
            "const scalar"
        ") const"
    );

    return 0;
}


template<class CloudType>
CML::scalar CML::InjectionModel<CloudType>::volumeToInject
(
    const scalar time0,
    const scalar time1
)
{
    notImplemented
    (
        "CML::scalar CML::InjectionModel<CloudType>::volumeToInject"
        "("
            "const scalar, "
            "const scalar"
        ") const"
    );

    return 0.0;
}


template<class CloudType>
CML::scalar CML::InjectionModel<CloudType>::averageParcelMass()
{
    label nTotal = parcelsToInject(0.0, timeEnd() - timeStart());
    return massTotal_/nTotal;
}


template<class CloudType>
template<class TrackData>
void CML::InjectionModel<CloudType>::inject(TrackData& td)
{
    if (!this->active())
    {
        return;
    }

    const scalar time = this->owner().db().time().value();

    label parcelsAdded = 0;
    scalar massAdded = 0.0;

    label newParcels = 0;
    scalar newVolume = 0.0;

    if (prepareForNextTimeStep(time, newParcels, newVolume))
    {
        newVolume += delayedVolume_;
        scalar delayedVolume = 0;

        const scalar trackTime = this->owner().solution().trackTime();
        const polyMesh& mesh = this->owner().mesh();
        typename TrackData::cloudType& cloud = td.cloud();

        // Duration of injection period during this timestep
        const scalar deltaT =
            max(0.0, min(trackTime, min(time - SOI_, timeEnd() - time0_)));

        // Pad injection time if injection starts during this timestep
        const scalar padTime = max(0.0, SOI_ - time0_);

        // Introduce new parcels linearly across carrier phase timestep
        for (label parcelI = 0; parcelI < newParcels; parcelI++)
        {
            if (validInjection(parcelI))
            {
                // Calculate the pseudo time of injection for parcel 'parcelI'
                scalar timeInj = time0_ + padTime + deltaT*parcelI/newParcels;

                // Determine the injection position and owner cell,
                // tetFace and tetPt
                label cellI = -1;
                label tetFaceI = -1;
                label tetPtI = -1;

                vector pos = vector::zero;

                setPositionAndCell
                (
                    parcelI,
                    newParcels,
                    timeInj,
                    pos,
                    cellI,
                    tetFaceI,
                    tetPtI
                );

                if (cellI > -1)
                {
                    // Lagrangian timestep
                    scalar dt = time - timeInj;

                    // Apply corrections to position for 2-D cases
                    meshTools::constrainToMeshCentre(mesh, pos);

                    // Create a new parcel
                    parcelType* pPtr = new parcelType
                    (
                        mesh,
                        pos,
                        cellI,
                        tetFaceI,
                        tetPtI
                    );

                    // Check/set new parcel thermo properties
                    cloud.setParcelThermoProperties(*pPtr, dt);

                    // Assign new parcel properties in injection model
                    setProperties(parcelI, newParcels, timeInj, *pPtr);

                    // Check/set new parcel injection properties
                    cloud.checkParcelProperties(*pPtr, dt, fullyDescribed());

                    // Apply correction to velocity for 2-D cases
                    meshTools::constrainDirection
                    (
                        mesh,
                        mesh.solutionD(),
                        pPtr->U()
                    );

                    // Number of particles per parcel
                    pPtr->nParticle() =
                        setNumberOfParticles
                        (
                            newParcels,
                            newVolume,
                            pPtr->d(),
                            pPtr->rho()
                        );

                    if ((pPtr->nParticle() >= 1.0) && (pPtr->move(td, dt)))
                    {
                        td.cloud().addParticle(pPtr);
                        massAdded += pPtr->nParticle()*pPtr->mass();
                        parcelsAdded++;
                    }
                    else
                    {
                        delayedVolume += pPtr->nParticle()*pPtr->volume();
                        delete pPtr;
                    }
                }
            }
        }

        delayedVolume_ = delayedVolume;
    }

    postInjectCheck(parcelsAdded, massAdded);
}


template<class CloudType>
template<class TrackData>
void CML::InjectionModel<CloudType>::injectSteadyState
(
    TrackData& td,
    const scalar trackTime
)
{
    if (!this->active())
    {
        return;
    }

    const polyMesh& mesh = this->owner().mesh();
    typename TrackData::cloudType& cloud = td.cloud();

    // Reset counters
    time0_ = 0.0;
    label parcelsAdded = 0;
    scalar massAdded = 0.0;

    // Set number of new parcels to inject based on first second of injection
    label newParcels = parcelsToInject(0.0, 1.0);

    // Inject new parcels
    for (label parcelI = 0; parcelI < newParcels; parcelI++)
    {
        // Volume to inject is split equally amongst all parcel streams
        scalar newVolume = volumeTotal_/newParcels;

        // Determine the injection position and owner cell,
        // tetFace and tetPt
        label cellI = -1;
        label tetFaceI = -1;
        label tetPtI = -1;

        vector pos = vector::zero;

        setPositionAndCell
        (
            parcelI,
            newParcels,
            0.0,
            pos,
            cellI,
            tetFaceI,
            tetPtI
        );

        if (cellI > -1)
        {
            // Apply corrections to position for 2-D cases
            meshTools::constrainToMeshCentre(mesh, pos);

            // Create a new parcel
            parcelType* pPtr = new parcelType
            (
                mesh,
                pos,
                cellI,
                tetFaceI,
                tetPtI
            );

            // Check/set new parcel thermo properties
            cloud.setParcelThermoProperties(*pPtr, 0.0);

            // Assign new parcel properties in injection model
            setProperties(parcelI, newParcels, 0.0, *pPtr);

            // Check/set new parcel injection properties
            cloud.checkParcelProperties(*pPtr, 0.0, fullyDescribed());

            // Apply correction to velocity for 2-D cases
            meshTools::constrainDirection
            (
                mesh,
                mesh.solutionD(),
                pPtr->U()
            );

            // Number of particles per parcel
            pPtr->nParticle() =
                setNumberOfParticles
                (
                    1,
                    newVolume,
                    pPtr->d(),
                    pPtr->rho()
                );

            // Add the new parcel
            td.cloud().addParticle(pPtr);

            massAdded += pPtr->nParticle()*pPtr->mass();
            parcelsAdded++;
        }
    }

    postInjectCheck(parcelsAdded, massAdded);
}


template<class CloudType>
void CML::InjectionModel<CloudType>::setPositionAndCell
(
    const label parcelI,
    const label nParcels,
    const scalar time,
    vector& position,
    label& cellOwner,
    label& tetFaceI,
    label& tetPtI
)
{
    notImplemented
    (
        "void CML::InjectionModel<CloudType>::setPositionAndCell"
        "("
            "const label, "
            "const label, "
            "const scalar, "
            "vector&, "
            "label&, "
            "label&, "
            "label&"
        ")"
    );
}


template<class CloudType>
void CML::InjectionModel<CloudType>::setProperties
(
    const label parcelI,
    const label nParcels,
    const scalar time,
    typename CloudType::parcelType& parcel
)
{
    notImplemented
    (
        "void CML::InjectionModel<CloudType>::setProperties"
        "("
            "const label, "
            "const label, "
            "const scalar, "
            "typename CloudType::parcelType&"
        ")"
    );
}


template<class CloudType>
bool CML::InjectionModel<CloudType>::fullyDescribed() const
{
    notImplemented
    (
        "bool CML::InjectionModel<CloudType>::fullyDescribed() const"
    );

    return false;
}


template<class CloudType>
void CML::InjectionModel<CloudType>::info(Ostream& os)
{
    os  << "    Total number of parcels added   = " << parcelsAddedTotal_ << nl
        << "    Total mass introduced           = " << massInjected_ << nl;

    if
    (
        this->owner().solution().transient()
     && this->owner().db().time().outputTime()
    )
    {
        this->setBaseProperty("massInjected", massInjected_);
        this->setBaseProperty("nInjections", nInjections_);
        this->setBaseProperty("parcelsAddedTotal", parcelsAddedTotal_);
        this->setBaseProperty("timeStep0", timeStep0_);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::autoPtr<CML::InjectionModel<CloudType> >
CML::InjectionModel<CloudType>::New
(
    const dictionary& dict,
    CloudType& owner
)
{
    const word modelType(dict.lookup("injectionModel"));

    Info<< "Selecting injection model " << modelType << endl;

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(modelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "InjectionModel<CloudType>::New"
            "("
                "const dictionary&, "
                "CloudType&"
            ")"
        )   << "Unknown injection model type "
            << modelType << nl << nl
            << "Valid injection model types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc() << exit(FatalError);
    }

    return autoPtr<InjectionModel<CloudType> >(cstrIter()(dict, owner));
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
