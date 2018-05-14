/*---------------------------------------------------------------------------*\
Copyright (C) 2004-6 H. Jasak
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    Caelus is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    BlockCGSolver

Description
    Preconditioned Conjugate Gradient solver.

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

\*---------------------------------------------------------------------------*/

#ifndef BlockCGSolver_HPP
#define BlockCGSolver_HPP

#include "BlockLduSolver.hpp"
#include "BlockIterativeSolver.hpp"
#include "BlockNoPrecon.hpp"


namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class BlockCGSolver Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class BlockCGSolver
:
    public BlockIterativeSolver<Type>
{

    //- Preconditioner
    autoPtr<BlockLduPrecon<Type> > preconPtr_;

    //- Disallow default bitwise copy construct
    BlockCGSolver(const BlockCGSolver<Type>&);

    //- Disallow default bitwise assignment
    void operator=(const BlockCGSolver<Type>&);


public:

    //- Runtime type information
    TypeName("CG");


    //- Construct from matrix components and solver data stream
    BlockCGSolver
    (
        const word& fieldName,
        const BlockLduMatrix<Type>& matrix,
        const dictionary& dict
    );

    virtual ~BlockCGSolver()
    {}

    // Member Functions
    //- Solve the matrix with this solver
    virtual BlockSolverPerformance<Type> solve
    (
        Field<Type>& x,
        const Field<Type>& b
    );
};

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct from matrix and solver data stream
template<class Type>
BlockCGSolver<Type>::BlockCGSolver
(
    const word& fieldName,
    const BlockLduMatrix<Type>& matrix,
    const dictionary& dict
)
:
    BlockIterativeSolver<Type>
    (
        fieldName,
        matrix,
        dict
    ),
    preconPtr_
    (
        BlockLduPrecon<Type>::New
        (
            matrix,
            this->dict()
        )
    )
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
typename CML::BlockSolverPerformance<Type> BlockCGSolver<Type>::solve
(
    Field<Type>& x,
    const Field<Type>& b
)
{
    // Create local references to avoid the spread this-> ugliness
    const BlockLduMatrix<Type>& matrix = this->matrix_;

    // Prepare solver performance
    BlockSolverPerformance<Type> solverPerf
    (
        typeName,
        this->fieldName()
    );

    scalar norm = this->normFactor(x, b);

    // Multiplication helper
    typename BlockCoeff<Type>::multiply mult;

    Field<Type> wA(x.size());

    // Calculate initial residual
    matrix.Amul(wA, x);
    Field<Type> rA(b - wA);

    solverPerf.initialResidual() = gSum(cmptMag(rA))/norm;
    solverPerf.finalResidual() = solverPerf.initialResidual();

    // Check convergence, solve if not converged

    if (not this->stop(solverPerf))
    {
        scalar rho = this->great_;
        scalar rhoOld = rho;

        scalar alpha, beta, wApA;

        Field<Type> pA(x.size());

        do
        {
            rhoOld = rho;

            // Execute preconditioning
            preconPtr_->precondition(wA, rA);

            // Update search directions
            rho = gSumProd(wA, rA);

            beta = rho/rhoOld;

            forAll (pA, i)
            {
                pA[i] = wA[i] + beta*pA[i];
            }

            // Update preconditioner residual
            matrix.Amul(wA, pA);

            wApA = gSumProd(wA, pA);

            // Check for singularity
            if (solverPerf.checkSingularity(mag(wApA)/norm))
            {
                break;
            }

            // Update solution and raw residual
            alpha = rho/wApA;

            forAll (x, i)
            {
                x[i] += alpha*pA[i];
            }

            forAll (rA, i)
            {
                rA[i] -= alpha*wA[i];
            }

            solverPerf.finalResidual() = gSum(cmptMag(rA))/norm;
            solverPerf.nIterations()++;
        } while (not this->stop(solverPerf));
    }

    return solverPerf;
}


} // End namespace CML
#endif
