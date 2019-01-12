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
    CML::ReactingCloud

Description
    Templated base class for reacting cloud

    - Adds to thermodynamic cloud
      - Variable composition (single phase)
      - Phase change

\*---------------------------------------------------------------------------*/

#ifndef ReactingCloud__H
#define ReactingCloud__H

#include "reactingCloud.hpp"
#include "CompositionModel.hpp"
#include "PhaseChangeModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes

template<class CloudType>
class CompositionModel;

template<class CloudType>
class PhaseChangeModel;

/*---------------------------------------------------------------------------*\
                      Class ReactingCloud Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ReactingCloud
:
    public CloudType,
    public reactingCloud
{
public:

    // Public typedefs

        //- Type of cloud this cloud was instantiated for
        typedef CloudType cloudType;

        //- Type of parcel the cloud was instantiated for
        typedef typename CloudType::particleType parcelType;

        //- Convenience typedef for this cloud type
        typedef ReactingCloud<CloudType> reactingCloudType;


private:

    // Private data

        //- Cloud copy pointer
        autoPtr<ReactingCloud<CloudType> > cloudCopyPtr_;


    // Private member functions

        //- Disallow default bitwise copy construct
        ReactingCloud(const ReactingCloud&);

        //- Disallow default bitwise assignment
        void operator=(const ReactingCloud&);


protected:

    // Protected data

        //- Parcel constant properties
        typename parcelType::constantProperties constProps_;


        // References to the cloud sub-models

            //- Reacting composition model
            autoPtr<CompositionModel<ReactingCloud<CloudType> > >
                compositionModel_;

            //- Reacting phase change model
            autoPtr<PhaseChangeModel<ReactingCloud<CloudType> > >
                phaseChangeModel_;


        // Sources

            //- Mass transfer fields - one per carrier phase specie
            PtrList<DimensionedField<scalar, volMesh> > rhoTrans_;


    // Protected Member Functions

        // New parcel helper functions

            //- Check that size of a composition field is valid
            void checkSuppliedComposition
            (
                const scalarField& YSupplied,
                const scalarField& Y,
                const word& YName
            );


        // Initialisation

            //- Set cloud sub-models
            void setModels();


        // Cloud evolution functions

            //- Reset state of cloud
            void cloudReset(ReactingCloud<CloudType>& c);


public:

    // Constructors

        //- Construct given carrier gas fields
        ReactingCloud
        (
            const word& cloudName,
            const volScalarField& rho,
            const volVectorField& U,
            const dimensionedVector& g,
            const SLGThermo& thermo,
            bool readFields = true
        );

        //- Copy constructor with new name
        ReactingCloud(ReactingCloud<CloudType>& c, const word& name);

        //- Copy constructor with new name - creates bare cloud
        ReactingCloud
        (
            const fvMesh& mesh,
            const word& name,
            const ReactingCloud<CloudType>& c
        );

        //- Construct and return clone based on (this) with new name
        virtual autoPtr<Cloud<parcelType> > clone(const word& name)
        {
            return autoPtr<Cloud<parcelType> >
            (
                new ReactingCloud(*this, name)
            );
        }

        //- Construct and return bare clone based on (this) with new name
        virtual autoPtr<Cloud<parcelType> > cloneBare(const word& name) const
        {
            return autoPtr<Cloud<parcelType> >
            (
                new ReactingCloud(this->mesh(), name, *this)
            );
        }


    //- Destructor
    virtual ~ReactingCloud();


    // Member Functions

        // Access

            //- Return a reference to the cloud copy
            inline const ReactingCloud& cloudCopy() const;

            //- Return the constant properties
            inline const typename parcelType::constantProperties&
                constProps() const;

            //- Return access to the constant properties
            inline typename parcelType::constantProperties& constProps();


            // Sub-models

                //- Return const access to reacting composition model
                inline const CompositionModel<ReactingCloud<CloudType> >&
                    composition() const;

                //- Return const access to reacting phase change model
                inline const PhaseChangeModel<ReactingCloud<CloudType> >&
                    phaseChange() const;

                //- Return reference to reacting phase change model
                inline PhaseChangeModel<ReactingCloud<CloudType> >&
                    phaseChange();


            // Sources

                //- Mass

                    //- Return reference to mass source for field i
                    inline DimensionedField<scalar, volMesh>&
                        rhoTrans(const label i);

                    //- Return const access to mass source fields
                    inline const PtrList<DimensionedField<scalar, volMesh> >&
                        rhoTrans() const;

                    //- Return reference to mass source fields
                    inline PtrList<DimensionedField<scalar, volMesh> >&
                        rhoTrans();

                    //- Return mass source term for specie i - specie eqn
                    inline tmp<fvScalarMatrix> SYi
                    (
                        const label i,
                        volScalarField& Yi
                    ) const;

                    //- Return tmp mass source for field i - fully explicit
                    inline tmp<DimensionedField<scalar, volMesh> >
                        Srho(const label i) const;

                    //- Return tmp total mass source for carrier phase
                    //  - fully explicit
                    inline tmp<DimensionedField<scalar, volMesh> > Srho() const;

                    //- Return total mass source term [kg/m3/s]
                    inline tmp<fvScalarMatrix> Srho(volScalarField& rho) const;


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

            //- Apply relaxation to (steady state) cloud sources
            void relaxSources(const ReactingCloud<CloudType>& cloudOldTime);

            //- Apply scaling to (transient) cloud sources
            void scaleSources();

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
inline const CML::ReactingCloud<CloudType>&
CML::ReactingCloud<CloudType>::cloudCopy() const
{
    return cloudCopyPtr_();
}


template<class CloudType>
inline const typename CloudType::particleType::constantProperties&
CML::ReactingCloud<CloudType>::constProps() const
{
    return constProps_;
}


template<class CloudType>
inline typename CloudType::particleType::constantProperties&
CML::ReactingCloud<CloudType>::constProps()
{
    return constProps_;
}


template<class CloudType>
inline const CML::CompositionModel<CML::ReactingCloud<CloudType> >&
CML::ReactingCloud<CloudType>::composition() const
{
    return compositionModel_;
}


template<class CloudType>
inline const CML::PhaseChangeModel<CML::ReactingCloud<CloudType> >&
CML::ReactingCloud<CloudType>::phaseChange() const
{
    return phaseChangeModel_;
}


template<class CloudType>
inline CML::PhaseChangeModel<CML::ReactingCloud<CloudType> >&
CML::ReactingCloud<CloudType>::phaseChange()
{
    return phaseChangeModel_();
}


template<class CloudType>
inline CML::DimensionedField<CML::scalar, CML::volMesh>&
CML::ReactingCloud<CloudType>::rhoTrans(const label i)
{
    return rhoTrans_[i];
}


template<class CloudType>
inline
const CML::PtrList<CML::DimensionedField<CML::scalar, CML::volMesh> >&
CML::ReactingCloud<CloudType>::rhoTrans() const
{
    return rhoTrans_;
}


template<class CloudType>
inline CML::PtrList<CML::DimensionedField<CML::scalar, CML::volMesh> >&
CML::ReactingCloud<CloudType>::rhoTrans()
{
    return rhoTrans_;
}


template<class CloudType>
inline CML::tmp<CML::fvScalarMatrix> CML::ReactingCloud<CloudType>::SYi
(
    const label i,
    volScalarField& Yi
) const
{
    if (this->solution().coupled())
    {
        if (this->solution().semiImplicit("Yi"))
        {
            tmp<volScalarField> trhoTrans
            (
                new volScalarField
                (
                    IOobject
                    (
                        this->name() + ":rhoTrans",
                        this->db().time().timeName(),
                        this->db(),
                        IOobject::NO_READ,
                        IOobject::NO_WRITE,
                        false
                    ),
                    this->mesh(),
                    dimensionedScalar("zero", dimMass/dimTime/dimVolume, 0.0)
                )
            );

            volScalarField& sourceField = trhoTrans();

            sourceField.internalField() =
                rhoTrans_[i]/(this->db().time().deltaTValue()*this->mesh().V());

            const dimensionedScalar YiSMALL("YiSMALL", dimless, SMALL);

            return
                fvm::Sp(neg(sourceField)*sourceField/(Yi + YiSMALL), Yi)
              + pos(sourceField)*sourceField;
        }
        else
        {
            tmp<fvScalarMatrix> tfvm(new fvScalarMatrix(Yi, dimMass/dimTime));
            fvScalarMatrix& fvm = tfvm();

            fvm.source() = -rhoTrans_[i]/this->db().time().deltaTValue();

            return tfvm;
        }
    }

    return tmp<fvScalarMatrix>(new fvScalarMatrix(Yi, dimMass/dimTime));
}


template<class CloudType>
inline CML::tmp<CML::DimensionedField<CML::scalar, CML::volMesh> >
CML::ReactingCloud<CloudType>::Srho(const label i) const
{
    tmp<DimensionedField<scalar, volMesh> > tRhoi
    (
        new DimensionedField<scalar, volMesh>
        (
            IOobject
            (
                this->name() + ":rhoTrans",
                this->db().time().timeName(),
                this->db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            this->mesh(),
            dimensionedScalar
            (
                "zero",
                rhoTrans_[0].dimensions()/dimTime/dimVolume,
                0.0
            )
        )
    );

    if (this->solution().coupled())
    {
        scalarField& rhoi = tRhoi();
        rhoi = rhoTrans_[i]/(this->db().time().deltaTValue()*this->mesh().V());
    }

    return tRhoi;
}


template<class CloudType>
inline CML::tmp<CML::DimensionedField<CML::scalar, CML::volMesh> >
CML::ReactingCloud<CloudType>::Srho() const
{
    tmp<DimensionedField<scalar, volMesh> > trhoTrans
    (
        new DimensionedField<scalar, volMesh>
        (
            IOobject
            (
                this->name() + ":rhoTrans",
                this->db().time().timeName(),
                this->db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            this->mesh(),
            dimensionedScalar
            (
                "zero",
                rhoTrans_[0].dimensions()/dimTime/dimVolume,
                0.0
            )
        )
    );

    if (this->solution().coupled())
    {
        scalarField& sourceField = trhoTrans();
        forAll(rhoTrans_, i)
        {
            sourceField += rhoTrans_[i];
        }

        sourceField /= this->db().time().deltaTValue()*this->mesh().V();
    }

    return trhoTrans;
}


template<class CloudType>
inline CML::tmp<CML::fvScalarMatrix>
CML::ReactingCloud<CloudType>::Srho(volScalarField& rho) const
{
    if (this->solution().coupled())
    {
        tmp<volScalarField> trhoTrans
        (
            new volScalarField
            (
                IOobject
                (
                    this->name() + ":rhoTrans",
                    this->db().time().timeName(),
                    this->db(),
                    IOobject::NO_READ,
                    IOobject::NO_WRITE,
                    false
                ),
                this->mesh(),
                dimensionedScalar("zero", dimMass/dimTime/dimVolume, 0.0)
            )
        );

        scalarField& sourceField = trhoTrans();

        if (this->solution().semiImplicit("rho"))
        {

            forAll(rhoTrans_, i)
            {
                sourceField += rhoTrans_[i];
            }
            sourceField /= this->db().time().deltaTValue()*this->mesh().V();

            return fvm::SuSp(trhoTrans()/rho, rho);
        }
        else
        {
            tmp<fvScalarMatrix> tfvm(new fvScalarMatrix(rho, dimMass/dimTime));
            fvScalarMatrix& fvm = tfvm();

            forAll(rhoTrans_, i)
            {
                sourceField += rhoTrans_[i];
            }

            fvm.source() = -trhoTrans()/this->db().time().deltaT();

            return tfvm;
        }
    }

    return tmp<fvScalarMatrix>(new fvScalarMatrix(rho, dimMass/dimTime));
}


// * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * * //

template<class CloudType>
void CML::ReactingCloud<CloudType>::setModels()
{
    compositionModel_.reset
    (
        CompositionModel<ReactingCloud<CloudType> >::New
        (
            this->subModelProperties(),
            *this
        ).ptr()
    );

    phaseChangeModel_.reset
    (
        PhaseChangeModel<ReactingCloud<CloudType> >::New
        (
            this->subModelProperties(),
            *this
        ).ptr()
    );
}


template<class CloudType>
void CML::ReactingCloud<CloudType>::checkSuppliedComposition
(
    const scalarField& YSupplied,
    const scalarField& Y,
    const word& YName
)
{
    if (YSupplied.size() != Y.size())
    {
        FatalErrorInFunction
            << YName << " supplied, but size is not compatible with "
            << "parcel composition: " << nl << "    "
            << YName << "(" << YSupplied.size() << ") vs required composition "
            << YName << "(" << Y.size() << ")" << nl
            << abort(FatalError);
    }
}


template<class CloudType>
void CML::ReactingCloud<CloudType>::cloudReset(ReactingCloud<CloudType>& c)
{
    CloudType::cloudReset(c);

    compositionModel_.reset(c.compositionModel_.ptr());
    phaseChangeModel_.reset(c.phaseChangeModel_.ptr());
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ReactingCloud<CloudType>::ReactingCloud
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
    reactingCloud(),
    cloudCopyPtr_(nullptr),
    constProps_(this->particleProperties()),
    compositionModel_(nullptr),
    phaseChangeModel_(nullptr),
    rhoTrans_(thermo.carrier().species().size())
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

    // Set storage for mass source fields and initialise to zero
    forAll(rhoTrans_, i)
    {
        const word& specieName = thermo.carrier().species()[i];
        rhoTrans_.set
        (
            i,
            new DimensionedField<scalar, volMesh>
            (
                IOobject
                (
                    this->name() + ":rhoTrans_" + specieName,
                    this->db().time().timeName(),
                    this->db(),
                    IOobject::READ_IF_PRESENT,
                    IOobject::AUTO_WRITE
                ),
                this->mesh(),
                dimensionedScalar("zero", dimMass, 0.0)
            )
        );
    }

    if (this->solution().resetSourcesOnStartup())
    {
        resetSourceTerms();
    }
}


template<class CloudType>
CML::ReactingCloud<CloudType>::ReactingCloud
(
    ReactingCloud<CloudType>& c,
    const word& name
)
:
    CloudType(c, name),
    reactingCloud(),
    cloudCopyPtr_(nullptr),
    constProps_(c.constProps_),
    compositionModel_(c.compositionModel_->clone()),
    phaseChangeModel_(c.phaseChangeModel_->clone()),
    rhoTrans_(c.rhoTrans_.size())
{
    forAll(c.rhoTrans_, i)
    {
        const word& specieName = this->thermo().carrier().species()[i];
        rhoTrans_.set
        (
            i,
            new DimensionedField<scalar, volMesh>
            (
                IOobject
                (
                    this->name() + ":rhoTrans_" + specieName,
                    this->db().time().timeName(),
                    this->db(),
                    IOobject::NO_READ,
                    IOobject::NO_WRITE,
                    false
                ),
                c.rhoTrans_[i]
            )
        );
    }
}


template<class CloudType>
CML::ReactingCloud<CloudType>::ReactingCloud
(
    const fvMesh& mesh,
    const word& name,
    const ReactingCloud<CloudType>& c
)
:
    CloudType(mesh, name, c),
    reactingCloud(),
    cloudCopyPtr_(nullptr),
    constProps_(),
    compositionModel_(c.compositionModel_->clone()),
//    compositionModel_(nullptr),
    phaseChangeModel_(nullptr),
    rhoTrans_(0)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::ReactingCloud<CloudType>::~ReactingCloud()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::ReactingCloud<CloudType>::setParcelThermoProperties
(
    parcelType& parcel,
    const scalar lagrangianDt
)
{
    CloudType::setParcelThermoProperties(parcel, lagrangianDt);

    parcel.Y() = composition().YMixture0();
}


template<class CloudType>
void CML::ReactingCloud<CloudType>::checkParcelProperties
(
    parcelType& parcel,
    const scalar lagrangianDt,
    const bool fullyDescribed
)
{
    CloudType::checkParcelProperties(parcel, lagrangianDt, fullyDescribed);

    if (fullyDescribed)
    {
        checkSuppliedComposition
        (
            parcel.Y(),
            composition().YMixture0(),
            "YMixture"
        );
    }

    // derived information - store initial mass
    parcel.mass0() = parcel.mass();
}


template<class CloudType>
void CML::ReactingCloud<CloudType>::storeState()
{
    cloudCopyPtr_.reset
    (
        static_cast<ReactingCloud<CloudType>*>
        (
            clone(this->name() + "Copy").ptr()
        )
    );
}


template<class CloudType>
void CML::ReactingCloud<CloudType>::restoreState()
{
    cloudReset(cloudCopyPtr_());
    cloudCopyPtr_.clear();
}


template<class CloudType>
void CML::ReactingCloud<CloudType>::resetSourceTerms()
{
    CloudType::resetSourceTerms();
    forAll(rhoTrans_, i)
    {
        rhoTrans_[i].field() = 0.0;
    }
}


template<class CloudType>
void CML::ReactingCloud<CloudType>::relaxSources
(
    const ReactingCloud<CloudType>& cloudOldTime
)
{
    CloudType::relaxSources(cloudOldTime);

    typedef DimensionedField<scalar, volMesh> dsfType;

    forAll(rhoTrans_, fieldi)
    {
        dsfType& rhoT = rhoTrans_[fieldi];
        const dsfType& rhoT0 = cloudOldTime.rhoTrans()[fieldi];
        this->relax(rhoT, rhoT0, "rho");
    }
}


template<class CloudType>
void CML::ReactingCloud<CloudType>::scaleSources()
{
    CloudType::scaleSources();

    typedef DimensionedField<scalar, volMesh> dsfType;

    forAll(rhoTrans_, fieldi)
    {
        dsfType& rhoT = rhoTrans_[fieldi];
        this->scale(rhoT, "rho");
    }
}


template<class CloudType>
void CML::ReactingCloud<CloudType>::evolve()
{
    if (this->solution().canEvolve())
    {
        typename parcelType::trackingData td(*this);

        this->solve(*this, td);
    }
}


template<class CloudType>
void CML::ReactingCloud<CloudType>::autoMap(const mapPolyMesh& mapper)
{
    Cloud<parcelType>::autoMap(mapper);

    this->updateMesh();
}


template<class CloudType>
void CML::ReactingCloud<CloudType>::info()
{
    CloudType::info();

    this->phaseChange().info(Info);
}


template<class CloudType>
void CML::ReactingCloud<CloudType>::writeFields() const
{
    if (compositionModel_.valid())
    {
        CloudType::particleType::writeFields(*this, this->composition());
    }
}


#endif

// ************************************************************************* //
