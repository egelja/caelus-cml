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
    BlockIterativeSolver

Description
    Virtual base class for a block iterative solvers

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

\*---------------------------------------------------------------------------*/

#ifndef BlockIterativeSolver_HPP
#define BlockIterativeSolver_HPP

#include "BlockLduSolver.hpp"


namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class BlockIterativeSolver Declaration
\*---------------------------------------------------------------------------*/

template <typename Type>
class BlockIterativeSolver : public BlockLduSolver<Type>
{
    //- Solver tolerance
    scalar tolerance_;

    //- Relative tolerance
    scalar relTolerance_;

    //- Minimum number of iterations
    //  (forced irrespective of convergence)
    label minIter_;

    //- Maximum number of iterations
    label maxIter_;

    //- Disallow default bitwise copy construct
    BlockIterativeSolver(const BlockIterativeSolver&);

    //- Disallow default bitwise assignment
    void operator=(const BlockIterativeSolver&);


protected:

    //- Return normalisation factor
    scalar normFactor
    (
        Field<Type>& x,
        const Field<Type>& b
    ) const;

    //- Is the stop criterion reached
    bool stop(BlockSolverPerformance<Type>& solverPerf) const;


public:

    //- Construct from matrix components and solver data stream
    BlockIterativeSolver
    (
        const word& fieldName,
        const BlockLduMatrix<Type>& matrix,
        const dictionary& dict
    );

    virtual ~BlockIterativeSolver()
    {}

    // Member Functions
    //- Return tolerance
    scalar tolerance() const
    {
        return tolerance_;
    }

    //- Return relative tolerance
    scalar relTolerance() const
    {
        return relTolerance_;
    }

    //- Return minimum number of iterations
    label minIter() const
    {
        return minIter_;
    }

    //- Return maximum number of iterations
    label maxIter() const
    {
        return maxIter_;
    }

    // Solve
    //- Solve the matrix with this solver
    virtual BlockSolverPerformance<Type> solve
    (
        Field<Type>& x,
        const Field<Type>& b
    ) = 0;
};

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct from matrix and solver data stream
template<class Type>
BlockIterativeSolver<Type>::BlockIterativeSolver
(
    const word& fieldName,
    const BlockLduMatrix<Type>& matrix,
    const dictionary& dict
)
:
    BlockLduSolver<Type>(fieldName, matrix, dict),
    tolerance_(readScalar(this->dict().lookup("tolerance"))),
    relTolerance_(readScalar(this->dict().lookup("relTol"))),
    minIter_(readLabel(this->dict().lookup("minIter"))),
    maxIter_(readLabel(this->dict().lookup("maxIter")))
{}

// * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * * //

template<class Type>
scalar BlockIterativeSolver<Type>::normFactor
(
    Field<Type>& x,
    const Field<Type>& b
) const
{
    const BlockLduMatrix<Type>& matrix = this->matrix_;

    // Calculate the normalisation factor
    const label nRows = x.size();

    Field<Type> pA(nRows);
    Field<Type> wA(nRows);

    // Calculate reference value of x
    Type xRef = gAverage(x);

    // Calculate A.x
    matrix.Amul(wA, x);

    // Calculate A.xRef, temporarily using pA for storage
    matrix.Amul
    (
        pA,
        Field<Type>(nRows, xRef)
    );

    scalar normFactor = gSum(mag(wA - pA) + mag(b - pA)) + this->small_;

    if (BlockLduMatrix<Type>::debug >= 2)
    {
        Info<< "Iterative solver normalisation factor = "
            << normFactor << endl;
    }

    return normFactor;
}


template<class Type>
bool BlockIterativeSolver<Type>::stop
(
    BlockSolverPerformance<Type>& solverPerf
) const
{
    if (solverPerf.nIterations() < minIter_)
    {
        return false;
    }

    if
    (
        solverPerf.nIterations() >= maxIter_
     || solverPerf.checkConvergence(tolerance_, relTolerance_)
    )
    {
        return true;
    }
    else
    {
        return false;
    }
}


} // End namespace CML
#endif
