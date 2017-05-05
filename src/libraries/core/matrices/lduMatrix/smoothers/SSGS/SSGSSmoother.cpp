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

#include "SSGSSmoother.hpp"
#include "restrict.hpp"

namespace CML
{
    defineTypeNameAndDebug(SSGSSmoother, 0);

    lduMatrix::smoother::addsymMatrixConstructorToTable<SSGSSmoother>
        addSSGSSmootherSymMatrixConstructorToTable_;
}

CML::SSGSSmoother::SSGSSmoother
(
    word const& fieldName,
    lduMatrix const& matrix,
    FieldField<Field, scalar> const& interfaceBouCoeffs,
    FieldField<Field, scalar> const& interfaceIntCoeffs,
    lduInterfaceFieldPtrsList const& interfaces
) :
    lduMatrix::smoother
    (
        fieldName,
        matrix,
        interfaceBouCoeffs,
        interfaceIntCoeffs,
        interfaces
    ),
    rD_(matrix_.diag()),
    D_(matrix_.diag()),
    rDuUpper_(matrix_.upper().size()),
    rDlUpper_(matrix_.upper().size())
{
    scalar* RESTRICT rDPtr = rD_.begin();
    scalar* RESTRICT DPtr = D_.begin();
    scalar* RESTRICT rDuUpperPtr = rDuUpper_.begin();
    scalar* RESTRICT rDlUpperPtr = rDlUpper_.begin();

    const label* const RESTRICT uPtr =
        matrix_.lduAddr().upperAddr().begin();
    const label* const RESTRICT lPtr =
        matrix_.lduAddr().lowerAddr().begin();
    const scalar* const RESTRICT upperPtr =
        matrix_.upper().begin();

    register label nCells = rD_.size();
    register label nFaces = matrix_.upper().size();

    for (register label face=0; face<nFaces; face++)
    {
        rDPtr[uPtr[face]] -= sqr(upperPtr[face])/(DPtr[lPtr[face]]+SMALL);
    }

    // Generate reciprocal diagonal
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

void CML::SSGSSmoother::smooth
(
    scalarField& x,
    scalarField const& b,
    direction const cmpt,
    label const nSweeps
) const
{
    const scalar* const RESTRICT rDuUpperPtr = rDuUpper_.begin();
    const scalar* const RESTRICT rDlUpperPtr = rDlUpper_.begin();

    const label* const RESTRICT uPtr =
        matrix_.lduAddr().upperAddr().begin();
    const label* const RESTRICT lPtr =
        matrix_.lduAddr().lowerAddr().begin();

    // Temporary storage for the residual
    scalarField r(rD_.size());
    scalar* RESTRICT rPtr = r.begin();

    for (label sweep=0; sweep<nSweeps; sweep++)
    {
        matrix_.residual
        (
            r,
            x,
            b,
            interfaceBouCoeffs_,
            interfaces_,
            cmpt
        );

        r *= rD_;

        register label nFaces = matrix_.upper().size();
        for (register label face=0; face<nFaces; face++)
        {
            rPtr[uPtr[face]] -= rDuUpperPtr[face]*rPtr[lPtr[face]];
        }

        register label nFacesM1 = nFaces - 1;
        for (register label face=nFacesM1; face>=0; face--)
        {
            rPtr[lPtr[face]] -= rDlUpperPtr[face]*rPtr[uPtr[face]];
        }

        x += r;
    }
}

