/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2014 Applied CCM
Copyright (C) 2016 OpenCFD Ltd
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

#include "smoothSolver.hpp"
#include "profiling.hpp"

namespace CML
{
    defineTypeNameAndDebug(smoothSolver, 0);

    lduMatrix::solver::addsymMatrixConstructorToTable<smoothSolver>
        addsmoothSolverSymMatrixConstructorToTable_;

    lduMatrix::solver::addasymMatrixConstructorToTable<smoothSolver>
        addsmoothSolverAsymMatrixConstructorToTable_;
}

CML::smoothSolver::smoothSolver
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
{
    readControls();
}

void CML::smoothSolver::readControls()
{
    lduMatrix::solver::readControls();
    nSweeps_ = controlDict_.lookupOrDefault<label>("nSweeps", 1);
}


CML::solverPerformance CML::smoothSolver::solve
(
    scalarField& x,
    const scalarField& b,
    const direction cmpt
) const
{
    // Setup class containing solver performance data
    solverPerformance solverPerf(typeName, fieldName_);

    // If the nSweeps_ is negative do a fixed number of sweeps
    if (nSweeps_ < 0)
    {
        addProfiling(solve, "lduMatrix::smoother." + fieldName_);

        autoPtr<lduMatrix::smoother> smootherPtr = lduMatrix::smoother::New
        (
            fieldName_,
            matrix_,
            interfaceBouCoeffs_,
            interfaceIntCoeffs_,
            interfaces_,
            controlDict_
        );

        smootherPtr->smooth
        (
            x,
            b,
            cmpt,
            -nSweeps_
        );

        solverPerf.nIterations() -= nSweeps_;
    }
    else
    {
        scalar normFactor = 0;

        {
            scalarField Ax(x.size());
            scalarField temp(x.size());

            // Calculate matrix-vector multiply A.x
            matrix_.Amul(Ax, x, interfaceBouCoeffs_, interfaces_, cmpt);

            // Calculate normalisation factor
            normFactor = this->normFactor(x, b, Ax, temp);

            // Calculate residual magnitude
            solverPerf.initialResidual() = gSumMag(b - Ax)/normFactor;
            solverPerf.finalResidual() = solverPerf.initialResidual();
        }

        if (lduMatrix::debug >= 2)
        {
            Info<< "   Normalisation factor = " << normFactor << endl;
        }


        // Check convergence, solve if not converged
        if (!solverPerf.checkConvergence
             (
                 tolerance_, 
                 relTol_,
                 solverPerf.nIterations(),
                 minIter_
             )
           )
        {
            addProfiling(solve, "lduMatrix::smoother." + fieldName_);

            autoPtr<lduMatrix::smoother> smootherPtr = lduMatrix::smoother::New
            (
                fieldName_,
                matrix_,
                interfaceBouCoeffs_,
                interfaceIntCoeffs_,
                interfaces_,
                controlDict_
            );

            // Smoothing loop
            do
            {
                smootherPtr->smooth
                (
                    x,
                    b,
                    cmpt,
                    nSweeps_
                );

                // Calculate the residual to check convergence
                solverPerf.finalResidual() = gSumMag
                (
                    matrix_.residual
                    (
                        x,
                        b,
                        interfaceBouCoeffs_,
                        interfaces_,
                        cmpt
                    )
                )/normFactor;
            } while
            (
                ((solverPerf.nIterations() += nSweeps_) < maxIter_
	        && 
                !(solverPerf.checkConvergence
                  (
                      tolerance_, 
                      relTol_, 
                      solverPerf.nIterations(),
                      minIter_
                  )
                 )
                )
            );
        }
    }

    return solverPerf;
}

