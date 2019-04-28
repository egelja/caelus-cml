/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::SurfaceFilmModel

Description
    Templated wall surface film model class.


\*---------------------------------------------------------------------------*/

#ifndef SurfaceFilmModel__H
#define SurfaceFilmModel__H

#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "CloudSubModelBase.hpp"
#include "surfaceFilmRegionModel.hpp"
#include "mathematicalConstants.hpp"
#include "mappedPatchBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
namespace regionModels
{
    namespace surfaceFilmModels
    {
        class surfaceFilmRegionModel;
    }
}

/*---------------------------------------------------------------------------*\
                      Class SurfaceFilmModel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class SurfaceFilmModel
:
    public CloudSubModelBase<CloudType>
{
protected:

    // Protected data

        //- Convenience typedef to the cloud's parcel type
        typedef typename CloudType::parcelType parcelType;

        //- Gravitational acceleration constant
        const dimensionedVector& g_;

        //- Ejected parcel type label - id assigned to identify parcel for
        //  post-processing. If not specified, defaults to originating cloud
        //  type
        label ejectedParcelType_;


        // Cached injector fields per film patch

            //- Parcel mass / patch face
            scalarList massParcelPatch_;

            //- Parcel diameter / patch face
            scalarList diameterParcelPatch_;

            //- Film velocity / patch face
            List<vector> UFilmPatch_;

            //- Film density / patch face
            scalarList rhoFilmPatch_;

            //- Film height of all film patches / patch face
            scalarListList deltaFilmPatch_;


        // Counters

            //- Number of parcels transferred to the film model
            label nParcelsTransferred_;

            //- Number of parcels injected from the film model
            label nParcelsInjected_;


    // Protected functions

        //- Cache the film fields in preparation for injection
        virtual void cacheFilmFields
        (
            const label filmPatchi,
            const label primaryPatchi,
            const regionModels::surfaceFilmModels::surfaceFilmRegionModel&
        );

        //- Set the individual parcel properties
        virtual void setParcelProperties
        (
            parcelType& p,
            const label filmFacei
        ) const;


public:

    //- Runtime type information
    TypeName("surfaceFilmModel");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        SurfaceFilmModel,
        dictionary,
        (
            const dictionary& dict,
            CloudType& owner
        ),
        (dict, owner)
    );


    // Constructors

        //- Construct null from owner
        SurfaceFilmModel(CloudType& owner);

        //- Construct from components
        SurfaceFilmModel
        (
            const dictionary& dict,
            CloudType& owner,
            const word& type
        );

        //- Construct copy
        SurfaceFilmModel(const SurfaceFilmModel<CloudType>& sfm);

        //- Construct and return a clone
        virtual autoPtr<SurfaceFilmModel<CloudType> > clone() const = 0;


    //- Destructor
    virtual ~SurfaceFilmModel();


    //- Selector
    static autoPtr<SurfaceFilmModel<CloudType> > New
    (
        const dictionary& dict,
        CloudType& owner
    );


    // Member Functions

        // Access

            //- Return gravitational acceleration constant
            inline const dimensionedVector& g() const;

            //- Return const access to the number of parcels transferred to the
            //  film model
            inline label nParcelsTransferred() const;

            //- Return non-const access to the number of parcels transferred to
            //  the film model
            inline label& nParcelsTransferred();

            //- Return const access to the number of parcels injected from the
            //  film model
            inline label nParcelsInjected() const;

            //- Return non-const access to the number of parcels injected from
            //  the film model
            inline label& nParcelsInjected();


        // Member Functions

            //- Transfer parcel from cloud to surface film
            //  Returns true if parcel is to be transferred
            virtual bool transferParcel
            (
                parcelType& p,
                const polyPatch& pp,
                bool& keepParticle
            ) = 0;

            //- Inject parcels into the cloud
            template<class TrackCloudType>
            void inject(TrackCloudType& cloud);


        // I-O

            //- Write surface film info to stream
            virtual void info(Ostream& os);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeSurfaceFilmModel(CloudType)                                        \
                                                                               \
    typedef CloudType::kinematicCloudType kinematicCloudType;                  \
    defineNamedTemplateTypeNameAndDebug                                        \
    (                                                                          \
        SurfaceFilmModel<kinematicCloudType>,                                  \
        0                                                                      \
    );                                                                         \
    defineTemplateRunTimeSelectionTable                                        \
    (                                                                          \
        SurfaceFilmModel<kinematicCloudType>,                                  \
        dictionary                                                             \
    );


#define makeSurfaceFilmModelType(SS, CloudType)                                \
                                                                               \
    typedef CloudType::kinematicCloudType kinematicCloudType;                  \
    defineNamedTemplateTypeNameAndDebug(SS<kinematicCloudType>, 0);            \
                                                                               \
    SurfaceFilmModel<kinematicCloudType>::                                     \
        adddictionaryConstructorToTable<SS<kinematicCloudType> >               \
            add##SS##CloudType##kinematicCloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
const CML::dimensionedVector& CML::SurfaceFilmModel<CloudType>::g() const
{
    return g_;
}


template<class CloudType>
CML::label& CML::SurfaceFilmModel<CloudType>::nParcelsTransferred()
{
    return nParcelsTransferred_;
}


template<class CloudType>
CML::label CML::SurfaceFilmModel<CloudType>::nParcelsTransferred() const
{
    return nParcelsTransferred_;
}


template<class CloudType>
CML::label& CML::SurfaceFilmModel<CloudType>::nParcelsInjected()
{
    return nParcelsInjected_;
}


template<class CloudType>
CML::label CML::SurfaceFilmModel<CloudType>::nParcelsInjected() const
{
    return nParcelsInjected_;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

using namespace CML::constant;

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::SurfaceFilmModel<CloudType>::SurfaceFilmModel(CloudType& owner)
:
    CloudSubModelBase<CloudType>(owner),
    g_(owner.g()),
    ejectedParcelType_(0),
    massParcelPatch_(0),
    diameterParcelPatch_(0),
    UFilmPatch_(0),
    rhoFilmPatch_(0),
    deltaFilmPatch_(0),
    nParcelsTransferred_(0),
    nParcelsInjected_(0)
{}


template<class CloudType>
CML::SurfaceFilmModel<CloudType>::SurfaceFilmModel
(
    const dictionary& dict,
    CloudType& owner,
    const word& type
)
:
    CloudSubModelBase<CloudType>(owner, dict, typeName, type),
    g_(owner.g()),
    ejectedParcelType_
    (
        this->coeffDict().lookupOrDefault("ejectedParcelType", -1)
    ),
    massParcelPatch_(0),
    diameterParcelPatch_(0),
    UFilmPatch_(0),
    rhoFilmPatch_(0),
    deltaFilmPatch_(owner.mesh().boundary().size()),
    nParcelsTransferred_(0),
    nParcelsInjected_(0)
{}


template<class CloudType>
CML::SurfaceFilmModel<CloudType>::SurfaceFilmModel
(
    const SurfaceFilmModel<CloudType>& sfm
)
:
    CloudSubModelBase<CloudType>(sfm),
    g_(sfm.g_),
    ejectedParcelType_(sfm.ejectedParcelType_),
    massParcelPatch_(sfm.massParcelPatch_),
    diameterParcelPatch_(sfm.diameterParcelPatch_),
    UFilmPatch_(sfm.UFilmPatch_),
    rhoFilmPatch_(sfm.rhoFilmPatch_),
    deltaFilmPatch_(sfm.deltaFilmPatch_),
    nParcelsTransferred_(sfm.nParcelsTransferred_),
    nParcelsInjected_(sfm.nParcelsInjected_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::SurfaceFilmModel<CloudType>::~SurfaceFilmModel()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
template<class TrackCloudType>
void CML::SurfaceFilmModel<CloudType>::inject(TrackCloudType& cloud)
{
    if (!this->active())
    {
        return;
    }

    // Retrieve the film model from the owner database
    const regionModels::surfaceFilmModels::surfaceFilmRegionModel& filmModel =
        this->owner().mesh().time().objectRegistry::template lookupObject
        <regionModels::surfaceFilmModels::surfaceFilmRegionModel>
        (
            "surfaceFilmProperties"
        );

    if (!filmModel.active())
    {
        return;
    }

    const labelList& filmPatches = filmModel.intCoupledPatchIDs();
    const labelList& primaryPatches = filmModel.primaryPatchIDs();

    const fvMesh& mesh = this->owner().mesh();
    const polyBoundaryMesh& pbm = mesh.boundaryMesh();

    forAll(filmPatches, i)
    {
        const label filmPatchi = filmPatches[i];
        const label primaryPatchi = primaryPatches[i];

        const labelList& injectorCellsPatch = pbm[primaryPatchi].faceCells();

        cacheFilmFields(filmPatchi, primaryPatchi, filmModel);

        const vectorField& Cf = mesh.C().boundaryField()[primaryPatchi];
        const vectorField& Sf = mesh.Sf().boundaryField()[primaryPatchi];
        const scalarField& magSf = mesh.magSf().boundaryField()[primaryPatchi];

        forAll(injectorCellsPatch, j)
        {
            if (diameterParcelPatch_[j] > 0)
            {
                const label celli = injectorCellsPatch[j];

                const scalar offset =
                    max
                    (
                        diameterParcelPatch_[j],
                        deltaFilmPatch_[primaryPatchi][j]
                    );
                const point pos = Cf[j] - 1.1*offset*Sf[j]/magSf[j];

                // Create a new parcel
                parcelType* pPtr =
                    new parcelType(this->owner().pMesh(), pos, celli);

                // Check/set new parcel thermo properties
                cloud.setParcelThermoProperties(*pPtr, 0.0);

                setParcelProperties(*pPtr, j);

                if (pPtr->nParticle() > 0.001)
                {
                    // Check new parcel properties
    //                cloud.checkParcelProperties(*pPtr, 0.0, true);
                    cloud.checkParcelProperties(*pPtr, 0.0, false);

                    // Add the new parcel to the cloud
                    cloud.addParticle(pPtr);

                    nParcelsInjected_++;
                }
                else
                {
                    // TODO: cache mass and re-distribute?
                    delete pPtr;
                }
            }
        }
    }
}


template<class CloudType>
void CML::SurfaceFilmModel<CloudType>::cacheFilmFields
(
    const label filmPatchi,
    const label primaryPatchi,
    const regionModels::surfaceFilmModels::surfaceFilmRegionModel& filmModel
)
{
    massParcelPatch_ = filmModel.cloudMassTrans().boundaryField()[filmPatchi];
    filmModel.toPrimary(filmPatchi, massParcelPatch_);

    diameterParcelPatch_ =
        filmModel.cloudDiameterTrans().boundaryField()[filmPatchi];
    filmModel.toPrimary(filmPatchi, diameterParcelPatch_, maxEqOp<scalar>());

    UFilmPatch_ = filmModel.Us().boundaryField()[filmPatchi];
    filmModel.toPrimary(filmPatchi, UFilmPatch_);

    rhoFilmPatch_ = filmModel.rho().boundaryField()[filmPatchi];
    filmModel.toPrimary(filmPatchi, rhoFilmPatch_);

    deltaFilmPatch_[primaryPatchi] =
        filmModel.delta().boundaryField()[filmPatchi];
    filmModel.toPrimary(filmPatchi, deltaFilmPatch_[primaryPatchi]);
}


template<class CloudType>
void CML::SurfaceFilmModel<CloudType>::setParcelProperties
(
    parcelType& p,
    const label filmFacei
) const
{
    // Set parcel properties
    scalar vol = mathematical::pi/6.0*pow3(diameterParcelPatch_[filmFacei]);
    p.d() = diameterParcelPatch_[filmFacei];
    p.U() = UFilmPatch_[filmFacei];
    p.rho() = rhoFilmPatch_[filmFacei];

    p.nParticle() = massParcelPatch_[filmFacei]/p.rho()/vol;

    if (ejectedParcelType_ >= 0)
    {
        p.typeId() = ejectedParcelType_;
    }
}


template<class CloudType>
void CML::SurfaceFilmModel<CloudType>::info(Ostream& os)
{
    label nTrans0 =
        this->template getModelProperty<label>("nParcelsTransferred");

    label nInject0 =
        this->template getModelProperty<label>("nParcelsInjected");

    label nTransTotal =
        nTrans0 + returnReduce(nParcelsTransferred_, sumOp<label>());

    label nInjectTotal =
        nInject0 + returnReduce(nParcelsInjected_, sumOp<label>());

    os  << "    Parcels absorbed into film      = " << nTransTotal << nl
        << "    New film detached parcels       = " << nInjectTotal << endl;

    if (this->writeTime())
    {
        this->setModelProperty("nParcelsTransferred", nTransTotal);
        this->setModelProperty("nParcelsInjected", nInjectTotal);
        nParcelsTransferred_ = 0;
        nParcelsInjected_ = 0;
    }
}


template<class CloudType>
CML::autoPtr<CML::SurfaceFilmModel<CloudType> >
CML::SurfaceFilmModel<CloudType>::New
(
    const dictionary& dict,
    CloudType& owner
)
{
    const word modelType(dict.lookup("surfaceFilmModel"));

    Info<< "Selecting surface film model " << modelType << endl;

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(modelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown surface film model type "
            << modelType << nl << nl
            << "Valid surface film model types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<SurfaceFilmModel<CloudType> >(cstrIter()(dict, owner));
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
