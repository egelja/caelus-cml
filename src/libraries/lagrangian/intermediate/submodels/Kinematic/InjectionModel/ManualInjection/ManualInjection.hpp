/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011 OpenFOAM Foundation
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
    CML::ManualInjection

Description
    Manual injection.

    User specifies:
      - Total mass to inject
      - Parcel positions in file \c positionsFile
      - Initial parcel velocity

    Properties:
      - Parcel diameters obtained by distribution model
      - All parcels introduced at SOI


\*---------------------------------------------------------------------------*/

#ifndef ManualInjection_H
#define ManualInjection_H

#include "InjectionModel.hpp"
#include "distributionModel.hpp"
#include "mathematicalConstants.hpp"
#include "PackedBoolList.hpp"
#include "Switch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class ManualInjection Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ManualInjection
:
    public InjectionModel<CloudType>
{
    // Private data

        //- Name of file containing positions data
        const word positionsFile_;

        //- Field of parcel positions
        vectorIOField positions_;

        //- Field of parcel diameters
        scalarList diameters_;

        //- List of cell labels corresponding to injector positions
        labelList injectorCells_;

        //- List of tetFace labels corresponding to injector positions
        labelList injectorTetFaces_;

        //- List of tetPt labels corresponding to injector positions
        labelList injectorTetPts_;

        //- Initial parcel velocity
        const vector U0_;

        //- Parcel size distribution model
        const autoPtr<distributionModel> sizeDistribution_;

        //- Flag to suppress errors if particle injection site is out-of-bounds
        Switch ignoreOutOfBounds_;


public:

    //- Runtime type information
    TypeName("manualInjection");


    // Constructors

        //- Construct from dictionary
        ManualInjection
        (
            const dictionary& dict,
            CloudType& owner,
            const word& modelName
        );

        //- Construct copy
        ManualInjection(const ManualInjection<CloudType>& im);

        //- Construct and return a clone
        virtual autoPtr<InjectionModel<CloudType> > clone() const
        {
            return autoPtr<InjectionModel<CloudType> >
            (
                new ManualInjection<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~ManualInjection();


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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ManualInjection<CloudType>::ManualInjection
(
    const dictionary& dict,
    CloudType& owner,
    const word& modelName
)
:
    InjectionModel<CloudType>(dict, owner, modelName, typeName),
    positionsFile_(this->coeffDict().lookup("positionsFile")),
    positions_
    (
        IOobject
        (
            positionsFile_,
            owner.db().time().constant(),
            owner.mesh(),
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
    diameters_(positions_.size()),
    injectorCells_(positions_.size(), -1),
    injectorTetFaces_(positions_.size(), -1),
    injectorTetPts_(positions_.size(), -1),
    U0_(this->coeffDict().lookup("U0")),
    sizeDistribution_
    (
        distributionModel::New
        (
            this->coeffDict().subDict("sizeDistribution"),
            owner.rndGen()
        )
    ),
    ignoreOutOfBounds_
    (
        this->coeffDict().lookupOrDefault("ignoreOutOfBounds", false)
    )
{
    updateMesh();

    // Construct parcel diameters
    forAll(diameters_, i)
    {
        diameters_[i] = sizeDistribution_->sample();
    }

    // Determine volume of particles to inject
    this->volumeTotal_ = sum(pow3(diameters_))*pi/6.0;
}


template<class CloudType>
CML::ManualInjection<CloudType>::ManualInjection
(
    const ManualInjection<CloudType>& im
)
:
    InjectionModel<CloudType>(im),
    positionsFile_(im.positionsFile_),
    positions_(im.positions_),
    diameters_(im.diameters_),
    injectorCells_(im.injectorCells_),
    injectorTetFaces_(im.injectorTetFaces_),
    injectorTetPts_(im.injectorTetPts_),
    U0_(im.U0_),
    sizeDistribution_(im.sizeDistribution_().clone().ptr()),
    ignoreOutOfBounds_(im.ignoreOutOfBounds_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::ManualInjection<CloudType>::~ManualInjection()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::ManualInjection<CloudType>::updateMesh()
{
    label nRejected = 0;

    PackedBoolList keep(positions_.size(), true);

    forAll(positions_, pI)
    {
        if
        (
            !this->findCellAtPosition
            (
                injectorCells_[pI],
                injectorTetFaces_[pI],
                injectorTetPts_[pI],
                positions_[pI],
                !ignoreOutOfBounds_
            )
        )
        {
            keep[pI] = false;
            nRejected++;
        }
    }

    if (nRejected > 0)
    {
        inplaceSubset(keep, positions_);
        inplaceSubset(keep, diameters_);
        inplaceSubset(keep, injectorCells_);
        inplaceSubset(keep, injectorTetFaces_);
        inplaceSubset(keep, injectorTetPts_);

        Info<< "    " << nRejected
            << " particles ignored, out of bounds" << endl;
    }
}


template<class CloudType>
CML::scalar CML::ManualInjection<CloudType>::timeEnd() const
{
    // Injection is instantaneous - but allow for a finite interval to
    // avoid numerical issues when interval is zero
    return ROOTVSMALL;
}


template<class CloudType>
CML::label CML::ManualInjection<CloudType>::parcelsToInject
(
    const scalar time0,
    const scalar time1
)
{
    if ((0.0 >= time0) && (0.0 < time1))
    {
        return positions_.size();
    }
    else
    {
        return 0;
    }
}


template<class CloudType>
CML::scalar CML::ManualInjection<CloudType>::volumeToInject
(
    const scalar time0,
    const scalar time1
)
{
    // All parcels introduced at SOI
    if ((0.0 >= time0) && (0.0 < time1))
    {
        return this->volumeTotal_;
    }
    else
    {
        return 0.0;
    }
}


template<class CloudType>
void CML::ManualInjection<CloudType>::setPositionAndCell
(
    const label parcelI,
    const label,
    const scalar,
    vector& position,
    label& cellOwner,
    label& tetFacei,
    label& tetPti
)
{
    position = positions_[parcelI];
    cellOwner = injectorCells_[parcelI];
    tetFacei = injectorTetFaces_[parcelI];
    tetPti = injectorTetPts_[parcelI];
}


template<class CloudType>
void CML::ManualInjection<CloudType>::setProperties
(
    const label parcelI,
    const label,
    const scalar,
    typename CloudType::parcelType& parcel
)
{
    // set particle velocity
    parcel.U() = U0_;

    // set particle diameter
    parcel.d() = diameters_[parcelI];
}


template<class CloudType>
bool CML::ManualInjection<CloudType>::fullyDescribed() const
{
    return false;
}


template<class CloudType>
bool CML::ManualInjection<CloudType>::validInjection(const label)
{
    return true;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
