/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "lduMatrix.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


namespace CML
{
    defineTypeNameAndDebug(lduMatrix, 1);

    const scalar lduMatrix::great_ = 1.0e+20;
    const scalar lduMatrix::small_ = 1.0e-20;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::lduMatrix::lduMatrix(const lduMesh& mesh)
:
    lduMesh_(mesh),
    lowerPtr_(nullptr),
    diagPtr_(nullptr),
    upperPtr_(nullptr)
{}


CML::lduMatrix::lduMatrix(const lduMatrix& A)
:
    lduMesh_(A.lduMesh_),
    lowerPtr_(nullptr),
    diagPtr_(nullptr),
    upperPtr_(nullptr)
{
    if (A.lowerPtr_)
    {
        lowerPtr_ = new scalarField(*(A.lowerPtr_));
    }

    if (A.diagPtr_)
    {
        diagPtr_ = new scalarField(*(A.diagPtr_));
    }

    if (A.upperPtr_)
    {
        upperPtr_ = new scalarField(*(A.upperPtr_));
    }
}


CML::lduMatrix::lduMatrix(lduMatrix& A, bool reUse)
:
    lduMesh_(A.lduMesh_),
    lowerPtr_(nullptr),
    diagPtr_(nullptr),
    upperPtr_(nullptr)
{
    if (reUse)
    {
        if (A.lowerPtr_)
        {
            lowerPtr_ = A.lowerPtr_;
            A.lowerPtr_ = nullptr;
        }

        if (A.diagPtr_)
        {
            diagPtr_ = A.diagPtr_;
            A.diagPtr_ = nullptr;
        }

        if (A.upperPtr_)
        {
            upperPtr_ = A.upperPtr_;
            A.upperPtr_ = nullptr;
        }
    }
    else
    {
        if (A.lowerPtr_)
        {
            lowerPtr_ = new scalarField(*(A.lowerPtr_));
        }

        if (A.diagPtr_)
        {
            diagPtr_ = new scalarField(*(A.diagPtr_));
        }

        if (A.upperPtr_)
        {
            upperPtr_ = new scalarField(*(A.upperPtr_));
        }
    }
}


CML::lduMatrix::lduMatrix
(
    const lduMesh& mesh,
    Istream& is
)
:
    lduMesh_(mesh),
    lowerPtr_(new scalarField(is)),
    diagPtr_(new scalarField(is)),
    upperPtr_(new scalarField(is))
{}


CML::lduMatrix::~lduMatrix()
{
    if (lowerPtr_)
    {
        delete lowerPtr_;
    }

    if (diagPtr_)
    {
        delete diagPtr_;
    }

    if (upperPtr_)
    {
        delete upperPtr_;
    }
}


CML::scalarField& CML::lduMatrix::lower()
{
    if (!lowerPtr_)
    {
        if (upperPtr_)
        {
            lowerPtr_ = new scalarField(*upperPtr_);
        }
        else
        {
            lowerPtr_ = new scalarField(lduAddr().lowerAddr().size(), 0.0);
        }
    }

    return *lowerPtr_;
}


CML::scalarField& CML::lduMatrix::diag()
{
    if (!diagPtr_)
    {
        diagPtr_ = new scalarField(lduAddr().size(), 0.0);
    }

    return *diagPtr_;
}


CML::scalarField& CML::lduMatrix::upper()
{
    if (!upperPtr_)
    {
        if (lowerPtr_)
        {
            upperPtr_ = new scalarField(*lowerPtr_);
        }
        else
        {
            upperPtr_ = new scalarField(lduAddr().lowerAddr().size(), 0.0);
        }
    }

    return *upperPtr_;
}


const CML::scalarField& CML::lduMatrix::lower() const
{
    if (!lowerPtr_ && !upperPtr_)
    {
        FatalErrorInFunction
            << "lowerPtr_ or upperPtr_ unallocated"
            << abort(FatalError);
    }

    if (lowerPtr_)
    {
        return *lowerPtr_;
    }
    else
    {
        return *upperPtr_;
    }
}


const CML::scalarField& CML::lduMatrix::diag() const
{
    if (!diagPtr_)
    {
        FatalErrorInFunction
            << "diagPtr_ unallocated"
            << abort(FatalError);
    }

    return *diagPtr_;
}


const CML::scalarField& CML::lduMatrix::upper() const
{
    if (!lowerPtr_ && !upperPtr_)
    {
        FatalErrorInFunction
            << "lowerPtr_ or upperPtr_ unallocated"
            << abort(FatalError);
    }

    if (upperPtr_)
    {
        return *upperPtr_;
    }
    else
    {
        return *lowerPtr_;
    }
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const lduMatrix& ldum)
{
    if (ldum.lowerPtr_)
    {
        os  << "Lower triangle = "
            << *ldum.lowerPtr_
            << endl << endl;
    }

    if (ldum.diagPtr_)
    {
        os  << "diagonal = "
            << *ldum.diagPtr_
            << endl << endl;
    }

    if (ldum.upperPtr_)
    {
        os  << "Upper triangle = "
            << *ldum.upperPtr_
            << endl << endl;
    }

    os.check("Ostream& operator<<(Ostream&, const lduMatrix&");

    return os;
}


// ************************************************************************* //
