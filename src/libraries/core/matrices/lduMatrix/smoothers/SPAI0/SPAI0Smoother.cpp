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

#include "SPAI0Smoother.hpp"
#include "spai0Preconditioner.hpp"
#include "restrict.hpp"

namespace CML
{
    defineTypeNameAndDebug(SPAI0Smoother, 0);

    lduMatrix::smoother::addsymMatrixConstructorToTable<SPAI0Smoother>
        addSPAI0SmootherSymMatrixConstructorToTable_;

    lduMatrix::smoother::addasymMatrixConstructorToTable<SPAI0Smoother>
        addSPAI0SmootherAsymMatrixConstructorToTable_;

}

CML::SPAI0Smoother::SPAI0Smoother
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
    spai0Preconditioner::approximateInverse(rD_, matrix_);
}

void CML::SPAI0Smoother::smooth
(
    scalarField& x,
    scalarField const& b,
    direction const cmpt,
    label const nSweeps
) const
{
    // Temporary storage for the residual
    scalarField r(rD_.size());

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

        x += r;
    }
}

