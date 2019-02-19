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
    CML::StandardChemistryModel

Description
    Extends base chemistry model by adding a thermo package, and ODE functions.
    Introduces chemistry equation system and evaluation of chemical source
    terms.


\*---------------------------------------------------------------------------*/

#ifndef StandardChemistryModel_HPP
#define StandardChemistryModel_HPP

#include "BasicChemistryModel_.hpp"
#include "Reaction.hpp"
#include "ODESystem.hpp"
#include "volFields.hpp"
#include "simpleMatrix.hpp"
#include "uniformField.hpp"
#include "extrapolatedCalculatedFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class fvMesh;

/*---------------------------------------------------------------------------*\
                      Class StandardChemistryModel Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo, class ThermoType>
class StandardChemistryModel
:
    public BasicChemistryModel<ReactionThermo>,
    public ODESystem
{

    //- Solve the reaction system for the given time step
    //  of given type and return the characteristic time
    template<class DeltaTType>
    scalar solve(const DeltaTType& deltaT);

    //- Disallow copy constructor
    StandardChemistryModel(const StandardChemistryModel&);

    //- Disallow default bitwise assignment
    void operator=(const StandardChemistryModel&);


protected:

    typedef ThermoType thermoType;

    //- Reference to the field of specie mass fractions
    PtrList<volScalarField>& Y_;

    //- Reactions
    const PtrList<Reaction<ThermoType> >& reactions_;

    //- Thermodynamic data of the species
    const PtrList<ThermoType>& specieThermo_;

    //- Number of species
    label nSpecie_;

    //- Number of reactions
    label nReaction_;

    //- Temperature below which the reaction rates are assumed 0
    scalar Treact_;

    //- List of reaction rate per specie [kg/m3/s]
    PtrList<DimensionedField<scalar, volMesh> > RR_;

    //- Temporary concentration field
    mutable scalarField c_;

    //- Temporary rate-of-change of concentration field
    mutable scalarField dcdt_;


    //- Write access to chemical source terms
    //  (e.g. for multi-chemistry model)
    inline PtrList<DimensionedField<scalar, volMesh> >& RR();


public:

    //- Runtime type information
    TypeName("standard");


    //- Construct from thermo
    StandardChemistryModel(ReactionThermo& thermo);


    //- Destructor
    virtual ~StandardChemistryModel()
    {}


    // Member Functions

    //- The reactions
    inline const PtrList<Reaction<ThermoType> >& reactions() const;

    //- Thermodynamic data of the species
    inline const PtrList<ThermoType>& specieThermo() const;

    //- The number of species
    virtual inline label nSpecie() const;

    //- The number of reactions
    virtual inline label nReaction() const;

    //- Temperature below which the reaction rates are assumed 0
    inline scalar Treact() const;

    //- Temperature below which the reaction rates are assumed 0
    inline scalar& Treact();

    //- dc/dt = omega, rate of change in concentration, for each species
    virtual void omega
    (
         const scalarField& c,
         const scalar T,
         const scalar p,
         scalarField& dcdt
    ) const;


    //- Return the reaction rate for iReaction and the reference
    //  species and charateristic times
    virtual scalar omegaI
    (
        label iReaction,
        const scalarField& c,
        const scalar T,
        const scalar p,
        scalar& pf,
        scalar& cf,
        label& lRef,
        scalar& pr,
        scalar& cr,
        label& rRef
    ) const;

    //- Calculates the reaction rates
    virtual void calculate();


    // Chemistry model functions (overriding abstract functions in
    // basicChemistryModel.H)

    //- Return const access to the chemical source terms for specie, i
    inline const DimensionedField<scalar, volMesh>& RR
    (
       const label i
    ) const;

    //- Return non const access to chemical source terms [kg/m3/s]
    virtual DimensionedField<scalar, volMesh>& RR
    (
        const label i
    );

    //- Return reaction rate of the speciei in reactionI
    virtual tmp<DimensionedField<scalar, volMesh> > calculateRR
    (
        const label reactionI,
        const label speciei
    ) const;

    //- Solve the reaction system for the given time step
    //  and return the characteristic time
    virtual scalar solve(const scalar deltaT);

    //- Solve the reaction system for the given time step
    //  and return the characteristic time
    virtual scalar solve(const scalarField& deltaT);

    //- Return the chemical time scale
    virtual tmp<volScalarField> tc() const;

    //- Return the heat release rate [kg/m/s3]
    virtual tmp<volScalarField> Qdot() const;


    // ODE functions (overriding abstract functions in ODE.hpp)

    //- Number of ODE's to solve
    inline virtual label nEqns() const;

    virtual void derivatives
    (
        const scalar t,
        const scalarField& c,
        scalarField& dcdt
    ) const;

    virtual void jacobian
    (
        const scalar t,
        const scalarField& c,
        scalarField& dcdt,
        scalarSquareMatrix& J
    ) const;

    virtual void solve
    (
        scalarField &c,
        scalar& T,
        scalar& p,
        scalar& deltaT,
        scalar& subDeltaT
    ) const = 0;
};


} // End namespace CML


#include "volFields.hpp"
#include "zeroGradientFvPatchFields.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ReactionThermo, class ThermoType>
inline CML::label
CML::StandardChemistryModel<ReactionThermo, ThermoType>::nEqns() const
{
    // nEqns = number of species + temperature + pressure
    return nSpecie_ + 2;
}


template<class ReactionThermo, class ThermoType>
inline CML::PtrList<CML::DimensionedField<CML::scalar, CML::volMesh> >&
CML::StandardChemistryModel<ReactionThermo, ThermoType>::RR()
{
    return RR_;
}


template<class ReactionThermo, class ThermoType>
inline const CML::PtrList<CML::Reaction<ThermoType> >&
CML::StandardChemistryModel<ReactionThermo, ThermoType>::reactions() const
{
    return reactions_;
}


template<class ReactionThermo, class ThermoType>
inline const CML::PtrList<ThermoType>&
CML::StandardChemistryModel<ReactionThermo, ThermoType>::specieThermo() const
{
    return specieThermo_;
}


template<class ReactionThermo, class ThermoType>
inline CML::label
CML::StandardChemistryModel<ReactionThermo, ThermoType>::nSpecie() const
{
    return nSpecie_;
}


template<class ReactionThermo, class ThermoType>
inline CML::label
CML::StandardChemistryModel<ReactionThermo, ThermoType>::nReaction() const
{
    return nReaction_;
}


template<class ReactionThermo, class ThermoType>
inline CML::scalar
CML::StandardChemistryModel<ReactionThermo, ThermoType>::Treact() const
{
    return Treact_;
}


template<class ReactionThermo, class ThermoType>
inline CML::scalar&
CML::StandardChemistryModel<ReactionThermo, ThermoType>::Treact()
{
    return Treact_;
}


template<class ReactionThermo, class ThermoType>
inline const CML::DimensionedField<CML::scalar, CML::volMesh>&
CML::StandardChemistryModel<ReactionThermo, ThermoType>::RR
(
    const label i
) const
{
    return RR_[i];
}

template<class ReactionThermo, class ThermoType>
CML::DimensionedField<CML::scalar, CML::volMesh>&
CML::StandardChemistryModel<ReactionThermo, ThermoType>::RR
(
    const label i
)
{
    return RR_[i];
}


#include "chemistrySolver.hpp"
#include "reactingMixture.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ReactionThermo, class ThermoType>
CML::StandardChemistryModel<ReactionThermo, ThermoType>::StandardChemistryModel
(
    ReactionThermo& thermo
)
:
    BasicChemistryModel<ReactionThermo>(thermo),
    ODESystem(),
    Y_(this->thermo().composition().Y()),
    reactions_
    (
        dynamic_cast<const reactingMixture<ThermoType>&>(this->thermo())
    ),
    specieThermo_
    (
        dynamic_cast<const reactingMixture<ThermoType>&>
            (this->thermo()).speciesData()
    ),

    nSpecie_(Y_.size()),
    nReaction_(reactions_.size()),
    Treact_
    (
        BasicChemistryModel<ReactionThermo>::template lookupOrDefault<scalar>
        (
            "Treact",
            0
        )
    ),
    RR_(nSpecie_),
    c_(nSpecie_),
    dcdt_(nSpecie_)
{
    // Create the fields for the chemistry sources
    forAll(RR_, fieldi)
    {
        RR_.set
        (
            fieldi,
            new DimensionedField<scalar, volMesh>
            (
                IOobject
                (
                    "RR." + Y_[fieldi].name(),
                    this->mesh().time().timeName(),
                    this->mesh(),
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                thermo.p().mesh(),
                dimensionedScalar("zero", dimMass/dimVolume/dimTime, 0)
            )
        );
    }

    Info<< "StandardChemistryModel: Number of species = " << nSpecie_
        << " and reactions = " << nReaction_ << endl;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ReactionThermo, class ThermoType>
void CML::StandardChemistryModel<ReactionThermo, ThermoType>::omega
(
    const scalarField& c,
    const scalar T,
    const scalar p,
    scalarField& dcdt
) const
{

    dcdt = Zero;

    forAll(reactions_, i)
    {
        const Reaction<ThermoType>& R = reactions_[i];

        R.omega(p, T, c, dcdt);
    }
}


template<class ReactionThermo, class ThermoType>
CML::scalar CML::StandardChemistryModel<ReactionThermo, ThermoType>::omegaI
(
    const label index,
    const scalarField& c,
    const scalar T,
    const scalar p,
    scalar& pf,
    scalar& cf,
    label& lRef,
    scalar& pr,
    scalar& cr,
    label& rRef
) const
{
    const Reaction<ThermoType>& R = reactions_[index];
    scalar w = R.omega(p, T, c, pf, cf, lRef, pr, cr, rRef);
    return(w);
}


template<class ReactionThermo, class ThermoType>
void CML::StandardChemistryModel<ReactionThermo, ThermoType>::derivatives
(
    const scalar time,
    const scalarField& c,
    scalarField& dcdt
) const
{
    const scalar T = c[nSpecie_];
    const scalar p = c[nSpecie_ + 1];

    forAll(c_, i)
    {
        c_[i] = max(c[i], 0);
    }

    omega(c_, T, p, dcdt);

    // Constant pressure
    // dT/dt = ...
    scalar rho = 0;
    scalar cSum = 0;
    for (label i = 0; i < nSpecie_; i++)
    {
        const scalar W = specieThermo_[i].W();
        cSum += c_[i];
        rho += W*c_[i];
    }
    scalar cp = 0;
    for (label i=0; i<nSpecie_; i++)
    {
        cp += c_[i]*specieThermo_[i].cp(p, T);
    }
    cp /= rho;

    scalar dT = 0;
    for (label i = 0; i < nSpecie_; i++)
    {
        const scalar hi = specieThermo_[i].ha(p, T);
        dT += hi*dcdt[i];
    }
    dT /= rho*cp;

    dcdt[nSpecie_] = -dT;

    // dp/dt = ...
    dcdt[nSpecie_ + 1] = 0;
}


template<class ReactionThermo, class ThermoType>
void CML::StandardChemistryModel<ReactionThermo, ThermoType>::jacobian
(
    const scalar t,
    const scalarField& c,
    scalarField& dcdt,
    scalarSquareMatrix& J
) const
{
    const scalar T = c[nSpecie_];
    const scalar p = c[nSpecie_ + 1];

    forAll(c_, i)
    {
        c_[i] = max(c[i], 0);
    }

    J = Zero;
    dcdt = Zero;

    // To compute the species derivatives of the temperature term,
    // the enthalpies of the individual species is needed
    scalarField hi(nSpecie_);
    scalarField cpi(nSpecie_);
    for (label i = 0; i < nSpecie_; i++)
    {
        hi[i] = specieThermo_[i].ha(p, T);
        cpi[i] = specieThermo_[i].cp(p, T);
    }
    scalar omegaI = 0;
    List<label> dummy;
    forAll(reactions_, ri)
    {
        const Reaction<ThermoType>& R = reactions_[ri];
        scalar kfwd, kbwd;
        R.dwdc(p, T, c_, J, dcdt, omegaI, kfwd, kbwd, false, dummy);
        R.dwdT(p, T, c_, omegaI, kfwd, kbwd, J, false, dummy, nSpecie_);
    }

    // The species derivatives of the temperature term are partially computed
    // while computing dwdc, they are completed hereunder:
    scalar cpMean = 0;
    scalar dcpdTMean = 0;
    for (label i=0; i<nSpecie_; i++)
    {
        cpMean += c_[i]*cpi[i]; // J/(m3.K)
        dcpdTMean += c_[i]*specieThermo_[i].dcpdT(p, T);
    }
    scalar dTdt = 0.0;
    for (label i=0; i<nSpecie_; i++)
    {
        dTdt += hi[i]*dcdt[i]; // J/(m3.s)
    }
    dTdt /= -cpMean; // K/s

    for (label i = 0; i < nSpecie_; i++)
    {
        J(nSpecie_, i) = 0;
        for (label j = 0; j < nSpecie_; j++)
        {
            J(nSpecie_, i) += hi[j]*J(j, i);
        }
        J(nSpecie_, i) += cpi[i]*dTdt; // J/(mol.s)
        J(nSpecie_, i) /= -cpMean;    // K/s / (mol/m3)
    }

    // ddT of dTdt
    J(nSpecie_, nSpecie_) = 0;
    for (label i = 0; i < nSpecie_; i++)
    {
        J(nSpecie_, nSpecie_) += cpi[i]*dcdt[i] + hi[i]*J(i, nSpecie_);
    }
    J(nSpecie_, nSpecie_) += dTdt*dcpdTMean;
    J(nSpecie_, nSpecie_) /= -cpMean;
    J(nSpecie_, nSpecie_) += dTdt/T;
}


template<class ReactionThermo, class ThermoType>
CML::tmp<CML::volScalarField>
CML::StandardChemistryModel<ReactionThermo, ThermoType>::tc() const
{
    tmp<volScalarField> ttc
    (
        new volScalarField
        (
            IOobject
            (
                "tc",
                this->time().timeName(),
                this->mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            this->mesh(),
            dimensionedScalar("zero", dimTime, SMALL),
            extrapolatedCalculatedFvPatchScalarField::typeName
        )
    );

    scalarField& tc = ttc();

    tmp<volScalarField> trho(this->thermo().rho());
    const scalarField& rho = trho();

    const scalarField& T = this->thermo().T();
    const scalarField& p = this->thermo().p();

    const label nReaction = reactions_.size();

    scalar pf, cf, pr, cr;
    label lRef, rRef;

    if (this->chemistry_)
    {
        forAll(rho, celli)
        {
            const scalar rhoi = rho[celli];
            const scalar Ti = T[celli];
            const scalar pi = p[celli];

            scalar cSum = 0;

            for (label i=0; i<nSpecie_; i++)
            {
                c_[i] = rhoi*Y_[i][celli]/specieThermo_[i].W();
                cSum += c_[i];
            }

            forAll(reactions_, i)
            {
                const Reaction<ThermoType>& R = reactions_[i];

                R.omega(pi, Ti, c_, pf, cf, lRef, pr, cr, rRef);

                forAll(R.rhs(), s)
                {
                    tc[celli] += R.rhs()[s].stoichCoeff*pf*cf;
                }
            }

            tc[celli] = nReaction*cSum/tc[celli];
        }
    }

    ttc().correctBoundaryConditions();

    return ttc;
}


template<class ReactionThermo, class ThermoType>
CML::tmp<CML::volScalarField>
CML::StandardChemistryModel<ReactionThermo, ThermoType>::Qdot() const
{
    tmp<volScalarField> tQdot
    (
        new volScalarField
        (
            IOobject
            (
                "Qdot",
                this->mesh_.time().timeName(),
                this->mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            this->mesh_,
            dimensionedScalar("zero", dimEnergy/dimVolume/dimTime, 0)
        )
    );

    if (this->chemistry_)
    {
        scalarField& Qdot = tQdot();

        forAll(Y_, i)
        {
            forAll(Qdot, celli)
            {
                const scalar hi = specieThermo_[i].Hc();
                Qdot[celli] -= hi*RR_[i][celli];
            }
        }
    }

    return tQdot;
}


template<class ReactionThermo, class ThermoType>
CML::tmp<CML::DimensionedField<CML::scalar, CML::volMesh> >
CML::StandardChemistryModel<ReactionThermo, ThermoType>::calculateRR
(
    const label ri,
    const label si
) const
{
    tmp<DimensionedField<scalar, volMesh> > tRR
    (
        new DimensionedField<scalar, volMesh>
        (
            IOobject
            (
                "RR",
                this->mesh().time().timeName(),
                this->mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            this->mesh(),
            dimensionedScalar("zero", dimMass/dimVolume/dimTime, 0)
        )
    );

    DimensionedField<scalar, volMesh>& RR = tRR();

    tmp<volScalarField> trho(this->thermo().rho());
    const scalarField& rho = trho();

    const scalarField& T = this->thermo().T();
    const scalarField& p = this->thermo().p();

    scalar pf, cf, pr, cr;
    label lRef, rRef;

    forAll(rho, celli)
    {
        const scalar rhoi = rho[celli];
        const scalar Ti = T[celli];
        const scalar pi = p[celli];

        for (label i=0; i<nSpecie_; i++)
        {
            const scalar Yi = Y_[i][celli];
            c_[i] = rhoi*Yi/specieThermo_[i].W();
        }

        const Reaction<ThermoType>& R = reactions_[ri];
        const scalar omegai = R.omega(pi, Ti, c_, pf, cf, lRef, pr, cr, rRef);

        forAll(R.lhs(), s)
        {
            if (si == R.lhs()[s].index)
            {
                RR[celli] -= R.lhs()[s].stoichCoeff*omegai;
            }
        }

        forAll(R.rhs(), s)
        {
            if (si == R.rhs()[s].index)
            {
                RR[celli] += R.rhs()[s].stoichCoeff*omegai;
            }
        }

        RR[celli] *= specieThermo_[si].W();
    }

    return tRR;
}


template<class ReactionThermo, class ThermoType>
void CML::StandardChemistryModel<ReactionThermo, ThermoType>::calculate()
{
    if (!this->chemistry_)
    {
        return;
    }

    tmp<volScalarField> trho(this->thermo().rho());
    const scalarField& rho = trho();

    const scalarField& T = this->thermo().T();
    const scalarField& p = this->thermo().p();

    forAll(rho, celli)
    {
        const scalar rhoi = rho[celli];
        const scalar Ti = T[celli];
        const scalar pi = p[celli];

        for (label i=0; i<nSpecie_; i++)
        {
            const scalar Yi = Y_[i][celli];
            c_[i] = rhoi*Yi/specieThermo_[i].W();
        }

        omega(c_, Ti, pi, dcdt_);

        for (label i=0; i<nSpecie_; i++)
        {
            RR_[i][celli] = dcdt_[i]*specieThermo_[i].W();
        }
    }
}


template<class ReactionThermo, class ThermoType>
template<class DeltaTType>
CML::scalar CML::StandardChemistryModel<ReactionThermo, ThermoType>::solve
(
    const DeltaTType& deltaT
)
{
    BasicChemistryModel<ReactionThermo>::correct();

    scalar deltaTMin = GREAT;

    if (!this->chemistry_)
    {
        return deltaTMin;
    }

    tmp<volScalarField> trho(this->thermo().rho());
    const scalarField& rho = trho();

    const scalarField& T = this->thermo().T();
    const scalarField& p = this->thermo().p();

    scalarField c0(nSpecie_);

    forAll(rho, celli)
    {
        scalar Ti = T[celli];

        if (Ti > Treact_)
        {
            const scalar rhoi = rho[celli];
            scalar pi = p[celli];

            for (label i=0; i<nSpecie_; i++)
            {
                c_[i] = rhoi*Y_[i][celli]/specieThermo_[i].W();
                c0[i] = c_[i];
            }

            // Initialise time progress
            scalar timeLeft = deltaT[celli];

            // Calculate the chemical source terms
            while (timeLeft > SMALL)
            {
                scalar dt = timeLeft;
                this->solve(c_, Ti, pi, dt, this->deltaTChem_[celli]);
                timeLeft -= dt;
            }

            deltaTMin = min(this->deltaTChem_[celli], deltaTMin);

            this->deltaTChem_[celli] =
                min(this->deltaTChem_[celli], this->deltaTChemMax_);

            for (label i=0; i<nSpecie_; i++)
            {
                RR_[i][celli] =
                    (c_[i] - c0[i])*specieThermo_[i].W()/deltaT[celli];
            }
        }
        else
        {
            for (label i=0; i<nSpecie_; i++)
            {
                RR_[i][celli] = 0;
            }
        }
    }

    return deltaTMin;
}


template<class ReactionThermo, class ThermoType>
CML::scalar CML::StandardChemistryModel<ReactionThermo, ThermoType>::solve
(
    const scalar deltaT
)
{
    // Don't allow the time-step to change more than a factor of 2
    return min
    (
        this->solve<uniformField<scalar> >(uniformField<scalar>(deltaT)),
        2*deltaT
    );
}


template<class ReactionThermo, class ThermoType>
CML::scalar CML::StandardChemistryModel<ReactionThermo, ThermoType>::solve
(
    const scalarField& deltaT
)
{
    return this->solve<scalarField>(deltaT);
}


#endif
