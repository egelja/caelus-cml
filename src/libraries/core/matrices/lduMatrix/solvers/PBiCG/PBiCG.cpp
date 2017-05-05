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

#include "PBiCG.hpp"
#include "restrict.hpp"

namespace CML
{
    defineTypeNameAndDebug(PBiCG, 0);

    lduMatrix::solver::addasymMatrixConstructorToTable<PBiCG>
        addPBiCGAsymMatrixConstructorToTable_;
}

CML::PBiCG::PBiCG
(
    const word& fieldName,
    const lduMatrix& matrix,
    const FieldField<Field, scalar>& interfaceBouCoeffs,
    const FieldField<Field, scalar>& interfaceIntCoeffs,
    const lduInterfaceFieldPtrsList& interfaces,
    const dictionary& solverControls
)
:
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

CML::lduMatrix::solverPerformance CML::PBiCG::solve
(
    scalarField& x,
    const scalarField& b,
    const direction cmpt
) const
{
    // Setup class containing solver performance data
    lduMatrix::solverPerformance solverPerf
    (
        lduMatrix::preconditioner::getName(controlDict_) + typeName,
        fieldName_
    );

    register label nCells = x.size();

    scalar* RESTRICT xPtr = x.begin();

    scalarField p(nCells);
    scalar* RESTRICT pPtr = p.begin();

    scalarField pT(nCells, 0.0);
    scalar* RESTRICT pTPtr = pT.begin();

    scalarField w(nCells);
    scalar* RESTRICT wPtr = w.begin();

    scalarField wT(nCells);
    scalar* RESTRICT wTPtr = wT.begin();

    scalar rho = matrix_.great_;
    scalar rhoOld = rho;

    // Calculate matrix-vector products A.x and T.x
    matrix_.Amul(w, x, interfaceBouCoeffs_, interfaces_, cmpt);
    matrix_.Tmul(wT, x, interfaceIntCoeffs_, interfaces_, cmpt);

    // Calculate initial residual and transpose residual fields
    scalarField r(b - w);
    scalarField rT(b - wT);
    scalar* RESTRICT rPtr = r.begin();
    scalar* RESTRICT rTPtr = rT.begin();

    // Calculate normalisation factor
    scalar normFactor = this->normFactor(x, b, w, p);

    if (lduMatrix::debug >= 2)
    {
        Info<< "   Normalisation factor = " << normFactor << endl;
    }

    // Calculate normalised residual norm
    solverPerf.initialResidual() = gSumMag(r)/normFactor;
    solverPerf.finalResidual() = solverPerf.initialResidual();

    // Check convergence, solve if not converged
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
            #include "iteratePBiCG.hpp"
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
            #include "iteratePBiCG.hpp"
        }
    }

    return solverPerf;
}

