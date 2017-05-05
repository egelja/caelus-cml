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

#include "DICPreconditioner.hpp"
#include "restrict.hpp"

namespace CML
{
    defineTypeNameAndDebug(DICPreconditioner, 0);

    lduMatrix::preconditioner::
        addsymMatrixConstructorToTable<DICPreconditioner>
        addDICPreconditionerSymMatrixConstructorToTable_;
}

CML::DICPreconditioner::DICPreconditioner
(
    lduMatrix::solver const& sol,
    dictionary const&
)
:
    lduMatrix::preconditioner(sol),
    rD_(sol.matrix().diag()),
    rDuUpper_(sol.matrix().upper().size()),
    rDlUpper_(sol.matrix().upper().size())
{
    this->approximateInverse();
}

void CML::DICPreconditioner::approximateInverse()
{
    scalar* RESTRICT rDPtr = rD_.begin();
    scalar* RESTRICT rDuUpperPtr = rDuUpper_.begin();
    scalar* RESTRICT rDlUpperPtr = rDlUpper_.begin();

    const label* const RESTRICT uPtr =
        solver_.matrix().lduAddr().upperAddr().begin();
    const label* const RESTRICT lPtr =
        solver_.matrix().lduAddr().lowerAddr().begin();
    const scalar* const RESTRICT upperPtr =
        solver_.matrix().upper().begin();

    register label nCells = rD_.size();
    register label nFaces = solver_.matrix().upper().size();

    bool zeroPivot = false;
    for (register label face=0; face<nFaces; face++)
    {
        scalar const Djj = rDPtr[lPtr[face]];
        if (Djj >= SMALL)
        {
            rDPtr[uPtr[face]] -= sqr(upperPtr[face])/Djj;
        }
        else
        {
            zeroPivot = true;
            break;
        }
    }

    if (zeroPivot)
    {
        for (register label cell=0; cell<nCells; cell++)
        {
            rDPtr[cell] = solver_.matrix().diag()[cell];
        }
        zeroPivot = false;
    }

    // Generate reciprocal DIC
    for (register label cell=0; cell<nCells; cell++)
    {
        rDPtr[cell] = 1.0/rDPtr[cell];
    }

    for (register label face=0; face<nFaces; face++)
    {
        rDuUpperPtr[face] = rDPtr[uPtr[face]]*upperPtr[face];
        rDlUpperPtr[face] = rDPtr[lPtr[face]]*upperPtr[face];
    }
}

void CML::DICPreconditioner::precondition
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

    const scalar* const RESTRICT rDuUpperPtr = rDuUpper_.begin();
    const scalar* const RESTRICT rDlUpperPtr = rDlUpper_.begin();

    register label nCells = w.size();
    register label nFaces = solver_.matrix().upper().size();
    register label nFacesM1 = nFaces - 1;

    for (register label cell=0; cell<nCells; cell++)
    {
        wPtr[cell] = rDPtr[cell]*rPtr[cell];
    }

    for (register label face=0; face<nFaces; face++)
    {
        wPtr[uPtr[face]] -= rDuUpperPtr[face]*wPtr[lPtr[face]];
    }

    for (register label face=nFacesM1; face>=0; face--)
    {
        wPtr[lPtr[face]] -= rDlUpperPtr[face]*wPtr[uPtr[face]];
    }
}

