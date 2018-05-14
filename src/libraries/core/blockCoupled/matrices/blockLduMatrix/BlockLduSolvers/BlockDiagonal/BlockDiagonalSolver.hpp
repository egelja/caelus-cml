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
    BlockDiagonalSolver

Description
    Solver for diagonal matrices

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

\*---------------------------------------------------------------------------*/

#ifndef BlockDiagonalSolver_HPP
#define BlockDiagonalSolver_HPP

#include "Field.hpp"
#include "typeInfo.hpp"
#include "BlockSolverPerformance.hpp"


namespace CML
{

template<class Type>
class BlockLduMatrix;

template<class Type>
class BlockLduSolver;

template<class Type>
class BlockSolverPerformance;

/*---------------------------------------------------------------------------*\
                      Class BlockDiagonalSolver Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class BlockDiagonalSolver
:
    public BlockLduSolver<Type>
{

    //- Disallow default bitwise copy construct
    BlockDiagonalSolver(const BlockDiagonalSolver<Type>&);

    //- Disallow default bitwise assignment
    void operator=(const BlockDiagonalSolver<Type>&);


public:

    //- Runtime type information
    TypeName("Diagonal");

    //- Construct from matrix
    BlockDiagonalSolver
    (
        const word& fieldName,
        const BlockLduMatrix<Type>& matrix,
        const dictionary& dict
    );

    // Member Functions
    //- Solve the matrix with this solver
    virtual BlockSolverPerformance<Type> solve
    (
        Field<Type>& x,
        const Field<Type>& b
    );
};

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct from matrix
template<class Type>
BlockDiagonalSolver<Type>::BlockDiagonalSolver
(
    const word& fieldName,
    const BlockLduMatrix<Type>& matrix,
    const dictionary& dict
)
:
    BlockLduSolver<Type>(fieldName, matrix, dict)
{}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
BlockSolverPerformance<Type>
BlockDiagonalSolver<Type>::solve
(
    Field<Type>& x,
    const Field<Type>& b
)
{
    CoeffField<Type> dD = inv(this->matrix_.diag());

    multiply(x, b, dD);

    return BlockSolverPerformance<Type>
    (
        this->typeName,
        this->fieldName(),
        pTraits<Type>::zero,
        pTraits<Type>::zero,
        0,
        true,
        false
    );
}


} // End namespace CML
#endif
