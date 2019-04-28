/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2018 OpenFOAM Foundation
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
    CML::solidChemistryModel

Description
    Extends base solid chemistry model by adding a thermo package, and ODE
    functions.
    Introduces chemistry equation system and evaluation of chemical source
    terms.

SourceFiles
    solidChemistryModel.cpp

\*---------------------------------------------------------------------------*/

#ifndef solidChemistryModel_HPP
#define solidChemistryModel_HPP

#include "Reaction.hpp"
#include "ODESystem.hpp"
#include "volFields.hpp"
#include "simpleMatrix.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class fvMesh;

/*---------------------------------------------------------------------------*\
                   Class solidChemistryModel Declaration
\*---------------------------------------------------------------------------*/

template<class CompType, class SolidThermo>
class solidChemistryModel
:
    public CompType,
    public ODESystem
{
    // Private Member Functions

    //- Disallow copy constructor
    solidChemistryModel(const solidChemistryModel&);

    //- Disallow default bitwise assignment
    void operator=(const solidChemistryModel&);


protected:

    //- Reference to solid mass fractions
    PtrList<volScalarField>& Ys_;

    //- Reactions
    const PtrList<Reaction<SolidThermo> >& reactions_;

    //- Thermodynamic data of solids
    const PtrList<SolidThermo>& solidThermo_;

    //- Number of solid components
    label nSolids_;

    //- Number of solid reactions
    label nReaction_;

    //- List of reaction rate per solid [kg/m3/s]
    PtrList<DimensionedField<scalar, volMesh> > RRs_;

    //- List of active reacting cells
    List<bool> reactingCells_;


    // Protected Member Functions

    //- Write access to source terms for solids
    inline PtrList<DimensionedField<scalar, volMesh> >& RRs()
    {
        return RRs_;
    }

    //- Set reacting status of cell, celli
    void setCellReacting(const label celli, const bool active);


public:

    //- Runtime type information
    TypeName("solidChemistryModel");


    //- Construct from thermo
    solidChemistryModel(typename CompType::reactionThermo& thermo);


    //- Destructor
    virtual ~solidChemistryModel()
    {}


    // Member Functions

    //- The reactions
    inline const PtrList<Reaction<SolidThermo> >& reactions() const
    {
        return reactions_;
    }

    //- The number of reactions
    inline label nReaction() const
    {
        return nReaction_;
    }

    //- dc/dt = omega, rate of change in concentration, for each species
    virtual scalarField omega
    (
        const scalarField& c,
        const scalar T,
        const scalar p,
        const bool updateC0 = false
    ) const = 0;

    //- Return the reaction rate for reaction r and the reference
    //  species and charateristic times
    virtual scalar omega
    (
        const Reaction<SolidThermo>& r,
        const scalarField& c,
        const scalar T,
        const scalar p,
        scalar& pf,
        scalar& cf,
        label& lRef,
        scalar& pr,
        scalar& cr,
        label& rRef
    ) const = 0;


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
    ) const = 0;

    //- Calculates the reaction rates
    virtual void calculate() = 0;


    // Solid Chemistry model functions

    //- Return const access to the chemical source terms for solids
    inline const DimensionedField<scalar, volMesh>& RRs
    (
        const label i
    ) const
    {
        return RRs_[i];
    }

    //- Return total solid source term
    inline tmp<DimensionedField<scalar, volMesh> > RRs() const
    {
        tmp<DimensionedField<scalar, volMesh> > tRRs
        (
            new DimensionedField<scalar, volMesh>
            (
                IOobject
                (
                    "RRs",
                    this->time().timeName(),
                    this->mesh(),
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                this->mesh(),
                dimensionedScalar("zero", dimMass/dimVolume/dimTime, 0.0)
            )
        );

        if (this->chemistry_)
        {
            DimensionedField<scalar, volMesh>& RRs = tRRs();
            for (label i=0; i < nSolids_; i++)
            {
                RRs += RRs_[i];
            }
        }
        return tRRs;
    }

    //- Solve the reaction system for the given time step
    //  and return the characteristic time
    virtual scalar solve(const scalar deltaT) = 0;

    //- Solve the reaction system for the given time step
    //  and return the characteristic time
    virtual scalar solve(const scalarField& deltaT);

    //- Return the chemical time scale
    virtual tmp<volScalarField> tc() const;

    //- Return the heat release rate [kg/m/s3]
    virtual tmp<volScalarField> Qdot() const;


    // ODE functions (overriding abstract functions in ode_.hpp)

    //- Number of ODE's to solve
    virtual label nEqns() const = 0;

    virtual void derivatives
    (
        const scalar t,
        const scalarField& c,
        scalarField& dcdt
    ) const = 0;

    virtual void jacobian
    (
        const scalar t,
        const scalarField& c,
        scalarField& dcdt,
        scalarSquareMatrix& dfdc
    ) const = 0;

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


#include "reactingMixture.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CompType, class SolidThermo>
CML::solidChemistryModel<CompType, SolidThermo>::solidChemistryModel
(
    typename CompType::reactionThermo& thermo
)
:
    CompType(thermo),
    ODESystem(),
    Ys_(this->solidThermo().composition().Y()),
    reactions_
    (
        dynamic_cast<const reactingMixture<SolidThermo>& >
        (
            this->solidThermo()
        )
    ),
    solidThermo_
    (
        dynamic_cast<const reactingMixture<SolidThermo>& >
        (
            this->solidThermo()
        ).speciesData()
    ),
    nSolids_(Ys_.size()),
    nReaction_(reactions_.size()),
    RRs_(nSolids_),
    reactingCells_(this->mesh().nCells(), true)
{
    // create the fields for the chemistry sources
    forAll(RRs_, fieldi)
    {
        RRs_.set
        (
            fieldi,
            new DimensionedField<scalar, volMesh>
            (
                IOobject
                (
                    "RRs." + Ys_[fieldi].name(),
                    this->mesh().time().timeName(),
                    this->mesh(),
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                this->mesh(),
                dimensionedScalar("zero", dimMass/dimVolume/dimTime, 0.0)
            )
        );
   }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CompType, class SolidThermo>
CML::scalar CML::solidChemistryModel<CompType, SolidThermo>::solve
(
    const scalarField& deltaT
)
{
    NotImplemented;
    return 0;
}


template<class CompType, class SolidThermo>
CML::tmp<CML::volScalarField>
CML::solidChemistryModel<CompType, SolidThermo>::tc() const
{
    NotImplemented;
    return volScalarField::null();
}


template<class CompType, class SolidThermo>
CML::tmp<CML::volScalarField>
CML::solidChemistryModel<CompType, SolidThermo>::Qdot() const
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
                IOobject::AUTO_WRITE,
                false
            ),
            this->mesh_,
            dimensionedScalar("zero", dimEnergy/dimVolume/dimTime, 0.0)
        )
    );

    if (this->chemistry_)
    {
        scalarField& Qdot = tQdot();

        forAll(Ys_, i)
        {
            forAll(Qdot, celli)
            {
                scalar hf = solidThermo_[i].Hc();
                Qdot[celli] -= hf*RRs_[i][celli];
            }
        }
    }

    return tQdot;
}


template<class CompType, class SolidThermo>
void CML::solidChemistryModel<CompType, SolidThermo>::setCellReacting
(
    const label celli,
    const bool active
)
{
    reactingCells_[celli] = active;
}


#endif
