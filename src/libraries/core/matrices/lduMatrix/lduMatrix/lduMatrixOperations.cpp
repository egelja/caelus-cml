/*---------------------------------------------------------------------------*\
Copyright (C) 2011 - 2015 OpenFOAM Foundation
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

Description
    lduMatrix member operations.

\*---------------------------------------------------------------------------*/

#include "lduMatrix.hpp"
#include "restrict.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void CML::lduMatrix::sumDiag()
{
    if (!lowerPtr_ && !upperPtr_)
    {
        return;
    }

    const scalarField& Lower = const_cast<const lduMatrix&>(*this).lower();
    const scalarField& Upper = const_cast<const lduMatrix&>(*this).upper();
    scalarField& Diag = diag();

    const labelUList& l = lduAddr().lowerAddr();
    const labelUList& u = lduAddr().upperAddr();

    for (register label face=0; face<l.size(); face++)
    {
        Diag[l[face]] += Lower[face];
        Diag[u[face]] += Upper[face];
    }
}


void CML::lduMatrix::negSumDiag()
{
    if (!lowerPtr_ && !upperPtr_)
    {
        return;
    }

    const scalarField& Lower = const_cast<const lduMatrix&>(*this).lower();
    const scalarField& Upper = const_cast<const lduMatrix&>(*this).upper();
    scalarField& Diag = diag();

    const labelUList& l = lduAddr().lowerAddr();
    const labelUList& u = lduAddr().upperAddr();

    for (register label face=0; face<l.size(); face++)
    {
        Diag[l[face]] -= Lower[face];
        Diag[u[face]] -= Upper[face];
    }
}


void CML::lduMatrix::sumMagOffDiag
(
    scalarField& sumOff
) const
{
    if (!lowerPtr_ && !upperPtr_)
    {
        return;
    }

    const scalarField& Lower = const_cast<const lduMatrix&>(*this).lower();
    const scalarField& Upper = const_cast<const lduMatrix&>(*this).upper();

    const labelUList& l = lduAddr().lowerAddr();
    const labelUList& u = lduAddr().upperAddr();

    for (register label face = 0; face < l.size(); face++)
    {
        sumOff[u[face]] += mag(Upper[face]);
        sumOff[l[face]] += mag(Lower[face]);
    }
}

void CML::lduMatrix::rowSum(scalarField& s) const
{
    const scalarField& Lower = const_cast<const lduMatrix&>(*this).lower();
    const scalarField& Upper = const_cast<const lduMatrix&>(*this).upper();

    scalarField const& Diag = diag();
    s = Diag;

    const labelUList& l = lduAddr().lowerAddr();
    const labelUList& u = lduAddr().upperAddr();

    for (register label face = 0; face < l.size(); face++)
    {
        s[u[face]] += Upper[face];
        s[l[face]] += Lower[face];
    }
}


void CML::lduMatrix::spai0(scalarField& s) const
{
    const scalarField& Lower = const_cast<const lduMatrix&>(*this).lower();
    const scalarField& Upper = const_cast<const lduMatrix&>(*this).upper();

    scalarField const& Diag = diag();
    s = Diag*Diag;

    const labelUList& l = lduAddr().lowerAddr();
    const labelUList& u = lduAddr().upperAddr();

    for (register label face = 0; face < l.size(); face++)
    {
        s[u[face]] += Upper[face]*Upper[face];
        s[l[face]] += Lower[face]*Lower[face];
    }

    s /= Diag;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void CML::lduMatrix::operator=(const lduMatrix& A)
{
    if (this == &A)
    {
        FatalError
            << "lduMatrix::operator=(const lduMatrix&) : "
            << "attempted assignment to self"
            << abort(FatalError);
    }

    if (A.lowerPtr_)
    {
        lower() = A.lower();
    }
    else if (lowerPtr_)
    {
        delete lowerPtr_;
        lowerPtr_ = nullptr;
    }

    if (A.upperPtr_)
    {
        upper() = A.upper();
    }
    else if (upperPtr_)
    {
        delete upperPtr_;
        upperPtr_ = nullptr;
    }

    if (A.diagPtr_)
    {
        diag() = A.diag();
    }
}


void CML::lduMatrix::negate()
{
    if (lowerPtr_)
    {
        lowerPtr_->negate();
    }

    if (upperPtr_)
    {
        upperPtr_->negate();
    }

    if (diagPtr_)
    {
        diagPtr_->negate();
    }
}


void CML::lduMatrix::operator+=(const lduMatrix& A)
{
    if (A.diagPtr_)
    {
        diag() += A.diag();
    }

    if (symmetric() && A.symmetric())
    {
        upper() += A.upper();
    }
    else if (symmetric() && A.asymmetric())
    {
        if (upperPtr_)
        {
            lower();
        }
        else
        {
            upper();
        }

        upper() += A.upper();
        lower() += A.lower();
    }
    else if (asymmetric() && A.symmetric())
    {
        if (A.upperPtr_)
        {
            lower() += A.upper();
            upper() += A.upper();
        }
        else
        {
            lower() += A.lower();
            upper() += A.lower();
        }

    }
    else if (asymmetric() && A.asymmetric())
    {
        lower() += A.lower();
        upper() += A.upper();
    }
    else if (diagonal())
    {
        if (A.upperPtr_)
        {
            upper() = A.upper();
        }

        if (A.lowerPtr_)
        {
            lower() = A.lower();
        }
    }
    else if (A.diagonal())
    {
    }
    else
    {
        if (debug > 1)
        {
            WarningInFunction
                << "Unknown matrix type combination"
                << endl;
        }
    }
}


void CML::lduMatrix::operator-=(const lduMatrix& A)
{
    if (A.diagPtr_)
    {
        diag() -= A.diag();
    }

    if (symmetric() && A.symmetric())
    {
        upper() -= A.upper();
    }
    else if (symmetric() && A.asymmetric())
    {
        if (upperPtr_)
        {
            lower();
        }
        else
        {
            upper();
        }

        upper() -= A.upper();
        lower() -= A.lower();
    }
    else if (asymmetric() && A.symmetric())
    {
        if (A.upperPtr_)
        {
            lower() -= A.upper();
            upper() -= A.upper();
        }
        else
        {
            lower() -= A.lower();
            upper() -= A.lower();
        }

    }
    else if (asymmetric() && A.asymmetric())
    {
        lower() -= A.lower();
        upper() -= A.upper();
    }
    else if (diagonal())
    {
        if (A.upperPtr_)
        {
            upper() = -A.upper();
        }

        if (A.lowerPtr_)
        {
            lower() = -A.lower();
        }
    }
    else if (A.diagonal())
    {
    }
    else
    {
        if (debug > 1)
        {
            WarningInFunction
                << "Unknown matrix type combination"
                << endl;
        }
    }
}


void CML::lduMatrix::operator*=(const scalarField& sf)
{
    if (diagPtr_)
    {
        *diagPtr_ *= sf;
    }

    // Non-uniform scaling causes a symmetric matrix
    // to become asymmetric
    if (symmetric() || asymmetric())
    {
        scalarField& upper = this->upper();
        scalarField& lower = this->lower();

        const labelUList& l = lduAddr().lowerAddr();
        const labelUList& u = lduAddr().upperAddr();

        for (register label face=0; face<upper.size(); face++)
        {
            upper[face] *= sf[l[face]];
        }

        for (register label face=0; face<lower.size(); face++)
        {
            lower[face] *= sf[u[face]];
        }
    }
}


void CML::lduMatrix::operator*=(scalar s)
{
    if (diagPtr_)
    {
        *diagPtr_ *= s;
    }

    if (upperPtr_)
    {
        *upperPtr_ *= s;
    }

    if (lowerPtr_)
    {
        *lowerPtr_ *= s;
    }
}


CML::tmp<CML::scalarField > CML::lduMatrix::H1() const
{
    tmp<scalarField > tH1
    (
        new scalarField(lduAddr().size(), 0.0)
    );

    if (lowerPtr_ || upperPtr_)
    {
        scalarField& H1_ = tH1();

        scalar* RESTRICT H1Ptr = H1_.begin();

        const label* RESTRICT uPtr = lduAddr().upperAddr().begin();
        const label* RESTRICT lPtr = lduAddr().lowerAddr().begin();

        const scalar* RESTRICT lowerPtr = lower().begin();
        const scalar* RESTRICT upperPtr = upper().begin();

        register const label nFaces = upper().size();

        for (register label face=0; face<nFaces; face++)
        {
            H1Ptr[uPtr[face]] -= upperPtr[face];
            H1Ptr[lPtr[face]] -= lowerPtr[face];
        }
    }

    return tH1;
}


// ************************************************************************* //
