/*---------------------------------------------------------------------------*\
Copyright (C) 2016 OpenFOAM Foundation
Copyright (C) 2016 - 2017 Applied CCM
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

#include "PBiCGStab.hpp"
#include "restrict.hpp"

namespace CML
{
    defineTypeNameAndDebug(PBiCGStab, 0);

    lduMatrix::solver::addasymMatrixConstructorToTable<PBiCGStab>
        addPBiCGStabAsymMatrixConstructorToTable_;

    lduMatrix::solver::addsymMatrixConstructorToTable<PBiCGStab>
        addPBiCGStabSymMatrixConstructorToTable_;
}

CML::PBiCGStab::PBiCGStab
(
    word const& fieldName,
    lduMatrix const& matrix,
    FieldField<Field, scalar> const& interfaceBouCoeffs,
    FieldField<Field, scalar> const& interfaceIntCoeffs,
    lduInterfaceFieldPtrsList const& interfaces,
    dictionary const& solverControls
) :
    lduMatrix::solver
    (
        fieldName,
        matrix,
        interfaceBouCoeffs,
        interfaceIntCoeffs,
        interfaces,
        solverControls
    )
{}

CML::solverPerformance CML::PBiCGStab::solve
(
    scalarField& x,
    scalarField const& b,
    direction const cmpt
) const
{
    // --- Setup class containing solver performance data
    solverPerformance solverPerf
    (
        lduMatrix::preconditioner::getName(controlDict_) + typeName,
        fieldName_
    );

    const label nCells = x.size();

    scalar* __restrict__ xPtr = x.begin();

    scalarField pA(nCells);
    scalar* __restrict__ pAPtr = pA.begin();

    scalarField w(nCells);
    scalar* __restrict__ wPtr = w.begin();

    // --- Calculate A.x
    matrix_.Amul(w, x, interfaceBouCoeffs_, interfaces_, cmpt);

    // --- Calculate initial residual field
    scalarField rA(b - w);
    scalar* __restrict__ rAPtr = rA.begin();

    // --- Calculate normalisation factor
    const scalar normFactor = this->normFactor(x, b, w, pA);

    if (lduMatrix::debug >= 2)
    {
        Info<< "   Normalisation factor = " << normFactor << endl;
    }

    // --- Calculate normalised residual norm
    solverPerf.initialResidual() = gSumMag(rA)/normFactor;
    solverPerf.finalResidual() = solverPerf.initialResidual();

    // --- Check convergence, solve if not converged
    if (this->eps_ >= solverPerf.initialResidual())
    {
        if
        (
            !
            solverPerf.checkConvergence
            (
                this->tolerance_, 
                solverPerf.nIterations(),
                this->minIter_
	    )
        )
        {
            #include "iteratePBiCGStab.hpp"
        }
    }
    else
    {
        if
        (
            !
            solverPerf.checkConvergence
            (
                this->tolerance_, 
                this->relTol_,
                solverPerf.nIterations(),
                this->minIter_
            )
        )
        {
            #include "iteratePBiCGStab.hpp"
        }
    }

    return solverPerf;
}
