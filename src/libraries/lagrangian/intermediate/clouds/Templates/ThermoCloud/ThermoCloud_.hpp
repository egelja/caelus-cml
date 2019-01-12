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
    CML::ThermoCloud

Description
    Templated base class for thermodynamic cloud

    - Adds to kinematic cloud
      - Heat transfer


\*---------------------------------------------------------------------------*/

#ifndef ThermoCloud__H
#define ThermoCloud__H

#include "KinematicCloud_.hpp"
#include "thermoCloud.hpp"
#include "SLGThermo.hpp"
#include "physicoChemicalConstants.hpp"
#include "ThermoParcel.hpp"

#include "HeatTransferModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes

template<class CloudType>
class HeatTransferModel;

/*---------------------------------------------------------------------------*\
                        Class ThermoCloud Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ThermoCloud
:
    public CloudType,
    public thermoCloud
{
public:

    // Public typedefs

        //- Type of cloud this cloud was instantiated for
        typedef CloudType cloudType;

        //- Type of parcel the cloud was instantiated for
        typedef typename CloudType::particleType parcelType;

        //- Convenience typedef for this cloud type
        typedef ThermoCloud<CloudType> thermoCloudType;


private:

    // Private data

        //- Cloud copy pointer
        autoPtr<ThermoCloud<CloudType> > cloudCopyPtr_;


    // Private member functions

        //- Disallow default bitwise copy construct
        ThermoCloud(const ThermoCloud&);

        //- Disallow default bitwise assignment
        void operator=(const ThermoCloud&);


protected:

    // Protected data

        //- Thermo parcel constant properties
        typename parcelType::constantProperties constProps_;


        // References to the carrier gas fields

            //- SLG thermodynamics package
            const SLGThermo& thermo_;

            //- Temperature [K]
            const volScalarField& T_;

            //- Pressure [Pa]
            const volScalarField& p_;


        // References to the cloud sub-models

            //- Heat transfer model
            autoPtr<HeatTransferModel<ThermoCloud<CloudType> > >
                heatTransferModel_;


        // Reference to the particle integration schemes

            //- Temperature integration
            autoPtr<integrationScheme> TIntegrator_;


        // Modelling options

            //- Include radiation
            Switch radiation_;

            //- Radiation sum of parcel projected areas
            autoPtr<DimensionedField<scalar, volMesh> > radAreaP_;

            //- Radiation sum of parcel temperature^4
            autoPtr<DimensionedField<scalar, volMesh> > radT4_;

            //- Radiation sum of parcel projected areas * temperature^4
            autoPtr<DimensionedField<scalar, volMesh> > radAreaPT4_;


        // Sources

            //- Sensible enthalpy transfer [J/kg]
            autoPtr<DimensionedField<scalar, volMesh> > hsTrans_;

            //- Coefficient for carrier phase hs equation [W/K]
            autoPtr<DimensionedField<scalar, volMesh> > hsCoeff_;


    // Protected Member Functions

         // Initialisation

            //- Set cloud sub-models
            void setModels();


        // Cloud evolution functions

            //- Reset state of cloud
            void cloudReset(ThermoCloud<CloudType>& c);


public:

    // Constructors

        //- Construct given carrier gas fields
        ThermoCloud
        (
            const word& cloudName,
            const volScalarField& rho,
            const volVectorField& U,
            const dimensionedVector& g,
            const SLGThermo& thermo,
            bool readFields = true
        );

        //- Copy constructor with new name
        ThermoCloud(ThermoCloud<CloudType>& c, const word& name);

        //- Copy constructor with new name - creates bare cloud
        ThermoCloud
        (
            const fvMesh& mesh,
            const word& name,
            const ThermoCloud<CloudType>& c
        );

        //- Construct and return clone based on (this) with new name
        virtual autoPtr<Cloud<parcelType> > clone(const word& name)
        {
            return autoPtr<Cloud<parcelType> >
            (
                new ThermoCloud(*this, name)
            );
        }

        //- Construct and return bare clone based on (this) with new name
        virtual autoPtr<Cloud<parcelType> > cloneBare(const word& name) const
        {
            return autoPtr<Cloud<parcelType> >
            (
                new ThermoCloud(this->mesh(), name, *this)
            );
        }


    //- Destructor
    virtual ~ThermoCloud();


    // Member Functions

        // Access

            //- Return a reference to the cloud copy
            inline const ThermoCloud& cloudCopy() const;

            //- Return the constant properties
            inline const typename parcelType::constantProperties&
                constProps() const;

            //- Return access to the constant properties
            inline typename parcelType::constantProperties& constProps();

            //- Return const access to thermo package
            inline const SLGThermo& thermo() const;

            //- Return const access to the carrier temperature field
            inline const volScalarField& T() const;

            //- Return const access to the carrier prressure field
            inline const volScalarField& p() const;


            // Sub-models

                //- Return reference to heat transfer model
                inline const HeatTransferModel<ThermoCloud<CloudType> >&
                    heatTransfer() const;


            // Integration schemes

                //-Return reference to velocity integration
                inline const integrationScheme& TIntegrator() const;


            // Modelling options

                 //- Radiation flag
                inline bool radiation() const;

                //- Radiation sum of parcel projected areas [m2]
                inline DimensionedField<scalar, volMesh>& radAreaP();

                //- Radiation sum of parcel projected areas [m2]
                inline const DimensionedField<scalar, volMesh>&
                    radAreaP() const;

                //- Radiation sum of parcel temperature^4 [K4]
                inline DimensionedField<scalar, volMesh>& radT4();

                //- Radiation sum of parcel temperature^4 [K4]
                inline const DimensionedField<scalar, volMesh>& radT4() const;

                //- Radiation sum of parcel projected area*temperature^4 [m2K4]
                inline DimensionedField<scalar, volMesh>& radAreaPT4();

                //- Radiation sum of parcel temperature^4 [m2K4]
                inline const DimensionedField<scalar, volMesh>&
                    radAreaPT4() const;


            // Sources

                // Enthalpy

                    //- Sensible enthalpy transfer [J/kg]
                    inline DimensionedField<scalar, volMesh>& hsTrans();

                    //- Sensible enthalpy transfer [J/kg]
                    inline const DimensionedField<scalar, volMesh>&
                        hsTrans() const;

                    //- Return coefficient for carrier phase hs equation
                    inline DimensionedField<scalar, volMesh>& hsCoeff();

                    //- Return const coefficient for carrier phase hs equation
                    inline const DimensionedField<scalar, volMesh>&
                        hsCoeff() const;

                    //- Return sensible enthalpy source term [J/kg/m3/s]
                    inline tmp<fvScalarMatrix> Sh(volScalarField& hs) const;


                // Radiation - overrides thermoCloud virtual abstract members

                    //- Return tmp equivalent particulate emission
                    inline tmp<volScalarField> Ep() const;

                    //- Return tmp equivalent particulate absorption
                    inline tmp<volScalarField> ap() const;

                    //- Return tmp equivalent particulate scattering factor
                    inline tmp<volScalarField> sigmap() const;


        // Check

            //- Maximum temperature
            inline scalar Tmax() const;

            //- Minimum temperature
            inline scalar Tmin() const;


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
            void relaxSources(const ThermoCloud<CloudType>& cloudOldTime);

            //- Apply scaling to (transient) cloud sources
            void scaleSources();

            //- Pre-evolve
            void preEvolve();

            //- Evolve the cloud
            void evolve();


        // Mapping

            //- Remap the cells of particles corresponding to the
            //  mesh topology change with a default tracking data object
            virtual void autoMap(const mapPolyMesh&);


        // I-O

            //- Print cloud information
            void info();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


using namespace CML::constant;

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
inline const CML::ThermoCloud<CloudType>&
CML::ThermoCloud<CloudType>::cloudCopy() const
{
    return cloudCopyPtr_();
}


template<class CloudType>
inline const typename CloudType::particleType::constantProperties&
CML::ThermoCloud<CloudType>::constProps() const
{
    return constProps_;
}


template<class CloudType>
inline typename CloudType::particleType::constantProperties&
CML::ThermoCloud<CloudType>::constProps()
{
    return constProps_;
}


template<class CloudType>
inline const CML::SLGThermo& CML::ThermoCloud<CloudType>::thermo() const
{
    return thermo_;
}


template<class CloudType>
inline const CML::volScalarField& CML::ThermoCloud<CloudType>::T() const
{
    return T_;
}


template<class CloudType>
inline const CML::volScalarField& CML::ThermoCloud<CloudType>::p() const
{
    return p_;
}


template<class CloudType>
inline const CML::HeatTransferModel<CML::ThermoCloud<CloudType> >&
CML::ThermoCloud<CloudType>::heatTransfer() const
{
    return heatTransferModel_;
}


template<class CloudType>
inline const CML::integrationScheme&
CML::ThermoCloud<CloudType>::TIntegrator() const
{
    return TIntegrator_;
}


template<class CloudType>
inline bool CML::ThermoCloud<CloudType>::radiation() const
{
    return radiation_;
}


template<class CloudType>
inline CML::DimensionedField<CML::scalar, CML::volMesh>&
CML::ThermoCloud<CloudType>::radAreaP()
{
    if (!radiation_)
    {
        FatalErrorInFunction
            << "Radiation field requested, but radiation model not active"
            << abort(FatalError);
    }

    return radAreaP_();
}


template<class CloudType>
inline const CML::DimensionedField<CML::scalar, CML::volMesh>&
CML::ThermoCloud<CloudType>::radAreaP() const
{
    if (!radiation_)
    {
        FatalErrorInFunction
            << "Radiation field requested, but radiation model not active"
            << abort(FatalError);
    }

    return radAreaP_();
}


template<class CloudType>
inline CML::DimensionedField<CML::scalar, CML::volMesh>&
CML::ThermoCloud<CloudType>::radT4()
{
    if (!radiation_)
    {
        FatalErrorInFunction
            << "Radiation field requested, but radiation model not active"
            << abort(FatalError);
    }

    return radT4_();
}


template<class CloudType>
inline const CML::DimensionedField<CML::scalar, CML::volMesh>&
CML::ThermoCloud<CloudType>::radT4() const
{
    if (!radiation_)
    {
        FatalErrorInFunction
            << "Radiation field requested, but radiation model not active"
            << abort(FatalError);
    }

    return radT4_();
}


template<class CloudType>
inline CML::DimensionedField<CML::scalar, CML::volMesh>&
CML::ThermoCloud<CloudType>::radAreaPT4()
{
    if (!radiation_)
    {
        FatalErrorInFunction
            << "Radiation field requested, but radiation model not active"
            << abort(FatalError);
    }

    return radAreaPT4_();
}


template<class CloudType>
inline const CML::DimensionedField<CML::scalar, CML::volMesh>&
CML::ThermoCloud<CloudType>::radAreaPT4() const
{
    if (!radiation_)
    {
        FatalErrorInFunction
            << "Radiation field requested, but radiation model not active"
            << abort(FatalError);
    }

    return radAreaPT4_();
}


template<class CloudType>
inline CML::DimensionedField<CML::scalar, CML::volMesh>&
CML::ThermoCloud<CloudType>::hsTrans()
{
    return hsTrans_();
}


template<class CloudType>
inline const CML::DimensionedField<CML::scalar, CML::volMesh>&
CML::ThermoCloud<CloudType>::hsTrans() const
{
    return hsTrans_();
}


template<class CloudType>
inline CML::DimensionedField<CML::scalar, CML::volMesh>&
CML::ThermoCloud<CloudType>::hsCoeff()
{
    return hsCoeff_();
}


template<class CloudType>
inline const CML::DimensionedField<CML::scalar, CML::volMesh>&
CML::ThermoCloud<CloudType>::hsCoeff() const
{
    return hsCoeff_();
}


template<class CloudType>
inline CML::tmp<CML::fvScalarMatrix>
CML::ThermoCloud<CloudType>::Sh(volScalarField& hs) const
{
    if (debug)
    {
        Info<< "hsTrans min/max = " << min(hsTrans()).value() << ", "
            << max(hsTrans()).value() << nl
            << "hsCoeff min/max = " << min(hsCoeff()).value() << ", "
            << max(hsCoeff()).value() << endl;
    }

    if (this->solution().coupled())
    {
        if (this->solution().semiImplicit("h"))
        {
            const volScalarField Cp(thermo_.thermo().Cp());
            const DimensionedField<scalar, volMesh>
                Vdt(this->mesh().V()*this->db().time().deltaT());

            return
                hsTrans()/Vdt
              - fvm::SuSp(hsCoeff()/(Cp*Vdt), hs)
              + hsCoeff()/(Cp*Vdt)*hs;
        }
        else
        {
            tmp<fvScalarMatrix> tfvm(new fvScalarMatrix(hs, dimEnergy/dimTime));
            fvScalarMatrix& fvm = tfvm();

            fvm.source() = -hsTrans()/(this->db().time().deltaT());

            return tfvm;
        }
    }

    return tmp<fvScalarMatrix>(new fvScalarMatrix(hs, dimEnergy/dimTime));
}


template<class CloudType>
inline CML::tmp<CML::volScalarField> CML::ThermoCloud<CloudType>::Ep() const
{
    tmp<volScalarField> tEp
    (
        new volScalarField
        (
            IOobject
            (
                this->name() + ":radiation:Ep",
                this->db().time().timeName(),
                this->db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            this->mesh(),
            dimensionedScalar("zero", dimMass/dimLength/pow3(dimTime), 0.0)
        )
    );

    if (radiation_)
    {
        scalarField& Ep = tEp().internalField();
        const scalar dt = this->db().time().deltaTValue();
        const scalarField& V = this->mesh().V();
        const scalar epsilon = constProps_.epsilon0();
        const scalarField& sumAreaPT4 = radAreaPT4_->field();

        Ep = sumAreaPT4*epsilon*physicoChemical::sigma.value()/V/dt;
    }

    return tEp;
}


template<class CloudType>
inline CML::tmp<CML::volScalarField> CML::ThermoCloud<CloudType>::ap() const
{
    tmp<volScalarField> tap
    (
        new volScalarField
        (
            IOobject
            (
                this->name() + ":radiation:ap",
                this->db().time().timeName(),
                this->db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            this->mesh(),
            dimensionedScalar("zero", dimless/dimLength, 0.0)
        )
    );

    if (radiation_)
    {
        scalarField& ap = tap().internalField();
        const scalar dt = this->db().time().deltaTValue();
        const scalarField& V = this->mesh().V();
        const scalar epsilon = constProps_.epsilon0();
        const scalarField& sumAreaP = radAreaP_->field();

        ap = sumAreaP*epsilon/V/dt;
    }

    return tap;
}


template<class CloudType>
inline CML::tmp<CML::volScalarField>
CML::ThermoCloud<CloudType>::sigmap() const
{
    tmp<volScalarField> tsigmap
    (
        new volScalarField
        (
            IOobject
            (
                this->name() + ":radiation:sigmap",
                this->db().time().timeName(),
                this->db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            this->mesh(),
            dimensionedScalar("zero", dimless/dimLength, 0.0)
        )
    );

    if (radiation_)
    {
        scalarField& sigmap = tsigmap().internalField();
        const scalar dt = this->db().time().deltaTValue();
        const scalarField& V = this->mesh().V();
        const scalar epsilon = constProps_.epsilon0();
        const scalar f = constProps_.f0();
        const scalarField& sumAreaP = radAreaP_->field();

        sigmap *= sumAreaP*(1.0 - f)*(1.0 - epsilon)/V/dt;
    }

    return tsigmap;
}


template<class CloudType>
inline CML::scalar CML::ThermoCloud<CloudType>::Tmax() const
{
    scalar T = -GREAT;
    label n = 0;
    forAllConstIter(typename ThermoCloud<CloudType>, *this, iter)
    {
        const parcelType& p = iter();
        T = max(T, p.T());
        n++;
    }

    reduce(T, maxOp<scalar>());
    reduce(n, sumOp<label>());

    if (n > 0)
    {
        return T;
    }
    else
    {
        return 0.0;
    }
}


template<class CloudType>
inline CML::scalar CML::ThermoCloud<CloudType>::Tmin() const
{
    scalar T = GREAT;
    label n = 0;
    forAllConstIter(typename ThermoCloud<CloudType>, *this, iter)
    {
        const parcelType& p = iter();
        T = min(T, p.T());
        n++;
    }

    reduce(T, minOp<scalar>());
    reduce(n, sumOp<label>());

    if (n > 0)
    {
        return T;
    }
    else
    {
        return 0.0;
    }
}


// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

template<class CloudType>
void CML::ThermoCloud<CloudType>::setModels()
{
    heatTransferModel_.reset
    (
        HeatTransferModel<ThermoCloud<CloudType> >::New
        (
            this->subModelProperties(),
            *this
        ).ptr()
    );

    TIntegrator_.reset
    (
        integrationScheme::New
        (
            "T",
            this->solution().integrationSchemes()
        ).ptr()
    );

    if (this->solution().coupled())
    {
        this->subModelProperties().lookup("radiation") >> radiation_;
    }

    if (radiation_)
    {
        radAreaP_.reset
        (
            new DimensionedField<scalar, volMesh>
            (
                IOobject
                (
                    this->name() + ":radAreaP",
                    this->db().time().timeName(),
                    this->db(),
                    IOobject::READ_IF_PRESENT,
                    IOobject::AUTO_WRITE
                ),
                this->mesh(),
                dimensionedScalar("zero", dimArea, 0.0)
            )
        );

        radT4_.reset
        (
            new DimensionedField<scalar, volMesh>
            (
                IOobject
                (
                    this->name() + ":radT4",
                    this->db().time().timeName(),
                    this->db(),
                    IOobject::READ_IF_PRESENT,
                    IOobject::AUTO_WRITE
                ),
                this->mesh(),
                dimensionedScalar("zero", pow4(dimTemperature), 0.0)
            )
        );

        radAreaPT4_.reset
        (
            new DimensionedField<scalar, volMesh>
            (
                IOobject
                (
                    this->name() + ":radAreaPT4",
                    this->db().time().timeName(),
                    this->db(),
                    IOobject::READ_IF_PRESENT,
                    IOobject::AUTO_WRITE
                ),
                this->mesh(),
                dimensionedScalar
                (
                    "zero",
                    sqr(dimLength)*pow4(dimTemperature),
                    0.0
                )
            )
        );
    }
}


template<class CloudType>
void CML::ThermoCloud<CloudType>::cloudReset(ThermoCloud<CloudType>& c)
{
    CloudType::cloudReset(c);

    heatTransferModel_.reset(c.heatTransferModel_.ptr());
    TIntegrator_.reset(c.TIntegrator_.ptr());

    radiation_ = c.radiation_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ThermoCloud<CloudType>::ThermoCloud
(
    const word& cloudName,
    const volScalarField& rho,
    const volVectorField& U,
    const dimensionedVector& g,
    const SLGThermo& thermo,
    bool readFields
)
:
    CloudType
    (
        cloudName,
        rho,
        U,
        thermo.thermo().mu(),
        g,
        false
    ),
    thermoCloud(),
    cloudCopyPtr_(nullptr),
    constProps_(this->particleProperties()),
    thermo_(thermo),
    T_(thermo.thermo().T()),
    p_(thermo.thermo().p()),
    heatTransferModel_(nullptr),
    TIntegrator_(nullptr),
    radiation_(false),
    radAreaP_(nullptr),
    radT4_(nullptr),
    radAreaPT4_(nullptr),
    hsTrans_
    (
        new DimensionedField<scalar, volMesh>
        (
            IOobject
            (
                this->name() + ":hsTrans",
                this->db().time().timeName(),
                this->db(),
                IOobject::READ_IF_PRESENT,
                IOobject::AUTO_WRITE
            ),
            this->mesh(),
            dimensionedScalar("zero", dimEnergy, 0.0)
        )
    ),
    hsCoeff_
    (
        new DimensionedField<scalar, volMesh>
        (
            IOobject
            (
                this->name() + ":hsCoeff",
                this->db().time().timeName(),
                this->db(),
                IOobject::READ_IF_PRESENT,
                IOobject::AUTO_WRITE
            ),
            this->mesh(),
            dimensionedScalar("zero", dimEnergy/dimTemperature, 0.0)
        )
    )
{
    if (this->solution().active())
    {
        setModels();

        if (readFields)
        {
            parcelType::readFields(*this);
            this->deleteLostParticles();
        }
    }

    if (this->solution().resetSourcesOnStartup())
    {
        resetSourceTerms();
    }
}


template<class CloudType>
CML::ThermoCloud<CloudType>::ThermoCloud
(
    ThermoCloud<CloudType>& c,
    const word& name
)
:
    CloudType(c, name),
    thermoCloud(),
    cloudCopyPtr_(nullptr),
    constProps_(c.constProps_),
    thermo_(c.thermo_),
    T_(c.T()),
    p_(c.p()),
    heatTransferModel_(c.heatTransferModel_->clone()),
    TIntegrator_(c.TIntegrator_->clone()),
    radiation_(c.radiation_),
    radAreaP_(nullptr),
    radT4_(nullptr),
    radAreaPT4_(nullptr),
    hsTrans_
    (
        new DimensionedField<scalar, volMesh>
        (
            IOobject
            (
                this->name() + ":hsTrans",
                this->db().time().timeName(),
                this->db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            c.hsTrans()
        )
    ),
    hsCoeff_
    (
        new DimensionedField<scalar, volMesh>
        (
            IOobject
            (
                this->name() + ":hsCoeff",
                this->db().time().timeName(),
                this->db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            c.hsCoeff()
        )
    )
{
    if (radiation_)
    {
        radAreaP_.reset
        (
            new DimensionedField<scalar, volMesh>
            (
                IOobject
                (
                    this->name() + ":radAreaP",
                    this->db().time().timeName(),
                    this->db(),
                    IOobject::NO_READ,
                    IOobject::NO_WRITE,
                    false
                ),
                c.radAreaP()
            )
        );

        radT4_.reset
        (
            new DimensionedField<scalar, volMesh>
            (
                IOobject
                (
                    this->name() + ":radT4",
                    this->db().time().timeName(),
                    this->db(),
                    IOobject::NO_READ,
                    IOobject::NO_WRITE,
                    false
                ),
                c.radT4()
            )
        );

        radAreaPT4_.reset
        (
            new DimensionedField<scalar, volMesh>
            (
                IOobject
                (
                    this->name() + ":radAreaPT4",
                    this->db().time().timeName(),
                    this->db(),
                    IOobject::NO_READ,
                    IOobject::NO_WRITE,
                    false
                ),
                c.radAreaPT4()
            )
        );
    }
}


template<class CloudType>
CML::ThermoCloud<CloudType>::ThermoCloud
(
    const fvMesh& mesh,
    const word& name,
    const ThermoCloud<CloudType>& c
)
:
    CloudType(mesh, name, c),
    thermoCloud(),
    cloudCopyPtr_(nullptr),
    constProps_(),
    thermo_(c.thermo()),
    T_(c.T()),
    p_(c.p()),
    heatTransferModel_(nullptr),
    TIntegrator_(nullptr),
    radiation_(false),
    radAreaP_(nullptr),
    radT4_(nullptr),
    radAreaPT4_(nullptr),
    hsTrans_(nullptr),
    hsCoeff_(nullptr)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::ThermoCloud<CloudType>::~ThermoCloud()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::ThermoCloud<CloudType>::setParcelThermoProperties
(
    parcelType& parcel,
    const scalar lagrangianDt
)
{
    CloudType::setParcelThermoProperties(parcel, lagrangianDt);

    parcel.T() = constProps_.T0();
    parcel.Cp() = constProps_.Cp0();
}


template<class CloudType>
void CML::ThermoCloud<CloudType>::checkParcelProperties
(
    parcelType& parcel,
    const scalar lagrangianDt,
    const bool fullyDescribed
)
{
    CloudType::checkParcelProperties(parcel, lagrangianDt, fullyDescribed);
}


template<class CloudType>
void CML::ThermoCloud<CloudType>::storeState()
{
    cloudCopyPtr_.reset
    (
        static_cast<ThermoCloud<CloudType>*>
        (
            clone(this->name() + "Copy").ptr()
        )
    );
}


template<class CloudType>
void CML::ThermoCloud<CloudType>::restoreState()
{
    cloudReset(cloudCopyPtr_());
    cloudCopyPtr_.clear();
}


template<class CloudType>
void CML::ThermoCloud<CloudType>::resetSourceTerms()
{
    CloudType::resetSourceTerms();
    hsTrans_->field() = 0.0;
    hsCoeff_->field() = 0.0;

    if (radiation_)
    {
        radAreaP_->field() = 0.0;
        radT4_->field() = 0.0;
        radAreaPT4_->field() = 0.0;
    }
}


template<class CloudType>
void CML::ThermoCloud<CloudType>::relaxSources
(
    const ThermoCloud<CloudType>& cloudOldTime
)
{
    CloudType::relaxSources(cloudOldTime);

    this->relax(hsTrans_(), cloudOldTime.hsTrans(), "h");
    this->relax(hsCoeff_(), cloudOldTime.hsCoeff(), "h");

    if (radiation_)
    {
        this->relax(radAreaP_(), cloudOldTime.radAreaP(), "radiation");
        this->relax(radT4_(), cloudOldTime.radT4(), "radiation");
        this->relax(radAreaPT4_(), cloudOldTime.radAreaPT4(), "radiation");
    }
}


template<class CloudType>
void CML::ThermoCloud<CloudType>::scaleSources()
{
    CloudType::scaleSources();

    this->scale(hsTrans_(), "h");
    this->scale(hsCoeff_(), "h");

    if (radiation_)
    {
        this->scale(radAreaP_(), "radiation");
        this->scale(radT4_(), "radiation");
        this->scale(radAreaPT4_(), "radiation");
    }
}


template<class CloudType>
void CML::ThermoCloud<CloudType>::preEvolve()
{
    CloudType::preEvolve();

    this->pAmbient() = thermo_.thermo().p().average().value();
}


template<class CloudType>
void CML::ThermoCloud<CloudType>::evolve()
{
    if (this->solution().canEvolve())
    {
        typename parcelType::trackingData td(*this);

        this->solve(*this, td);
    }
}


template<class CloudType>
void CML::ThermoCloud<CloudType>::autoMap(const mapPolyMesh& mapper)
{
    Cloud<parcelType>::autoMap(mapper);

    this->updateMesh();
}


template<class CloudType>
void CML::ThermoCloud<CloudType>::info()
{
    CloudType::info();

    Info<< "    Temperature min/max             = " << Tmin() << ", " << Tmax()
        << endl;
}


#endif

// ************************************************************************* //
