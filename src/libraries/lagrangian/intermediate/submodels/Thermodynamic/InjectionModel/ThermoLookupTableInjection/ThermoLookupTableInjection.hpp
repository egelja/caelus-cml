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
    CML::ThermoLookupTableInjection

Description
    Particle injection sources read from look-up table. Each row corresponds to
    an injection site.

    (
        (x y z) (u v w) d rho mDot T cp  // injector 1
        (x y z) (u v w) d rho mDot T cp  // injector 2
        ...
        (x y z) (u v w) d rho mDot T cp  // injector N
    );

    where:
        x, y, z = global cartesian co-ordinates [m]
        u, v, w = global cartesian velocity components [m/s]
        d       = diameter [m]
        rho     = density [kg/m3]
        mDot    = mass flow rate [kg/s]
        T       = temperature [K]
        cp      = specific heat capacity [J/kg/K]


\*---------------------------------------------------------------------------*/

#ifndef ThermoLookupTableInjection_H
#define ThermoLookupTableInjection_H

#include "InjectionModel.hpp"
#include "kinematicParcelInjectionDataIOList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                Class ThermoLookupTableInjection Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ThermoLookupTableInjection
:
    public InjectionModel<CloudType>
{
    // Private data

        //- Name of file containing injector/parcel data
        const word inputFileName_;

        //- Injection duration - common to all injection sources
        scalar duration_;

        //- Number of parcels per injector - common to all injection sources
        const scalar parcelsPerSecond_;

        //- Flag to indicate to randomise injection positions
        bool randomise_;

        //- List of injectors
        kinematicParcelInjectionDataIOList injectors_;

        //- List of cell labels corresoponding to injector positions
        labelList injectorCells_;

        //- List of tetFace labels corresoponding to injector positions
        labelList injectorTetFaces_;

        //- List of tetPt labels corresoponding to injector positions
        labelList injectorTetPts_;


public:

    //- Runtime type information
    TypeName("thermoLookupTableInjection");


    // Constructors

        //- Construct from dictionary
        ThermoLookupTableInjection
        (
            const dictionary& dict,
            CloudType& owner,
            const word& modelName
        );

        //- Construct copy from owner cloud and injection model
        ThermoLookupTableInjection
        (
            CloudType& owner,
            const ThermoLookupTableInjection<CloudType>& im
        );

        //- Construct and return a clone using supplied owner cloud
        virtual autoPtr<InjectionModel<CloudType> > clone() const
        {
            return autoPtr<InjectionModel<CloudType> >
            (
                new ThermoLookupTableInjection<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~ThermoLookupTableInjection();


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
            virtual bool fullyDescribed() const
            {
                return true;
            }

            //- Return flag to identify whether or not injection of parcelI is
            //  permitted
            virtual bool validInjection(const label parcelI);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ThermoLookupTableInjection<CloudType>::
ThermoLookupTableInjection
(
    const dictionary& dict,
    CloudType& owner,
    const word& modelName
)
:
    InjectionModel<CloudType>(dict, owner, modelName, typeName),
    inputFileName_(this->coeffDict().lookup("inputFile")),
    duration_(readScalar(this->coeffDict().lookup("duration"))),
    parcelsPerSecond_
    (
        readScalar(this->coeffDict().lookup("parcelsPerSecond"))
    ),
    randomise_(readBool(this->coeffDict().lookup("randomise"))),
    injectors_
    (
        IOobject
        (
            inputFileName_,
            owner.db().time().constant(),
            owner.db(),
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
    injectorCells_(0),
    injectorTetFaces_(0),
    injectorTetPts_(0)
{
    duration_ = owner.db().time().userTimeToTime(duration_);

    // Set/cache the injector cells
    injectorCells_.setSize(injectors_.size());
    injectorTetFaces_.setSize(injectors_.size());
    injectorTetPts_.setSize(injectors_.size());

    updateMesh();

    // Determine volume of particles to inject
    this->volumeTotal_ = 0.0;
    forAll(injectors_, i)
    {
        this->volumeTotal_ += injectors_[i].mDot()/injectors_[i].rho();
    }
    this->volumeTotal_ *= duration_;
}


template<class CloudType>
CML::ThermoLookupTableInjection<CloudType>::ThermoLookupTableInjection
(
    const ThermoLookupTableInjection<CloudType>& im
)
:
    InjectionModel<CloudType>(im),
    inputFileName_(im.inputFileName_),
    duration_(im.duration_),
    parcelsPerSecond_(im.parcelsPerSecond_),
    randomise_(im.randomise_),
    injectors_(im.injectors_),
    injectorCells_(im.injectorCells_),
    injectorTetFaces_(im.injectorTetFaces_),
    injectorTetPts_(im.injectorTetPts_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::ThermoLookupTableInjection<CloudType>::~ThermoLookupTableInjection()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::ThermoLookupTableInjection<CloudType>::updateMesh()
{
    // Set/cache the injector cells
    forAll(injectors_, i)
    {
        this->findCellAtPosition
        (
            injectorCells_[i],
            injectorTetFaces_[i],
            injectorTetPts_[i],
            injectors_[i].x()
        );
    }
}


template<class CloudType>
CML::scalar CML::ThermoLookupTableInjection<CloudType>::timeEnd() const
{
    return this->SOI_ + duration_;
}


template<class CloudType>
CML::label CML::ThermoLookupTableInjection<CloudType>::parcelsToInject
(
    const scalar time0,
    const scalar time1
)
{
    if ((time0 >= 0.0) && (time0 < duration_))
    {
        return floor(injectorCells_.size()*(time1 - time0)*parcelsPerSecond_);
    }
    else
    {
        return 0;
    }
}


template<class CloudType>
CML::scalar CML::ThermoLookupTableInjection<CloudType>::volumeToInject
(
    const scalar time0,
    const scalar time1
)
{
    scalar volume = 0.0;
    if ((time0 >= 0.0) && (time0 < duration_))
    {
        forAll(injectors_, i)
        {
            volume += injectors_[i].mDot()/injectors_[i].rho()*(time1 - time0);
        }
    }

    return volume;
}


template<class CloudType>
void CML::ThermoLookupTableInjection<CloudType>::setPositionAndCell
(
    const label parcelI,
    const label nParcels,
    const scalar time,
    vector& position,
    label& cellOwner,
    label& tetFacei,
    label& tetPti
)
{
    label injectorI = 0;
    if (randomise_)
    {
        Random& rnd = this->owner().rndGen();
        injectorI = rnd.sampleAB<label>(0, injectorCells_.size());
    }
    else
    {
        injectorI = parcelI*injectorCells_.size()/nParcels;
    }

    position = injectors_[injectorI].x();
    cellOwner = injectorCells_[injectorI];
    tetFacei = injectorTetFaces_[injectorI];
    tetPti = injectorTetPts_[injectorI];
}


template<class CloudType>
void CML::ThermoLookupTableInjection<CloudType>::setProperties
(
    const label parcelI,
    const label nParcels,
    const scalar,
    typename CloudType::parcelType* pPtr
)
{
    label injectorI = parcelI*injectorCells_.size()/nParcels;

    // set particle velocity
    parcel.U() = injectors_[injectorI].U();

    // set particle diameter
    parcel.d() = injectors_[injectorI].d();

    // set particle density
    parcel.rho() = injectors_[injectorI].rho();

    // set particle temperature
    parcel.T() = injectors_[injectorI].T();

    // set particle specific heat capacity
    parcel.cp() = injectors_[injectorI].cp();
}


template<class CloudType>
bool CML::ThermoLookupTableInjection<CloudType>::fullyDescribed() const
{
    return true;
}


template<class CloudType>
bool CML::ThermoLookupTableInjection<CloudType>::validInjection(const label)
{
    return true;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
