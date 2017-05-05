/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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

#include "GaussSeidelSmoother.hpp"
#include "restrict.hpp"

namespace CML
{
    defineTypeNameAndDebug(GaussSeidelSmoother, 0);

    lduMatrix::smoother::addsymMatrixConstructorToTable<GaussSeidelSmoother>
        addGaussSeidelSmootherSymMatrixConstructorToTable_;

    lduMatrix::smoother::addasymMatrixConstructorToTable<GaussSeidelSmoother>
        addGaussSeidelSmootherAsymMatrixConstructorToTable_;
}

CML::GaussSeidelSmoother::GaussSeidelSmoother
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

void CML::GaussSeidelSmoother::smooth
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
    register scalar* RESTRICT xPtr = x.begin();

    register const label nCells = x.size();

    scalarField bPrime(nCells);
    register scalar* RESTRICT bPrimePtr = bPrime.begin();

    register const scalar* const RESTRICT diagPtr = matrix_.diag().begin();
    register const scalar* const RESTRICT upperPtr =
        matrix_.upper().begin();
    register const scalar* const RESTRICT lowerPtr =
        matrix_.lower().begin();

    register const label* const RESTRICT uPtr =
        matrix_.lduAddr().upperAddr().begin();

    register const label* const RESTRICT ownStartPtr =
        matrix_.lduAddr().ownerStartAddr().begin();


    // Parallel boundary initialisation.  The parallel boundary is treated
    // as an effective jacobi interface in the boundary.
    // Note: there is a change of sign in the coupled
    // interface update.  The reason for this is that the
    // internal coefficients are all located at the l.h.s. of
    // the matrix whereas the "implicit" coefficients on the
    // coupled boundaries are all created as if the
    // coefficient contribution is of a b-kind (i.e. they
    // have a sign as if they are on the r.h.s. of the matrix.
    // To compensate for this, it is necessary to turn the
    // sign of the contribution.

    FieldField<Field, scalar> mBouCoeffs(interfaceBouCoeffs_.size());

    forAll(mBouCoeffs, patchi)
    {
        if (interfaces_.set(patchi))
        {
            mBouCoeffs.set(patchi, -interfaceBouCoeffs_[patchi]);
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

        register scalar curX;
        register label fStart;
        register label fEnd = ownStartPtr[0];

        for (register label cellI=0; cellI<nCells; cellI++)
        {
            // Start and end of this row
            fStart = fEnd;
            fEnd = ownStartPtr[cellI + 1];

            // Get the accumulated neighbour side
            curX = bPrimePtr[cellI];

            // Accumulate the owner product side
            for (register label curFace=fStart; curFace<fEnd; curFace++)
            {
                curX -= upperPtr[curFace]*xPtr[uPtr[curFace]];
            }

            // Finish current x
            curX /= diagPtr[cellI];

            // Distribute the neighbour side using current x
            for (register label curFace=fStart; curFace<fEnd; curFace++)
            {
                bPrimePtr[uPtr[curFace]] -= lowerPtr[curFace]*curX;
            }

            xPtr[cellI] = curX;
        }
    }
}

void CML::GaussSeidelSmoother::smooth
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


