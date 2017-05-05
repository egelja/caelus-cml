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

Description
    Multiply a given vector (second argument) by the matrix or its transpose
    and return the result in the first argument.

\*---------------------------------------------------------------------------*/

#include "lduMatrix.hpp"
#include "restrict.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void CML::lduMatrix::Amul
(
    scalarField& Apsi,
    const tmp<scalarField>& tpsi,
    const FieldField<Field, scalar>& interfaceBouCoeffs,
    const lduInterfaceFieldPtrsList& interfaces,
    const direction cmpt
) const
{
    // Reset multiplication result to zero
    Apsi = scalar(0.0);

    scalar* RESTRICT ApsiPtr = Apsi.begin();

    const scalarField& psi = tpsi();
    const scalar* const RESTRICT psiPtr = psi.begin();

    // Initialise the update of interfaced interfaces
    initMatrixInterfaces
    (
        interfaceBouCoeffs,
        interfaces,
        psi,
        Apsi,
        cmpt
    );

    // Protection for multiplication of incomplete matrices
    if (hasDiag())
    {
        const scalar* const RESTRICT diagPtr = diag().begin();

        register const label nCells = diag().size();
        for (register label cell=0; cell<nCells; cell++)
        {
            ApsiPtr[cell] += diagPtr[cell]*psiPtr[cell];
        }
    }

    if (hasUpper() || hasLower())
    {
        const label* const RESTRICT uPtr = lduAddr().upperAddr().begin();
        const label* const RESTRICT lPtr = lduAddr().lowerAddr().begin();

        const scalar* const RESTRICT upperPtr = upper().begin();
        const scalar* const RESTRICT lowerPtr = lower().begin();

        register const label nFaces = upper().size();

        for (register label face=0; face<nFaces; face++)
        {
            ApsiPtr[uPtr[face]] += lowerPtr[face]*psiPtr[lPtr[face]];
            ApsiPtr[lPtr[face]] += upperPtr[face]*psiPtr[uPtr[face]];
        }
    }

    // Update interface interfaces
    updateMatrixInterfaces
    (
        interfaceBouCoeffs,
        interfaces,
        psi,
        Apsi,
        cmpt
    );

    tpsi.clear();
}


void CML::lduMatrix::Tmul
(
    scalarField& Tpsi,
    const tmp<scalarField>& tpsi,
    const FieldField<Field, scalar>& interfaceIntCoeffs,
    const lduInterfaceFieldPtrsList& interfaces,
    const direction cmpt
) const
{
    // Reset multiplication result to zero  //NEW
    Tpsi = scalar(0.0);

    scalar* RESTRICT TpsiPtr = Tpsi.begin();

    const scalarField& psi = tpsi();
    const scalar* const RESTRICT psiPtr = psi.begin();

    // Initialise the update of interfaced interfaces
    initMatrixInterfaces
    (
        interfaceIntCoeffs,
        interfaces,
        psi,
        Tpsi,
        cmpt
    );

    // Protection for multiplication of incomplete matrices
    if (hasDiag())
    {
        const scalar* const RESTRICT diagPtr = diag().begin();

        register const label nCells = diag().size();
        for (register label cell=0; cell<nCells; cell++)
        {
            TpsiPtr[cell] += diagPtr[cell]*psiPtr[cell];
        }
    }

    if (hasUpper() || hasLower())
    {
        const label* const RESTRICT uPtr = lduAddr().upperAddr().begin();
        const label* const RESTRICT lPtr = lduAddr().lowerAddr().begin();

        const scalar* const RESTRICT lowerPtr = lower().begin();
        const scalar* const RESTRICT upperPtr = upper().begin();

        register const label nFaces = upper().size();
        for (register label face=0; face<nFaces; face++)
        {
            TpsiPtr[uPtr[face]] += upperPtr[face]*psiPtr[lPtr[face]];
            TpsiPtr[lPtr[face]] += lowerPtr[face]*psiPtr[uPtr[face]];
        }
    }

    // Update interface interfaces
    updateMatrixInterfaces
    (
        interfaceIntCoeffs,
        interfaces,
        psi,
        Tpsi,
        cmpt
    );

    tpsi.clear();
}


void CML::lduMatrix::sumA
(
    scalarField& sumA,
    const FieldField<Field, scalar>& interfaceBouCoeffs,
    const lduInterfaceFieldPtrsList& interfaces
) const
{
    scalar* RESTRICT sumAPtr = sumA.begin();

    const scalar* RESTRICT diagPtr = diag().begin();

    const label* RESTRICT uPtr = lduAddr().upperAddr().begin();
    const label* RESTRICT lPtr = lduAddr().lowerAddr().begin();

    const scalar* RESTRICT lowerPtr = lower().begin();
    const scalar* RESTRICT upperPtr = upper().begin();

    register const label nCells = diag().size();
    register const label nFaces = upper().size();

    for (register label cell=0; cell<nCells; cell++)
    {
        sumAPtr[cell] = diagPtr[cell];
    }

    for (register label face=0; face<nFaces; face++)
    {
        sumAPtr[uPtr[face]] += lowerPtr[face];
        sumAPtr[lPtr[face]] += upperPtr[face];
    }

    // Add the interface internal coefficients to diagonal
    // and the interface boundary coefficients to the sum-off-diagonal
    forAll(interfaces, patchI)
    {
        if (interfaces.set(patchI))
        {
            const labelUList& pa = lduAddr().patchAddr(patchI);
            const scalarField& pCoeffs = interfaceBouCoeffs[patchI];

            forAll(pa, face)
            {
                sumAPtr[pa[face]] -= pCoeffs[face];
            }
        }
    }
}


void CML::lduMatrix::residual
(
    scalarField& rA,
    const scalarField& psi,
    const scalarField& source,
    const FieldField<Field, scalar>& interfaceBouCoeffs,
    const lduInterfaceFieldPtrsList& interfaces,
    const direction cmpt
) const
{
    // Reset multiplication result to zero
    rA = scalar(0.0);

    scalar* RESTRICT rAPtr = rA.begin();

    const scalar* const RESTRICT psiPtr = psi.begin();

    const scalar* const RESTRICT sourcePtr = source.begin();

    // Parallel boundary initialisation.
    // Note: there is a change of sign in the coupled
    // interface update.  The reason for this is that the
    // internal coefficients are all located at the l.h.s. of
    // the matrix whereas the "implicit" coefficients on the
    // coupled boundaries are all created as if the
    // coefficient contribution is of a source-kind (i.e. they
    // have a sign as if they are on the r.h.s. of the matrix.
    // To compensate for this, it is necessary to turn the
    // sign of the contribution.

    FieldField<Field, scalar> mBouCoeffs(interfaceBouCoeffs.size());

    forAll(mBouCoeffs, patchi)
    {
        if (interfaces.set(patchi))
        {
            mBouCoeffs.set(patchi, -interfaceBouCoeffs[patchi]);
        }
    }

    // Initialise the update of interfaced interfaces
    initMatrixInterfaces
    (
        mBouCoeffs,
        interfaces,
        psi,
        rA,
        cmpt
    );

    // Protection for multiplication of incomplete matrices
    if (hasDiag())
    {
        const scalar* const RESTRICT diagPtr = diag().begin();

        register const label nCells = diag().size();
        for (register label cell=0; cell<nCells; cell++)
        {
            rAPtr[cell] += diagPtr[cell]*psiPtr[cell]; // Aditive part from initMatrixInterfaces
            rAPtr[cell] = sourcePtr[cell] - rAPtr[cell];
        }   
    }

    if (hasUpper() || hasLower())
    {
        const label* const RESTRICT uPtr = lduAddr().upperAddr().begin();
        const label* const RESTRICT lPtr = lduAddr().lowerAddr().begin();

        const scalar* const RESTRICT upperPtr = upper().begin();
        const scalar* const RESTRICT lowerPtr = lower().begin();

        register const label nFaces = upper().size();

        for (register label face=0; face<nFaces; face++)
        {
            rAPtr[uPtr[face]] -= lowerPtr[face]*psiPtr[lPtr[face]];
            rAPtr[lPtr[face]] -= upperPtr[face]*psiPtr[uPtr[face]];
        }
    }

    // Update interface interfaces
    updateMatrixInterfaces
    (
        mBouCoeffs,
        interfaces,
        psi,
        rA,
        cmpt
    );
}


CML::tmp<CML::scalarField> CML::lduMatrix::residual
(
    const scalarField& psi,
    const scalarField& source,
    const FieldField<Field, scalar>& interfaceBouCoeffs,
    const lduInterfaceFieldPtrsList& interfaces,
    const direction cmpt
) const
{
    tmp<scalarField> trA(new scalarField(psi.size()));
    residual(trA(), psi, source, interfaceBouCoeffs, interfaces, cmpt);
    return trA;
}


// ************************************************************************* //
