/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2014 - 2016 Applied CCM
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

namespace CML
{
    defineTypeNameAndDebug(DILUPreconditioner, 0);

    lduMatrix::preconditioner::
        addasymMatrixConstructorToTable<DILUPreconditioner>
        addDILUPreconditionerAsymMatrixConstructorToTable_;
}

CML::DILUPreconditioner::DILUPreconditioner
(
    lduMatrix::solver const& sol,
    dictionary const&
) :
    lduMatrix::preconditioner(sol),
    rD_(sol.matrix().diag())
{
    this->approximateInverse(rD_, sol.matrix());
}

void CML::DILUPreconditioner::approximateInverse
(
    scalarField& rD,
    lduMatrix const& matrix
)
{
    scalar* RESTRICT rDPtr = rD.begin();

    const label* const RESTRICT uPtr = matrix.lduAddr().upperAddr().begin();
    const label* const RESTRICT lPtr = matrix.lduAddr().lowerAddr().begin();

    const scalar* const RESTRICT upperPtr = matrix.upper().begin();
    const scalar* const RESTRICT lowerPtr = matrix.lower().begin();

    register label nFaces = matrix.upper().size();

    bool zeroPivot = false;
    for (register label face=0; face<nFaces; face++)
    {
         scalar const Djj = rDPtr[lPtr[face]];
        if (Djj >= SMALL)
        {
            rDPtr[uPtr[face]] -= upperPtr[face]*lowerPtr[face]/Djj;
        }
        else
        {
            zeroPivot = true;
            break;
        }
    }

    // Calculate the reciprocal of the preconditioned diagonal
    register label nCells = rD.size();

    if (zeroPivot)
    {
        for (register label cell=0; cell<nCells; cell++)
        {
            rDPtr[cell] = matrix.diag()[cell];
        }
        zeroPivot = false;
    }
    
    for (register label cell=0; cell<nCells; cell++)
    {
        rDPtr[cell] = scalar(1.0)/rDPtr[cell];
    }
}

void CML::DILUPreconditioner::precondition
(
    scalarField& w,
    scalarField const& r,
    direction const
) const
{
    scalar* RESTRICT wPtr = w.begin();
    scalar const* RESTRICT rPtr = r.begin();
    scalar const* RESTRICT rDPtr = rD_.begin();

    label const* const RESTRICT uPtr =
        solver_.matrix().lduAddr().upperAddr().begin();
    label const* const RESTRICT lPtr =
        solver_.matrix().lduAddr().lowerAddr().begin();
    label const* const RESTRICT losortPtr =
        solver_.matrix().lduAddr().losortAddr().begin();

    const scalar* const RESTRICT upperPtr =
        solver_.matrix().upper().begin();
    const scalar* const RESTRICT lowerPtr =
        solver_.matrix().lower().begin();

    register label nCells = w.size();
    register label nFaces = solver_.matrix().upper().size();
    register label nFacesM1 = nFaces - 1;

    for (register label cell=0; cell<nCells; cell++)
    {
        wPtr[cell] = rDPtr[cell]*rPtr[cell];
    }

    register label sface;

    for (register label face=0; face<nFaces; face++)
    {
        sface = losortPtr[face];
        wPtr[uPtr[sface]] -=
            rDPtr[uPtr[sface]]*lowerPtr[sface]*wPtr[lPtr[sface]];
    }

    for (register label face=nFacesM1; face>=0; face--)
    {
        wPtr[lPtr[face]] -=
            rDPtr[lPtr[face]]*upperPtr[face]*wPtr[uPtr[face]];
    }
}

void CML::DILUPreconditioner::preconditionT
(
    scalarField& wT,
    scalarField const& rT,
    direction const
) const
{
    scalar* RESTRICT wTPtr = wT.begin();
    scalar const* RESTRICT rTPtr = rT.begin();
    scalar const* RESTRICT rDPtr = rD_.begin();

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

