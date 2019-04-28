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
    CML::FieldActivatedInjection

Description
    Injection at specified positions, with the conditions:

    For injection to be allowed
      \verbatim
      factor*referenceField[celli] >= thresholdField[celli]
      \endverbatim
      where:
        - \c referenceField is the field used to supply the look-up values
        - \c thresholdField supplies the values beyond which the injection is
            permitted.

    Limited to a user-supplied number of injections per injector location


\*---------------------------------------------------------------------------*/

#ifndef FieldActivatedInjection_H
#define FieldActivatedInjection_H

#include "InjectionModel.hpp"
#include "distributionModel.hpp"
#include "volFieldsFwd.hpp"
#include "volFields.hpp"
#include "mathematicalConstants.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class FieldActivatedInjection Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class FieldActivatedInjection
:
    public InjectionModel<CloudType>
{
    // Private data

        // Model parameters

            //- Factor to apply to reference field
            const scalar factor_;

            //- Reference field
            const volScalarField& referenceField_;

            //- Threshold field
            const volScalarField& thresholdField_;


        // Injector properties

            //- Name of file containing positions data
            const word positionsFile_;

            //- Field of injector (x,y,z) positions
            vectorIOField positions_;

            //- List of cell labels corresponding to injector positions
            labelList injectorCells_;

            //- List of tetFace labels corresponding to injector positions
            labelList injectorTetFaces_;

            //- List of tetPt labels corresponding to injector positions
            labelList injectorTetPts_;

            //- Number of parcels per injector
            const label nParcelsPerInjector_;

            //- List of number of parcels injected for each injector
            labelList nParcelsInjected_;


        // Parcel properties

            //- Initial parcel velocity
            const vector U0_;

            //- List of parcel diameters
            scalarList diameters_;

            //- Parcel size distribution model
            const autoPtr<distributionModel>
                sizeDistribution_;


public:

    //- Runtime type information
    TypeName("fieldActivatedInjection");


    // Constructors

        //- Construct from dictionary
        FieldActivatedInjection
        (
            const dictionary& dict,
            CloudType& owner,
            const word& modelName
        );

        //- Construct copy
        FieldActivatedInjection(const FieldActivatedInjection<CloudType>& im);

        //- Construct and return a clone
        virtual autoPtr<InjectionModel<CloudType> > clone() const
        {
            return autoPtr<InjectionModel<CloudType> >
            (
                new FieldActivatedInjection<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~FieldActivatedInjection();


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
CML::FieldActivatedInjection<CloudType>::FieldActivatedInjection
(
    const dictionary& dict,
    CloudType& owner,
    const word& modelName
)
:
    InjectionModel<CloudType>(dict, owner, modelName, typeName),
    factor_(readScalar(this->coeffDict().lookup("factor"))),
    referenceField_
    (
        owner.db().objectRegistry::template lookupObject<volScalarField>
        (
            this->coeffDict().lookup("referenceField")
        )
    ),
    thresholdField_
    (
        owner.db().objectRegistry::template lookupObject<volScalarField>
        (
            this->coeffDict().lookup("thresholdField")
        )
    ),
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
    injectorCells_(positions_.size()),
    injectorTetFaces_(positions_.size()),
    injectorTetPts_(positions_.size()),
    nParcelsPerInjector_
    (
        readLabel(this->coeffDict().lookup("parcelsPerInjector"))
    ),
    nParcelsInjected_(positions_.size(), 0),
    U0_(this->coeffDict().lookup("U0")),
    diameters_(positions_.size()),
    sizeDistribution_
    (
        distributionModel::New
        (
            this->coeffDict().subDict("sizeDistribution"),
            owner.rndGen()
        )
    )
{
    // Construct parcel diameters - one per injector cell
    forAll(diameters_, i)
    {
        diameters_[i] = sizeDistribution_->sample();
    }

    // Determine total volume of particles to inject
    this->volumeTotal_ =
        nParcelsPerInjector_*sum(pow3(diameters_))*pi/6.0;

    updateMesh();
}


template<class CloudType>
CML::FieldActivatedInjection<CloudType>::FieldActivatedInjection
(
    const FieldActivatedInjection<CloudType>& im
)
:
    InjectionModel<CloudType>(im),
    factor_(im.factor_),
    referenceField_(im.referenceField_),
    thresholdField_(im.thresholdField_),
    positionsFile_(im.positionsFile_),
    positions_(im.positions_),
    injectorCells_(im.injectorCells_),
    injectorTetFaces_(im.injectorTetFaces_),
    injectorTetPts_(im.injectorTetPts_),
    nParcelsPerInjector_(im.nParcelsPerInjector_),
    nParcelsInjected_(im.nParcelsInjected_),
    U0_(im.U0_),
    diameters_(im.diameters_),
    sizeDistribution_(im.sizeDistribution_().clone().ptr())
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::FieldActivatedInjection<CloudType>::~FieldActivatedInjection()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::FieldActivatedInjection<CloudType>::updateMesh()
{
    // Set/cache the injector cells
    forAll(positions_, i)
    {
        this->findCellAtPosition
        (
            injectorCells_[i],
            injectorTetFaces_[i],
            injectorTetPts_[i],
            positions_[i]
        );
    }
}


template<class CloudType>
CML::scalar CML::FieldActivatedInjection<CloudType>::timeEnd() const
{
    return GREAT;
}


template<class CloudType>
CML::label CML::FieldActivatedInjection<CloudType>::parcelsToInject
(
    const scalar time0,
    const scalar time1
)
{
    if (sum(nParcelsInjected_) < nParcelsPerInjector_*positions_.size())
    {
        return positions_.size();
    }
    else
    {
        return 0;
    }
}


template<class CloudType>
CML::scalar CML::FieldActivatedInjection<CloudType>::volumeToInject
(
    const scalar time0,
    const scalar time1
)
{
    if (sum(nParcelsInjected_) < nParcelsPerInjector_*positions_.size())
    {
        return this->volumeTotal_/nParcelsPerInjector_;
    }
    else
    {
        return 0;
    }
}


template<class CloudType>
void CML::FieldActivatedInjection<CloudType>::setPositionAndCell
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
void CML::FieldActivatedInjection<CloudType>::setProperties
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
bool CML::FieldActivatedInjection<CloudType>::fullyDescribed() const
{
    return false;
}


template<class CloudType>
bool CML::FieldActivatedInjection<CloudType>::validInjection
(
    const label parcelI
)
{
    const label celli = injectorCells_[parcelI];

    if
    (
        nParcelsInjected_[parcelI] < nParcelsPerInjector_
     && factor_*referenceField_[celli] > thresholdField_[celli]
    )
    {
        nParcelsInjected_[parcelI]++;
        return true;
    }

    return false;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
