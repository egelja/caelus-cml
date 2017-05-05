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

#include "DILUPreconditioner.hpp"
#include "restrict.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(DILUPreconditioner, 0);

    lduMatrix::preconditioner::
        addasymMatrixConstructorToTable<DILUPreconditioner>
        addDILUPreconditionerAsymMatrixConstructorToTable_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::DILUPreconditioner::DILUPreconditioner
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

void CML::DILUPreconditioner::calcReciprocalD
(
    scalarField& rD,
    const lduMatrix& matrix
)
{
    scalar* RESTRICT rDPtr = rD.begin();

    const label* const RESTRICT uPtr = matrix.lduAddr().upperAddr().begin();
    const label* const RESTRICT lPtr = matrix.lduAddr().lowerAddr().begin();

    const scalar* const RESTRICT upperPtr = matrix.upper().begin();
    const scalar* const RESTRICT lowerPtr = matrix.lower().begin();

    register label nFaces = matrix.upper().size();
    for (register label face=0; face<nFaces; face++)
    {
        rDPtr[uPtr[face]] -= upperPtr[face]*lowerPtr[face]/rDPtr[lPtr[face]];
    }


    // Calculate the reciprocal of the preconditioned diagonal
    register label nCells = rD.size();

    for (register label cell=0; cell<nCells; cell++)
    {
        rDPtr[cell] = 1.0/rDPtr[cell];
    }
}


void CML::DILUPreconditioner::precondition
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
    const label* const RESTRICT losortPtr =
        solver_.matrix().lduAddr().losortAddr().begin();

    const scalar* const RESTRICT upperPtr =
        solver_.matrix().upper().begin();
    const scalar* const RESTRICT lowerPtr =
        solver_.matrix().lower().begin();

    register label nCells = wA.size();
    register label nFaces = solver_.matrix().upper().size();
    register label nFacesM1 = nFaces - 1;

    for (register label cell=0; cell<nCells; cell++)
    {
        wAPtr[cell] = rDPtr[cell]*rAPtr[cell];
    }


    register label sface;

    for (register label face=0; face<nFaces; face++)
    {
        sface = losortPtr[face];
        wAPtr[uPtr[sface]] -=
            rDPtr[uPtr[sface]]*lowerPtr[sface]*wAPtr[lPtr[sface]];
    }

    for (register label face=nFacesM1; face>=0; face--)
    {
        wAPtr[lPtr[face]] -=
            rDPtr[lPtr[face]]*upperPtr[face]*wAPtr[uPtr[face]];
    }
}


void CML::DILUPreconditioner::preconditionT
(
    scalarField& wT,
    const scalarField& rT,
    const direction
) const
{
    scalar* RESTRICT wTPtr = wT.begin();
    const scalar* RESTRICT rTPtr = rT.begin();
    const scalar* RESTRICT rDPtr = rD_.begin();

    const label* const RESTRICT uPtr =
        solver_.matrix().lduAddr().upperAddr().begin();
    const label* const RESTRICT lPtr =
        solver_.matrix().lduAddr().lowerAddr().begin();
    const label* const RESTRICT losortPtr =
        solver_.matrix().lduAddr().losortAddr().begin();

    const scalar* const RESTRICT upperPtr =
        solver_.matrix().upper().begin();
    const scalar* const RESTRICT lowerPtr =
        solver_.matrix().lower().begin();

    register label nCells = wT.size();
    register label nFaces = solver_.matrix().upper().size();
    register label nFacesM1 = nFaces - 1;

    for (register label cell=0; cell<nCells; cell++)
    {
        wTPtr[cell] = rDPtr[cell]*rTPtr[cell];
    }

    for (register label face=0; face<nFaces; face++)
    {
        wTPtr[uPtr[face]] -=
            rDPtr[uPtr[face]]*upperPtr[face]*wTPtr[lPtr[face]];
    }


    register label sface;

    for (register label face=nFacesM1; face>=0; face--)
    {
        sface = losortPtr[face];
        wTPtr[lPtr[sface]] -=
            rDPtr[lPtr[sface]]*lowerPtr[sface]*wTPtr[uPtr[sface]];
    }
}


// ************************************************************************* //
