/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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

#include "DICPreconditioner.hpp"
#include "restrict.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(DICPreconditioner, 0);

    lduMatrix::preconditioner::
        addsymMatrixConstructorToTable<DICPreconditioner>
        addDICPreconditionerSymMatrixConstructorToTable_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::DICPreconditioner::DICPreconditioner
(
    const lduMatrix::solver& sol,
    const dictionary&
)
:
    lduMatrix::preconditioner(sol),
    rD_(sol.matrix().diag())
{
    calcReciprocalD(rD_, sol.matrix());
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::DICPreconditioner::calcReciprocalD
(
    scalarField& rD,
    const lduMatrix& matrix
)
{
    scalar* RESTRICT rDPtr = rD.begin();

    const label* const RESTRICT uPtr = matrix.lduAddr().upperAddr().begin();
    const label* const RESTRICT lPtr = matrix.lduAddr().lowerAddr().begin();
    const scalar* const RESTRICT upperPtr = matrix.upper().begin();

    // Calculate the DIC diagonal
    register const label nFaces = matrix.upper().size();
    for (register label face=0; face<nFaces; face++)
    {
        rDPtr[uPtr[face]] -= upperPtr[face]*upperPtr[face]/rDPtr[lPtr[face]];
    }


    // Calculate the reciprocal of the preconditioned diagonal
    register const label nCells = rD.size();

    for (register label cell=0; cell<nCells; cell++)
    {
        rDPtr[cell] = 1.0/rDPtr[cell];
    }
}


void CML::DICPreconditioner::precondition
(
    scalarField& wA,
    const scalarField& rA,
    const direction
) const
{
    scalar* RESTRICT wAPtr = wA.begin();
    const scalar* RESTRICT rAPtr = rA.begin();
    const scalar* RESTRICT rDPtr = rD_.begin();

    const label* const RESTRICT uPtr =
        solver_.matrix().lduAddr().upperAddr().begin();
    const label* const RESTRICT lPtr =
        solver_.matrix().lduAddr().lowerAddr().begin();
    const scalar* const RESTRICT upperPtr =
        solver_.matrix().upper().begin();

    register label nCells = wA.size();
    register label nFaces = solver_.matrix().upper().size();
    register label nFacesM1 = nFaces - 1;

    for (register label cell=0; cell<nCells; cell++)
    {
        wAPtr[cell] = rDPtr[cell]*rAPtr[cell];
    }

    for (register label face=0; face<nFaces; face++)
    {
        wAPtr[uPtr[face]] -= rDPtr[uPtr[face]]*upperPtr[face]*wAPtr[lPtr[face]];
    }

    for (register label face=nFacesM1; face>=0; face--)
    {
        wAPtr[lPtr[face]] -= rDPtr[lPtr[face]]*upperPtr[face]*wAPtr[uPtr[face]];
    }
}


// ************************************************************************* //
