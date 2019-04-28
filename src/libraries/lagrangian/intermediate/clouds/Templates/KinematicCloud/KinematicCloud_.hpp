/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
Copyright (C) 2016 OpenCFD Ltd.
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
    CML::KinematicCloud

Description
    Templated base class for kinematic cloud

    - cloud function objects

    - particle forces, e.g.
      - buoyancy
      - drag
      - pressure gradient
      - ...

    - sub-models:
      - dispersion model
      - injection model
      - patch interaction model
      - stochastic collision model
      - surface film model

SourceFiles
    KinematicCloudI.hpp
    KinematicCloud_.cpp

\*---------------------------------------------------------------------------*/

#ifndef KinematicCloud__H
#define KinematicCloud__H

#include "particle.hpp"
#include "Cloud.hpp"
#include "kinematicCloud.hpp"
#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "Random.hpp"
#include "fvMesh.hpp"
#include "volFields.hpp"
#include "fvMatrices.hpp"
#include "cloudSolution.hpp"

#include "ParticleForceList.hpp"
#include "CloudFunctionObjectList.hpp"
#include "fvmSup.hpp"
#include "SortableList.hpp"
#include "integrationScheme.hpp"
#include "interpolation.hpp"
#include "subCycleTime.hpp"

#include "InjectionModelList.hpp"
#include "DispersionModel.hpp"
#include "PatchInteractionModel.hpp"
#include "StochasticCollisionModel.hpp"
#include "SurfaceFilmModel_.hpp"

#include "profiling.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes

class integrationScheme;

template<class CloudType>
class InjectionModelList;

template<class CloudType>
class DispersionModel;

template<class CloudType>
class PatchInteractionModel;

template<class CloudType>
class SurfaceFilmModel;

template<class CloudType>
class StochasticCollisionModel;


/*---------------------------------------------------------------------------*\
                       Class KinematicCloud Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class KinematicCloud
:
    public CloudType,
    public kinematicCloud
{
public:

    // Public typedefs

        //- Type of cloud this cloud was instantiated for
        typedef CloudType cloudType;

        //- Type of parcel the cloud was instantiated for
        typedef typename CloudType::particleType parcelType;

        //- Convenience typedef for this cloud type
        typedef KinematicCloud<CloudType> kinematicCloudType;

        //- Force models type
        typedef ParticleForceList<KinematicCloud<CloudType> > forceType;

        //- Function object type
        typedef CloudFunctionObjectList<KinematicCloud<CloudType> >
            functionType;


private:

    // Private data

        //- Cloud copy pointer
        autoPtr<KinematicCloud<CloudType> > cloudCopyPtr_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        KinematicCloud(const KinematicCloud&);

        //- Disallow default bitwise assignment
        void operator=(const KinematicCloud&);


protected:

    // Protected data

        //- References to the mesh and time databases
        const fvMesh& mesh_;

        //- Dictionary of particle properties
        IOdictionary particleProperties_;

        //- Dictionary of output properties
        IOdictionary outputProperties_;

        //- Solution properties
        cloudSolution solution_;

        //- Parcel constant properties
        typename parcelType::constantProperties constProps_;

        //- Sub-models dictionary
        const dictionary subModelProperties_;

        //- Random number generator - used by some injection routines
        mutable Random rndGen_;

        //- Cell occupancy information for each parcel, (demand driven)
        autoPtr<List<DynamicList<parcelType*> > > cellOccupancyPtr_;

        //- Cell length scale
        scalarField cellLengthScale_;


        // References to the carrier gas fields

            //- Density [kg/m3]
            const volScalarField& rho_;

            //- Velocity [m/s]
            const volVectorField& U_;

            //- Dynamic viscosity [Pa.s]
            const volScalarField& mu_;


        // Environmental properties

            //- Gravity
            const dimensionedVector& g_;

            //- Averaged ambient domain pressure
            scalar pAmbient_;


        //- Optional particle forces
        forceType forces_;

        //- Optional cloud function objects
        functionType functions_;


        // References to the cloud sub-models

            //- Injector models
            InjectionModelList<KinematicCloud<CloudType> > injectors_;

            //- Dispersion model
            autoPtr<DispersionModel<KinematicCloud<CloudType> > >
                dispersionModel_;

            //- Patch interaction model
            autoPtr<PatchInteractionModel<KinematicCloud<CloudType> > >
                patchInteractionModel_;

            //- Stochastic collision model
            autoPtr<StochasticCollisionModel<KinematicCloud<CloudType> > >
                stochasticCollisionModel_;

            //- Surface film model
            autoPtr<SurfaceFilmModel<KinematicCloud<CloudType> > >
                surfaceFilmModel_;


        // Reference to the particle integration schemes

            //- Velocity integration
            autoPtr<integrationScheme> UIntegrator_;


        // Sources

            //- Momentum
            autoPtr<DimensionedField<vector, volMesh> > UTrans_;

            //- Coefficient for carrier phase U equation
            autoPtr<DimensionedField<scalar, volMesh> > UCoeff_;


        // Initialisation

            //- Set cloud sub-models
            void setModels();


        // Cloud evolution functions

            //- Solve the cloud - calls all evolution functions
            template<class TrackCloudType>
            void solve
            (
                TrackCloudType& cloud,
                typename parcelType::trackingData& td
            );

            //- Build the cellOccupancy
            void buildCellOccupancy();

            //- Update (i.e. build) the cellOccupancy if it has
            //  already been used
            void updateCellOccupancy();

            //- Evolve the cloud
            template<class TrackCloudType>
            void evolveCloud
            (
                TrackCloudType& cloud,
                typename parcelType::trackingData& td
            );

            //- Post-evolve
            void postEvolve();

            //- Reset state of cloud
            void cloudReset(KinematicCloud<CloudType>& c);


public:

    // Constructors

        //- Construct given carrier gas fields
        KinematicCloud
        (
            const word& cloudName,
            const volScalarField& rho,
            const volVectorField& U,
            const volScalarField& mu,
            const dimensionedVector& g,
            bool readFields = true
        );

        //- Copy constructor with new name
        KinematicCloud
        (
            KinematicCloud<CloudType>& c,
            const word& name
        );

        //- Copy constructor with new name - creates bare cloud
        KinematicCloud
        (
            const fvMesh& mesh,
            const word& name,
            const KinematicCloud<CloudType>& c
        );

        //- Construct and return clone based on (this) with new name
        virtual autoPtr<Cloud<parcelType> > clone(const word& name)
        {
            return autoPtr<Cloud<parcelType> >
            (
                new KinematicCloud(*this, name)
            );
        }

        //- Construct and return bare clone based on (this) with new name
        virtual autoPtr<Cloud<parcelType> > cloneBare(const word& name) const
        {
            return autoPtr<Cloud<parcelType> >
            (
                new KinematicCloud(this->mesh(), name, *this)
            );
        }


    //- Destructor
    virtual ~KinematicCloud();


    // Member Functions

        // Access

            //- Return a reference to the cloud copy
            inline const KinematicCloud& cloudCopy() const;


            // References to the mesh and databases

                //- Return reference to the mesh
                inline const fvMesh& mesh() const;

                //- Return particle properties dictionary
                inline const IOdictionary& particleProperties() const;

                //- Return output properties dictionary
                inline const IOdictionary& outputProperties() const;

                //- Return non-const access to the output properties dictionary
                inline IOdictionary& outputProperties();

                //- Return const access to the solution properties
                inline const cloudSolution& solution() const;

                //- Return access to the solution properties
                inline cloudSolution& solution();

                //- Return the constant properties
                inline const typename parcelType::constantProperties&
                    constProps() const;

                //- Return access to the constant properties
                inline typename parcelType::constantProperties& constProps();

                    //- Return reference to the sub-models dictionary
                inline const dictionary& subModelProperties() const;


            // Cloud data

                //- Return reference to the random object
                inline Random& rndGen() const;

                //- Return the cell occupancy information for each
                //  parcel, non-const access, the caller is
                //  responsible for updating it for its own purposes
                //  if particles are removed or created.
                inline List<DynamicList<parcelType*> >& cellOccupancy();

                //- Return the cell length scale
                inline const scalarField& cellLengthScale() const;


            // References to the carrier gas fields

                //- Return carrier gas velocity
                inline const volVectorField& U() const;

                //- Return carrier gas density
                inline const volScalarField& rho() const;

                //- Return carrier gas dynamic viscosity
                inline const volScalarField& mu() const;


            // Environmental properties

                //- Gravity
                inline const dimensionedVector& g() const;

                //- Return const-access to the ambient pressure
                inline scalar pAmbient() const;

                //- Return reference to the ambient pressure
                inline scalar& pAmbient();


            //- Optional particle forces
            inline const forceType& forces() const;

            //- Return the optional particle forces
            inline forceType& forces();

            //- Optional cloud function objects
            inline functionType& functions();


            // Sub-models

                //- Return const access to the injection model
                inline const InjectionModelList<KinematicCloud<CloudType> >&
                    injectors() const;

                //- Return reference to the injection model
                inline InjectionModelList<KinematicCloud<CloudType> >&
                    injectors();

                //- Return const-access to the dispersion model
                inline const DispersionModel<KinematicCloud<CloudType> >&
                    dispersion() const;

                //- Return reference to the dispersion model
                inline DispersionModel<KinematicCloud<CloudType> >&
                    dispersion();

                //- Return const-access to the patch interaction model
                inline const PatchInteractionModel<KinematicCloud<CloudType> >&
                    patchInteraction() const;

                //- Return reference to the patch interaction model
                inline PatchInteractionModel<KinematicCloud<CloudType> >&
                    patchInteraction();

                //- Return const-access to the stochastic collision model
                inline const
                    StochasticCollisionModel<KinematicCloud<CloudType> >&
                    stochasticCollision() const;

                //- Return reference to the stochastic collision model
                inline StochasticCollisionModel<KinematicCloud<CloudType> >&
                    stochasticCollision();

                //- Return const-access to the surface film model
                inline const SurfaceFilmModel<KinematicCloud<CloudType> >&
                    surfaceFilm() const;

                //- Return reference to the surface film model
                inline SurfaceFilmModel<KinematicCloud<CloudType> >&
                    surfaceFilm();


            // Integration schemes

                //-Return reference to velocity integration
                inline const integrationScheme& UIntegrator() const;


            // Sources

                // Momentum

                    //- Return reference to momentum source
                    inline DimensionedField<vector, volMesh>& UTrans();

                    //- Return const reference to momentum source
                    inline const DimensionedField<vector, volMesh>&
                        UTrans() const;

                     //- Return coefficient for carrier phase U equation
                    inline DimensionedField<scalar, volMesh>& UCoeff();

                    //- Return const coefficient for carrier phase U equation
                    inline const DimensionedField<scalar, volMesh>&
                        UCoeff() const;

                    //- Return tmp momentum source term
                    inline tmp<fvVectorMatrix> SU(volVectorField& U) const;


        // Check

            //- Total number of parcels
            inline label nParcels() const;

            //- Total mass in system
            inline scalar massInSystem() const;

            //- Total linear momentum of the system
            inline vector linearMomentumOfSystem() const;

            //- Total linear kinetic energy in the system
            inline scalar linearKineticEnergyOfSystem() const;

            //- Total rotational kinetic energy in the system
            inline scalar rotationalKineticEnergyOfSystem() const;

            //- Mean diameter Dij
            inline scalar Dij(const label i, const label j) const;

            //- Max diameter
            inline scalar Dmax() const;


            // Fields

                //- Volume swept rate of parcels per cell
                inline const tmp<volScalarField> vDotSweep() const;

                //- Return the particle volume fraction field
                //  Note: for particles belonging to this cloud only
                inline const tmp<volScalarField> theta() const;

                //- Return the particle mass fraction field
                //  Note: for particles belonging to this cloud only
                inline const tmp<volScalarField> alpha() const;

                //- Return the particle effective density field
                //  Note: for particles belonging to this cloud only
                inline const tmp<volScalarField> rhoEff() const;


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

            //- Relax field
            template<class Type>
            void relax
            (
                DimensionedField<Type, volMesh>& field,
                const DimensionedField<Type, volMesh>& field0,
                const word& name
            ) const;

            //- Scale field
            template<class Type>
            void scale
            (
                DimensionedField<Type, volMesh>& field,
                const word& name
            ) const;

            //- Apply relaxation to (steady state) cloud sources
            void relaxSources(const KinematicCloud<CloudType>& cloudOldTime);

            //- Apply scaling to (transient) cloud sources
            void scaleSources();

            //- Pre-evolve
            void preEvolve();

            //- Evolve the cloud
            void evolve();

            //- Particle motion
            template<class TrackCloudType>
            void motion
            (
                TrackCloudType& cloud,
                typename parcelType::trackingData& td
            );

            //- Calculate the patch normal and velocity to interact with,
            //  accounting for patch motion if required.
            void patchData
            (
                const parcelType& p,
                const polyPatch& pp,
                vector& normal,
                vector& Up
            ) const;


        // Mapping

            //- Update mesh
            void updateMesh();

            //- Remap the cells of particles corresponding to the
            //  mesh topology change with a default tracking data object
            virtual void autoMap(const mapPolyMesh&);


        // I-O

            //- Print cloud information
            void info();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
inline const CML::KinematicCloud<CloudType>&
CML::KinematicCloud<CloudType>::cloudCopy() const
{
    return cloudCopyPtr_();
}


template<class CloudType>
inline const CML::fvMesh& CML::KinematicCloud<CloudType>::mesh() const
{
    return mesh_;
}


template<class CloudType>
inline const CML::IOdictionary&
CML::KinematicCloud<CloudType>::particleProperties() const
{
    return particleProperties_;
}


template<class CloudType>
inline const CML::IOdictionary&
CML::KinematicCloud<CloudType>::outputProperties() const
{
    return outputProperties_;
}


template<class CloudType>
inline CML::IOdictionary& CML::KinematicCloud<CloudType>::outputProperties()
{
    return outputProperties_;
}


template<class CloudType>
inline const CML::cloudSolution&
CML::KinematicCloud<CloudType>::solution() const
{
    return solution_;
}


template<class CloudType>
inline CML::cloudSolution& CML::KinematicCloud<CloudType>::solution()
{
    return solution_;
}


template<class CloudType>
inline const typename CloudType::particleType::constantProperties&
CML::KinematicCloud<CloudType>::constProps() const
{
    return constProps_;
}


template<class CloudType>
inline typename CloudType::particleType::constantProperties&
CML::KinematicCloud<CloudType>::constProps()
{
    return constProps_;
}


template<class CloudType>
inline const CML::dictionary&
CML::KinematicCloud<CloudType>::subModelProperties() const
{
    return subModelProperties_;
}


template<class CloudType>
inline const CML::volScalarField& CML::KinematicCloud<CloudType>::rho() const
{
    return rho_;
}


template<class CloudType>
inline const CML::volVectorField& CML::KinematicCloud<CloudType>::U() const
{
    return U_;
}


template<class CloudType>
inline const CML::volScalarField& CML::KinematicCloud<CloudType>::mu() const
{
    return mu_;
}


template<class CloudType>
inline const CML::dimensionedVector& CML::KinematicCloud<CloudType>::g() const
{
    return g_;
}


template<class CloudType>
inline CML::scalar CML::KinematicCloud<CloudType>::pAmbient() const
{
    return pAmbient_;
}


template<class CloudType>
inline CML::scalar& CML::KinematicCloud<CloudType>::pAmbient()
{
    return pAmbient_;
}


template<class CloudType>
//inline const typename CloudType::parcelType::forceType&
inline const typename CML::KinematicCloud<CloudType>::forceType&
CML::KinematicCloud<CloudType>::forces() const
{
    return forces_;
}


template<class CloudType>
inline typename CML::KinematicCloud<CloudType>::forceType&
CML::KinematicCloud<CloudType>::forces()
{
    return forces_;
}


template<class CloudType>
inline typename CML::KinematicCloud<CloudType>::functionType&
CML::KinematicCloud<CloudType>::functions()
{
    return functions_;
}


template<class CloudType>
inline const CML::InjectionModelList<CML::KinematicCloud<CloudType> >&
CML::KinematicCloud<CloudType>::injectors() const
{
    return injectors_;
}


template<class CloudType>
inline CML::InjectionModelList<CML::KinematicCloud<CloudType> >&
CML::KinematicCloud<CloudType>::injectors()
{
    return injectors_;
}


template<class CloudType>
inline const CML::DispersionModel<CML::KinematicCloud<CloudType> >&
CML::KinematicCloud<CloudType>::dispersion() const
{
    return dispersionModel_;
}


template<class CloudType>
inline CML::DispersionModel<CML::KinematicCloud<CloudType> >&
CML::KinematicCloud<CloudType>::dispersion()
{
    return dispersionModel_();
}


template<class CloudType>
inline const CML::PatchInteractionModel<CML::KinematicCloud<CloudType> >&
CML::KinematicCloud<CloudType>::patchInteraction() const
{
    return patchInteractionModel_;
}


template<class CloudType>
inline CML::PatchInteractionModel<CML::KinematicCloud<CloudType> >&
CML::KinematicCloud<CloudType>::patchInteraction()
{
    return patchInteractionModel_();
}


template<class CloudType>
inline const CML::StochasticCollisionModel<CML::KinematicCloud<CloudType> >&
CML::KinematicCloud<CloudType>::stochasticCollision() const
{
    return stochasticCollisionModel_();
}


template<class CloudType>
inline CML::StochasticCollisionModel<CML::KinematicCloud<CloudType> >&
CML::KinematicCloud<CloudType>::stochasticCollision()
{
    return stochasticCollisionModel_();
}


template<class CloudType>
inline const CML::SurfaceFilmModel<CML::KinematicCloud<CloudType> >&
CML::KinematicCloud<CloudType>::surfaceFilm() const
{
    return surfaceFilmModel_();
}


template<class CloudType>
inline CML::SurfaceFilmModel<CML::KinematicCloud<CloudType> >&
CML::KinematicCloud<CloudType>::surfaceFilm()
{
    return surfaceFilmModel_();
}


template<class CloudType>
inline const CML::integrationScheme&
CML::KinematicCloud<CloudType>::UIntegrator() const
{
    return UIntegrator_;
}


template<class CloudType>
inline CML::label CML::KinematicCloud<CloudType>::nParcels() const
{
    return this->size();
}


template<class CloudType>
inline CML::scalar CML::KinematicCloud<CloudType>::massInSystem() const
{
    scalar sysMass = 0.0;
    forAllConstIter(typename KinematicCloud<CloudType>, *this, iter)
    {
         const parcelType& p = iter();
         sysMass += p.nParticle()*p.mass();
    }

    return sysMass;
}


template<class CloudType>
inline CML::vector
CML::KinematicCloud<CloudType>::linearMomentumOfSystem() const
{
    vector linearMomentum(Zero);

    forAllConstIter(typename KinematicCloud<CloudType>, *this, iter)
    {
        const parcelType& p = iter();

        linearMomentum += p.nParticle()*p.mass()*p.U();
    }

    return linearMomentum;
}


template<class CloudType>
inline CML::scalar
CML::KinematicCloud<CloudType>::linearKineticEnergyOfSystem() const
{
    scalar linearKineticEnergy = 0.0;

    forAllConstIter(typename KinematicCloud<CloudType>, *this, iter)
    {
        const parcelType& p = iter();

        linearKineticEnergy += p.nParticle()*0.5*p.mass()*(p.U() & p.U());
    }

    return linearKineticEnergy;
}


template<class CloudType>
inline CML::scalar CML::KinematicCloud<CloudType>::Dij
(
    const label i,
    const label j
) const
{
    scalar si = 0.0;
    scalar sj = 0.0;
    forAllConstIter(typename KinematicCloud<CloudType>, *this, iter)
    {
        const parcelType& p = iter();
        si += p.nParticle()*pow(p.d(), i);
        sj += p.nParticle()*pow(p.d(), j);
    }

    reduce(si, sumOp<scalar>());
    reduce(sj, sumOp<scalar>());
    sj = max(sj, VSMALL);

    return si/sj;
}


template<class CloudType>
inline CML::scalar CML::KinematicCloud<CloudType>::Dmax() const
{
    scalar d = -GREAT;
    forAllConstIter(typename KinematicCloud<CloudType>, *this, iter)
    {
        const parcelType& p = iter();
        d = max(d, p.d());
    }

    reduce(d, maxOp<scalar>());

    return max(0.0, d);
}


template<class CloudType>
inline CML::Random& CML::KinematicCloud<CloudType>::rndGen() const
{
    return rndGen_;
}


template<class CloudType>
inline CML::List<CML::DynamicList<typename CloudType::particleType*> >&
CML::KinematicCloud<CloudType>::cellOccupancy()
{
    if (cellOccupancyPtr_.empty())
    {
        buildCellOccupancy();
    }

    return cellOccupancyPtr_();
}


template<class CloudType>
inline const CML::scalarField&
CML::KinematicCloud<CloudType>::cellLengthScale() const
{
    return cellLengthScale_;
}


template<class CloudType>
inline CML::DimensionedField<CML::vector, CML::volMesh>&
CML::KinematicCloud<CloudType>::UTrans()
{
    return UTrans_();
}


template<class CloudType>
inline const CML::DimensionedField<CML::vector, CML::volMesh>&
CML::KinematicCloud<CloudType>::UTrans() const
{
    return UTrans_();
}


template<class CloudType>
inline CML::DimensionedField<CML::scalar, CML::volMesh>&
CML::KinematicCloud<CloudType>::UCoeff()
{
    return UCoeff_();
}


template<class CloudType>
inline const CML::DimensionedField<CML::scalar, CML::volMesh>&
CML::KinematicCloud<CloudType>::UCoeff() const
{
    return UCoeff_();
}


template<class CloudType>
inline CML::tmp<CML::fvVectorMatrix>
CML::KinematicCloud<CloudType>::SU(volVectorField& U) const
{
    if (debug)
    {
        Info<< "UTrans min/max = " << min(UTrans()).value() << ", "
            << max(UTrans()).value() << nl
            << "UCoeff min/max = " << min(UCoeff()).value() << ", "
            << max(UCoeff()).value() << endl;
    }

    if (solution_.coupled())
    {
        if (solution_.semiImplicit("U"))
        {
            const DimensionedField<scalar, volMesh>
                Vdt(mesh_.V()*this->db().time().deltaT());

            return UTrans()/Vdt - fvm::Sp(UCoeff()/Vdt, U) + UCoeff()/Vdt*U;
        }
        else
        {
            tmp<fvVectorMatrix> tfvm(new fvVectorMatrix(U, dimForce));
            fvVectorMatrix& fvm = tfvm();

            fvm.source() = -UTrans()/(this->db().time().deltaT());

            return tfvm;
        }
    }

    return tmp<fvVectorMatrix>(new fvVectorMatrix(U, dimForce));
}


template<class CloudType>
inline const CML::tmp<CML::volScalarField>
CML::KinematicCloud<CloudType>::vDotSweep() const
{
    tmp<volScalarField> tvDotSweep
    (
        new volScalarField
        (
            IOobject
            (
                this->name() + ":vDotSweep",
                this->db().time().timeName(),
                this->db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            mesh_,
            dimensionedScalar("zero", dimless/dimTime, 0.0),
            extrapolatedCalculatedFvPatchScalarField::typeName
        )
    );

    volScalarField& vDotSweep = tvDotSweep();
    forAllConstIter(typename KinematicCloud<CloudType>, *this, iter)
    {
        const parcelType& p = iter();
        const label celli = p.cell();

        vDotSweep[celli] += p.nParticle()*p.areaP()*mag(p.U() - U_[celli]);
    }

    vDotSweep.internalField() /= mesh_.V();
    vDotSweep.correctBoundaryConditions();

    return tvDotSweep;
}


template<class CloudType>
inline const CML::tmp<CML::volScalarField>
CML::KinematicCloud<CloudType>::theta() const
{
    tmp<volScalarField> ttheta
    (
        new volScalarField
        (
            IOobject
            (
                this->name() + ":theta",
                this->db().time().timeName(),
                this->db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            mesh_,
            dimensionedScalar("zero", dimless, 0.0),
            extrapolatedCalculatedFvPatchScalarField::typeName
        )
    );

    volScalarField& theta = ttheta();
    forAllConstIter(typename KinematicCloud<CloudType>, *this, iter)
    {
        const parcelType& p = iter();
        const label celli = p.cell();

        theta[celli] += p.nParticle()*p.volume();
    }

    theta.internalField() /= mesh_.V();
    theta.correctBoundaryConditions();

    return ttheta;
}


template<class CloudType>
inline const CML::tmp<CML::volScalarField>
CML::KinematicCloud<CloudType>::alpha() const
{
    tmp<volScalarField> talpha
    (
        new volScalarField
        (
            IOobject
            (
                this->name() + ":alpha",
                this->db().time().timeName(),
                this->db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            mesh_,
            dimensionedScalar("zero", dimless, 0.0)
        )
    );

    scalarField& alpha = talpha().internalField();
    forAllConstIter(typename KinematicCloud<CloudType>, *this, iter)
    {
        const parcelType& p = iter();
        const label celli = p.cell();

        alpha[celli] += p.nParticle()*p.mass();
    }

    alpha /= (mesh_.V()*rho_);

    return talpha;
}


template<class CloudType>
inline const CML::tmp<CML::volScalarField>
CML::KinematicCloud<CloudType>::rhoEff() const
{
    tmp<volScalarField> trhoEff
    (
        new volScalarField
        (
            IOobject
            (
                this->name() + ":rhoEff",
                this->db().time().timeName(),
                this->db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            mesh_,
            dimensionedScalar("zero", dimDensity, 0.0)
        )
    );

    scalarField& rhoEff = trhoEff().internalField();
    forAllConstIter(typename KinematicCloud<CloudType>, *this, iter)
    {
        const parcelType& p = iter();
        const label celli = p.cell();

        rhoEff[celli] += p.nParticle()*p.mass();
    }

    rhoEff /= mesh_.V();

    return trhoEff;
}


// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

template<class CloudType>
void CML::KinematicCloud<CloudType>::setModels()
{
    dispersionModel_.reset
    (
        DispersionModel<KinematicCloud<CloudType> >::New
        (
            subModelProperties_,
            *this
        ).ptr()
    );

    patchInteractionModel_.reset
    (
        PatchInteractionModel<KinematicCloud<CloudType> >::New
        (
            subModelProperties_,
            *this
        ).ptr()
    );

    stochasticCollisionModel_.reset
    (
        StochasticCollisionModel<KinematicCloud<CloudType> >::New
        (
            subModelProperties_,
            *this
        ).ptr()
    );

    surfaceFilmModel_.reset
    (
        SurfaceFilmModel<KinematicCloud<CloudType> >::New
        (
            subModelProperties_,
            *this
        ).ptr()
    );

    UIntegrator_.reset
    (
        integrationScheme::New
        (
            "U",
            solution_.integrationSchemes()
        ).ptr()
    );
}


template<class CloudType>
template<class TrackCloudType>
void CML::KinematicCloud<CloudType>::solve
(
    TrackCloudType& cloud,
    typename parcelType::trackingData& td
)
{
    addProfiling(prof, "cloud::solve");

    if (solution_.steadyState())
    {
        cloud.storeState();

        cloud.preEvolve();

        evolveCloud(cloud, td);

        if (solution_.coupled())
        {
            cloud.relaxSources(cloud.cloudCopy());
        }
    }
    else
    {
        cloud.preEvolve();

        evolveCloud(cloud, td);

        if (solution_.coupled())
        {
            cloud.scaleSources();
        }
    }

    cloud.info();

    cloud.postEvolve();

    if (solution_.steadyState())
    {
        cloud.restoreState();
    }
}


template<class CloudType>
void CML::KinematicCloud<CloudType>::buildCellOccupancy()
{
    if (cellOccupancyPtr_.empty())
    {
        cellOccupancyPtr_.reset
        (
            new List<DynamicList<parcelType*> >(mesh_.nCells())
        );
    }
    else if (cellOccupancyPtr_().size() != mesh_.nCells())
    {
        // If the size of the mesh has changed, reset the
        // cellOccupancy size

        cellOccupancyPtr_().setSize(mesh_.nCells());
    }

    List<DynamicList<parcelType*> >& cellOccupancy = cellOccupancyPtr_();

    forAll(cellOccupancy, cO)
    {
        cellOccupancy[cO].clear();
    }

    forAllIter(typename KinematicCloud<CloudType>, *this, iter)
    {
        cellOccupancy[iter().cell()].append(&iter());
    }
}


template<class CloudType>
void CML::KinematicCloud<CloudType>::updateCellOccupancy()
{
    // Only build the cellOccupancy if the pointer is set, i.e. it has
    // been requested before.

    if (cellOccupancyPtr_.valid())
    {
        buildCellOccupancy();
    }
}


template<class CloudType>
template<class TrackCloudType>
void CML::KinematicCloud<CloudType>::evolveCloud
(
    TrackCloudType& cloud,
    typename parcelType::trackingData& td
)
{
    if (solution_.coupled())
    {
        cloud.resetSourceTerms();
    }

    if (solution_.transient())
    {
        label preInjectionSize = this->size();

        this->surfaceFilm().inject(cloud);

        // Update the cellOccupancy if the size of the cloud has changed
        // during the injection.
        if (preInjectionSize != this->size())
        {
            updateCellOccupancy();
            preInjectionSize = this->size();
        }

        injectors_.inject(cloud, td);


        // Assume that motion will update the cellOccupancy as necessary
        // before it is required.
        cloud.motion(cloud, td);

        stochasticCollision().update(td, solution_.trackTime());
    }
    else
    {
//        this->surfaceFilm().injectSteadyState(cloud);

        injectors_.injectSteadyState(cloud, td, solution_.trackTime());

        td.part() = parcelType::trackingData::tpLinearTrack;
        CloudType::move(cloud, td, solution_.trackTime());
    }
}


template<class CloudType>
void CML::KinematicCloud<CloudType>::postEvolve()
{
    Info<< endl;

    if (debug)
    {
        this->writePositions();
    }

    this->dispersion().cacheFields(false);

    forces_.cacheFields(false);

    functions_.postEvolve();

    solution_.nextIter();

    if (this->db().time().outputTime())
    {
        outputProperties_.writeObject
        (
            IOstream::ASCII,
            IOstream::currentVersion,
            this->db().time().writeCompression()
        );
    }
}


template<class CloudType>
void CML::KinematicCloud<CloudType>::cloudReset(KinematicCloud<CloudType>& c)
{
    CloudType::cloudReset(c);

    rndGen_ = c.rndGen_;

    forces_.transfer(c.forces_);

    functions_.transfer(c.functions_);

    injectors_.transfer(c.injectors_);

    dispersionModel_.reset(c.dispersionModel_.ptr());
    patchInteractionModel_.reset(c.patchInteractionModel_.ptr());
    stochasticCollisionModel_.reset(c.stochasticCollisionModel_.ptr());
    surfaceFilmModel_.reset(c.surfaceFilmModel_.ptr());

    UIntegrator_.reset(c.UIntegrator_.ptr());
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::KinematicCloud<CloudType>::KinematicCloud
(
    const word& cloudName,
    const volScalarField& rho,
    const volVectorField& U,
    const volScalarField& mu,
    const dimensionedVector& g,
    bool readFields
)
:
    CloudType(rho.mesh(), cloudName, false),
    kinematicCloud(),
    cloudCopyPtr_(nullptr),
    mesh_(rho.mesh()),
    particleProperties_
    (
        IOobject
        (
            cloudName + "Properties",
            rho.mesh().time().constant(),
            rho.mesh(),
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    ),
    outputProperties_
    (
        IOobject
        (
            cloudName + "OutputProperties",
            mesh_.time().timeName(),
            "uniform"/cloud::prefix/cloudName,
            mesh_,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE
        )
    ),
    solution_(mesh_, particleProperties_.subDict("solution")),
    constProps_(particleProperties_),
    subModelProperties_
    (
        particleProperties_.subOrEmptyDict("subModels", solution_.active())
    ),
    rndGen_(0),
    cellOccupancyPtr_(),
    cellLengthScale_(mag(cbrt(mesh_.V()))),
    rho_(rho),
    U_(U),
    mu_(mu),
    g_(g),
    pAmbient_(0.0),
    forces_
    (
        *this,
        mesh_,
        subModelProperties_.subOrEmptyDict
        (
            "particleForces",
            solution_.active()
        ),
        solution_.active()
    ),
    functions_
    (
        *this,
        particleProperties_.subOrEmptyDict("cloudFunctions"),
        solution_.active()
    ),
    injectors_
    (
        subModelProperties_.subOrEmptyDict("injectionModels"),
        *this
    ),
    dispersionModel_(nullptr),
    patchInteractionModel_(nullptr),
    stochasticCollisionModel_(nullptr),
    surfaceFilmModel_(nullptr),
    UIntegrator_(nullptr),
    UTrans_
    (
        new DimensionedField<vector, volMesh>
        (
            IOobject
            (
                this->name() + ":UTrans",
                this->db().time().timeName(),
                this->db(),
                IOobject::READ_IF_PRESENT,
                IOobject::AUTO_WRITE
            ),
            mesh_,
            dimensionedVector("zero", dimMass*dimVelocity, Zero)
        )
    ),
    UCoeff_
    (
        new DimensionedField<scalar, volMesh>
        (
            IOobject
            (
                this->name() + ":UCoeff",
                this->db().time().timeName(),
                this->db(),
                IOobject::READ_IF_PRESENT,
                IOobject::AUTO_WRITE
            ),
            mesh_,
            dimensionedScalar("zero",  dimMass, 0.0)
        )
    )
{
    if (solution_.active())
    {
        setModels();

        if (readFields)
        {
            parcelType::readFields(*this);
            this->deleteLostParticles();
        }
    }

    if (solution_.resetSourcesOnStartup())
    {
        resetSourceTerms();
    }
}


template<class CloudType>
CML::KinematicCloud<CloudType>::KinematicCloud
(
    KinematicCloud<CloudType>& c,
    const word& name
)
:
    CloudType(c.mesh_, name, c),
    kinematicCloud(),
    cloudCopyPtr_(nullptr),
    mesh_(c.mesh_),
    particleProperties_(c.particleProperties_),
    outputProperties_(c.outputProperties_),
    solution_(c.solution_),
    constProps_(c.constProps_),
    subModelProperties_(c.subModelProperties_),
    rndGen_(c.rndGen_),
    cellOccupancyPtr_(nullptr),
    cellLengthScale_(c.cellLengthScale_),
    rho_(c.rho_),
    U_(c.U_),
    mu_(c.mu_),
    g_(c.g_),
    pAmbient_(c.pAmbient_),
    forces_(c.forces_),
    functions_(c.functions_),
    injectors_(c.injectors_),
    dispersionModel_(c.dispersionModel_->clone()),
    patchInteractionModel_(c.patchInteractionModel_->clone()),
    stochasticCollisionModel_(c.stochasticCollisionModel_->clone()),
    surfaceFilmModel_(c.surfaceFilmModel_->clone()),
    UIntegrator_(c.UIntegrator_->clone()),
    UTrans_
    (
        new DimensionedField<vector, volMesh>
        (
            IOobject
            (
                this->name() + ":UTrans",
                this->db().time().timeName(),
                this->db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            c.UTrans_()
        )
    ),
    UCoeff_
    (
        new DimensionedField<scalar, volMesh>
        (
            IOobject
            (
                name + ":UCoeff",
                this->db().time().timeName(),
                this->db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            c.UCoeff_()
        )
    )
{}


template<class CloudType>
CML::KinematicCloud<CloudType>::KinematicCloud
(
    const fvMesh& mesh,
    const word& name,
    const KinematicCloud<CloudType>& c
)
:
    CloudType(mesh, name, IDLList<parcelType>()),
    kinematicCloud(),
    cloudCopyPtr_(nullptr),
    mesh_(mesh),
    particleProperties_
    (
        IOobject
        (
            name + "Properties",
            mesh.time().constant(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        )
    ),
    outputProperties_
    (
        IOobject
        (
            name + "OutputProperties",
            mesh_.time().timeName(),
            "uniform"/cloud::prefix/name,
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        )
    ),
    solution_(mesh),
    constProps_(),
    subModelProperties_(dictionary::null),
    rndGen_(0),
    cellOccupancyPtr_(nullptr),
    cellLengthScale_(c.cellLengthScale_),
    rho_(c.rho_),
    U_(c.U_),
    mu_(c.mu_),
    g_(c.g_),
    pAmbient_(c.pAmbient_),
    forces_(*this, mesh),
    functions_(*this),
    injectors_(*this),
    dispersionModel_(nullptr),
    patchInteractionModel_(nullptr),
    stochasticCollisionModel_(nullptr),
    surfaceFilmModel_(nullptr),
    UIntegrator_(nullptr),
    UTrans_(nullptr),
    UCoeff_(nullptr)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::KinematicCloud<CloudType>::~KinematicCloud()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::KinematicCloud<CloudType>::setParcelThermoProperties
(
    parcelType& parcel,
    const scalar lagrangianDt
)
{
    parcel.rho() = constProps_.rho0();
}


template<class CloudType>
void CML::KinematicCloud<CloudType>::checkParcelProperties
(
    parcelType& parcel,
    const scalar lagrangianDt,
    const bool fullyDescribed
)
{
    const scalar carrierDt = mesh_.time().deltaTValue();
    parcel.stepFraction() = (carrierDt - lagrangianDt)/carrierDt;

    if (parcel.typeId() == -1)
    {
        parcel.typeId() = constProps_.parcelTypeId();
    }
}


template<class CloudType>
void CML::KinematicCloud<CloudType>::storeState()
{
    cloudCopyPtr_.reset
    (
        static_cast<KinematicCloud<CloudType>*>
        (
            clone(this->name() + "Copy").ptr()
        )
    );
}


template<class CloudType>
void CML::KinematicCloud<CloudType>::restoreState()
{
    cloudReset(cloudCopyPtr_());
    cloudCopyPtr_.clear();
}


template<class CloudType>
void CML::KinematicCloud<CloudType>::resetSourceTerms()
{
    UTrans().field() = Zero;
    UCoeff().field() = 0.0;
}


template<class CloudType>
template<class Type>
void CML::KinematicCloud<CloudType>::relax
(
    DimensionedField<Type, volMesh>& field,
    const DimensionedField<Type, volMesh>& field0,
    const word& name
) const
{
    const scalar coeff = solution_.relaxCoeff(name);
    field = field0 + coeff*(field - field0);
}


template<class CloudType>
template<class Type>
void CML::KinematicCloud<CloudType>::scale
(
    DimensionedField<Type, volMesh>& field,
    const word& name
) const
{
    const scalar coeff = solution_.relaxCoeff(name);
    field *= coeff;
}


template<class CloudType>
void CML::KinematicCloud<CloudType>::relaxSources
(
    const KinematicCloud<CloudType>& cloudOldTime
)
{
    this->relax(UTrans_(), cloudOldTime.UTrans(), "U");
    this->relax(UCoeff_(), cloudOldTime.UCoeff(), "U");
}


template<class CloudType>
void CML::KinematicCloud<CloudType>::scaleSources()
{
    this->scale(UTrans_(), "U");
    this->scale(UCoeff_(), "U");
}


template<class CloudType>
void CML::KinematicCloud<CloudType>::preEvolve()
{
    // force calculation of mesh dimensions - needed for parallel runs
    // with topology change due to lazy evaluation of valid mesh dimensions
    label nGeometricD = mesh_.nGeometricD();

    Info<< "\nSolving " << nGeometricD << "-D cloud " << this->name() << endl;

    this->dispersion().cacheFields(true);
    forces_.cacheFields(true);
    updateCellOccupancy();

    pAmbient_ = constProps_.dict().template
        lookupOrDefault<scalar>("pAmbient", pAmbient_);

    functions_.preEvolve();
}


template<class CloudType>
void CML::KinematicCloud<CloudType>::evolve()
{
    if (solution_.canEvolve())
    {
        typename parcelType::trackingData td(*this);

        solve(*this, td);
    }
}


template<class CloudType>
template<class TrackCloudType>
void CML::KinematicCloud<CloudType>::motion
(
    TrackCloudType& cloud,
    typename parcelType::trackingData& td
)
{
    td.part() = parcelType::trackingData::tpLinearTrack;
    CloudType::move(cloud, td, solution_.trackTime());

    updateCellOccupancy();
}


template<class CloudType>
void CML::KinematicCloud<CloudType>::patchData
(
    const parcelType& p,
    const polyPatch& pp,
    vector& nw,
    vector& Up
) const
{
    p.patchData(nw, Up);

    // If this is a wall patch, then there may be a non-zero tangential velocity
    // component; the lid velocity in a lid-driven cavity case, for example. We
    // want the particle to interact with this velocity, so we look it up in the
    // velocity field and use it to set the wall-tangential component.
    if (!mesh_.moving() && isA<wallPolyPatch>(pp))
    {
        const label patchi = pp.index();
        const label patchFacei = pp.whichFace(p.face());

        // We only want to use the boundary condition value only if it is set
        // by the boundary condition. If the boundary values are extrapolated
        // (e.g., slip conditions) then they represent the motion of the fluid
        // just inside the domain rather than that of the wall itself.
        if (U_.boundaryField()[patchi].fixesValue())
        {
            const vector Uw1 = U_.boundaryField()[patchi][patchFacei];
            const vector& Uw0 =
                U_.oldTime().boundaryField()[patchi][patchFacei];

            const scalar f = p.currentTimeFraction();

            const vector Uw = Uw0 + f*(Uw1 - Uw0);

            const tensor nnw = nw*nw;

            Up = (nnw & Up) + Uw - (nnw & Uw);
        }
    }
}


template<class CloudType>
void CML::KinematicCloud<CloudType>::updateMesh()
{
    updateCellOccupancy();
    injectors_.updateMesh();
    cellLengthScale_ = mag(cbrt(mesh_.V()));
}


template<class CloudType>
void CML::KinematicCloud<CloudType>::autoMap(const mapPolyMesh& mapper)
{
    Cloud<parcelType>::autoMap(mapper);

    updateMesh();
}


template<class CloudType>
void CML::KinematicCloud<CloudType>::info()
{
    vector linearMomentum = linearMomentumOfSystem();
    reduce(linearMomentum, sumOp<vector>());

    scalar linearKineticEnergy = linearKineticEnergyOfSystem();
    reduce(linearKineticEnergy, sumOp<scalar>());

    Info<< "Cloud: " << this->name() << nl
        << "    Current number of parcels       = "
        << returnReduce(this->size(), sumOp<label>()) << nl
        << "    Current mass in system          = "
        << returnReduce(massInSystem(), sumOp<scalar>()) << nl
        << "    Linear momentum                 = "
        << linearMomentum << nl
        << "   |Linear momentum|                = "
        << mag(linearMomentum) << nl
        << "    Linear kinetic energy           = "
        << linearKineticEnergy << nl;

    injectors_.info(Info);
    this->surfaceFilm().info(Info);
    this->patchInteraction().info(Info);
}


#endif

// ************************************************************************* //
