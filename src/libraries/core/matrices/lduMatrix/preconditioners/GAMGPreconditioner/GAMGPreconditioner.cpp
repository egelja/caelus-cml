/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2014 Applied CCM
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

\*---------------------------------------------------------------------------*/

#include "GAMGPreconditioner.hpp"

namespace CML
{
    defineTypeNameAndDebug(GAMGPreconditioner, 0);

    lduMatrix::preconditioner::addsymMatrixConstructorToTable
    <GAMGPreconditioner> addGAMGPreconditionerSymMatrixConstructorToTable_;

    lduMatrix::preconditioner::addasymMatrixConstructorToTable
    <GAMGPreconditioner> addGAMGPreconditionerAsymMatrixConstructorToTable_;
}

CML::GAMGPreconditioner::GAMGPreconditioner
(
    const lduMatrix::solver& sol,
    const dictionary& solverControls
)
:
    GAMGSolver
    (
        sol.fieldName(),
        sol.matrix(),
        sol.interfaceBouCoeffs(),
        sol.interfaceIntCoeffs(),
        sol.interfaces(),
        solverControls
    ),
    lduMatrix::preconditioner(sol),
    nVcycles_(2)
{
    readControls();
}

CML::GAMGPreconditioner::~GAMGPreconditioner()
{}

void CML::GAMGPreconditioner::readControls()
{
    GAMGSolver::readControls();
    nVcycles_ = controlDict_.lookupOrDefault<label>("nVcycles", 2);
}

void CML::GAMGPreconditioner::precondition
(
    scalarField& w,
    const scalarField& r,
    const direction cmpt
) const
{
    w = 0.0;
    scalarField Aw(w.size());
    scalarField finestCorrection(w.size());
    scalarField finestResidual(r);

    // Create coarse grid correction fields
    PtrList<scalarField> coarseCorrFields;

    // Create coarse grid sources
    PtrList<scalarField> coarseSources;

    // Create the smoothers for all levels
    PtrList<lduMatrix::smoother> smoothers;

    // Initialise the above data structures
    initVcycle(coarseCorrFields, coarseSources, smoothers);

    // Perform n V-cycles and precondition the residual at
    // the end of each cycle
    for (label cycle=0; cycle<nVcycles_; cycle++)
    {
        // Smooth the error
        Vcycle
        (
            smoothers,
            w,
            r,
            Aw,
            finestCorrection,
            finestResidual,
            coarseCorrFields,
            coarseSources,
            cmpt
        );

        // Precondition the residual
        if (cycle < nVcycles_-1)
        {
            // Calculate finest level residual field
            matrix_.Amul(Aw, w, interfaceBouCoeffs_, interfaces_, cmpt);
            finestResidual = r;
            finestResidual -= Aw;
        }
    }
}

