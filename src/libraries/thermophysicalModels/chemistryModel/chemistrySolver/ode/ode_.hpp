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
    CML::ode

Description
    An ODE solver for chemistry


\*---------------------------------------------------------------------------*/

#ifndef ode__H
#define ode__H

#include "chemistrySolver.hpp"
#include "ODESolver.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                            Class ode Declaration
\*---------------------------------------------------------------------------*/

template<class ODEChemistryType>
class ode
:
    public chemistrySolver<ODEChemistryType>
{
    // Private data

        dictionary coeffsDict_;
        const word solverName_;
        autoPtr<ODESolver> odeSolver_;

        // Model constants

            scalar eps_;


public:

    //- Runtime type information
    TypeName("ODE");


    // Constructors

        //- Construct from components
        ode
        (
            const fvMesh& mesh,
            const word& ODEModeNewlName,
            const word& thermoType
        );


    //- Destructor
    virtual ~ode();


    // Member Functions

        virtual scalar solve
        (
            scalarField& c,
            const scalar T,
            const scalar p,
            const scalar t0,
            const scalar dt
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "ODEChemistryModel.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ODEChemistryType>
CML::ode<ODEChemistryType>::ode
(
    const fvMesh& mesh,
    const word& ODEModelName,
    const word& thermoType
)
:
    chemistrySolver<ODEChemistryType>(mesh, ODEModelName, thermoType),
    coeffsDict_(this->subDict("odeCoeffs")),
    solverName_(coeffsDict_.lookup("solver")),
    odeSolver_(ODESolver::New(solverName_, *this)),
    eps_(readScalar(coeffsDict_.lookup("eps")))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class ODEChemistryType>
CML::ode<ODEChemistryType>::~ode()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ODEChemistryType>
CML::scalar CML::ode<ODEChemistryType>::solve
(
    scalarField& c,
    const scalar T,
    const scalar p,
    const scalar t0,
    const scalar dt
) const
{
    label nSpecie = this->nSpecie();
    scalarField c1(this->nEqns(), 0.0);

    // copy the concentration, T and P to the total solve-vector
    for (label i = 0; i < nSpecie; i++)
    {
        c1[i] = c[i];
    }
    c1[nSpecie] = T;
    c1[nSpecie+1] = p;

    scalar dtEst = dt;

    odeSolver_->solve
    (
        *this,
        t0,
        t0 + dt,
        c1,
        eps_,
        dtEst
    );

    forAll(c, i)
    {
        c[i] = max(0.0, c1[i]);
    }

    return dtEst;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
