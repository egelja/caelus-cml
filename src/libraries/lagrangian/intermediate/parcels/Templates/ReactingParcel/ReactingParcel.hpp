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
#include "PhaseChangeModel.hpp"
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
    // Private data

        //- Size in bytes of the fields
        static const std::size_t sizeofFields_;


public:

    //- Class to hold reacting parcel constant properties
    class constantProperties
    :
        public ParcelType::constantProperties
    {
        // Private data

            //- Minimum pressure [Pa]
            demandDrivenEntry<scalar> pMin_;

            //- Constant volume flag - e.g. during mass transfer
            demandDrivenEntry<bool> constantVolume_;


    public:

        // Constructors

            //- Null constructor
            constantProperties();

            //- Copy constructor
            constantProperties(const constantProperties& cp);

            //- Construct from dictionary
            constantProperties(const dictionary& parentDict);


        // Access

            //- Return const access to the minimum pressure
            inline scalar pMin() const;

            //- Return const access to the constant volume flag
            inline bool constantVolume() const;
    };


    class trackingData
    :
        public ParcelType::trackingData
    {
    private:

        // Private data

            // Interpolators for continuous phase fields

                //- Interpolator for continuous phase pressure field
                autoPtr<interpolation<scalar> > pInterp_;


            // Cached continuous phase properties

                //- Pressure [Pa]
                scalar pc_;


    public:

        typedef typename ParcelType::trackingData::trackPart trackPart;

        // Constructors

            //- Construct from components
            template<class TrackCloudType>
            inline trackingData
            (
                const TrackCloudType& cloud,
                trackPart part = ParcelType::trackingData::tpLinearTrack
            );


        // Member functions

            //- Return const access to the interpolator for continuous phase
            //  pressure field
            inline const interpolation<scalar>& pInterp() const;

            //- Return the continuous phase pressure
            inline scalar pc() const;

            //- Access the continuous phase pressure
            inline scalar& pc();
    };


protected:

    // Protected data

        // Parcel properties

            //- Initial mass [kg]
            scalar mass0_;

            //- Mass fractions of mixture []
            scalarField Y_;


    // Protected Member Functions

        //- Calculate Phase change
        template<class TrackCloudType>
        void calcPhaseChange
        (
            TrackCloudType& cloud,
            trackingData& td,
            const scalar dt,           // timestep
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
            scalarField& dMassPC,      // mass transfer - local to parcel
            scalar& Sh,                // explicit parcel enthalpy source
            scalar& N,                 // flux of species emitted from parcel
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

        //- Runtime type information
        TypeName("ReactingParcel");

        //- String representation of properties
        AddToPropertyList
        (
            ParcelType,
            " mass0"
          + " nPhases(Y1..YN)"
        );


    // Constructors

        //- Construct from mesh, coordinates and topology
        //  Other properties initialised as null
        inline ReactingParcel
        (
            const polyMesh& mesh,
            const barycentric& coordinates,
            const label celli,
            const label tetFacei,
            const label tetPti
        );

        //- Construct from a position and a cell, searching for the rest of the
        //  required topology. Other properties are initialised as null.
        inline ReactingParcel
        (
            const polyMesh& mesh,
            const vector& position,
            const label celli
        );

        //- Construct from components
        inline ReactingParcel
        (
            const polyMesh& mesh,
            const barycentric& coordinates,
            const label celli,
            const label tetFacei,
            const label tetPti,
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

            //- Return const access to initial mass [kg]
            inline scalar mass0() const;

            //- Return const access to mass fractions of mixture []
            inline const scalarField& Y() const;


        // Edit

            //- Return access to initial mass [kg]
            inline scalar& mass0();

            //- Return access to mass fractions of mixture []
            inline scalarField& Y();


        // Main calculation loop

            //- Set cell values
            template<class TrackCloudType>
            void setCellValues(TrackCloudType& cloud, trackingData& td);

            //- Correct cell values using latest transfer information
            template<class TrackCloudType>
            void cellValueSourceCorrection
            (
                TrackCloudType& cloud,
                trackingData& td,
                const scalar dt
            );

            //- Correct surface values due to emitted species
            template<class TrackCloudType>
            void correctSurfaceValues
            (
                TrackCloudType& cloud,
                trackingData& td,
                const scalar T,
                const scalarField& Cs,
                scalar& rhos,
                scalar& mus,
                scalar& Prs,
                scalar& kappas
            );

            //- Update parcel properties over the time interval
            template<class TrackCloudType>
            void calc
            (
                TrackCloudType& cloud,
                trackingData& td,
                const scalar dt
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
    pMin_(this->dict_, 0.0),
    constantVolume_(this->dict_, false)
{}


template<class ParcelType>
inline CML::ReactingParcel<ParcelType>::constantProperties::constantProperties
(
    const constantProperties& cp
)
:
    ParcelType::constantProperties(cp),
    pMin_(cp.pMin_),
    constantVolume_(cp.constantVolume_)
{}


template<class ParcelType>
inline CML::ReactingParcel<ParcelType>::constantProperties::constantProperties
(
    const dictionary& parentDict
)
:
    ParcelType::constantProperties(parentDict),
    pMin_(this->dict_, "pMin", 1000.0),
    constantVolume_(this->dict_, word("constantVolume"))
{}


template<class ParcelType>
inline CML::ReactingParcel<ParcelType>::ReactingParcel
(
    const polyMesh& mesh,
    const barycentric& coordinates,
    const label celli,
    const label tetFacei,
    const label tetPti
)
:
    ParcelType(mesh, coordinates, celli, tetFacei, tetPti),
    mass0_(0.0),
    Y_(0)
{}


template<class ParcelType>
inline CML::ReactingParcel<ParcelType>::ReactingParcel
(
    const polyMesh& mesh,
    const vector& position,
    const label celli
)
:
    ParcelType(mesh, position, celli),
    mass0_(0.0),
    Y_(0)
{}


template<class ParcelType>
inline CML::ReactingParcel<ParcelType>::ReactingParcel
(
    const polyMesh& mesh,
    const barycentric& coordinates,
    const label celli,
    const label tetFacei,
    const label tetPti,
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
        coordinates,
        celli,
        tetFacei,
        tetPti,
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
    Y_(Y0)
{
    // Set initial parcel mass
    mass0_ = this->mass();
}


// * * * * * * * * * constantProperties Member Functions * * * * * * * * * * //

template<class ParcelType>
inline CML::scalar
CML::ReactingParcel<ParcelType>::constantProperties::pMin() const
{
    return pMin_.value();
}


template<class ParcelType>
inline bool
CML::ReactingParcel<ParcelType>::constantProperties::constantVolume() const
{
    return constantVolume_.value();
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
template<class TrackCloudType>
inline CML::ReactingParcel<ParcelType>::trackingData::trackingData
(
    const TrackCloudType& cloud,
    trackPart part
)
:
    ParcelType::trackingData(cloud, part),
    pInterp_
    (
        interpolation<scalar>::New
        (
            cloud.solution().interpolationSchemes(),
            cloud.p()
        )
    ),
    pc_(Zero)
{}


template<class ParcelType>
inline const CML::interpolation<CML::scalar>&
CML::ReactingParcel<ParcelType>::trackingData::pInterp() const
{
    return pInterp_();
}


template<class ParcelType>
inline CML::scalar CML::ReactingParcel<ParcelType>::trackingData::pc() const
{
    return pc_;
}


template<class ParcelType>
inline CML::scalar& CML::ReactingParcel<ParcelType>::trackingData::pc()
{
    return pc_;
}


// ************************************************************************* //


using namespace CML::constant::mathematical;

// * * * * * * * * * * *  Protected Member Functions * * * * * * * * * * * * //

template<class ParcelType>
template<class TrackCloudType>
void CML::ReactingParcel<ParcelType>::calcPhaseChange
(
    TrackCloudType& cloud,
    trackingData& td,
    const scalar dt,
    const scalar Re,
    const scalar Pr,
    const scalar Ts,
    const scalar nus,
    const scalar d,
    const scalar T,
    const scalar mass,
    const label idPhase,
    const scalar YPhase,
    const scalarField& Y,
    scalarField& dMassPC,
    scalar& Sh,
    scalar& N,
    scalar& NCpW,
    scalarField& Cs
)
{
    typedef typename TrackCloudType::reactingCloudType reactingCloudType;
    const CompositionModel<reactingCloudType>& composition =
        cloud.composition();
    PhaseChangeModel<reactingCloudType>& phaseChange = cloud.phaseChange();

    if (!phaseChange.active() || (YPhase < SMALL))
    {
        return;
    }

    scalarField X(composition.liquids().X(Y));

    scalar Tvap = phaseChange.Tvap(X);

    if (T < Tvap)
    {
        return;
    }

    const scalar TMax = phaseChange.TMax(td.pc(), X);
    const scalar Tdash = min(T, TMax);
    const scalar Tsdash = min(Ts, TMax);

    scalarField hmm(dMassPC);

    // Calculate mass transfer due to phase change
    phaseChange.calculate
    (
        dt,
        this->cell(),
        Re,
        Pr,
        d,
        nus,
        Tdash,
        Tsdash,
        td.pc(),
        td.Tc(),
        X,
        dMassPC
    );

    // Limit phase change mass by availability of each specie
    dMassPC = min(mass*YPhase*Y, dMassPC);

    const scalar dMassTot = sum(dMassPC);

    // Add to cumulative phase change mass
    phaseChange.addToPhaseChangeMass(this->nParticle_*dMassTot);

    forAll(dMassPC, i)
    {
        const label cid = composition.localToCarrierId(idPhase, i);

        const scalar dh = phaseChange.dh(cid, i, td.pc(), Tdash);
        Sh -= dMassPC[i]*dh/dt;
    }


    // Update molar emissions
    if (cloud.heatTransfer().BirdCorrection())
    {
        // Average molecular weight of carrier mix - assumes perfect gas
        const scalar Wc = td.rhoc()*RR*td.Tc()/td.pc();

        forAll(dMassPC, i)
        {
            const label cid = composition.localToCarrierId(idPhase, i);

            const scalar Cp = composition.carrier().Cp(cid, td.pc(), Tsdash);
            const scalar W = composition.carrier().Wi(cid);
            const scalar Ni = dMassPC[i]/(this->areaS(d)*dt*W);

            const scalar Dab =
                composition.liquids().properties()[i].D(td.pc(), Tsdash, Wc);

            // Molar flux of species coming from the particle (kmol/m^2/s)
            N += Ni;

            // Sum of Ni*Cpi*Wi of emission species
            NCpW += Ni*Cp*W;

            // Concentrations of emission species
            Cs[cid] += Ni*d/(2.0*Dab);
        }
    }
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


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParcelType>
CML::ReactingParcel<ParcelType>::ReactingParcel
(
    const ReactingParcel<ParcelType>& p
)
:
    ParcelType(p),
    mass0_(p.mass0_),
    Y_(p.Y_)
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
    Y_(p.Y_)
{}


// * * * * * * * * * * * * *  Member Functions * * * * * * * * * * * * * * * //

template<class ParcelType>
template<class TrackCloudType>
void CML::ReactingParcel<ParcelType>::setCellValues
(
    TrackCloudType& cloud,
    trackingData& td
)
{
    ParcelType::setCellValues(cloud, td);

    td.pc() = td.pInterp().interpolate
    (
        this->coordinates(),
        this->currentTetIndices()
    );

    if (td.pc() < cloud.constProps().pMin())
    {
        if (debug)
        {
            WarningInFunction
                << "Limiting observed pressure in cell " << this->cell()
                << " to " << cloud.constProps().pMin() <<  nl << endl;
        }

        td.pc() = cloud.constProps().pMin();
    }
}


template<class ParcelType>
template<class TrackCloudType>
void CML::ReactingParcel<ParcelType>::cellValueSourceCorrection
(
    TrackCloudType& cloud,
    trackingData& td,
    const scalar dt
)
{
    scalar addedMass = 0.0;
    scalar maxMassI = 0.0;
    forAll(cloud.rhoTrans(), i)
    {
        scalar dm = cloud.rhoTrans(i)[this->cell()];
        maxMassI = max(maxMassI, mag(dm));
        addedMass += dm;
    }

    if (maxMassI < ROOTVSMALL)
    {
        return;
    }

    const scalar massCell = this->massCell(td);

    td.rhoc() += addedMass/cloud.pMesh().cellVolumes()[this->cell()];

    const scalar massCellNew = massCell + addedMass;
    td.Uc() = (td.Uc()*massCell + cloud.UTrans()[this->cell()])/massCellNew;

    scalar CpEff = 0.0;
    forAll(cloud.rhoTrans(), i)
    {
        scalar Y = cloud.rhoTrans(i)[this->cell()]/addedMass;
        CpEff += Y*cloud.composition().carrier().Cp(i, td.pc(), td.Tc());
    }

    const scalar Cpc = td.CpInterp().psi()[this->cell()];
    td.Cpc() = (massCell*Cpc + addedMass*CpEff)/massCellNew;

    td.Tc() += cloud.hsTrans()[this->cell()]/(td.Cpc()*massCellNew);

    if (td.Tc() < cloud.constProps().TMin())
    {
        if (debug)
        {
            WarningInFunction
                << "Limiting observed temperature in cell " << this->cell()
                << " to " << cloud.constProps().TMin() <<  nl << endl;
        }

        td.Tc() = cloud.constProps().TMin();
    }
}


template<class ParcelType>
template<class TrackCloudType>
void CML::ReactingParcel<ParcelType>::correctSurfaceValues
(
    TrackCloudType& cloud,
    trackingData& td,
    const scalar T,
    const scalarField& Cs,
    scalar& rhos,
    scalar& mus,
    scalar& Prs,
    scalar& kappas
)
{
    // No correction if total concentration of emitted species is small
    if (!cloud.heatTransfer().BirdCorrection() || (sum(Cs) < SMALL))
    {
        return;
    }

    const SLGThermo& thermo = cloud.thermo();

    // Far field carrier  molar fractions
    scalarField Xinf(thermo.carrier().species().size());

    forAll(Xinf, i)
    {
        Xinf[i] = thermo.carrier().Y(i)[this->cell()]/thermo.carrier().Wi(i);
    }
    Xinf /= sum(Xinf);

    // Molar fraction of far field species at particle surface
    const scalar Xsff = 1.0 - min(sum(Cs)*RR*this->T_/td.pc(), 1.0);

    // Surface carrier total molar concentration
    const scalar CsTot = td.pc()/(RR*this->T_);

    // Surface carrier composition (molar fraction)
    scalarField Xs(Xinf.size());

    // Surface carrier composition (mass fraction)
    scalarField Ys(Xinf.size());

    forAll(Xs, i)
    {
        // Molar concentration of species at particle surface
        const scalar Csi = Cs[i] + Xsff*Xinf[i]*CsTot;

        Xs[i] = (2.0*Csi + Xinf[i]*CsTot)/3.0;
        Ys[i] = Xs[i]*thermo.carrier().Wi(i);
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
        const scalar W = thermo.carrier().Wi(i);
        const scalar sqrtW = sqrt(W);
        const scalar cbrtW = cbrt(W);

        rhos += Xs[i]*W;
        mus += Ys[i]*sqrtW*thermo.carrier().mu(i, td.pc(), T);
        kappas += Ys[i]*cbrtW*thermo.carrier().kappa(i, td.pc(), T);
        Cps += Xs[i]*thermo.carrier().Cp(i, td.pc(), T);

        sumYiSqrtW += Ys[i]*sqrtW;
        sumYiCbrtW += Ys[i]*cbrtW;
    }

    Cps = max(Cps, ROOTVSMALL);

    rhos *= td.pc()/(RR*T);
    rhos = max(rhos, ROOTVSMALL);

    mus /= sumYiSqrtW;
    mus = max(mus, ROOTVSMALL);

    kappas /= sumYiCbrtW;
    kappas = max(kappas, ROOTVSMALL);

    Prs = Cps*mus/kappas;
}


template<class ParcelType>
template<class TrackCloudType>
void CML::ReactingParcel<ParcelType>::calc
(
    TrackCloudType& cloud,
    trackingData& td,
    const scalar dt
)
{
    typedef typename TrackCloudType::reactingCloudType reactingCloudType;
    const CompositionModel<reactingCloudType>& composition =
        cloud.composition();


    // Define local properties at beginning of time step
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    const scalar np0 = this->nParticle_;
    const scalar d0 = this->d_;
    const vector& U0 = this->U_;
    const scalar T0 = this->T_;
    const scalar mass0 = this->mass();


    // Calc surface values
    scalar Ts, rhos, mus, Prs, kappas;
    this->calcSurfaceValues(cloud, td, T0, Ts, rhos, mus, Prs, kappas);
    scalar Res = this->Re(rhos, U0, td.Uc(), d0, mus);


    // Sources
    // ~~~~~~~

    // Explicit momentum source for particle
    vector Su = Zero;

    // Linearised momentum source coefficient
    scalar Spu = 0.0;

    // Momentum transfer from the particle to the carrier phase
    vector dUTrans = Zero;

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
        cloud,
        td,
        dt,
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

    this->Cp_ = composition.Cp(0, Y_, td.pc(), T0);

    // Update particle density or diameter
    if (cloud.constProps().constantVolume())
    {
        this->rho_ = mass1/this->volume();
    }
    else
    {
        this->d_ = cbrt(mass1/this->rho_*6.0/pi);
    }

    // Remove the particle when mass falls below minimum threshold
    if (np0*mass1 < cloud.constProps().minParcelMass())
    {
        td.keepParticle = false;

        if (cloud.solution().coupled())
        {
            scalar dm = np0*mass0;

            // Absorb parcel into carrier phase
            forAll(Y_, i)
            {
                scalar dmi = dm*Y_[i];
                label gid = composition.localToCarrierId(0, i);
                scalar hs = composition.carrier().Hs(gid, td.pc(), T0);

                cloud.rhoTrans(gid)[this->cell()] += dmi;
                cloud.hsTrans()[this->cell()] += dmi*hs;
            }
            cloud.UTrans()[this->cell()] += dm*U0;

            cloud.phaseChange().addToPhaseChangeMass(np0*mass1);
        }

        return;
    }

    // Correct surface values due to emitted species
    correctSurfaceValues(cloud, td, Ts, Cs, rhos, mus, Prs, kappas);
    Res = this->Re(rhos, U0, td.Uc(), this->d(), mus);


    // 3. Compute heat- and momentum transfers
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Heat transfer
    // ~~~~~~~~~~~~~

    // Calculate new particle temperature
    this->T_ =
        this->calcHeatTransfer
        (
            cloud,
            td,
            dt,
            Res,
            Prs,
            kappas,
            NCpW,
            Sh,
            dhsTrans,
            Sph
        );

    this->Cp_ = composition.Cp(0, Y_, td.pc(), T0);


    // Motion
    // ~~~~~~

    // Calculate new particle velocity
    this->U_ =
        this->calcVelocity(cloud, td, dt, Res, mus, mass1, Su, dUTrans, Spu);


    // 4. Accumulate carrier phase source terms
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    if (cloud.solution().coupled())
    {
        // Transfer mass lost to carrier mass, momentum and enthalpy sources
        forAll(dMass, i)
        {
            scalar dm = np0*dMass[i];
            label gid = composition.localToCarrierId(0, i);
            scalar hs = composition.carrier().Hs(gid, td.pc(), T0);

            cloud.rhoTrans(gid)[this->cell()] += dm;
            cloud.UTrans()[this->cell()] += dm*U0;
            cloud.hsTrans()[this->cell()] += dm*hs;
        }

        // Update momentum transfer
        cloud.UTrans()[this->cell()] += np0*dUTrans;
        cloud.UCoeff()[this->cell()] += np0*Spu;

        // Update sensible enthalpy transfer
        cloud.hsTrans()[this->cell()] += np0*dhsTrans;
        cloud.hsCoeff()[this->cell()] += np0*Sph;

        // Update radiation fields
        if (cloud.radiation())
        {
            const scalar ap = this->areaP();
            const scalar T4 = pow4(T0);
            cloud.radAreaP()[this->cell()] += dt*np0*ap;
            cloud.radT4()[this->cell()] += dt*np0*T4;
            cloud.radAreaPT4()[this->cell()] += dt*np0*ap*T4;
        }
    }
}



// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class ParcelType>
CML::string CML::ReactingParcel<ParcelType>::propertyList_ =
    CML::ReactingParcel<ParcelType>::propertyList();

template<class ParcelType>
const std::size_t CML::ReactingParcel<ParcelType>::sizeofFields_
(
    sizeof(scalar)
);


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
    Y_(0)
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
            is.read(reinterpret_cast<char*>(&mass0_), sizeofFields_);
            is >> Ymix;
        }

        Y_.transfer(Ymix);
    }

    is.check(FUNCTION_NAME);
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
            ReactingParcel<ParcelType>::sizeofFields_
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
