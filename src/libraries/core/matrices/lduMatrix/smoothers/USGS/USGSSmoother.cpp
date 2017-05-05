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

#include "USGSSmoother.hpp"
#include "USGSPreconditioner.hpp"
#include "restrict.hpp"

namespace CML
{
    defineTypeNameAndDebug(USGSSmoother, 0);

    lduMatrix::smoother::addasymMatrixConstructorToTable<USGSSmoother>
        addUSGSSmootherAsymMatrixConstructorToTable_;
}

CML::USGSSmoother::USGSSmoother
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
    rD_(matrix_.diag())
{
    USGSPreconditioner::approximateInverse(rD_, matrix_);
}

void CML::USGSSmoother::smooth
(
    scalarField& psi,
    scalarField const& source,
    direction const cmpt,
    label const nSweeps
) const
{
    const scalar* const RESTRICT rDPtr = rD_.begin();

    const label* const RESTRICT uPtr =
        matrix_.lduAddr().upperAddr().begin();
    const label* const RESTRICT lPtr =
        matrix_.lduAddr().lowerAddr().begin();

    const scalar* const RESTRICT upperPtr = matrix_.upper().begin();
    const scalar* const RESTRICT lowerPtr = matrix_.lower().begin();

    // Temporary storage for the residual
    scalarField rA(rD_.size());
    scalar* RESTRICT rAPtr = rA.begin();

    for (label sweep=0; sweep<nSweeps; sweep++)
    {
        matrix_.residual
        (
            rA,
            psi,
            source,
            interfaceBouCoeffs_,
            interfaces_,
            cmpt
        );

        rA *= rD_;

        register label nFaces = matrix_.upper().size();
        for (register label face=0; face<nFaces; face++)
        {
            register label u = uPtr[face];
            rAPtr[u] -= rDPtr[u]*lowerPtr[face]*rAPtr[lPtr[face]];
        }

        register label nFacesM1 = nFaces - 1;
        for (register label face=nFacesM1; face>=0; face--)
        {
            register label l = lPtr[face];
            rAPtr[l] -= rDPtr[l]*upperPtr[face]*rAPtr[uPtr[face]];
        }

        psi += rA;
    }
}

