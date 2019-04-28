/*---------------------------------------------------------------------------*\
Copyright (C) 2016-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::TDACChemistryModel

Description
    Extends StandardChemistryModel by adding the TDAC method.

    References:
    \verbatim
        Contino, F., Jeanmart, H., Lucchini, T., & D’Errico, G. (2011).
        Coupling of in situ adaptive tabulation and dynamic adaptive chemistry:
        An effective method for solving combustion in engine simulations.
        Proceedings of the Combustion Institute, 33(2), 3057-3064.

        Contino, F., Lucchini, T., D'Errico, G., Duynslaegher, C.,
        Dias, V., & Jeanmart, H. (2012).
        Simulations of advanced combustion modes using detailed chemistry
        combined with tabulation and mechanism reduction techniques.
        SAE International Journal of Engines,
        5(2012-01-0145), 185-196.

        Contino, F., Foucher, F., Dagaut, P., Lucchini, T., D’Errico, G., &
        Mounaïm-Rousselle, C. (2013).
        Experimental and numerical analysis of nitric oxide effect on the
        ignition of iso-octane in a single cylinder HCCI engine.
        Combustion and Flame, 160(8), 1476-1483.

        Contino, F., Masurier, J. B., Foucher, F., Lucchini, T., D’Errico, G., &
        Dagaut, P. (2014).
        CFD simulations using the TDAC method to model iso-octane combustion
        for a large range of ozone seeding and temperature conditions
        in a single cylinder HCCI engine.
        Fuel, 137, 179-184.
    \endverbatim


\*---------------------------------------------------------------------------*/

#ifndef TDACChemistryModel_HPP
#define TDACChemistryModel_HPP

#include "StandardChemistryModel.hpp"
#include "chemistryReductionMethod.hpp"
#include "chemistryTabulationMethod.hpp"
#include "OFstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class TDACChemistryModel Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo, class ThermoType>
class TDACChemistryModel
:
    public StandardChemistryModel<ReactionThermo, ThermoType>
{

    bool variableTimeStep_;

    label timeSteps_;

    // Mechanism reduction
    label NsDAC_;
    scalarField completeC_;
    scalarField simplifiedC_;
    Field<bool> reactionsDisabled_;
    List<List<specieElement> > specieComp_;
    Field<label> completeToSimplifiedIndex_;
    DynamicList<label> simplifiedToCompleteIndex_;
    autoPtr<chemistryReductionMethod<ReactionThermo, ThermoType> >
        mechRed_;

    // Tabulation
    autoPtr<chemistryTabulationMethod<ReactionThermo, ThermoType> >
        tabulation_;

    // Log file for the average time spent reducing the chemistry
    autoPtr<OFstream> cpuReduceFile_;

    // Write average number of species
    autoPtr<OFstream> nActiveSpeciesFile_;

    //- Log file for the average time spent adding tabulated data
    autoPtr<OFstream> cpuAddFile_;

    //- Log file for the average time spent growing tabulated data
    autoPtr<OFstream> cpuGrowFile_;

    //- Log file for the average time spent retrieving tabulated data
    autoPtr<OFstream> cpuRetrieveFile_;

    //- Log file for average time spent solving the chemistry
    autoPtr<OFstream> cpuSolveFile_;

    // Field containing information about tabulation:
    // 0 -> add (direct integration)
    // 1 -> grow
    // 2 -> retrieve
    volScalarField tabulationResults_;


    // Private Member Functions

    //- Disallow copy constructor
    TDACChemistryModel(const TDACChemistryModel&);

    //- Disallow default bitwise assignment
    void operator=(const TDACChemistryModel&);

    //- Solve the reaction system for the given time step
    //  of given type and return the characteristic time
    //  Variable number of species added
    template<class DeltaTType>
    scalar solve(const DeltaTType& deltaT);


public:

    //- Runtime type information
    TypeName("TDAC");


    //- Construct from thermo
    TDACChemistryModel(ReactionThermo& thermo);


    //- Destructor
    virtual ~TDACChemistryModel()
    {}


    // Member Functions

    //- Return true if the time-step is variable and/or non-uniform
    inline bool variableTimeStep() const;

    //- Return the number of chemistry evaluations (used by ISAT)
    inline label timeSteps() const;

    //- Create and return a TDAC log file of the given name
    inline autoPtr<OFstream> logFile(const word& name) const;

    inline PtrList<volScalarField>& Y();

    //- dc/dt = omega, rate of change in concentration, for each species
    virtual void omega
    (
        const scalarField& c,
        const scalar T,
        const scalar p,
        scalarField& dcdt
    ) const;

    //- Return the reaction rate for reaction r and the reference
    //  species and charateristic times
    virtual scalar omega
    (
        const Reaction<ThermoType>& r,
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


    // Chemistry model functions (overriding functions in
    // StandardChemistryModel to use the private solve function)

    //- Solve the reaction system for the given time step
    //  and return the characteristic time
    virtual scalar solve(const scalar deltaT);

    //- Solve the reaction system for the given time step
    //  and return the characteristic time
    virtual scalar solve(const scalarField& deltaT);


    // ODE functions (overriding functions in StandardChemistryModel to take
    // into account the variable number of species)

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
       scalarField& c,
       scalar& T,
       scalar& p,
       scalar& deltaT,
       scalar& subDeltaT
    ) const = 0;


    // Mechanism reduction access functions

    inline void setNsDAC(const label newNsDAC);

    inline void setNSpecie(const label newNs);

    inline scalarField& completeC();

    inline scalarField& simplifiedC();

    inline Field<bool>& reactionsDisabled();

    inline bool active(const label i) const;

    inline void setActive(const label i);

    inline DynamicList<label>& simplifiedToCompleteIndex();

    inline Field<label>& completeToSimplifiedIndex();

    inline const Field<label>& completeToSimplifiedIndex() const;

    inline List<List<specieElement> >& specieComp();

    inline
    autoPtr<chemistryReductionMethod<ReactionThermo, ThermoType> >&  mechRed();

    tmp<volScalarField> tabulationResults() const
    {
        return tabulationResults_;
    }

    void setTabulationResultsAdd(const label celli);

    void setTabulationResultsGrow(const label celli);

    void setTabulationResultsRetrieve(const label celli);

    inline void resetTabulationResults();
};


} // End namespace CML


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ReactionThermo, class ThermoType>
inline bool
CML::TDACChemistryModel<ReactionThermo, ThermoType>::variableTimeStep() const
{
    return variableTimeStep_;
}


template<class ReactionThermo, class ThermoType>
inline CML::label
CML::TDACChemistryModel<ReactionThermo, ThermoType>::timeSteps() const
{
    return timeSteps_;
}


template<class ReactionThermo, class ThermoType>
inline CML::autoPtr<CML::OFstream>
CML::TDACChemistryModel<ReactionThermo, ThermoType>::
logFile(const word& name) const
{
    mkDir(this->mesh().time().path()/"TDAC"/this->group());
    return autoPtr<OFstream>
    (
        new OFstream
        (
            this->mesh().time().path()/"TDAC"/this->group()/name
        )
    );
}


template<class ReactionThermo, class ThermoType>
inline CML::PtrList<CML::volScalarField>&
CML::TDACChemistryModel<ReactionThermo, ThermoType>::Y()
{
    return this->Y_;
}


template<class ReactionThermo, class ThermoType>
inline
CML::autoPtr<CML::chemistryReductionMethod<ReactionThermo, ThermoType> >&
CML::TDACChemistryModel<ReactionThermo, ThermoType>::mechRed()
{
    return mechRed_;
}


template<class ReactionThermo, class ThermoType>
inline void CML::TDACChemistryModel<ReactionThermo, ThermoType>::setActive
(
    const label i
)
{
    this->thermo().composition().setActive(i);
}


template<class ReactionThermo, class ThermoType>
inline bool CML::TDACChemistryModel<ReactionThermo, ThermoType>::active
(
    const label i
) const
{
    return this->thermo().composition().active(i);
}


template<class ReactionThermo, class ThermoType>
inline void CML::TDACChemistryModel<ReactionThermo, ThermoType>::
setNsDAC(const label newNsDAC)
{
    NsDAC_ = newNsDAC;
}


template<class ReactionThermo, class ThermoType>
inline void CML::TDACChemistryModel<ReactionThermo, ThermoType>::
setNSpecie(const label newNs)
{
    this->nSpecie_ = newNs;
}


template<class ReactionThermo, class ThermoType>
inline CML::DynamicList<CML::label>&
CML::TDACChemistryModel<ReactionThermo, ThermoType>::
simplifiedToCompleteIndex()
{
    return simplifiedToCompleteIndex_;
}


template<class ReactionThermo, class ThermoType>
inline CML::Field<CML::label>&
CML::TDACChemistryModel<ReactionThermo, ThermoType>::
completeToSimplifiedIndex()
{
    return completeToSimplifiedIndex_;
}


template<class ReactionThermo, class ThermoType>
inline const CML::Field<CML::label>&
CML::TDACChemistryModel<ReactionThermo, ThermoType>::
completeToSimplifiedIndex() const
{
    return completeToSimplifiedIndex_;
}


template<class ReactionThermo, class ThermoType>
inline CML::Field<bool>&
CML::TDACChemistryModel<ReactionThermo, ThermoType>::reactionsDisabled()
{
    return reactionsDisabled_;
}


template<class ReactionThermo, class ThermoType>
inline CML::scalarField&
CML::TDACChemistryModel<ReactionThermo, ThermoType>::completeC()
{
    return completeC_;
}


template<class ReactionThermo, class ThermoType>
inline CML::scalarField&
CML::TDACChemistryModel<ReactionThermo, ThermoType>::simplifiedC()
{
    return simplifiedC_;
}


template<class ReactionThermo, class ThermoType>
inline CML::List<CML::List<CML::specieElement> >&
CML::TDACChemistryModel<ReactionThermo, ThermoType>::specieComp()
{
    return specieComp_;
}


template<class ReactionThermo, class ThermoType>
void CML::TDACChemistryModel<ReactionThermo, ThermoType>::
resetTabulationResults()
{
    forAll(tabulationResults_, tabi)
    {
        tabulationResults_[tabi] = 2;
    }
}


#include "uniformField.hpp"
#include "localEulerDdtScheme.hpp"
#include "clockTime.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ReactionThermo, class ThermoType>
CML::TDACChemistryModel<ReactionThermo, ThermoType>::TDACChemistryModel
(
    ReactionThermo& thermo
)
:
    StandardChemistryModel<ReactionThermo, ThermoType>(thermo),
    variableTimeStep_
    (
        this->mesh().time().controlDict().lookupOrDefault
        (
            "adjustTimeStep",
            false
        )
     || fv::localEulerDdt::enabled(this->mesh())
    ),
    timeSteps_(0),
    NsDAC_(this->nSpecie_),
    completeC_(this->nSpecie_, 0),
    reactionsDisabled_(this->reactions_.size(), false),
    specieComp_(this->nSpecie_),
    completeToSimplifiedIndex_(this->nSpecie_, -1),
    simplifiedToCompleteIndex_(this->nSpecie_),
    tabulationResults_
    (
        IOobject
        (
            thermo.phasePropertyName("TabulationResults"),
            this->time().timeName(),
            this->mesh(),
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        this->mesh(),
        scalar(0)
    )
{
    basicSpecieMixture& composition = this->thermo().composition();

    // Store the species composition according to the species index
    speciesTable speciesTab = composition.species();

    const HashTable<List<specieElement> >& specComp =
        dynamicCast<const reactingMixture<ThermoType>&>(this->thermo())
       .specieComposition();

    forAll(specieComp_, i)
    {
        specieComp_[i] = specComp[this->Y()[i].member()];
    }

    mechRed_ = chemistryReductionMethod<ReactionThermo, ThermoType>::New
    (
        *this,
        *this
    );

    // When the mechanism reduction method is used, the 'active' flag for every
    // species should be initialized (by default 'active' is true)
    if (mechRed_->active())
    {
        forAll(this->Y(), i)
        {
            IOobject header
            (
                this->Y()[i].name(),
                this->mesh().time().timeName(),
                this->mesh(),
                IOobject::NO_READ
            );

            // Check if the species file is provided, if not set inactive
            // and NO_WRITE
            if (!header.headerOk())
            {
                composition.setInactive(i);
                this->Y()[i].writeOpt() = IOobject::NO_WRITE;
            }
        }
    }

    tabulation_ = chemistryTabulationMethod<ReactionThermo, ThermoType>::New
    (
        *this,
        *this
    );

    if (mechRed_->log())
    {
        cpuReduceFile_ = logFile("cpu_reduce.out");
        nActiveSpeciesFile_ = logFile("nActiveSpecies.out");
    }

    if (tabulation_->log())
    {
        cpuAddFile_ = logFile("cpu_add.out");
        cpuGrowFile_ = logFile("cpu_grow.out");
        cpuRetrieveFile_ = logFile("cpu_retrieve.out");
    }

    if (mechRed_->log() || tabulation_->log())
    {
        cpuSolveFile_ = logFile("cpu_solve.out");
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ReactionThermo, class ThermoType>
void CML::TDACChemistryModel<ReactionThermo, ThermoType>::omega
(
    const scalarField& c, // Contains all species even when mechRed is active
    const scalar T,
    const scalar p,
    scalarField& dcdt
) const
{
    const bool reduced = mechRed_->active();

    scalar pf, cf, pr, cr;
    label lRef, rRef;

    dcdt = Zero;

    forAll(this->reactions_, i)
    {
        if (!reactionsDisabled_[i])
        {
            const Reaction<ThermoType>& R = this->reactions_[i];

            scalar omegai = R.omega
            (
                p, T, c, pf, cf, lRef, pr, cr, rRef
            );

            forAll(R.lhs(), s)
            {
                label si = R.lhs()[s].index;
                if (reduced)
                {
                    si = completeToSimplifiedIndex_[si];
                }

                const scalar sl = R.lhs()[s].stoichCoeff;
                dcdt[si] -= sl*omegai;
            }
            forAll(R.rhs(), s)
            {
                label si = R.rhs()[s].index;
                if (reduced)
                {
                    si = completeToSimplifiedIndex_[si];
                }

                const scalar sr = R.rhs()[s].stoichCoeff;
                dcdt[si] += sr*omegai;
            }
        }
    }
}


template<class ReactionThermo, class ThermoType>
CML::scalar CML::TDACChemistryModel<ReactionThermo, ThermoType>::omega
(
    const Reaction<ThermoType>& R,
    const scalarField& c, // Contains all species even when mechRed is active
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
    const scalar kf = R.kf(p, T, c);
    const scalar kr = R.kr(kf, p, T, c);

    const label Nl = R.lhs().size();
    const label Nr = R.rhs().size();

    label slRef = 0;
    lRef = R.lhs()[slRef].index;

    pf = kf;
    for (label s=1; s<Nl; s++)
    {
        const label si = R.lhs()[s].index;

        if (c[si] < c[lRef])
        {
            const scalar exp = R.lhs()[slRef].exponent;
            pf *= pow(max(c[lRef], 0), exp);
            lRef = si;
            slRef = s;
        }
        else
        {
            const scalar exp = R.lhs()[s].exponent;
            pf *= pow(max(c[si], 0), exp);
        }
    }
    cf = max(c[lRef], 0);

    {
        const scalar exp = R.lhs()[slRef].exponent;
        if (exp < 1)
        {
            if (cf > SMALL)
            {
                pf *= pow(cf, exp - 1);
            }
            else
            {
                pf = 0;
            }
        }
        else
        {
            pf *= pow(cf, exp - 1);
        }
    }

    label srRef = 0;
    rRef = R.rhs()[srRef].index;

    // Find the matrix element and element position for the rhs
    pr = kr;
    for (label s=1; s<Nr; s++)
    {
        const label si = R.rhs()[s].index;
        if (c[si] < c[rRef])
        {
            const scalar exp = R.rhs()[srRef].exponent;
            pr *= pow(max(c[rRef], 0), exp);
            rRef = si;
            srRef = s;
        }
        else
        {
            const scalar exp = R.rhs()[s].exponent;
            pr *= pow(max(c[si], 0), exp);
        }
    }
    cr = max(c[rRef], 0);

    {
        const scalar exp = R.rhs()[srRef].exponent;
        if (exp < 1)
        {
            if (cr > SMALL)
            {
                pr *= pow(cr, exp - 1);
            }
            else
            {
                pr = 0;
            }
        }
        else
        {
            pr *= pow(cr, exp - 1);
        }
    }

    return pf*cf - pr*cr;
}


template<class ReactionThermo, class ThermoType>
void CML::TDACChemistryModel<ReactionThermo, ThermoType>::derivatives
(
    const scalar time,
    const scalarField& c,
    scalarField& dcdt
) const
{
    const bool reduced = mechRed_->active();

    const scalar T = c[this->nSpecie_];
    const scalar p = c[this->nSpecie_ + 1];

    if (reduced)
    {
        // When using DAC, the ODE solver submit a reduced set of species the
        // complete set is used and only the species in the simplified mechanism
        // are updated
        this->c_ = completeC_;

        // Update the concentration of the species in the simplified mechanism
        // the other species remain the same and are used only for third-body
        // efficiencies
        for (label i=0; i<NsDAC_; i++)
        {
            this->c_[simplifiedToCompleteIndex_[i]] = max(c[i], 0);
        }
    }
    else
    {
        for (label i=0; i<this->nSpecie(); i++)
        {
            this->c_[i] = max(c[i], 0);
        }
    }

    omega(this->c_, T, p, dcdt);

    // Constant pressure
    // dT/dt = ...
    scalar rho = 0;
    for (label i=0; i<this->c_.size(); i++)
    {
        const scalar W = this->specieThermo_[i].W();
        rho += W*this->c_[i];
    }

    scalar cp = 0;
    for (label i=0; i<this->c_.size(); i++)
    {
        // cp function returns [J/(kmol K)]
        cp += this->c_[i]*this->specieThermo_[i].cp(p, T);
    }
    cp /= rho;

    // When mechanism reduction is active
    // dT is computed on the reduced set since dcdt is null
    // for species not involved in the simplified mechanism
    scalar dT = 0;
    for (label i=0; i<this->nSpecie_; i++)
    {
        label si;
        if (reduced)
        {
            si = simplifiedToCompleteIndex_[i];
        }
        else
        {
            si = i;
        }

        // ha function returns [J/kmol]
        const scalar hi = this->specieThermo_[si].ha(p, T);
        dT += hi*dcdt[i];
    }
    dT /= rho*cp;

    dcdt[this->nSpecie_] = -dT;

    // dp/dt = ...
    dcdt[this->nSpecie_ + 1] = 0;
}


template<class ReactionThermo, class ThermoType>
void CML::TDACChemistryModel<ReactionThermo, ThermoType>::jacobian
(
    const scalar t,
    const scalarField& c,
    scalarField& dcdt,
    scalarSquareMatrix& J
) const
{
    const bool reduced = mechRed_->active();

    // If the mechanism reduction is active, the computed Jacobian
    // is compact (size of the reduced set of species)
    // but according to the information of the complete set
    // (i.e. for the third-body efficiencies)

    const scalar T = c[this->nSpecie_];
    const scalar p = c[this->nSpecie_ + 1];

    if (reduced)
    {
        this->c_ = completeC_;
        for (label i=0; i<NsDAC_; i++)
        {
            this->c_[simplifiedToCompleteIndex_[i]] = max(c[i], 0);
        }
    }
    else
    {
        forAll(this->c_, i)
        {
            this->c_[i] = max(c[i], 0);
        }
    }

    J = Zero;
    dcdt = Zero;
    scalarField hi(this->c_.size());
    scalarField cpi(this->c_.size());
    forAll(hi, i)
    {
        hi[i] = this->specieThermo_[i].ha(p, T);
        cpi[i] = this->specieThermo_[i].cp(p, T);
    }

    scalar omegaI = 0;

    forAll(this->reactions_, ri)
    {
        if (!reactionsDisabled_[ri])
        {
            const Reaction<ThermoType>& R = this->reactions_[ri];
            scalar kfwd, kbwd;
            R.dwdc
            (
                p,
                T,
                this->c_,
                J,
                dcdt,
                omegaI,
                kfwd,
                kbwd,
                reduced,
                completeToSimplifiedIndex_
            );
            R.dwdT
            (
                p,
                T,
                this->c_,
                omegaI,
                kfwd,
                kbwd,
                J,
                reduced,
                completeToSimplifiedIndex_,
                this->nSpecie_
            );
        }
    }

    // The species derivatives of the temperature term are partially computed
    // while computing dwdc, they are completed hereunder:
    scalar cpMean = 0;
    scalar dcpdTMean = 0;
    forAll(this->c_, i)
    {
        cpMean += this->c_[i]*cpi[i]; // J/(m3.K)
        // Already multiplied by rho
        dcpdTMean += this->c_[i]*this->specieThermo_[i].dcpdT(p, T);
    }

    scalar dTdt = 0;
    forAll(hi, i)
    {
        if (reduced)
        {
            const label si = completeToSimplifiedIndex_[i];
            if (si != -1)
            {
                dTdt += hi[i]*dcdt[si]; // J/(m3.s)
            }
        }
        else
        {
            dTdt += hi[i]*dcdt[i]; // J/(m3.s)
        }
    }
    dTdt /= -cpMean; // K/s
    dcdt[this->nSpecie_] = dTdt;

    for (label i = 0; i < this->nSpecie_; i++)
    {
        J(this->nSpecie_, i) = 0;
        for (label j = 0; j < this->nSpecie_; j++)
        {
            const label sj = reduced ? simplifiedToCompleteIndex_[j] : j;
            J(this->nSpecie_, i) += hi[sj]*J(j, i);
        }
        const label si = reduced ? simplifiedToCompleteIndex_[i] : i;
        J(this->nSpecie_, i) += cpi[si]*dTdt; // J/(mol.s)
        J(this->nSpecie_, i) /= -cpMean;    // K/s / (mol/m3)
    }

    // ddT of dTdt
    J(this->nSpecie_, this->nSpecie_) = 0;
    for (label i = 0; i < this->nSpecie_; i++)
    {
        const label si = reduced ? simplifiedToCompleteIndex_[i] : i;
        J(this->nSpecie_, this->nSpecie_) +=
            cpi[si]*dcdt[i]
          + hi[si]*J(i, this->nSpecie_);
    }
    J(this->nSpecie_, this->nSpecie_) += dTdt*dcpdTMean;
    J(this->nSpecie_, this->nSpecie_) /= -cpMean;
    J(this->nSpecie_, this->nSpecie_) += dTdt/T;
}


template<class ReactionThermo, class ThermoType>
template<class DeltaTType>
CML::scalar CML::TDACChemistryModel<ReactionThermo, ThermoType>::solve
(
    const DeltaTType& deltaT
)
{
    // Increment counter of time-step
    timeSteps_++;

    const bool reduced = mechRed_->active();

    label nAdditionalEqn = (tabulation_->variableTimeStep() ? 1 : 0);

    basicSpecieMixture& composition = this->thermo().composition();

    // CPU time analysis
    const clockTime clockTime_= clockTime();
    clockTime_.timeIncrement();
    scalar reduceMechCpuTime_ = 0;
    scalar addNewLeafCpuTime_ = 0;
    scalar growCpuTime_ = 0;
    scalar solveChemistryCpuTime_ = 0;
    scalar searchISATCpuTime_ = 0;

    this->resetTabulationResults();

    // Average number of active species
    scalar nActiveSpecies = 0;
    scalar nAvg = 0;

    BasicChemistryModel<ReactionThermo>::correct();

    scalar deltaTMin = GREAT;

    if (!this->chemistry_)
    {
        return deltaTMin;
    }

    const volScalarField rho
    (
        IOobject
        (
            "rho",
            this->time().timeName(),
            this->mesh(),
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        ),
        this->thermo().rho()
    );

    const scalarField& T = this->thermo().T();
    const scalarField& p = this->thermo().p();

    scalarField c(this->nSpecie_);
    scalarField c0(this->nSpecie_);

    // Composition vector (Yi, T, p)
    scalarField phiq(this->nEqns() + nAdditionalEqn);

    scalarField Rphiq(this->nEqns() + nAdditionalEqn);

    forAll(rho, celli)
    {
        const scalar rhoi = rho[celli];
        scalar pi = p[celli];
        scalar Ti = T[celli];

        for (label i=0; i<this->nSpecie_; i++)
        {
            c[i] = rhoi*this->Y_[i][celli]/this->specieThermo_[i].W();
            c0[i] = c[i];
            phiq[i] = this->Y()[i][celli];
        }
        phiq[this->nSpecie()]=Ti;
        phiq[this->nSpecie() + 1]=pi;
        if (tabulation_->variableTimeStep())
        {
            phiq[this->nSpecie() + 2] = deltaT[celli];
        }


        // Initialise time progress
        scalar timeLeft = deltaT[celli];

        // Not sure if this is necessary
        Rphiq = Zero;

        clockTime_.timeIncrement();

        // When tabulation is active (short-circuit evaluation for retrieve)
        // It first tries to retrieve the solution of the system with the
        // information stored through the tabulation method
        if (tabulation_->active() && tabulation_->retrieve(phiq, Rphiq))
        {
            // Retrieved solution stored in Rphiq
            for (label i=0; i<this->nSpecie(); i++)
            {
                c[i] = rhoi*Rphiq[i]/this->specieThermo_[i].W();
            }

            searchISATCpuTime_ += clockTime_.timeIncrement();
        }
        // This position is reached when tabulation is not used OR
        // if the solution is not retrieved.
        // In the latter case, it adds the information to the tabulation
        // (it will either expand the current data or add a new stored point).
        else
        {
            // Store total time waiting to attribute to add or grow
            scalar timeTmp = clockTime_.timeIncrement();

            if (reduced)
            {
                // Reduce mechanism change the number of species (only active)
                mechRed_->reduceMechanism(c, Ti, pi);
                nActiveSpecies += mechRed_->NsSimp();
                nAvg++;
                scalar timeIncr = clockTime_.timeIncrement();
                reduceMechCpuTime_ += timeIncr;
                timeTmp += timeIncr;
            }

            // Calculate the chemical source terms
            while (timeLeft > SMALL)
            {
                scalar dt = timeLeft;
                if (reduced)
                {
                    // completeC_ used in the overridden ODE methods
                    // to update only the active species
                    completeC_ = c;

                    // Solve the reduced set of ODE
                    this->solve
                    (
                        simplifiedC_, Ti, pi, dt, this->deltaTChem_[celli]
                    );

                    for (label i=0; i<NsDAC_; i++)
                    {
                        c[simplifiedToCompleteIndex_[i]] = simplifiedC_[i];
                    }
                }
                else
                {
                    this->solve(c, Ti, pi, dt, this->deltaTChem_[celli]);
                }
                timeLeft -= dt;
            }

            {
                scalar timeIncr = clockTime_.timeIncrement();
                solveChemistryCpuTime_ += timeIncr;
                timeTmp += timeIncr;
            }

            // If tabulation is used, we add the information computed here to
            // the stored points (either expand or add)
            if (tabulation_->active())
            {
                forAll(c, i)
                {
                    Rphiq[i] = c[i]/rhoi*this->specieThermo_[i].W();
                }
                if (tabulation_->variableTimeStep())
                {
                    Rphiq[Rphiq.size()-3] = Ti;
                    Rphiq[Rphiq.size()-2] = pi;
                    Rphiq[Rphiq.size()-1] = deltaT[celli];
                }
                else
                {
                    Rphiq[Rphiq.size()-2] = Ti;
                    Rphiq[Rphiq.size()-1] = pi;
                }
                label growOrAdd =
                    tabulation_->add(phiq, Rphiq, rhoi, deltaT[celli]);
                if (growOrAdd)
                {
                    this->setTabulationResultsAdd(celli);
                    addNewLeafCpuTime_ += clockTime_.timeIncrement() + timeTmp;
                }
                else
                {
                    this->setTabulationResultsGrow(celli);
                    growCpuTime_ += clockTime_.timeIncrement() + timeTmp;
                }
            }

            // When operations are done and if mechanism reduction is active,
            // the number of species (which also affects nEqns) is set back
            // to the total number of species (stored in the mechRed object)
            if (reduced)
            {
                this->nSpecie_ = mechRed_->nSpecie();
            }
            deltaTMin = min(this->deltaTChem_[celli], deltaTMin);

            this->deltaTChem_[celli] =
                min(this->deltaTChem_[celli], this->deltaTChemMax_);
        }

        // Set the RR vector (used in the solver)
        for (label i=0; i<this->nSpecie_; i++)
        {
            this->RR_[i][celli] =
                (c[i] - c0[i])*this->specieThermo_[i].W()/deltaT[celli];
        }
    }

    if (mechRed_->log() || tabulation_->log())
    {
        cpuSolveFile_()
            << this->time().timeOutputValue()
            << "    " << solveChemistryCpuTime_ << endl;
    }

    if (mechRed_->log())
    {
        cpuReduceFile_()
            << this->time().timeOutputValue()
            << "    " << reduceMechCpuTime_ << endl;
    }

    if (tabulation_->active())
    {
        // Every time-step, look if the tabulation should be updated
        tabulation_->update();

        // Write the performance of the tabulation
        tabulation_->writePerformance();

        if (tabulation_->log())
        {
            cpuRetrieveFile_()
                << this->time().timeOutputValue()
                << "    " << searchISATCpuTime_ << endl;

            cpuGrowFile_()
                << this->time().timeOutputValue()
                << "    " << growCpuTime_ << endl;

            cpuAddFile_()
                << this->time().timeOutputValue()
                << "    " << addNewLeafCpuTime_ << endl;
        }
    }

    if (reduced && nAvg && mechRed_->log())
    {
        // Write average number of species
        nActiveSpeciesFile_()
            << this->time().timeOutputValue()
            << "    " << nActiveSpecies/nAvg << endl;
    }

    if (Pstream::parRun())
    {
        List<bool> active(composition.active());
        Pstream::listCombineGather(active, orEqOp<bool>());
        Pstream::listCombineScatter(active);

        forAll(active, i)
        {
            if (active[i])
            {
                composition.setActive(i);
            }
        }
    }

    forAll(this->Y(), i)
    {
        if (composition.active(i))
        {
            this->Y()[i].writeOpt() = IOobject::AUTO_WRITE;
        }
    }

    return deltaTMin;
}


template<class ReactionThermo, class ThermoType>
CML::scalar CML::TDACChemistryModel<ReactionThermo, ThermoType>::solve
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
CML::scalar CML::TDACChemistryModel<ReactionThermo, ThermoType>::solve
(
    const scalarField& deltaT
)
{
    return this->solve<scalarField>(deltaT);
}


template<class ReactionThermo, class ThermoType>
void CML::TDACChemistryModel<ReactionThermo, ThermoType>::
setTabulationResultsAdd
(
    const label celli
)
{
    tabulationResults_[celli] = 0;
}


template<class ReactionThermo, class ThermoType>
void CML::TDACChemistryModel<ReactionThermo, ThermoType>::
setTabulationResultsGrow(const label celli)
{
    tabulationResults_[celli] = 1;
}


template<class ReactionThermo, class ThermoType>
void CML::TDACChemistryModel<ReactionThermo, ThermoType>::
setTabulationResultsRetrieve
(
    const label celli
)
{
    tabulationResults_[celli] = 2;
}


#endif
