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
    CML::ReactingParcel

Description
    Reacting parcel class with one/two-way coupling with the continuous
    phase.


\*---------------------------------------------------------------------------*/

#ifndef ReactingParcel_H
#define ReactingParcel_H

#include "particle.hpp"
#include "SLGThermo.hpp"
#include "specie.hpp"
#include "CompositionModel.hpp"
#include "mathematicalConstants.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class ParcelType>
class ReactingParcel;

template<class ParcelType>
Ostream& operator<<
(
    Ostream&,
    const ReactingParcel<ParcelType>&
);

/*---------------------------------------------------------------------------*\
                        Class ReactingParcel Declaration
\*---------------------------------------------------------------------------*/

template<class ParcelType>
class ReactingParcel
:
    public ParcelType
{
public:

    //- Class to hold reacting particle constant properties
    class constantProperties
    :
        public ParcelType::constantProperties
    {
        // Private data

            //- Minimum pressure [Pa]
            scalar pMin_;

            //- Constant volume flag - e.g. during mass transfer
            Switch constantVolume_;

            //- Vaporisation temperature [K]
            scalar Tvap_;

            //- Boiling point [K]
            scalar Tbp_;


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
                const scalar Cp0,
                const scalar epsilon0,
                const scalar f0,
                const scalar Pr,
                const scalar pMin,
                const Switch& constantVolume,
                const scalar Tvap,
                const scalar Tbp
            );


        // Access

            //- Return const access to the minimum pressure
            inline scalar pMin() const;

            //- Return const access to the constant volume flag
            inline Switch constantVolume() const;

            //- Return const access to the vaporisation temperature
            inline scalar Tvap() const;

            //- Return const access to the boiling point
            inline scalar Tbp() const;
    };


    template<class CloudType>
    class TrackingData
    :
        public ParcelType::template TrackingData<CloudType>
    {
    private:

        // Private data

            // Interpolators for continuous phase fields

                //- Interpolator for continuous phase pressure field
                autoPtr<interpolation<scalar> > pInterp_;


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

            //- Return const access to the interpolator for continuous phase
            //  pressure field
            inline const interpolation<scalar>& pInterp() const;
    };


protected:

    // Protected data

        // Parcel properties

            //- Initial particle mass [kg]
            scalar mass0_;

            //- Mass fractions of mixture []
            scalarField Y_;


        // Cell-based quantities

            //- Pressure [Pa]
            scalar pc_;


    // Protected Member Functions

        //- Calculate Phase change
        template<class TrackData>
        void calcPhaseChange
        (
            TrackData& td,
            const scalar dt,           // timestep
            const label cellI,         // owner cell
            const scalar Re,           // Reynolds number
            const scalar Pr,           // Prandtl number
            const scalar Ts,           // Surface temperature
            const scalar nus,          // Surface kinematic viscosity
            const scalar d,            // diameter
            const scalar T,            // temperature
            const scalar mass,         // mass
            const label idPhase,       // id of phase involved in phase change
            const scalar YPhase,       // total mass fraction
            const scalarField& YComponents, // component mass fractions
            scalarField& dMassPC,      // mass transfer - local to particle
            scalar& Sh,                // explicit particle enthalpy source
            scalar& N,                 // flux of species emitted from particle
            scalar& NCpW,              // sum of N*Cp*W of emission species
            scalarField& Cs            // carrier conc. of emission species
        );

        //- Update mass fraction
        scalar updateMassFraction
        (
            const scalar mass0,
            const scalarField& dMass,
            scalarField& Y
        ) const;


public:

    // Static data members

        //- String representation of properties
        static string propHeader;

        //- Runtime type information
        TypeName("ReactingParcel");


    // Constructors

        //- Construct from owner, position, and cloud owner
        //  Other properties initialised as null
        inline ReactingParcel
        (
            const polyMesh& mesh,
            const vector& position,
            const label cellI,
            const label tetFaceI,
            const label tetPtI
        );

        //- Construct from components
        inline ReactingParcel
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
            const scalarField& Y0,
            const constantProperties& constProps
        );

        //- Construct from Istream
        ReactingParcel
        (
            const polyMesh& mesh,
            Istream& is,
            bool readFields = true
        );

        //- Construct as a copy
        ReactingParcel
        (
            const ReactingParcel& p,
            const polyMesh& mesh
        );

        //- Construct as a copy
        ReactingParcel(const ReactingParcel& p);

        //- Construct and return a (basic particle) clone
        virtual autoPtr<particle> clone() const
        {
            return autoPtr<particle>(new ReactingParcel<ParcelType>(*this));
        }

        //- Construct and return a (basic particle) clone
        virtual autoPtr<particle> clone(const polyMesh& mesh) const
        {
            return autoPtr<particle>
            (
                new ReactingParcel<ParcelType>(*this, mesh)
            );
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

            autoPtr<ReactingParcel<ParcelType> > operator()(Istream& is) const
            {
                return autoPtr<ReactingParcel<ParcelType> >
                (
                    new ReactingParcel<ParcelType>(mesh_, is, true)
                );
            }
        };


    // Member Functions

        // Access

            //- Return const access to initial mass
            inline scalar mass0() const;

            //- Return const access to mass fractions of mixture
            inline const scalarField& Y() const;

            //- Return the owner cell pressure
            inline scalar pc() const;

            //- Return reference to the owner cell pressure
            inline scalar& pc();


        // Edit

            //- Return access to initial mass
            inline scalar& mass0();

            //- Return access to mass fractions of mixture
            inline scalarField& Y();


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

            //- Correct surface values due to emitted species
            template<class TrackData>
            void correctSurfaceValues
            (
                TrackData& td,
                const label cellI,
                const scalar T,
                const scalarField& Cs,
                scalar& rhos,
                scalar& mus,
                scalar& Prs,
                scalar& kappas
            );

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
            template<class CloudType, class CompositionType>
            static void readFields
            (
                CloudType& c,
                const CompositionType& compModel
            );

            //- Read - no composition
            template<class CloudType>
            static void readFields(CloudType& c);

            //- Write
            template<class CloudType, class CompositionType>
            static void writeFields
            (
                const CloudType& c,
                const CompositionType& compModel
            );

            //- Write - composition supplied
            template<class CloudType>
            static void writeFields(const CloudType& c);


    // Ostream Operator

        friend Ostream& operator<< <ParcelType>
        (
            Ostream&,
            const ReactingParcel<ParcelType>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParcelType>
inline
CML::ReactingParcel<ParcelType>::constantProperties::constantProperties()
:
    ParcelType::constantProperties(),
    pMin_(0.0),
    constantVolume_(false),
    Tvap_(0.0),
    Tbp_(0.0)
{}


template<class ParcelType>
inline CML::ReactingParcel<ParcelType>::constantProperties::constantProperties
(
    const constantProperties& cp
)
:
    ParcelType::constantProperties(cp),
    pMin_(cp.pMin_),
    constantVolume_(cp.constantVolume_),
    Tvap_(cp.Tvap_),
    Tbp_(cp.Tbp_)
{}


template<class ParcelType>
inline CML::ReactingParcel<ParcelType>::constantProperties::constantProperties
(
    const dictionary& parentDict,
    const bool readFields
)
:
    ParcelType::constantProperties(parentDict, readFields),
    pMin_(0.0),
    constantVolume_(false),
    Tvap_(0.0),
    Tbp_(0.0)
{
    if (readFields)
    {
        this->dict().lookup("pMin") >> pMin_;
        this->dict().lookup("constantVolume") >> constantVolume_;
        this->dict().lookup("Tvap") >> Tvap_;
        this->dict().lookup("Tbp") >> Tbp_;
    }
}


template<class ParcelType>
inline CML::ReactingParcel<ParcelType>::constantProperties::constantProperties
(
    const label parcelTypeId,
    const scalar rhoMin,
    const scalar rho0,
    const scalar minParticleMass,
    const scalar youngsModulus,
    const scalar poissonsRatio,
    const scalar T0,
    const scalar TMin,
    const scalar Cp0,
    const scalar epsilon0,
    const scalar f0,
    const scalar Pr,
    const scalar pMin,
    const Switch& constantVolume,
    const scalar Tvap,
    const scalar Tbp
)
:
    ParcelType::constantProperties
    (
        parcelTypeId,
        rhoMin,
        rho0,
        minParticleMass,
        youngsModulus,
        poissonsRatio,
        T0,
        TMin,
        Cp0,
        epsilon0,
        f0,
        Pr
    ),
    pMin_(pMin),
    constantVolume_(constantVolume),
    Tvap_(Tvap),
    Tbp_(Tbp)
{}


template<class ParcelType>
inline CML::ReactingParcel<ParcelType>::ReactingParcel
(
    const polyMesh& mesh,
    const vector& position,
    const label cellI,
    const label tetFaceI,
    const label tetPtI
)
:
    ParcelType(mesh, position, cellI, tetFaceI, tetPtI),
    mass0_(0.0),
    Y_(0),
    pc_(0.0)
{}


template<class ParcelType>
inline CML::ReactingParcel<ParcelType>::ReactingParcel
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
    const scalarField& Y0,
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
    mass0_(0.0),
    Y_(Y0),
    pc_(0.0)
{
    // Set initial parcel mass
    mass0_ = this->mass();
}


// * * * * * * * * * constantProperties Member Functions * * * * * * * * * * //

template<class ParcelType>
inline CML::scalar
CML::ReactingParcel<ParcelType>::constantProperties::pMin() const
{
    return pMin_;
}


template<class ParcelType>
inline CML::Switch
CML::ReactingParcel<ParcelType>::constantProperties::constantVolume() const
{
    return constantVolume_;
}


template<class ParcelType>
inline CML::scalar
CML::ReactingParcel<ParcelType>::constantProperties::Tvap() const
{
    return Tvap_;
}


template<class ParcelType>
inline CML::scalar
CML::ReactingParcel<ParcelType>::constantProperties::Tbp() const
{
    return Tbp_;
}


// * * * * * * * * * * ThermoParcel Member Functions * * * * * * * * * * * * //

template<class ParcelType>
inline CML::scalar CML::ReactingParcel<ParcelType>::mass0() const
{
    return mass0_;
}


template<class ParcelType>
inline const CML::scalarField& CML::ReactingParcel<ParcelType>::Y() const
{
    return Y_;
}


template<class ParcelType>
inline CML::scalar CML::ReactingParcel<ParcelType>::pc() const
{
    return pc_;
}


template<class ParcelType>
inline CML::scalar& CML::ReactingParcel<ParcelType>::pc()
{
    return pc_;
}


template<class ParcelType>
inline CML::scalar& CML::ReactingParcel<ParcelType>::mass0()
{
    return mass0_;
}


template<class ParcelType>
inline CML::scalarField& CML::ReactingParcel<ParcelType>::Y()
{
    return Y_;
}


// ************************************************************************* //
template<class ParcelType>
template<class CloudType>
inline CML::ReactingParcel<ParcelType>::TrackingData<CloudType>::TrackingData
(
    CloudType& cloud,
    trackPart part
)
:
    ParcelType::template TrackingData<CloudType>(cloud, part),
    pInterp_
    (
        interpolation<scalar>::New
        (
            cloud.solution().interpolationSchemes(),
            cloud.p()
        )
    )
{}


template<class ParcelType>
template<class CloudType>
inline const CML::interpolation<CML::scalar>&
CML::ReactingParcel<ParcelType>::TrackingData<CloudType>::pInterp() const
{
    return pInterp_();
}


// ************************************************************************* //


using namespace CML::constant::mathematical;

// * * * * * * * * * * *  Protected Member Functions * * * * * * * * * * * * //

template<class ParcelType>
template<class TrackData>
void CML::ReactingParcel<ParcelType>::setCellValues
(
    TrackData& td,
    const scalar dt,
    const label cellI
)
{
    ParcelType::setCellValues(td, dt, cellI);

    pc_ = td.pInterp().interpolate
    (
        this->position(),
        this->currentTetIndices()
    );

    if (pc_ < td.cloud().constProps().pMin())
    {
        if (debug)
        {
            WarningIn
            (
                "void CML::ReactingParcel<ParcelType>::setCellValues"
                "("
                    "TrackData&, "
                    "const scalar, "
                    "const label"
                ")"
            )   << "Limiting observed pressure in cell " << cellI << " to "
                << td.cloud().constProps().pMin() <<  nl << endl;
        }

        pc_ = td.cloud().constProps().pMin();
    }
}


template<class ParcelType>
template<class TrackData>
void CML::ReactingParcel<ParcelType>::cellValueSourceCorrection
(
    TrackData& td,
    const scalar dt,
    const label cellI
)
{
    scalar addedMass = 0.0;
    forAll(td.cloud().rhoTrans(), i)
    {
        addedMass += td.cloud().rhoTrans(i)[cellI];
    }

    if (addedMass < ROOTVSMALL)
    {
        return;
    }

    const scalar massCell = this->massCell(cellI);

    this->rhoc_ += addedMass/td.cloud().pMesh().cellVolumes()[cellI];

    const scalar massCellNew = massCell + addedMass;
    this->Uc_ += td.cloud().UTrans()[cellI]/massCellNew;

    scalar CpEff = 0.0;
    if (addedMass > ROOTVSMALL)
    {
        forAll(td.cloud().rhoTrans(), i)
        {
            scalar Y = td.cloud().rhoTrans(i)[cellI]/addedMass;
            CpEff += Y*td.cloud().composition().carrier().Cp(i, this->Tc_);
        }
    }

    const scalar Cpc = td.CpInterp().psi()[cellI];
    this->Cpc_ = (massCell*Cpc + addedMass*CpEff)/massCellNew;

    this->Tc_ += td.cloud().hsTrans()[cellI]/(this->Cpc_*massCellNew);

    if (this->Tc_ < td.cloud().constProps().TMin())
    {
        if (debug)
        {
            WarningIn
            (
                "void CML::ReactingParcel<ParcelType>::"
                "cellValueSourceCorrection"
                "("
                    "TrackData&, "
                    "const scalar, "
                    "const label"
                ")"
            )   << "Limiting observed temperature in cell " << cellI << " to "
                << td.cloud().constProps().TMin() <<  nl << endl;
        }

        this->Tc_ = td.cloud().constProps().TMin();
    }

//  constant pressure
//  this->pc_ = this->pc_;
}


template<class ParcelType>
template<class TrackData>
void CML::ReactingParcel<ParcelType>::correctSurfaceValues
(
    TrackData& td,
    const label cellI,
    const scalar T,
    const scalarField& Cs,
    scalar& rhos,
    scalar& mus,
    scalar& Prs,
    scalar& kappas
)
{
    // No correction if total concentration of emitted species is small
    if (sum(Cs) < SMALL)
    {
        return;
    }

    const SLGThermo& thermo = td.cloud().thermo();

    // Far field carrier  molar fractions
    scalarField Xinf(td.cloud().thermo().carrier().species().size());

    forAll(Xinf, i)
    {
        Xinf[i] = thermo.carrier().Y(i)[cellI]/thermo.carrier().W(i);
    }
    Xinf /= sum(Xinf);

    // Molar fraction of far field species at particle surface
    const scalar Xsff = 1.0 - min(sum(Cs)*specie::RR*this->T_/pc_, 1.0);

    // Surface carrier total molar concentration
    const scalar CsTot = pc_/(specie::RR*this->T_);

    // Surface carrier composition (molar fraction)
    scalarField Xs(Xinf.size());

    // Surface carrier composition (mass fraction)
    scalarField Ys(Xinf.size());

    forAll(Xs, i)
    {
        // Molar concentration of species at particle surface
        const scalar Csi = Cs[i] + Xsff*Xinf[i]*CsTot;

        Xs[i] = (2.0*Csi + Xinf[i]*CsTot)/3.0;
        Ys[i] = Xs[i]*thermo.carrier().W(i);
    }
    Xs /= sum(Xs);
    Ys /= sum(Ys);


    rhos = 0;
    mus = 0;
    kappas = 0;
    scalar Cps = 0;
    scalar sumYiSqrtW = 0;
    scalar sumYiCbrtW = 0;

    forAll(Ys, i)
    {
        const scalar W = thermo.carrier().W(i);
        const scalar sqrtW = sqrt(W);
        const scalar cbrtW = cbrt(W);

        rhos += Xs[i]*W;
        mus += Ys[i]*sqrtW*thermo.carrier().mu(i, T);
        kappas += Ys[i]*cbrtW*thermo.carrier().kappa(i, T);
        Cps += Xs[i]*thermo.carrier().Cp(i, T);

        sumYiSqrtW += Ys[i]*sqrtW;
        sumYiCbrtW += Ys[i]*cbrtW;
    }

    Cps = max(Cps, ROOTVSMALL);

    rhos *= pc_/(specie::RR*T);
    rhos = max(rhos, ROOTVSMALL);

    mus /= sumYiSqrtW;
    mus = max(mus, ROOTVSMALL);

    kappas /= sumYiCbrtW;
    kappas = max(kappas, ROOTVSMALL);

    Prs = Cps*mus/kappas;
}


template<class ParcelType>
CML::scalar CML::ReactingParcel<ParcelType>::updateMassFraction
(
    const scalar mass0,
    const scalarField& dMass,
    scalarField& Y
) const
{
    scalar mass1 = mass0 - sum(dMass);

    // only update the mass fractions if the new particle mass is finite
    if (mass1 > ROOTVSMALL)
    {
        forAll(Y, i)
        {
            Y[i] = (Y[i]*mass0 - dMass[i])/mass1;
        }
    }

    return mass1;
}


template<class ParcelType>
template<class TrackData>
void CML::ReactingParcel<ParcelType>::calc
(
    TrackData& td,
    const scalar dt,
    const label cellI
)
{
    typedef typename TrackData::cloudType::reactingCloudType reactingCloudType;
    const CompositionModel<reactingCloudType>& composition =
        td.cloud().composition();


    // Define local properties at beginning of time step
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    const scalar np0 = this->nParticle_;
    const scalar d0 = this->d_;
    const vector& U0 = this->U_;
    const scalar T0 = this->T_;
    const scalar mass0 = this->mass();


    // Calc surface values
    scalar Ts, rhos, mus, Prs, kappas;
    this->calcSurfaceValues(td, cellI, T0, Ts, rhos, mus, Prs, kappas);
    scalar Res = this->Re(U0, d0, rhos, mus);


    // Sources
    //~~~~~~~~

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


    // 1. Compute models that contribute to mass transfer - U, T held constant
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Phase change
    // ~~~~~~~~~~~~

    // Mass transfer due to phase change
    scalarField dMassPC(Y_.size(), 0.0);

    // Molar flux of species emitted from the particle (kmol/m^2/s)
    scalar Ne = 0.0;

    // Sum Ni*Cpi*Wi of emission species
    scalar NCpW = 0.0;

    // Surface concentrations of emitted species
    scalarField Cs(composition.carrier().species().size(), 0.0);

    // Calc mass and enthalpy transfer due to phase change
    calcPhaseChange
    (
        td,
        dt,
        cellI,
        Res,
        Prs,
        Ts,
        mus/rhos,
        d0,
        T0,
        mass0,
        0,
        1.0,
        Y_,
        dMassPC,
        Sh,
        Ne,
        NCpW,
        Cs
    );


    // 2. Update the parcel properties due to change in mass
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    scalarField dMass(dMassPC);
    scalar mass1 = updateMassFraction(mass0, dMass, Y_);

    this->Cp_ = composition.Cp(0, Y_, pc_, T0);

    // Update particle density or diameter
    if (td.cloud().constProps().constantVolume())
    {
        this->rho_ = mass1/this->volume();
    }
    else
    {
        this->d_ = cbrt(mass1/this->rho_*6.0/pi);
    }

    // Remove the particle when mass falls below minimum threshold
    if (np0*mass1 < td.cloud().constProps().minParticleMass())
    {
        td.keepParticle = false;

        if (td.cloud().solution().coupled())
        {
            scalar dm = np0*mass0;

            // Absorb parcel into carrier phase
            forAll(Y_, i)
            {
                scalar dmi = dm*Y_[i];
                label gid = composition.localToGlobalCarrierId(0, i);
                scalar hs = composition.carrier().Hs(gid, T0);

                td.cloud().rhoTrans(gid)[cellI] += dmi;
                td.cloud().hsTrans()[cellI] += dmi*hs;
            }
            td.cloud().UTrans()[cellI] += dm*U0;

            td.cloud().phaseChange().addToPhaseChangeMass(np0*mass1);
        }

        return;
    }

    // Correct surface values due to emitted species
    correctSurfaceValues(td, cellI, Ts, Cs, rhos, mus, Prs, kappas);
    Res = this->Re(U0, this->d_, rhos, mus);


    // 3. Compute heat- and momentum transfers
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Heat transfer
    // ~~~~~~~~~~~~~

    // Calculate new particle temperature
    this->T_ =
        this->calcHeatTransfer
        (
            td,
            dt,
            cellI,
            Res,
            Prs,
            kappas,
            NCpW,
            Sh,
            dhsTrans,
            Sph
        );

    this->Cp_ = composition.Cp(0, Y_, pc_, T0);


    // Motion
    // ~~~~~~

    // Calculate new particle velocity
    this->U_ =
        this->calcVelocity(td, dt, cellI, Res, mus, mass1, Su, dUTrans, Spu);


    // 4. Accumulate carrier phase source terms
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    if (td.cloud().solution().coupled())
    {
        // Transfer mass lost to carrier mass, momentum and enthalpy sources
        forAll(dMass, i)
        {
            scalar dm = np0*dMass[i];
            label gid = composition.localToGlobalCarrierId(0, i);
            scalar hs = composition.carrier().Hs(gid, T0);

            td.cloud().rhoTrans(gid)[cellI] += dm;
            td.cloud().UTrans()[cellI] += dm*U0;
            td.cloud().hsTrans()[cellI] += dm*hs;
        }

        // Update momentum transfer
        td.cloud().UTrans()[cellI] += np0*dUTrans;
        td.cloud().UCoeff()[cellI] += np0*Spu;

        // Update sensible enthalpy transfer
        td.cloud().hsTrans()[cellI] += np0*dhsTrans;
        td.cloud().hsCoeff()[cellI] += np0*Sph;
    }
}


template<class ParcelType>
template<class TrackData>
void CML::ReactingParcel<ParcelType>::calcPhaseChange
(
    TrackData& td,
    const scalar dt,
    const label cellI,
    const scalar Re,
    const scalar Pr,
    const scalar Ts,
    const scalar nus,
    const scalar d,
    const scalar T,
    const scalar mass,
    const label idPhase,
    const scalar YPhase,
    const scalarField& YComponents,
    scalarField& dMassPC,
    scalar& Sh,
    scalar& N,
    scalar& NCpW,
    scalarField& Cs
)
{
    if
    (
        !td.cloud().phaseChange().active()
     || T < td.cloud().constProps().Tvap()
     || YPhase < SMALL
    )
    {
        return;
    }

    typedef typename TrackData::cloudType::reactingCloudType reactingCloudType;
    const CompositionModel<reactingCloudType>& composition =
        td.cloud().composition();


    // Calculate mass transfer due to phase change
    td.cloud().phaseChange().calculate
    (
        dt,
        cellI,
        Re,
        Pr,
        d,
        nus,
        T,
        Ts,
        pc_,
        this->Tc_,
        YComponents,
        dMassPC
    );

    // Limit phase change mass by availability of each specie
    dMassPC = min(mass*YPhase*YComponents, dMassPC);

    const scalar dMassTot = sum(dMassPC);

    // Add to cumulative phase change mass
    td.cloud().phaseChange().addToPhaseChangeMass(this->nParticle_*dMassTot);

    forAll(dMassPC, i)
    {
        const label idc = composition.localToGlobalCarrierId(idPhase, i);
        const label idl = composition.globalIds(idPhase)[i];

        const scalar dh = td.cloud().phaseChange().dh(idc, idl, pc_, T);
        Sh -= dMassPC[i]*dh/dt;
    }


    // Update molar emissions
    if (td.cloud().heatTransfer().BirdCorrection())
    {
        // Average molecular weight of carrier mix - assumes perfect gas
        const scalar Wc = this->rhoc_*specie::RR*this->Tc_/this->pc_;


        forAll(dMassPC, i)
        {
            const label idc = composition.localToGlobalCarrierId(idPhase, i);
            const label idl = composition.globalIds(idPhase)[i];

            const scalar Cp = composition.carrier().Cp(idc, Ts);
            const scalar W = composition.carrier().W(idc);
            const scalar Ni = dMassPC[i]/(this->areaS(d)*dt*W);

            const scalar Dab =
                composition.liquids().properties()[idl].D(pc_, Ts, Wc);

            // Molar flux of species coming from the particle (kmol/m^2/s)
            N += Ni;

            // Sum of Ni*Cpi*Wi of emission species
            NCpW += Ni*Cp*W;

            // Concentrations of emission species
            Cs[idc] += Ni*d/(2.0*Dab);
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParcelType>
CML::ReactingParcel<ParcelType>::ReactingParcel
(
    const ReactingParcel<ParcelType>& p
)
:
    ParcelType(p),
    mass0_(p.mass0_),
    Y_(p.Y_),
    pc_(p.pc_)
{}


template<class ParcelType>
CML::ReactingParcel<ParcelType>::ReactingParcel
(
    const ReactingParcel<ParcelType>& p,
    const polyMesh& mesh
)
:
    ParcelType(p, mesh),
    mass0_(p.mass0_),
    Y_(p.Y_),
    pc_(p.pc_)
{}


// * * * * * * * * * * * * * * IOStream operators  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class ParcelType>
CML::string CML::ReactingParcel<ParcelType>::propHeader =
    ParcelType::propHeader
  + " mass0"
  + " nPhases(Y1..YN)";


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParcelType>
CML::ReactingParcel<ParcelType>::ReactingParcel
(
    const polyMesh& mesh,
    Istream& is,
    bool readFields
)
:
    ParcelType(mesh, is, readFields),
    mass0_(0.0),
    Y_(0),
    pc_(0.0)
{
    if (readFields)
    {
        DynamicList<scalar> Ymix;

        if (is.format() == IOstream::ASCII)
        {
            is >> mass0_ >> Ymix;
        }
        else
        {
            is.read
            (
                reinterpret_cast<char*>(&mass0_),
              + sizeof(mass0_)
            );
            is >> Ymix;
        }

        Y_.transfer(Ymix);
    }

    // Check state of Istream
    is.check
    (
        "ReactingParcel<ParcelType>::ReactingParcel"
        "("
            "const polyMesh&, "
            "Istream&, "
            "bool"
        ")"
    );
}


template<class ParcelType>
template<class CloudType>
void CML::ReactingParcel<ParcelType>::readFields(CloudType& c)
{
    if (!c.size())
    {
        return;
    }

    ParcelType::readFields(c);
}


template<class ParcelType>
template<class CloudType, class CompositionType>
void CML::ReactingParcel<ParcelType>::readFields
(
    CloudType& c,
    const CompositionType& compModel
)
{
    if (!c.size())
    {
        return;
    }

    ParcelType::readFields(c);

    IOField<scalar> mass0(c.fieldIOobject("mass0", IOobject::MUST_READ));
    c.checkFieldIOobject(c, mass0);

    label i = 0;
    forAllIter(typename Cloud<ReactingParcel<ParcelType> >, c, iter)
    {
        ReactingParcel<ParcelType>& p = iter();
        p.mass0_ = mass0[i++];
    }

    // Get names and sizes for each Y...
    const wordList& phaseTypes = compModel.phaseTypes();
    const label nPhases = phaseTypes.size();
    wordList stateLabels(nPhases, "");
    if (compModel.nPhase() == 1)
    {
        stateLabels = compModel.stateLabels()[0];
    }


    // Set storage for each Y... for each parcel
    forAllIter(typename Cloud<ReactingParcel<ParcelType> >, c, iter)
    {
        ReactingParcel<ParcelType>& p = iter();
        p.Y_.setSize(nPhases, 0.0);
    }

    // Populate Y for each parcel
    forAll(phaseTypes, j)
    {
        IOField<scalar> Y
        (
            c.fieldIOobject
            (
                "Y" + phaseTypes[j] + stateLabels[j],
                 IOobject::MUST_READ
            )
        );

        label i = 0;
        forAllIter(typename Cloud<ReactingParcel<ParcelType> >, c, iter)
        {
            ReactingParcel<ParcelType>& p = iter();
            p.Y_[j] = Y[i++];
        }
    }
}


template<class ParcelType>
template<class CloudType>
void CML::ReactingParcel<ParcelType>::writeFields(const CloudType& c)
{
    ParcelType::writeFields(c);
}


template<class ParcelType>
template<class CloudType, class CompositionType>
void CML::ReactingParcel<ParcelType>::writeFields
(
    const CloudType& c,
    const CompositionType& compModel
)
{
    ParcelType::writeFields(c);

    const label np = c.size();

    if (np > 0)
    {
        IOField<scalar> mass0(c.fieldIOobject("mass0", IOobject::NO_READ), np);

        label i = 0;
        forAllConstIter(typename Cloud<ReactingParcel<ParcelType> >, c, iter)
        {
            const ReactingParcel<ParcelType>& p = iter();
            mass0[i++] = p.mass0_;
        }
        mass0.write();

        // Write the composition fractions
        const wordList& phaseTypes = compModel.phaseTypes();
        wordList stateLabels(phaseTypes.size(), "");
        if (compModel.nPhase() == 1)
        {
            stateLabels = compModel.stateLabels()[0];
        }

        forAll(phaseTypes, j)
        {
            IOField<scalar> Y
            (
                c.fieldIOobject
                (
                    "Y" + phaseTypes[j] + stateLabels[j],
                    IOobject::NO_READ
                ),
                np
            );
            label i = 0;
            forAllConstIter
            (
                typename Cloud<ReactingParcel<ParcelType> >,
                c,
                iter
            )
            {
                const ReactingParcel<ParcelType>& p = iter();
                Y[i++] = p.Y()[j];
            }

            Y.write();
        }
    }
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class ParcelType>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const ReactingParcel<ParcelType>& p
)
{
    if (os.format() == IOstream::ASCII)
    {
        os  << static_cast<const ParcelType&>(p)
            << token::SPACE << p.mass0()
            << token::SPACE << p.Y();
    }
    else
    {
        os  << static_cast<const ParcelType&>(p);
        os.write
        (
            reinterpret_cast<const char*>(&p.mass0_),
            sizeof(p.mass0())
        );
        os  << p.Y();
    }

    // Check state of Ostream
    os.check
    (
        "Ostream& operator<<(Ostream&, const ReactingParcel<ParcelType>&)"
    );

    return os;
}


#endif

// ************************************************************************* //
