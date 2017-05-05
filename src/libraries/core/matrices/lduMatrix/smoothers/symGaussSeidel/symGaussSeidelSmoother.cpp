/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
Copyright (C) 2014 Applied CCM
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
#include "symGaussSeidelSmoother.hpp"

namespace CML
{
    defineTypeNameAndDebug(symGaussSeidelSmoother, 0);

    lduMatrix::smoother::addsymMatrixConstructorToTable<symGaussSeidelSmoother>
        addsymGaussSeidelSmootherSymMatrixConstructorToTable_;

    lduMatrix::smoother::addasymMatrixConstructorToTable<symGaussSeidelSmoother>
        addsymGaussSeidelSmootherAsymMatrixConstructorToTable_;
}

CML::symGaussSeidelSmoother::symGaussSeidelSmoother
(
    const word& fieldName,
    const lduMatrix& matrix,
    const FieldField<Field, scalar>& interfaceBouCoeffs,
    const FieldField<Field, scalar>& interfaceIntCoeffs,
    const lduInterfaceFieldPtrsList& interfaces
)
:
    lduMatrix::smoother
    (
        fieldName,
        matrix,
        interfaceBouCoeffs,
        interfaceIntCoeffs,
        interfaces
    )
{}

void CML::symGaussSeidelSmoother::smooth
(
    const word& fieldName_,
    scalarField& x,
    const lduMatrix& matrix_,
    const scalarField& b,
    const FieldField<Field, scalar>& interfaceBouCoeffs_,
    const lduInterfaceFieldPtrsList& interfaces_,
    const direction cmpt,
    const label nSweeps
)
{
    register scalar* __restrict__ xPtr = x.begin();

    register const label nCells = x.size();

    scalarField bPrime(nCells);
    register scalar* __restrict__ bPrimePtr = bPrime.begin();

    register const scalar* const __restrict__ diagPtr = matrix_.diag().begin();
    register const scalar* const __restrict__ upperPtr =
        matrix_.upper().begin();
    register const scalar* const __restrict__ lowerPtr =
        matrix_.lower().begin();

    register const label* const __restrict__ uPtr =
        matrix_.lduAddr().upperAddr().begin();

    register const label* const __restrict__ ownStartPtr =
        matrix_.lduAddr().ownerStartAddr().begin();


    // Parallel boundary initialisation.  The parallel boundary is treated
    // as an effective jacobi interface in the boundary.
    // Note: there is a change of sign in the coupled
    // interface update.  The reason for this is that the
    // internal coefficients are all located at the l.h.s. of
    // the matrix whereas the "implicit" coefficients on the
    // coupled boundaries are all created as if the
    // coefficient contribution is of a source-kind (i.e. they
    // have a sign as if they are on the r.h.s. of the matrix.
    // To compensate for this, it is necessary to turn the
    // sign of the contribution.

    FieldField<Field, scalar>& mBouCoeffs =
    const_cast<FieldField<Field, scalar>&>
    (
        interfaceBouCoeffs_
    );

    forAll(mBouCoeffs, patchi)
    {
        if (interfaces_.set(patchi))
        {
            mBouCoeffs[patchi].negate();
        }
    }

    for (label sweep=0; sweep<nSweeps; sweep++)
    {
        bPrime = b;

        matrix_.initMatrixInterfaces
        (
            mBouCoeffs,
            interfaces_,
            x,
            bPrime,
            cmpt
        );

        matrix_.updateMatrixInterfaces
        (
            mBouCoeffs,
            interfaces_,
            x,
            bPrime,
            cmpt
        );

        register scalar xi;
        register label fStart;
        register label fEnd = ownStartPtr[0];

        // Forward sweep
        for (register label celli=0; celli<nCells; celli++)
        {
            // Go forward
            fStart = fEnd;
            fEnd = ownStartPtr[celli + 1];

            // Get the accumulated neighbour side
            xi = bPrimePtr[celli];

            // Accumulate the owner product side
            for (register label facei=fStart; facei<fEnd; facei++)
            {
                xi -= upperPtr[facei]*xPtr[uPtr[facei]];
            }

            // Finish current x
            xi /= diagPtr[celli];

            // Distribute the neighbour side using current x
            for (register label facei=fStart; facei<fEnd; facei++)
            {
                bPrimePtr[uPtr[facei]] -= lowerPtr[facei]*xi;
            }

            xPtr[celli] = xi;
        }

        fStart = ownStartPtr[nCells];

        // Backward sweep
        for (register label celli=nCells-1; celli>=0; celli--)
        {
            // Go backward
            fEnd = fStart;
            fStart = ownStartPtr[celli];

            // Get the accumulated neighbour side
            xi = bPrimePtr[celli];

            // Accumulate the owner product side
            for (register label facei=fStart; facei<fEnd; facei++)
            {
                xi -= upperPtr[facei]*xPtr[uPtr[facei]];
            }

            // Finish x for this cell
            xi /= diagPtr[celli];

            // Distribute the neighbour side using x for this cell
            for (register label facei=fStart; facei<fEnd; facei++)
            {
                bPrimePtr[uPtr[facei]] -= lowerPtr[facei]*xi;
            }

            xPtr[celli] = xi;
        }
    }

    // Restore interfaceBouCoeffs_
    forAll(mBouCoeffs, patchi)
    {
        if (interfaces_.set(patchi))
        {
            mBouCoeffs[patchi].negate();
        }
    }
}

void CML::symGaussSeidelSmoother::smooth
(
    scalarField& x,
    const scalarField& b,
    const direction cmpt,
    const label nSweeps
) const
{
    smooth
    (
        fieldName_,
        x,
        matrix_,
        b,
        interfaceBouCoeffs_,
        interfaces_,
        cmpt,
        nSweeps
    );
}

