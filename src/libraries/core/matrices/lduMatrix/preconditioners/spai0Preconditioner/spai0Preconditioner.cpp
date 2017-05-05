/*---------------------------------------------------------------------------*\
Copyright (C) 2016 Applied CCM
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

#include "spai0Preconditioner.hpp"
#include "restrict.hpp"

namespace CML
{
    defineTypeNameAndDebug(spai0Preconditioner, 0);

    lduMatrix::preconditioner::
        addsymMatrixConstructorToTable<spai0Preconditioner>
        addspai0PreconditionerSymMatrixConstructorToTable_;

    lduMatrix::preconditioner::
        addasymMatrixConstructorToTable<spai0Preconditioner>
        addspai0PreconditionerAsymMatrixConstructorToTable_;
}

CML::spai0Preconditioner::spai0Preconditioner
(
    lduMatrix::solver const& sol,
    dictionary const&
) :
    lduMatrix::preconditioner(sol),
    rD_(sol.matrix().diag().size())
{
    this->approximateInverse(rD_, sol.matrix());
}

void CML::spai0Preconditioner::approximateInverse
(
    scalarField& rD,
    lduMatrix const& matrix
)
{
    scalar* RESTRICT rDPtr = rD.begin();
    const scalar* RESTRICT DPtr = matrix.diag().begin();

    register label nCells = rD.size();

    // Generate reciprocal spai0
    for (register label cell=0; cell<nCells; cell++)
    {
        rDPtr[cell] = DPtr[cell]*DPtr[cell];
    }

    const label* const RESTRICT uPtr = matrix.lduAddr().upperAddr().begin();
    const label* const RESTRICT lPtr = matrix.lduAddr().lowerAddr().begin();

    const scalar* const RESTRICT upperPtr = matrix.upper().begin();
    const scalar* const RESTRICT lowerPtr = matrix.lower().begin();

    register label nFaces = matrix.upper().size();

    for (register label face=0; face<nFaces; face++)
    {
        rDPtr[uPtr[face]] += upperPtr[face]*upperPtr[face];
        rDPtr[lPtr[face]] += lowerPtr[face]*lowerPtr[face];
    }

    // Generate reciprocal diagonal
    for (register label cell=0; cell<nCells; cell++)
    {
        rDPtr[cell] = DPtr[cell]/rDPtr[cell];
    }
   
}

void CML::spai0Preconditioner::precondition
(
    scalarField& w,
    scalarField const& r,
    direction const
) const
{
    scalar* RESTRICT wPtr = w.begin();
    const scalar* RESTRICT rPtr = r.begin();
    const scalar* RESTRICT rDPtr = rD_.begin();

    register label nCells = w.size();

    for (register label cell=0; cell<nCells; cell++)
    {
        wPtr[cell] = rDPtr[cell]*rPtr[cell];
    }
}


