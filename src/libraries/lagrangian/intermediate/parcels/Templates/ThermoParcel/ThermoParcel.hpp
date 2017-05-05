/*---------------------------------------------------------------------------*\
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
    CML::ThermoParcel

Description
    Thermodynamic parcel class with one/two-way coupling with the continuous
    phase. Includes Kinematic parcel sub-models, plus:
    - heat transfer


\*---------------------------------------------------------------------------*/

#ifndef ThermoParcel_H
#define ThermoParcel_H

#include "particle.hpp"
#include "SLGThermo.hpp"
#include "physicoChemicalConstants.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class ParcelType>
class ThermoParcel;

template<class ParcelType>
Ostream& operator<<
(
    Ostream&,
    const ThermoParcel<ParcelType>&
);

/*---------------------------------------------------------------------------*\
                       Class ThermoParcel Declaration
\*---------------------------------------------------------------------------*/

template<class ParcelType>
class ThermoParcel
:
    public ParcelType
{
public:

    //- Class to hold thermo particle constant properties
    class constantProperties
    :
        public ParcelType::constantProperties
    {

        // Private data

            //- Particle initial temperature [K]
            scalar T0_;

            //- Minimum temperature [K]
            scalar TMin_;

            //- Maximum temperature [K]
            scalar TMax_;

            //- Particle specific heat capacity [J/(kg.K)]
            scalar Cp0_;

            //- Particle emissivity [] (radiation)
            scalar epsilon0_;

            //- Particle scattering factor [] (radiation)
            scalar f0_;

            //- Default carrier Prandtl number []
            scalar Pr_;


    public:

        // Constructors

            //- Null constructor
            constantProperties();

            //- Copy constructor
            constantProperties(const constantProperties& cp);

            //- Constructor from dictionary
            constantProperties
            (
                const dictionary& parentDict,
                const bool readFields = true
            );

            //- Construct from components
            constantProperties
            (
                const label parcelTypeId,
                const scalar rhoMin,
                const scalar rho0,
                const scalar minParticleMass,
                const scalar youngsModulus,
                const scalar poissonsRatio,
                const scalar T0,
                const scalar TMin,
                const scalar TMax,
                const scalar Cp0,
                const scalar epsilon0,
                const scalar f0,
                const scalar Pr
            );


        // Member functions

            // Access

                //- Return const access to the particle initial temperature [K]
                inline scalar T0() const;

                //- Return const access to minimum temperature [K]
                inline scalar TMin() const;

                //- Return const access to maximum temperature [K]
                inline scalar TMax() const;

                //- Return non-const access to maximum temperature [K]
                inline scalar& TMax();

                //- Return const access to the particle specific heat capacity
                //  [J/(kg.K)]
                inline scalar Cp0() const;

                //- Return const access to the particle emissivity []
                //  Active for radiation only
                inline scalar epsilon0() const;

                //- Return const access to the particle scattering factor []
                //  Active for radiation only
                inline scalar f0() const;

                //- Return const access to the default carrier Prandtl number []
                inline scalar Pr() const;
    };


    template<class CloudType>
    class TrackingData
    :
        public ParcelType::template TrackingData<CloudType>
    {
    private:

        // Private data

            //- Local copy of carrier specific heat field
            //  Cp not stored on carrier thermo, but returned as tmp<...>
            const volScalarField Cp_;

            //- Local copy of carrier thermal conductivity field
            //  kappa not stored on carrier thermo, but returned as tmp<...>
//            const volScalarField kappa_;


            // Interpolators for continuous phase fields

                //- Temperature field interpolator
                autoPtr<interpolation<scalar> > TInterp_;

                //- Specific heat capacity field interpolator
                autoPtr<interpolation<scalar> > CpInterp_;

                //- Thermal conductivity field interpolator
//                autoPtr<interpolation<scalar> > kappaInterp_;

                //- Radiation field interpolator
                autoPtr<interpolation<scalar> > GInterp_;


    public:

        typedef typename ParcelType::template TrackingData<CloudType>::trackPart
            trackPart;

        // Constructors

            //- Construct from components
            inline TrackingData
            (
                CloudType& cloud,
                trackPart part = ParcelType::template
                    TrackingData<CloudType>::tpLinearTrack
            );


        // Member functions

            //- Return access to the locally stored carrier Cp field
            inline const volScalarField& Cp() const;

            //- Return access to the locally stored carrier kappa field
//            inline const volScalarField& kappa() const;

            //- Return const access to the interpolator for continuous
            //  phase temperature field
            inline const interpolation<scalar>& TInterp() const;

            //- Return const access to the interpolator for continuous
            //  phase specific heat capacity field
            inline const interpolation<scalar>& CpInterp() const;

            //- Return const access to the interpolator for continuous
            //  phase thermal conductivity field
//            inline const interpolation<scalar>& kappaInterp() const;

            //- Return const access to the interpolator for continuous
            //  radiation field
            inline const interpolation<scalar>& GInterp() const;
    };


protected:

    // Protected data

        // Parcel properties

            //- Temperature [K]
            scalar T_;

            //- Specific heat capacity [J/(kg.K)]
            scalar Cp_;


        // Cell-based quantities

            //- Temperature [K]
            scalar Tc_;

            //- Specific heat capacity [J/(kg.K)]
            scalar Cpc_;


    // Protected Member Functions

        //- Calculate new particle temperature
        template<class TrackData>
        scalar calcHeatTransfer
        (
            TrackData& td,
            const scalar dt,           // timestep
            const label cellI,         // owner cell
            const scalar Re,           // Reynolds number
            const scalar Pr,           // Prandtl number - surface
            const scalar kappa,        // Thermal conductivity - surface
            const scalar NCpW,         // Sum of N*Cp*W of emission species
            const scalar Sh,           // explicit particle enthalpy source
            scalar& dhsTrans,          // sensible enthalpy transfer to carrier
            scalar& Sph                // linearised heat transfer coefficient
        );


public:

    // Static data members

        //- Runtime type information
        TypeName("ThermoParcel");

        //- String representation of properties
        AddToPropertyList
        (
            ParcelType,
            " T"
          + " Cp"
        );


    // Constructors

        //- Construct from owner, position, and cloud owner
        //  Other properties initialised as null
        inline ThermoParcel
        (
            const polyMesh& mesh,
            const vector& position,
            const label cellI,
            const label tetFaceI,
            const label tetPtI
        );

        //- Construct from components
        inline ThermoParcel
        (
            const polyMesh& mesh,
            const vector& position,
            const label cellI,
            const label tetFaceI,
            const label tetPtI,
            const label typeId,
            const scalar nParticle0,
            const scalar d0,
            const scalar dTarget0,
            const vector& U0,
            const vector& f0,
            const vector& angularMomentum0,
            const vector& torque0,
            const constantProperties& constProps
        );

        //- Construct from Istream
        ThermoParcel
        (
            const polyMesh& mesh,
            Istream& is,
            bool readFields = true
        );

        //- Construct as a copy
        ThermoParcel(const ThermoParcel& p);

        //- Construct as a copy
        ThermoParcel(const ThermoParcel& p, const polyMesh& mesh);

        //- Construct and return a (basic particle) clone
        virtual autoPtr<particle> clone() const
        {
            return autoPtr<particle>(new ThermoParcel(*this));
        }

        //- Construct and return a (basic particle) clone
        virtual autoPtr<particle> clone(const polyMesh& mesh) const
        {
            return autoPtr<particle>(new ThermoParcel(*this, mesh));
        }

        //- Factory class to read-construct particles used for
        //  parallel transfer
        class iNew
        {
            const polyMesh& mesh_;

        public:

            iNew(const polyMesh& mesh)
            :
                mesh_(mesh)
            {}

            autoPtr<ThermoParcel<ParcelType> > operator()(Istream& is) const
            {
                return autoPtr<ThermoParcel<ParcelType> >
                (
                    new ThermoParcel<ParcelType>(mesh_, is, true)
                );
            }
        };


    // Member Functions

        // Access

            //- Return const access to temperature
            inline scalar T() const;

            //- Return const access to specific heat capacity
            inline scalar Cp() const;

            //- Return the parcel sensible enthalpy
            inline scalar hs() const;

            //- Return const access to carrier temperature
            inline scalar Tc() const;

            //- Return const access to carrier specific heat capacity
            inline scalar Cpc() const;


        // Edit

            //- Return access to temperature
            inline scalar& T();

            //- Return access to specific heat capacity
            inline scalar& Cp();


        // Main calculation loop

            //- Set cell values
            template<class TrackData>
            void setCellValues
            (
                TrackData& td,
                const scalar dt,
                const label cellI
            );

            //- Correct cell values using latest transfer information
            template<class TrackData>
            void cellValueSourceCorrection
            (
                TrackData& td,
                const scalar dt,
                const label cellI
            );

            //- Calculate surface thermo properties
            template<class TrackData>
            void calcSurfaceValues
            (
                TrackData& td,
                const label cellI,
                const scalar T,
                scalar& Ts,
                scalar& rhos,
                scalar& mus,
                scalar& Pr,
                scalar& kappas
            ) const;

            //- Update parcel properties over the time interval
            template<class TrackData>
            void calc
            (
                TrackData& td,
                const scalar dt,
                const label cellI
            );


        // I-O

            //- Read
            template<class CloudType>
            static void readFields(CloudType& c);

            //- Write
            template<class CloudType>
            static void writeFields(const CloudType& c);


    // Ostream Operator

        friend Ostream& operator<< <ParcelType>
        (
            Ostream&,
            const ThermoParcel<ParcelType>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParcelType>
inline CML::ThermoParcel<ParcelType>::constantProperties::constantProperties()
:
    ParcelType::constantProperties(),
    T0_(0.0),
    TMin_(0.0),
    TMax_(VGREAT),
    Cp0_(0.0),
    epsilon0_(0.0),
    f0_(0.0),
    Pr_(0.0)
{}


template<class ParcelType>
inline CML::ThermoParcel<ParcelType>::constantProperties::constantProperties
(
    const constantProperties& cp
)
:
    ParcelType::constantProperties(cp),
    T0_(cp.T0_),
    TMin_(cp.TMin_),
    TMax_(cp.TMax_),
    Cp0_(cp.Cp0_),
    epsilon0_(cp.epsilon0_),
    f0_(cp.f0_),
    Pr_(cp.Pr_)
{}


template<class ParcelType>
inline CML::ThermoParcel<ParcelType>::constantProperties::constantProperties
(
    const dictionary& parentDict,
    const bool readFields
)
:
    ParcelType::constantProperties(parentDict, readFields),
    T0_(0.0),
    TMin_(200),
    TMax_(5000),
    Cp0_(0.0),
    epsilon0_(0.0),
    f0_(0.0),
    Pr_(0.0)
{
    if (readFields)
    {
        if (this->dict().readIfPresent("TMin", TMin_))
        {
            Info<< "    employing parcel TMin of " << TMin_ << endl;
        }
        if (this->dict().readIfPresent("TMax", TMax_))
        {
            Info<< "    employing parcel TMax of " << TMax_ << endl;
        }

        this->dict().lookup("T0") >> T0_;
        this->dict().lookup("Cp0") >> Cp0_;
        this->dict().lookup("epsilon0") >> epsilon0_;
        this->dict().lookup("f0") >> f0_;
        this->dict().lookup("Pr") >> Pr_;
    }
}


template<class ParcelType>
inline CML::ThermoParcel<ParcelType>::constantProperties::constantProperties
(
    const label parcelTypeId,
    const scalar rhoMin,
    const scalar rho0,
    const scalar minParticleMass,
    const scalar youngsModulus,
    const scalar poissonsRatio,
    const scalar T0,
    const scalar TMin,
    const scalar TMax,
    const scalar Cp0,
    const scalar epsilon0,
    const scalar f0,
    const scalar Pr
)
:
    ParcelType::constantProperties
    (
        parcelTypeId,
        rhoMin,
        rho0,
        minParticleMass,
        youngsModulus,
        poissonsRatio
    ),
    T0_(T0),
    TMin_(TMin),
    TMax_(TMax),
    Cp0_(Cp0),
    epsilon0_(epsilon0),
    f0_(f0),
    Pr_(Pr)
{}


template<class ParcelType>
inline CML::ThermoParcel<ParcelType>::ThermoParcel
(
    const polyMesh& mesh,
    const vector& position,
    const label cellI,
    const label tetFaceI,
    const label tetPtI
)
:
    ParcelType(mesh, position, cellI, tetFaceI, tetPtI),
    T_(0.0),
    Cp_(0.0),
    Tc_(0.0),
    Cpc_(0.0)
{}


template<class ParcelType>
inline CML::ThermoParcel<ParcelType>::ThermoParcel
(
    const polyMesh& mesh,
    const vector& position,
    const label cellI,
    const label tetFaceI,
    const label tetPtI,
    const label typeId,
    const scalar nParticle0,
    const scalar d0,
    const scalar dTarget0,
    const vector& U0,
    const vector& f0,
    const vector& angularMomentum0,
    const vector& torque0,
    const constantProperties& constProps
)
:
    ParcelType
    (
        mesh,
        position,
        cellI,
        tetFaceI,
        tetPtI,
        typeId,
        nParticle0,
        d0,
        dTarget0,
        U0,
        f0,
        angularMomentum0,
        torque0,
        constProps
    ),
    T_(constProps.T0()),
    Cp_(constProps.Cp0()),
    Tc_(0.0),
    Cpc_(0.0)
{}


// * * * * * * * * * constantProperties Member Functions * * * * * * * * * * //

template<class ParcelType>
inline CML::scalar
CML::ThermoParcel<ParcelType>::constantProperties::T0() const
{
    return T0_;
}


template<class ParcelType>
inline CML::scalar
CML::ThermoParcel<ParcelType>::constantProperties::TMin() const
{
    return TMin_;
}


template<class ParcelType>
inline CML::scalar
CML::ThermoParcel<ParcelType>::constantProperties::TMax() const
{
    return TMax_;
}


template<class ParcelType>
inline CML::scalar&
CML::ThermoParcel<ParcelType>::constantProperties::TMax()
{
    return TMax_;
}


template<class ParcelType>
inline CML::scalar
CML::ThermoParcel<ParcelType>::constantProperties::Cp0() const
{
    return Cp0_;
}


template<class ParcelType>
inline CML::scalar
CML::ThermoParcel<ParcelType>::constantProperties::epsilon0() const
{
    return epsilon0_;
}


template<class ParcelType>
inline CML::scalar
CML::ThermoParcel<ParcelType>::constantProperties::f0() const
{
    return f0_;
}


template<class ParcelType>
inline CML::scalar
CML::ThermoParcel<ParcelType>::constantProperties::Pr() const
{
    return Pr_;
}

// * * * * * * * * * * ThermoParcel Member Functions * * * * * * * * * * * * //

template<class ParcelType>
inline CML::scalar CML::ThermoParcel<ParcelType>::T() const
{
    return T_;
}


template<class ParcelType>
inline CML::scalar CML::ThermoParcel<ParcelType>::Cp() const
{
    return Cp_;
}


template<class ParcelType>
inline CML::scalar CML::ThermoParcel<ParcelType>::hs() const
{
    return Cp_*(T_ - 298.15);
}


template<class ParcelType>
inline CML::scalar CML::ThermoParcel<ParcelType>::Tc() const
{
    return Tc_;
}


template<class ParcelType>
inline CML::scalar CML::ThermoParcel<ParcelType>::Cpc() const
{
    return Cpc_;
}


template<class ParcelType>
inline CML::scalar& CML::ThermoParcel<ParcelType>::T()
{
    return T_;
}


template<class ParcelType>
inline CML::scalar& CML::ThermoParcel<ParcelType>::Cp()
{
    return Cp_;
}


// ************************************************************************* //

template<class ParcelType>
template<class CloudType>
inline CML::ThermoParcel<ParcelType>::TrackingData<CloudType>::TrackingData
(
    CloudType& cloud,
    trackPart part
)
:
    ParcelType::template TrackingData<CloudType>(cloud, part),
    Cp_(cloud.thermo().thermo().Cp()),
//    kappa_(cloud.thermo().thermo().kappa()),
    TInterp_
    (
        interpolation<scalar>::New
        (
            cloud.solution().interpolationSchemes(),
            cloud.T()
        )
    ),
    CpInterp_
    (
        interpolation<scalar>::New
        (
            cloud.solution().interpolationSchemes(),
            Cp_
        )
    ),
//    kappaInterp_
//    (
//        interpolation<scalar>::New
//        (
//            cloud.solution().interpolationSchemes(),
//            kappa_
//        )
//    ),
    GInterp_(NULL)
{
    if (cloud.radiation())
    {
        GInterp_.reset
        (
            interpolation<scalar>::New
            (
                cloud.solution().interpolationSchemes(),
                cloud.mesh().objectRegistry::template
                    lookupObject<volScalarField>("G")
            ).ptr()
        );
    }
}


template<class ParcelType>
template<class CloudType>
inline const CML::volScalarField&
CML::ThermoParcel<ParcelType>::TrackingData<CloudType>::Cp() const
{
    return Cp_;
}


//template<class ParcelType>
//template<class CloudType>
//inline const CML::volScalarField&
//CML::ThermoParcel<ParcelType>::TrackingData<CloudType>::kappa() const
//{
//    return kappa_;
//}


template<class ParcelType>
template<class CloudType>
inline const CML::interpolation<CML::scalar>&
CML::ThermoParcel<ParcelType>::TrackingData<CloudType>::TInterp() const
{
    return TInterp_();
}


template<class ParcelType>
template<class CloudType>
inline const CML::interpolation<CML::scalar>&
CML::ThermoParcel<ParcelType>::TrackingData<CloudType>::CpInterp() const
{
    return CpInterp_();
}


//template<class ParcelType>
//template<class CloudType>
//inline const CML::interpolation<CML::scalar>&
//CML::ThermoParcel<ParcelType>::TrackingData<CloudType>::kappaInterp() const
//{
//    return kappaInterp_();
//}


template<class ParcelType>
template<class CloudType>
inline const CML::interpolation<CML::scalar>&
CML::ThermoParcel<ParcelType>::TrackingData<CloudType>::GInterp() const
{
    if (!GInterp_.valid())
    {
        FatalErrorIn
        (
            "inline const CML::interpolation<CML::scalar>&"
            "CML::ThermoParcel<ParcelType>::TrackingData<CloudType>::"
            "GInterp() const"
        )   << "Radiation G interpolation object not set"
            << abort(FatalError);
    }

    return GInterp_();
}


// ************************************************************************* //

using namespace CML::constant;

// * * * * * * * * * * *  Protected Member Functions * * * * * * * * * * * * //

template<class ParcelType>
template<class TrackData>
void CML::ThermoParcel<ParcelType>::setCellValues
(
    TrackData& td,
    const scalar dt,
    const label cellI
)
{
    ParcelType::setCellValues(td, dt, cellI);

    tetIndices tetIs = this->currentTetIndices();

    Cpc_ = td.CpInterp().interpolate(this->position(), tetIs);

    Tc_ = td.TInterp().interpolate(this->position(), tetIs);

    if (Tc_ < td.cloud().constProps().TMin())
    {
        if (debug)
        {
            WarningIn
            (
                "void CML::ThermoParcel<ParcelType>::setCellValues"
                "("
                    "TrackData&, "
                    "const scalar, "
                    "const label"
                ")"
            )   << "Limiting observed temperature in cell " << cellI << " to "
                << td.cloud().constProps().TMin() <<  nl << endl;
        }

        Tc_ = td.cloud().constProps().TMin();
    }
}


template<class ParcelType>
template<class TrackData>
void CML::ThermoParcel<ParcelType>::cellValueSourceCorrection
(
    TrackData& td,
    const scalar dt,
    const label cellI
)
{
    this->Uc_ += td.cloud().UTrans()[cellI]/this->massCell(cellI);

    const scalar CpMean = td.CpInterp().psi()[cellI];
    Tc_ += td.cloud().hsTrans()[cellI]/(CpMean*this->massCell(cellI));

    if (Tc_ < td.cloud().constProps().TMin())
    {
        if (debug)
        {
            WarningIn
            (
                "void CML::ThermoParcel<ParcelType>::cellValueSourceCorrection"
                "("
                    "TrackData&, "
                    "const scalar, "
                    "const label"
                ")"
            )   << "Limiting observed temperature in cell " << cellI << " to "
                << td.cloud().constProps().TMin() <<  nl << endl;
        }

        Tc_ = td.cloud().constProps().TMin();
    }
}


template<class ParcelType>
template<class TrackData>
void CML::ThermoParcel<ParcelType>::calcSurfaceValues
(
    TrackData& td,
    const label cellI,
    const scalar T,
    scalar& Ts,
    scalar& rhos,
    scalar& mus,
    scalar& Pr,
    scalar& kappas
) const
{
    // Surface temperature using two thirds rule
    Ts = (2.0*T + Tc_)/3.0;

    if (Ts < td.cloud().constProps().TMin())
    {
        if (debug)
        {
            WarningIn
            (
                "void CML::ThermoParcel<ParcelType>::calcSurfaceValues"
                "("
                    "TrackData&, "
                    "const label, "
                    "const scalar, "
                    "scalar&, "
                    "scalar&, "
                    "scalar&, "
                    "scalar&, "
                    "scalar&"
                ") const"
            )   << "Limiting parcel surface temperature to "
                << td.cloud().constProps().TMin() <<  nl << endl;
        }

        Ts = td.cloud().constProps().TMin();
    }

    // Assuming thermo props vary linearly with T for small d(T)
    const scalar TRatio = Tc_/Ts;

    rhos = this->rhoc_*TRatio;

    tetIndices tetIs = this->currentTetIndices();
    mus = td.muInterp().interpolate(this->position(), tetIs)/TRatio;
//    kappas = td.kappaInterp().interpolate(this->position(), tetIs)/TRatio;

    Pr = td.cloud().constProps().Pr();
    Pr = max(ROOTVSMALL, Pr);

    kappas = Cpc_*mus/Pr;
    kappas = max(ROOTVSMALL, kappas);
}


template<class ParcelType>
template<class TrackData>
void CML::ThermoParcel<ParcelType>::calc
(
    TrackData& td,
    const scalar dt,
    const label cellI
)
{
    // Define local properties at beginning of time step
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    const scalar np0 = this->nParticle_;
    const scalar mass0 = this->mass();


    // Calc surface values
    // ~~~~~~~~~~~~~~~~~~~
    scalar Ts, rhos, mus, Pr, kappas;
    calcSurfaceValues(td, cellI, this->T_, Ts, rhos, mus, Pr, kappas);

    // Reynolds number
    scalar Re = this->Re(this->U_, this->d_, rhos, mus);


    // Sources
    // ~~~~~~~

    // Explicit momentum source for particle
    vector Su = vector::zero;

    // Linearised momentum source coefficient
    scalar Spu = 0.0;

    // Momentum transfer from the particle to the carrier phase
    vector dUTrans = vector::zero;

    // Explicit enthalpy source for particle
    scalar Sh = 0.0;

    // Linearised enthalpy source coefficient
    scalar Sph = 0.0;

    // Sensible enthalpy transfer from the particle to the carrier phase
    scalar dhsTrans = 0.0;


    // Heat transfer
    // ~~~~~~~~~~~~~

    // Sum Ni*Cpi*Wi of emission species
    scalar NCpW = 0.0;

    // Calculate new particle temperature
    this->T_ =
        this->calcHeatTransfer
        (
            td,
            dt,
            cellI,
            Re,
            Pr,
            kappas,
            NCpW,
            Sh,
            dhsTrans,
            Sph
        );


    // Motion
    // ~~~~~~

    // Calculate new particle velocity
    this->U_ =
        this->calcVelocity(td, dt, cellI, Re, mus, mass0, Su, dUTrans, Spu);


    //  Accumulate carrier phase source terms
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    if (td.cloud().solution().coupled())
    {
        // Update momentum transfer
        td.cloud().UTrans()[cellI] += np0*dUTrans;

        // Update momentum transfer coefficient
        td.cloud().UCoeff()[cellI] += np0*Spu;

        // Update sensible enthalpy transfer
        td.cloud().hsTrans()[cellI] += np0*dhsTrans;

        // Update sensible enthalpy coefficient
        td.cloud().hsCoeff()[cellI] += np0*Sph;

        // Update radiation fields
        if (td.cloud().radiation())
        {
            const scalar ap = this->areaP();
            const scalar T4 = pow4(this->T_);
            td.cloud().radAreaP()[cellI] += dt*np0*ap;
            td.cloud().radT4()[cellI] += dt*np0*T4;
            td.cloud().radAreaPT4()[cellI] += dt*np0*ap*T4;
        }
    }
}


template<class ParcelType>
template<class TrackData>
CML::scalar CML::ThermoParcel<ParcelType>::calcHeatTransfer
(
    TrackData& td,
    const scalar dt,
    const label cellI,
    const scalar Re,
    const scalar Pr,
    const scalar kappa,
    const scalar NCpW,
    const scalar Sh,
    scalar& dhsTrans,
    scalar& Sph
)
{
    if (!td.cloud().heatTransfer().active())
    {
        return T_;
    }

    const scalar d = this->d();
    const scalar rho = this->rho();

    // Calc heat transfer coefficient
    scalar htc = td.cloud().heatTransfer().htc(d, Re, Pr, kappa, NCpW);

    if (mag(htc) < ROOTVSMALL && !td.cloud().radiation())
    {
        return
            max
            (
                T_ + dt*Sh/(this->volume(d)*rho*Cp_),
                td.cloud().constProps().TMin()
            );
    }

    htc = max(htc, ROOTVSMALL);
    const scalar As = this->areaS(d);

    scalar ap = Tc_ + Sh/As/htc;
    scalar bp = 6.0*(Sh/As + htc*(Tc_ - T_));
    if (td.cloud().radiation())
    {
        tetIndices tetIs = this->currentTetIndices();
        const scalar Gc = td.GInterp().interpolate(this->position(), tetIs);
        const scalar sigma = physicoChemical::sigma.value();
        const scalar epsilon = td.cloud().constProps().epsilon0();

        ap = (ap + epsilon*Gc/(4.0*htc))/(1.0 + epsilon*sigma*pow3(T_)/htc);
        bp += 6.0*(epsilon*(Gc/4.0 - sigma*pow4(T_)));
    }
    bp /= rho*d*Cp_*(ap - T_) + ROOTVSMALL;

    // Integrate to find the new parcel temperature
    IntegrationScheme<scalar>::integrationResult Tres =
        td.cloud().TIntegrator().integrate(T_, dt, ap*bp, bp);

    scalar Tnew =
        min
        (
            max
            (
                Tres.value(),
                td.cloud().constProps().TMin()
            ),
            td.cloud().constProps().TMax()
        );

    Sph = dt*htc*As;

    dhsTrans += Sph*(Tres.average() - Tc_);

    return Tnew;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParcelType>
CML::ThermoParcel<ParcelType>::ThermoParcel
(
    const ThermoParcel<ParcelType>& p
)
:
    ParcelType(p),
    T_(p.T_),
    Cp_(p.Cp_),
    Tc_(p.Tc_),
    Cpc_(p.Cpc_)
{}


template<class ParcelType>
CML::ThermoParcel<ParcelType>::ThermoParcel
(
    const ThermoParcel<ParcelType>& p,
    const polyMesh& mesh
)
:
    ParcelType(p, mesh),
    T_(p.T_),
    Cp_(p.Cp_),
    Tc_(p.Tc_),
    Cpc_(p.Cpc_)
{}


// * * * * * * * * * * * * * * IOStream operators  * * * * * * * * * * * * * //
// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class ParcelType>
CML::string CML::ThermoParcel<ParcelType>::propertyList_ =
    CML::ThermoParcel<ParcelType>::propertyList();


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParcelType>
CML::ThermoParcel<ParcelType>::ThermoParcel
(
    const polyMesh& mesh,
    Istream& is,
    bool readFields
)
:
    ParcelType(mesh, is, readFields),
    T_(0.0),
    Cp_(0.0),
    Tc_(0.0),
    Cpc_(0.0)
{
    if (readFields)
    {
        if (is.format() == IOstream::ASCII)
        {
            T_ = readScalar(is);
            Cp_ = readScalar(is);
        }
        else
        {
            label size = reinterpret_cast<const char*>(&Cp_) - reinterpret_cast<const char*>(&T_) + sizeof(Cp_);
            is.read(reinterpret_cast<char*>(&T_), size);
        }
    }

    // Check state of Istream
    is.check
    (
        "ThermoParcel::ThermoParcel(const polyMesh&, Istream&, bool)"
    );
}


template<class ParcelType>
template<class CloudType>
void CML::ThermoParcel<ParcelType>::readFields(CloudType& c)
{
    if (!c.size())
    {
        return;
    }

    ParcelType::readFields(c);

    IOField<scalar> T(c.fieldIOobject("T", IOobject::MUST_READ));
    c.checkFieldIOobject(c, T);

    IOField<scalar> Cp(c.fieldIOobject("Cp", IOobject::MUST_READ));
    c.checkFieldIOobject(c, Cp);


    label i = 0;
    forAllIter(typename Cloud<ThermoParcel<ParcelType> >, c, iter)
    {
        ThermoParcel<ParcelType>& p = iter();

        p.T_ = T[i];
        p.Cp_ = Cp[i];
        i++;
    }
}


template<class ParcelType>
template<class CloudType>
void CML::ThermoParcel<ParcelType>::writeFields(const CloudType& c)
{
    ParcelType::writeFields(c);

    label np = c.size();

    IOField<scalar> T(c.fieldIOobject("T", IOobject::NO_READ), np);
    IOField<scalar> Cp(c.fieldIOobject("Cp", IOobject::NO_READ), np);

    label i = 0;
    forAllConstIter(typename Cloud<ThermoParcel<ParcelType> >, c, iter)
    {
        const ThermoParcel<ParcelType>& p = iter();

        T[i] = p.T_;
        Cp[i] = p.Cp_;
        i++;
    }

    T.write();
    Cp.write();
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class ParcelType>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const ThermoParcel<ParcelType>& p
)
{
    if (os.format() == IOstream::ASCII)
    {
        os  << static_cast<const ParcelType&>(p)
            << token::SPACE << p.T()
            << token::SPACE << p.Cp();
    }
    else
    {
        os  << static_cast<const ParcelType&>(p);

        label size = reinterpret_cast<const char*>(&p.Cp_) - reinterpret_cast<const char*>(&p.T_) + sizeof(p.Cp_);
        os.write(reinterpret_cast<const char*>(&p.T_), size);
    }

    // Check state of Ostream
    os.check
    (
        "Ostream& operator<<(Ostream&, const ThermoParcel<ParcelType>&)"
    );

    return os;
}


#endif

// ************************************************************************* //
