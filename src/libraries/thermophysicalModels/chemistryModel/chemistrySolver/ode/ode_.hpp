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
    CML::ode

Description
    An ODE solver for chemistry


\*---------------------------------------------------------------------------*/

#ifndef ode__HPP
#define ode__HPP

#include "chemistrySolver.hpp"
#include "ODESolver.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                            Class ode Declaration
\*---------------------------------------------------------------------------*/

template<class ChemistryModel>
class ode
:
    public chemistrySolver<ChemistryModel>
{

    dictionary coeffsDict_;

    mutable autoPtr<ODESolver> odeSolver_;

    // Solver data
    mutable scalarField cTp_;


public:

    //- Runtime type information
    TypeName("ode");


    //- Construct from thermo
    ode(typename ChemistryModel::reactionThermo& thermo);


    //- Destructor
    virtual ~ode()
    {}


    // Member Functions

    //- Update the concentrations and return the chemical time
    virtual void solve
    (
        scalarField& c,
        scalar& T,
        scalar& p,
        scalar& deltaT,
        scalar& subDeltaT
    ) const;

};


} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ChemistryModel>
CML::ode<ChemistryModel>::ode(typename ChemistryModel::reactionThermo& thermo)
:
    chemistrySolver<ChemistryModel>(thermo),
    coeffsDict_(this->subDict("odeCoeffs")),
    odeSolver_(ODESolver::New(*this, coeffsDict_)),
    cTp_(this->nEqns())
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ChemistryModel>
void CML::ode<ChemistryModel>::solve
(
    scalarField& c,
    scalar& T,
    scalar& p,
    scalar& deltaT,
    scalar& subDeltaT
) const
{
    // Reset the size of the ODE system to the simplified size when mechanism
    // reduction is active
    if (odeSolver_->resize())
    {
        odeSolver_->resizeField(cTp_);
    }

    const label nSpecie = this->nSpecie();

    // Copy the concentration, T and P to the total solve-vector
    for (int i=0; i<nSpecie; i++)
    {
        cTp_[i] = c[i];
    }
    cTp_[nSpecie] = T;
    cTp_[nSpecie+1] = p;

    odeSolver_->solve(0, deltaT, cTp_, subDeltaT);

    for (int i=0; i<nSpecie; i++)
    {
        c[i] = max(0.0, cTp_[i]);
    }
    T = cTp_[nSpecie];
    p = cTp_[nSpecie+1];
}

#endif
