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

#include "PCG.hpp"
#include "restrict.hpp"

namespace CML
{
    defineTypeNameAndDebug(PCG, 0);

    lduMatrix::solver::addsymMatrixConstructorToTable<PCG>
        addPCGSymMatrixConstructorToTable_;
}

CML::PCG::PCG
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

CML::solverPerformance CML::PCG::solve
(
    scalarField& x,
    scalarField const& b,
    direction const cmpt
) const
{
    // Setup class containing solver performance data
    solverPerformance solverPerf
    (
        lduMatrix::preconditioner::getName(controlDict_) + typeName,
        fieldName_
    );

    register label nCells = x.size();

    scalar* RESTRICT xPtr = x.begin();

    scalarField p(nCells);
    scalar* RESTRICT pPtr = p.begin();

    scalarField w(nCells);
    scalar* RESTRICT wPtr = w.begin();

    scalar rho = matrix_.great_;
    scalar rhoOld = rho;

    // Calculate A.x
    matrix_.Amul(w, x, interfaceBouCoeffs_, interfaces_, cmpt);

    // Calculate initial residual field
    scalarField r(b - w);
    scalar* RESTRICT rPtr = r.begin();

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
            #include "iteratePCG.hpp"
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
            #include "iteratePCG.hpp"
        }
    }

    return solverPerf;
}


