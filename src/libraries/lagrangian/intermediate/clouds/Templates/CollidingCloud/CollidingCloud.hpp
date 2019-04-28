/*---------------------------------------------------------------------------*\
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
    CML::CollidingCloud

Description
    Adds coolisions to kinematic clouds


\*---------------------------------------------------------------------------*/

#ifndef CollidingCloud_H
#define CollidingCloud_H

#include "particle.hpp"
#include "Cloud.hpp"
#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "fvMesh.hpp"
#include "volFields.hpp"
#include "CollisionModel.hpp"
#include "NoCollision.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes

template<class CloudType>
class CollisionModel;

/*---------------------------------------------------------------------------*\
                       Class CollidingCloud Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class CollidingCloud
:
    public CloudType
{
public:

    // Public typedefs

        //- Type of cloud this cloud was instantiated for
        typedef CloudType cloudType;

        //- Type of parcel the cloud was instantiated for
        typedef typename CloudType::particleType parcelType;

        //- Convenience typedef for this cloud type
        typedef CollidingCloud<CloudType> collidingCloudType;


private:

    // Private data

        //- Cloud copy pointer
        autoPtr<CollidingCloud<CloudType> > cloudCopyPtr_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        CollidingCloud(const CollidingCloud&);

        //- Disallow default bitwise assignment
        void operator=(const CollidingCloud&);


protected:

    // Protected data

        //- Thermo parcel constant properties
        typename parcelType::constantProperties constProps_;


        // References to the cloud sub-models

            //- Collision model
            autoPtr<CollisionModel<CollidingCloud<CloudType> > >
                collisionModel_;


        // Initialisation

            //- Set cloud sub-models
            void setModels();


        // Cloud evolution functions

            //- Move-collide particles
            template<class TrackCloudType>
            void moveCollide
            (
                TrackCloudType& cloud,
                typename parcelType::trackingData& td,
                const scalar deltaT
            );

            //- Reset state of cloud
            void cloudReset(CollidingCloud<CloudType>& c);


public:

    // Constructors

        //- Construct given carrier gas fields
        CollidingCloud
        (
            const word& cloudName,
            const volScalarField& rho,
            const volVectorField& U,
            const volScalarField& mu,
            const dimensionedVector& g,
            bool readFields = true
        );

        //- Copy constructor with new name
        CollidingCloud
        (
            CollidingCloud<CloudType>& c,
            const word& name
        );

        //- Copy constructor with new name - creates bare cloud
        CollidingCloud
        (
            const fvMesh& mesh,
            const word& name,
            const CollidingCloud<CloudType>& c
        );

        //- Construct and return clone based on (this) with new name
        virtual autoPtr<Cloud<parcelType> > clone(const word& name)
        {
            return autoPtr<Cloud<parcelType> >
            (
                new CollidingCloud(*this, name)
            );
        }

        //- Construct and return bare clone based on (this) with new name
        virtual autoPtr<Cloud<parcelType> > cloneBare(const word& name) const
        {
            return autoPtr<Cloud<parcelType> >
            (
                new CollidingCloud(this->mesh(), name, *this)
            );
        }


    //- Destructor
    virtual ~CollidingCloud();


    // Member Functions

        // Access

            //- Return a reference to the cloud copy
            inline const CollidingCloud& cloudCopy() const;

            //- Return the constant properties
            inline const typename parcelType::constantProperties&
                constProps() const;


            // Sub-models

                //- Return const access to the collision model
                inline const CollisionModel<CollidingCloud<CloudType> >&
                    collision() const;

                //- Return reference to the collision model
                inline CollisionModel<CollidingCloud<CloudType> >&
                    collision();

        // Check

            //- Total rotational kinetic energy in the system
            inline scalar rotationalKineticEnergyOfSystem() const;


        // Cloud evolution functions

            //- Store the current cloud state
            void storeState();

            //- Reset the current cloud to the previously stored state
            void restoreState();

            //- Evolve the cloud
            void evolve();

            //- Particle motion
            template<class TrackCloudType>
            void motion
            (
                TrackCloudType& cloud,
                typename parcelType::trackingData& td
            );


        // I-O

            //- Print cloud information
            void info();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
inline const CML::CollidingCloud<CloudType>&
CML::CollidingCloud<CloudType>::cloudCopy() const
{
    return cloudCopyPtr_();
}


template<class CloudType>
inline const typename CloudType::particleType::constantProperties&
CML::CollidingCloud<CloudType>::constProps() const
{
    return constProps_;
}


template<class CloudType>
inline const CML::CollisionModel<CML::CollidingCloud<CloudType> >&
CML::CollidingCloud<CloudType>::collision() const
{
    return collisionModel_();
}


template<class CloudType>
inline CML::CollisionModel<CML::CollidingCloud<CloudType> >&
CML::CollidingCloud<CloudType>::collision()
{
    return collisionModel_();
}


template<class CloudType>
inline CML::scalar
CML::CollidingCloud<CloudType>::rotationalKineticEnergyOfSystem() const
{
    scalar rotationalKineticEnergy = 0.0;

    forAllConstIter(typename CollidingCloud<CloudType>, *this, iter)
    {
        const parcelType& p = iter();

        rotationalKineticEnergy +=
            p.nParticle()*0.5*p.momentOfInertia()*(p.omega() & p.omega());
    }

    return rotationalKineticEnergy;
}


// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

template<class CloudType>
void CML::CollidingCloud<CloudType>::setModels()
{
    collisionModel_.reset
    (
        CollisionModel<CollidingCloud<CloudType> >::New
        (
            this->subModelProperties(),
            *this
        ).ptr()
    );
}


template<class CloudType>
template<class TrackCloudType>
void  CML::CollidingCloud<CloudType>::moveCollide
(
    TrackCloudType& cloud,
    typename parcelType::trackingData& td,
    const scalar deltaT
)
{
    td.part() = parcelType::trackingData::tpVelocityHalfStep;
    CloudType::move(cloud, td, deltaT);

    td.part() = parcelType::trackingData::tpLinearTrack;
    CloudType::move(cloud, td, deltaT);

    // td.part() = parcelType::trackingData::tpRotationalTrack;
    // CloudType::move(cloud, td, deltaT);

    this->updateCellOccupancy();

    this->collision().collide();

    td.part() = parcelType::trackingData::tpVelocityHalfStep;
    CloudType::move(cloud, td, deltaT);
}



template<class CloudType>
void CML::CollidingCloud<CloudType>::cloudReset(CollidingCloud<CloudType>& c)
{
    CloudType::cloudReset(c);

    collisionModel_.reset(c.collisionModel_.ptr());
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::CollidingCloud<CloudType>::CollidingCloud
(
    const word& cloudName,
    const volScalarField& rho,
    const volVectorField& U,
    const volScalarField& mu,
    const dimensionedVector& g,
    bool readFields
)
:
    CloudType(cloudName, rho, U, mu, g, false),
    constProps_(this->particleProperties()),
    collisionModel_(nullptr)
{
    if (this->solution().active())
    {
        setModels();

        if (readFields)
        {
            parcelType::readFields(*this);
            this->deleteLostParticles();
        }

        if
        (
            this->solution().steadyState()
         && !isType<NoCollision<CollidingCloud<CloudType>>>(collisionModel_())
        )
        {
            FatalErrorInFunction
                << "Collision modelling not currently available "
                << "for steady state calculations" << exit(FatalError);
        }
    }
}


template<class CloudType>
CML::CollidingCloud<CloudType>::CollidingCloud
(
    CollidingCloud<CloudType>& c,
    const word& name
)
:
    CloudType(c, name),
    collisionModel_(c.collisionModel_->clone())
{}


template<class CloudType>
CML::CollidingCloud<CloudType>::CollidingCloud
(
    const fvMesh& mesh,
    const word& name,
    const CollidingCloud<CloudType>& c
)
:
    CloudType(mesh, name, c),
    collisionModel_(nullptr)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::CollidingCloud<CloudType>::~CollidingCloud()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::CollidingCloud<CloudType>::storeState()
{
    cloudCopyPtr_.reset
    (
        static_cast<CollidingCloud<CloudType>*>
        (
            clone(this->name() + "Copy").ptr()
        )
    );
}


template<class CloudType>
void CML::CollidingCloud<CloudType>::restoreState()
{
    cloudReset(cloudCopyPtr_());
    cloudCopyPtr_.clear();
}


template<class CloudType>
void CML::CollidingCloud<CloudType>::evolve()
{
    if (this->solution().canEvolve())
    {
        typename parcelType::trackingData td(*this);

        this->solve(*this, td);
    }
}


template<class CloudType>
template<class TrackCloudType>
void  CML::CollidingCloud<CloudType>::motion
(
    TrackCloudType& cloud,
    typename parcelType::trackingData& td
)
{
    // Sympletic leapfrog integration of particle forces:
    // + apply half deltaV with stored force
    // + move positions with new velocity
    // + calculate forces in new position
    // + apply half deltaV with new force

    label nSubCycles = collision().nSubCycles();

    if (nSubCycles > 1)
    {
        Info<< "    " << nSubCycles << " move-collide subCycles" << endl;

        subCycleTime moveCollideSubCycle
        (
            const_cast<Time&>(this->db().time()),
            nSubCycles
        );

        while(!(++moveCollideSubCycle).end())
        {
            moveCollide(cloud, td, this->db().time().deltaTValue());
        }

        moveCollideSubCycle.endSubCycle();
    }
    else
    {
        moveCollide(cloud, td, this->db().time().deltaTValue());
    }
}


template<class CloudType>
void CML::CollidingCloud<CloudType>::info()
{
    CloudType::info();

    scalar rotationalKineticEnergy = rotationalKineticEnergyOfSystem();
    reduce(rotationalKineticEnergy, sumOp<scalar>());

    Info<< "    Rotational kinetic energy       = "
        << rotationalKineticEnergy << nl;
}

#endif

// ************************************************************************* //
