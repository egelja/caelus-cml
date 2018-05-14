/*---------------------------------------------------------------------------*\
Copyright (C) 2004-2011 H. Jasak
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
    BlockGaussSeidelSolver

Description
    Gauss-Seidel solver

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Minor corrections have been ported from FOAM-Extend code.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

\*---------------------------------------------------------------------------*/

#ifndef BlockGaussSeidelSolver_HPP
#define BlockGaussSeidelSolver_HPP

#include "BlockLduSolver.hpp"
#include "BlockIterativeSolver.hpp"
#include "BlockSolverPerformance.hpp"
#include "BlockGaussSeidelPrecon.hpp"


namespace CML
{

/*---------------------------------------------------------------------------*\
                   Class BlockGaussSeidelSolver Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class BlockGaussSeidelSolver
:
    public BlockIterativeSolver<Type>
{

    //- Gauss-Seidel preconditioner
    BlockGaussSeidelPrecon<Type> gs_;

    //- Number of sweeps before evaluating residual
    label nSweeps_;

    //- Disallow default bitwise copy construct
    BlockGaussSeidelSolver(const BlockGaussSeidelSolver<Type>&);

    //- Disallow default bitwise assignment
    void operator=(const BlockGaussSeidelSolver<Type>&);


public:

    //- Runtime type information
    TypeName("GaussSeidel");

    //- Construct from matrix components and solver data stream
    BlockGaussSeidelSolver
    (
        const word& fieldName,
        const BlockLduMatrix<Type>& matrix,
        const dictionary& dict
    );

    virtual ~BlockGaussSeidelSolver()
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

// Construct from matrix and solver data stream
template<class Type>
BlockGaussSeidelSolver<Type>::BlockGaussSeidelSolver
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
    gs_(matrix),
    nSweeps_(readLabel(this->dict().lookup("nSweeps")))
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
typename CML::BlockSolverPerformance<Type>
BlockGaussSeidelSolver<Type>::solve
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

    Field<Type> wA(x.size());

    // Calculate residual.  Note: sign of residual swapped for efficiency
    matrix.Amul(wA, x);
    wA -= b;

    solverPerf.initialResidual() = gSum(cmptMag(wA))/norm;
    solverPerf.finalResidual() = solverPerf.initialResidual();

    // Check convergence, solve if not converged

    if (not this->stop(solverPerf))
    {
        // Iteration loop

        do
        {
            for (label i = 0; i < nSweeps_; i++)
            {
                gs_.precondition(x, b);

                solverPerf.nIterations()++;
            }

            // Re-calculate residual.  Note: sign of residual swapped
            // for efficiency
            matrix.Amul(wA, x);
            wA -= b;

            solverPerf.finalResidual() = gSum(cmptMag(wA))/norm;
        } while (not this->stop(solverPerf));
    }

    return solverPerf;
}


} // End namespace CML
#endif
