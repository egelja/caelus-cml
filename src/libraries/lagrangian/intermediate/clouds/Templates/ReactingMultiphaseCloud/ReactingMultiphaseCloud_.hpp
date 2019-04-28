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
    CML::ReactingMultiphaseCloud

Description
    Templated base class for multiphase reacting cloud

    - Adds to reacting cloud
      - multiphase composition
      - devolatilisatsion
      - surface reactions

\*---------------------------------------------------------------------------*/

#ifndef ReactingMultiphaseCloud__H
#define ReactingMultiphaseCloud__H

#include "reactingMultiphaseCloud.hpp"
#include "DevolatilisationModel.hpp"
#include "SurfaceReactionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes

template<class CloudType>
class DevolatilisationModel;

template<class CloudType>
class SurfaceReactionModel;

/*---------------------------------------------------------------------------*\
                   Class ReactingMultiphaseCloud Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ReactingMultiphaseCloud
:
    public CloudType,
    public reactingMultiphaseCloud
{
public:

    // Public typedefs

        //- Type of cloud this cloud was instantiated for
        typedef CloudType cloudType;

        //- Type of parcel the cloud was instantiated for
        typedef typename CloudType::particleType parcelType;

        //- Convenience typedef for this cloud type
        typedef ReactingMultiphaseCloud<CloudType> reactingMultiphaseCloudType;


private:

    // Private data

        //- Cloud copy pointer
        autoPtr<ReactingMultiphaseCloud<CloudType> > cloudCopyPtr_;


    // Private member functions

        //- Disallow default bitwise copy construct
        ReactingMultiphaseCloud(const ReactingMultiphaseCloud&);

        //- Disallow default bitwise assignment
        void operator=(const ReactingMultiphaseCloud&);


protected:

    // Protected data

        //- Parcel constant properties
        typename parcelType::constantProperties constProps_;


        // References to the cloud sub-models

            //- Devolatilisation model
            autoPtr
            <
                DevolatilisationModel<ReactingMultiphaseCloud<CloudType> >
            >
            devolatilisationModel_;

            //- Surface reaction model
            autoPtr
            <
                SurfaceReactionModel<ReactingMultiphaseCloud<CloudType> >
            >
            surfaceReactionModel_;


        // Check

            //- Total mass transferred to continuous phase via devolatilisation
            scalar dMassDevolatilisation_;

            //- Total mass transferred to continuous phase via surface
            //  reactions
            scalar dMassSurfaceReaction_;


    // Protected Member Functions

        // Initialisation

            //- Set cloud sub-models
            void setModels();


        // Cloud evolution functions

            //- Reset state of cloud
            void cloudReset(ReactingMultiphaseCloud<CloudType>& c);


public:

    // Constructors

        //- Construct given carrier gas fields
        ReactingMultiphaseCloud
        (
            const word& cloudName,
            const volScalarField& rho,
            const volVectorField& U,
            const dimensionedVector& g,
            const SLGThermo& thermo,
            bool readFields = true
        );


        //- Copy constructor with new name
        ReactingMultiphaseCloud
        (
            ReactingMultiphaseCloud<CloudType>& c,
            const word& name
        );

        //- Copy constructor with new name - creates bare cloud
        ReactingMultiphaseCloud
        (
            const fvMesh& mesh,
            const word& name,
            const ReactingMultiphaseCloud<CloudType>& c
        );

        //- Construct and return clone based on (this) with new name
        virtual autoPtr<Cloud<parcelType> > clone(const word& name)
        {
            return autoPtr<Cloud<parcelType> >
            (
                new ReactingMultiphaseCloud(*this, name)
            );
        }

        //- Construct and return bare clone based on (this) with new name
        virtual autoPtr<Cloud<parcelType> > cloneBare(const word& name) const
        {
            return autoPtr<Cloud<parcelType> >
            (
                new ReactingMultiphaseCloud(this->mesh(), name, *this)
            );
        }


    //- Destructor
    virtual ~ReactingMultiphaseCloud();


    // Member Functions

        // Access

            //- Return a reference to the cloud copy
            inline const ReactingMultiphaseCloud& cloudCopy() const;

            //- Return the constant properties
            inline const typename parcelType::constantProperties&
                constProps() const;

            //- Return access to the constant properties
            inline typename parcelType::constantProperties& constProps();


            // Sub-models

                //- Return const access to devolatilisation model
                inline const DevolatilisationModel
                <
                    ReactingMultiphaseCloud<CloudType>
                >&
                devolatilisation() const;

                //- Return reference to devolatilisation model
                inline DevolatilisationModel
                <
                    ReactingMultiphaseCloud<CloudType>
                >&
                devolatilisation();

                //- Return const access to reacting surface reaction model
                inline const SurfaceReactionModel
                <
                    ReactingMultiphaseCloud<CloudType>
                >&
                surfaceReaction() const;

                //- Return reference to reacting surface reaction model
                inline SurfaceReactionModel
                <
                    ReactingMultiphaseCloud<CloudType>
                >&
                surfaceReaction();


        // Cloud evolution functions

            //- Set parcel thermo properties
            void setParcelThermoProperties
            (
                parcelType& parcel,
                const scalar lagrangianDt
            );

            //- Check parcel properties
            void checkParcelProperties
            (
                parcelType& parcel,
                const scalar lagrangianDt,
                const bool fullyDescribed
            );

            //- Store the current cloud state
            void storeState();

            //- Reset the current cloud to the previously stored state
            void restoreState();

            //- Reset the cloud source terms
            void resetSourceTerms();

            //- Evolve the cloud
            void evolve();


        // Mapping

            //- Remap the cells of particles corresponding to the
            //  mesh topology change with a default tracking data object
            virtual void autoMap(const mapPolyMesh&);


        // I-O

            //- Print cloud information
            void info();

            //- Write the field data for the cloud
            virtual void writeFields() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
inline const CML::ReactingMultiphaseCloud<CloudType>&
CML::ReactingMultiphaseCloud<CloudType>::cloudCopy() const
{
    return cloudCopyPtr_();
}


template<class CloudType>
inline const typename CloudType::particleType::constantProperties&
CML::ReactingMultiphaseCloud<CloudType>::constProps() const
{
    return constProps_;
}


template<class CloudType>
inline typename CloudType::particleType::constantProperties&
CML::ReactingMultiphaseCloud<CloudType>::constProps()
{
    return constProps_;
}


template<class CloudType>
inline const CML::DevolatilisationModel
<
    CML::ReactingMultiphaseCloud<CloudType>
>&
CML::ReactingMultiphaseCloud<CloudType>::devolatilisation() const
{
    return devolatilisationModel_;
}


template<class CloudType>
inline CML::DevolatilisationModel
<
    CML::ReactingMultiphaseCloud<CloudType>
>&
CML::ReactingMultiphaseCloud<CloudType>::devolatilisation()
{
    return devolatilisationModel_();
}


template<class CloudType>
inline const CML::SurfaceReactionModel
<
    CML::ReactingMultiphaseCloud<CloudType>
>&
CML::ReactingMultiphaseCloud<CloudType>::surfaceReaction() const
{
    return surfaceReactionModel_;
}


template<class CloudType>
inline CML::SurfaceReactionModel
<
    CML::ReactingMultiphaseCloud<CloudType>
>&
CML::ReactingMultiphaseCloud<CloudType>::surfaceReaction()
{
    return surfaceReactionModel_();
}


// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

template<class CloudType>
void CML::ReactingMultiphaseCloud<CloudType>::setModels()
{
    devolatilisationModel_.reset
    (
        DevolatilisationModel<ReactingMultiphaseCloud<CloudType> >::New
        (
            this->subModelProperties(),
            *this
        ).ptr()
    );

    surfaceReactionModel_.reset
    (
        SurfaceReactionModel<ReactingMultiphaseCloud<CloudType> >::New
        (
            this->subModelProperties(),
            *this
        ).ptr()
    );
}


template<class CloudType>
void CML::ReactingMultiphaseCloud<CloudType>::cloudReset
(
    ReactingMultiphaseCloud<CloudType>& c
)
{
    CloudType::cloudReset(c);

    devolatilisationModel_.reset(c.devolatilisationModel_.ptr());
    surfaceReactionModel_.reset(c.surfaceReactionModel_.ptr());

    dMassDevolatilisation_ = c.dMassDevolatilisation_;
    dMassSurfaceReaction_ = c.dMassSurfaceReaction_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ReactingMultiphaseCloud<CloudType>::ReactingMultiphaseCloud
(
    const word& cloudName,
    const volScalarField& rho,
    const volVectorField& U,
    const dimensionedVector& g,
    const SLGThermo& thermo,
    bool readFields
)
:
    CloudType(cloudName, rho, U, g, thermo, false),
    reactingMultiphaseCloud(),
    cloudCopyPtr_(nullptr),
    constProps_(this->particleProperties()),
    devolatilisationModel_(nullptr),
    surfaceReactionModel_(nullptr),
    dMassDevolatilisation_(0.0),
    dMassSurfaceReaction_(0.0)
{
    if (this->solution().active())
    {
        setModels();

        if (readFields)
        {
            parcelType::readFields(*this, this->composition());
            this->deleteLostParticles();
        }
    }

    if (this->solution().resetSourcesOnStartup())
    {
        resetSourceTerms();
    }
}


template<class CloudType>
CML::ReactingMultiphaseCloud<CloudType>::ReactingMultiphaseCloud
(
    ReactingMultiphaseCloud<CloudType>& c,
    const word& name
)
:
    CloudType(c, name),
    reactingMultiphaseCloud(),
    cloudCopyPtr_(nullptr),
    constProps_(c.constProps_),
    devolatilisationModel_(c.devolatilisationModel_->clone()),
    surfaceReactionModel_(c.surfaceReactionModel_->clone()),
    dMassDevolatilisation_(c.dMassDevolatilisation_),
    dMassSurfaceReaction_(c.dMassSurfaceReaction_)
{}


template<class CloudType>
CML::ReactingMultiphaseCloud<CloudType>::ReactingMultiphaseCloud
(
    const fvMesh& mesh,
    const word& name,
    const ReactingMultiphaseCloud<CloudType>& c
)
:
    CloudType(mesh, name, c),
    reactingMultiphaseCloud(),
    cloudCopyPtr_(nullptr),
    constProps_(),
    devolatilisationModel_(nullptr),
    surfaceReactionModel_(nullptr),
    dMassDevolatilisation_(0.0),
    dMassSurfaceReaction_(0.0)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::ReactingMultiphaseCloud<CloudType>::~ReactingMultiphaseCloud()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::ReactingMultiphaseCloud<CloudType>::setParcelThermoProperties
(
    parcelType& parcel,
    const scalar lagrangianDt
)
{
    CloudType::setParcelThermoProperties(parcel, lagrangianDt);

    label idGas = this->composition().idGas();
    label idLiquid = this->composition().idLiquid();
    label idSolid = this->composition().idSolid();

    parcel.YGas() = this->composition().Y0(idGas);
    parcel.YLiquid() = this->composition().Y0(idLiquid);
    parcel.YSolid() = this->composition().Y0(idSolid);
}


template<class CloudType>
void CML::ReactingMultiphaseCloud<CloudType>::checkParcelProperties
(
    parcelType& parcel,
    const scalar lagrangianDt,
    const bool fullyDescribed
)
{
    CloudType::checkParcelProperties(parcel, lagrangianDt, fullyDescribed);

    if (fullyDescribed)
    {
        label idGas = this->composition().idGas();
        label idLiquid = this->composition().idLiquid();
        label idSolid = this->composition().idSolid();

        this->checkSuppliedComposition
        (
            parcel.YGas(),
            this->composition().Y0(idGas),
            "YGas"
        );
        this->checkSuppliedComposition
        (
            parcel.YLiquid(),
            this->composition().Y0(idLiquid),
            "YLiquid"
        );
        this->checkSuppliedComposition
        (
            parcel.YSolid(),
            this->composition().Y0(idSolid),
            "YSolid"
        );
    }
}


template<class CloudType>
void CML::ReactingMultiphaseCloud<CloudType>::storeState()
{
    cloudCopyPtr_.reset
    (
        static_cast<ReactingMultiphaseCloud<CloudType>*>
        (
            clone(this->name() + "Copy").ptr()
        )
    );
}


template<class CloudType>
void CML::ReactingMultiphaseCloud<CloudType>::restoreState()
{
    cloudReset(cloudCopyPtr_());
    cloudCopyPtr_.clear();
}


template<class CloudType>
void CML::ReactingMultiphaseCloud<CloudType>::resetSourceTerms()
{
    CloudType::resetSourceTerms();
}


template<class CloudType>
void CML::ReactingMultiphaseCloud<CloudType>::evolve()
{
    if (this->solution().canEvolve())
    {
        typename parcelType::trackingData td(*this);

        this->solve(*this, td);
    }
}


template<class CloudType>
void CML::ReactingMultiphaseCloud<CloudType>::autoMap
(
    const mapPolyMesh& mapper
)
{
    Cloud<parcelType>::autoMap(mapper);

    this->updateMesh();
}


template<class CloudType>
void CML::ReactingMultiphaseCloud<CloudType>::info()
{
    CloudType::info();

    this->devolatilisation().info(Info);
    this->surfaceReaction().info(Info);
}


template<class CloudType>
void CML::ReactingMultiphaseCloud<CloudType>::writeFields() const
{
    if (this->compositionModel_.valid())
    {
        CloudType::particleType::writeFields(*this, this->composition());
    }
}


#endif

// ************************************************************************* //
