/*---------------------------------------------------------------------------*\
Copyright (C) 2016-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::MatrixBlock

Description
    A templated block of an (m x n) matrix of type \<MatrixType\>.

        CML::ConstMatrixBlock: block of a const matrix
        CML::MatrixBlock: block of a non-const matrix

    The block may be assigned to a block of another matrix or to a VectorSpace
    or MatrixSpace e.g. \c tensor.  Conversion of a column block to a \c
    Field<T> is also provide.


\*---------------------------------------------------------------------------*/

#ifndef MatrixBlock_HPP
#define MatrixBlock_HPP

#include "Matrix.hpp"
#include "MatrixSpace.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class ConstMatrixBlock Declaration
\*---------------------------------------------------------------------------*/

template<class MatrixType>
class ConstMatrixBlock
{

    //- Const reference to the parent matrix
    const MatrixType& matrix_;

    // Block size
    const label mRows_;
    const label nCols_;

    // Block location in parent matrix
    const label rowStart_;
    const label colStart_;

public:

    typedef typename MatrixType::cmptType cmptType;

    //- Construct block for matrix, size and location
    inline ConstMatrixBlock
    (
        const MatrixType& matrix,
        const label m,
        const label n,
        const label mStart,
        const label nStart
    );


    // Member Functions

    //- Return the number of rows in the block
    inline label m() const
    {
        return mRows_;
    }

    //- Return the number of columns in the block
    inline label n() const
    {
        return nCols_;
    }

    //- (i, j) const element access operator
    inline const cmptType& operator()
    (
        const label i,
        const label j
    ) const;

    //- Convert a column of a matrix to a Field
    operator Field<cmptType>() const;
};


/*---------------------------------------------------------------------------*\
                          Class MatrixBlock Declaration
\*---------------------------------------------------------------------------*/

template<class MatrixType>
class MatrixBlock
{

    //- Reference to the parent matrix
    MatrixType& matrix_;

    // Block size
    const label mRows_;
    const label nCols_;

    // Block location in parent matrix
    const label rowStart_;
    const label colStart_;

public:

    typedef typename MatrixType::cmptType cmptType;

    //- Construct block for matrix, size and location
    inline MatrixBlock
    (
        MatrixType& matrix,
        const label m,
        const label n,
        const label mStart,
        const label nStart
    );


    // Member Functions

    //- Return the number of rows in the block
    inline label m() const
    {
        return mRows_;
    }

    //- Return the number of columns in the block
    inline label n() const
    {
        return nCols_;
    }

    //- (i, j) const element access operator
    inline const cmptType& operator()
    (
        const label i,
        const label j
    ) const;

    //- (i, j) element access operator
    inline cmptType& operator()(const label i, const label j);

    //- Convert a column of a matrix to a Field
    operator Field<cmptType>() const;


    // Member operators

    //- Assignment to a compatible matrix
    template<class Form>
    void operator=(const Matrix<Form, cmptType>&);

    //- Assignment to a compatible const block
    void operator=(const ConstMatrixBlock<MatrixType>&);

    //- Assignment to a compatible block
    void operator=(const MatrixBlock<MatrixType>&);

    //- Assignment to a compatible const block
    template<class MatrixType2>
    void operator=(const ConstMatrixBlock<MatrixType2>&);

    //- Assignment to a compatible block
    template<class MatrixType2>
    void operator=(const MatrixBlock<MatrixType2>&);

    //- Assignment to a compatible MatrixSpace
    template<class MSForm, direction Nrows, direction Ncols>
    void operator=(const MatrixSpace<MSForm, cmptType, Nrows, Ncols>&);

    //- Assignment to a compatible MatrixSpace block
    template
    <
        template<class, direction, direction> class Block,
        class SubTensor,
        direction BRowStart,
        direction BColStart
    >
    void operator=(const Block<SubTensor, BRowStart, BColStart>&);

    //- Assignment to a compatible VectorSpace (column-vector)
    template<class VSForm, direction Ncmpts>
    void operator=(const VectorSpace<VSForm, cmptType, Ncmpts>&);

    //- Assignment to a compatible VectorSpace (column-vector) block
    template
    <
        template<class, direction> class Block,
        class SubVector,
        direction BStart
    >
    void operator=(const Block<SubVector, BStart>&);

    //- Assignment to a Field (column-vector)
    void operator=(const Field<cmptType>&);
};


} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class MatrixType>
CML::ConstMatrixBlock<MatrixType>::ConstMatrixBlock
(
    const MatrixType& matrix,
    const label m,
    const label n,
    const label mStart,
    const label nStart
)
:
    matrix_(matrix),
    mRows_(m),
    nCols_(n),
    rowStart_(mStart),
    colStart_(nStart)
{
    #ifdef FULLDEBUG
    if
    (
        rowStart_ + mRows_ > matrix.m()
     || colStart_ + nCols_ > matrix.n()
    )
    {
        FatalErrorInFunction
            << "Block addresses outside matrix"
            << abort(FatalError);
    }
    #endif
}


template<class MatrixType>
CML::MatrixBlock<MatrixType>::MatrixBlock
(
    MatrixType& matrix,
    const label m,
    const label n,
    const label mStart,
    const label nStart
)
:
    matrix_(matrix),
    mRows_(m),
    nCols_(n),
    rowStart_(mStart),
    colStart_(nStart)
{
    #ifdef FULLDEBUG
    if
    (
        rowStart_ + mRows_ > matrix.m()
     || colStart_ + nCols_ > matrix.n()
    )
    {
        FatalErrorInFunction
            << "Block addresses outside matrix"
            << abort(FatalError);
    }
    #endif
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class MatrixType>
inline const typename MatrixType::cmptType&
CML::ConstMatrixBlock<MatrixType>::operator()
(
    const label i,
    const label j
) const
{
    #ifdef FULLDEBUG
    if (i<0 || i>=mRows_)
    {
        FatalErrorInFunction
            << "Index " << i << " out of range 0 ... " << mRows_-1
            << abort(FatalError);
    }
    if (j<0 || j>=nCols_)
    {
        FatalErrorInFunction
            << "Index " << j << " out of range 0 ... " << nCols_-1
            << abort(FatalError);
    }
    #endif

    return matrix_(i + rowStart_, j + colStart_);
}


template<class MatrixType>
inline const typename MatrixType::cmptType&
CML::MatrixBlock<MatrixType>::operator()
(
    const label i,
    const label j
) const
{
    #ifdef FULLDEBUG
    if (i<0 || i>=mRows_)
    {
        FatalErrorInFunction
            << "Index " << i << " out of range 0 ... " << mRows_-1
            << abort(FatalError);
    }
    if (j<0 || j>=nCols_)
    {
        FatalErrorInFunction
            << "Index " << j << " out of range 0 ... " << nCols_-1
            << abort(FatalError);
    }
    #endif

    return matrix_(i + rowStart_, j + colStart_);
}


template<class MatrixType>
inline typename MatrixType::cmptType&
CML::MatrixBlock<MatrixType>::operator()
(
    const label i,
    const label j
)
{
    #ifdef FULLDEBUG
    if (i<0 || i>=mRows_)
    {
        FatalErrorInFunction
            << "Index " << i << " out of range 0 ... " << mRows_-1
            << abort(FatalError);
    }
    if (j<0 || j>=nCols_)
    {
        FatalErrorInFunction
            << "Index " << j << " out of range 0 ... " << nCols_-1
            << abort(FatalError);
    }
    #endif

    return matrix_(i + rowStart_, j + colStart_);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class MatrixType>
CML::ConstMatrixBlock<MatrixType>::operator Field<cmptType>() const
{
    if (nCols_ != 1)
    {
        FatalErrorInFunction
            << "Number of columns " << nCols_ << " != 1"
            << abort(FatalError);
    }

    Field<cmptType> f(mRows_);

    forAll(f, i)
    {
        f[i] = operator()(i, 0);
    }

    return f;
}


template<class MatrixType>
CML::MatrixBlock<MatrixType>::operator Field<cmptType>() const
{
    if (nCols_ != 1)
    {
        FatalErrorInFunction
            << "Number of columns " << nCols_ << " != 1"
            << abort(FatalError);
    }

    Field<cmptType> f(mRows_);

    forAll(f, i)
    {
        f[i] = operator()(i, 0);
    }

    return f;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class MatrixType>
template<class Form>
void CML::MatrixBlock<MatrixType>::operator=
(
    const Matrix<Form, cmptType>& Mb
)
{
    if (mRows_ != Mb.m() || nCols_ != Mb.n())
    {
        FatalErrorInFunction
            << "Attempt to assign blocks of different sizes: "
            << mRows_ << "x" << nCols_ << " != "
            << Mb.m() << "x" << Mb.n()
            << abort(FatalError);
    }

    for (label i=0; i<mRows_; i++)
    {
        for (label j=0; j<nCols_; j++)
        {
            (*this)(i, j) = Mb(i, j);
        }
    }
}


template<class MatrixType>
void CML::MatrixBlock<MatrixType>::operator=
(
    const ConstMatrixBlock<MatrixType>& Mb
)
{
    if (this != &Mb)
    {
        if (mRows_ != Mb.m() || nCols_ != Mb.n())
        {
            FatalErrorInFunction
                << "Attempt to assign blocks of different sizes: "
                << mRows_ << "x" << nCols_ << " != "
                << Mb.m() << "x" << Mb.n()
                << abort(FatalError);
        }

        for (label i=0; i<mRows_; i++)
        {
            for (label j=0; j<nCols_; j++)
            {
                (*this)(i, j) = Mb(i, j);
            }
        }
    }
}


template<class MatrixType>
void CML::MatrixBlock<MatrixType>::operator=
(
    const MatrixBlock<MatrixType>& Mb
)
{
    if (this != &Mb)
    {
        if (mRows_ != Mb.m() || nCols_ != Mb.n())
        {
            FatalErrorInFunction
                << "Attempt to assign blocks of different sizes: "
                << mRows_ << "x" << nCols_ << " != "
                << Mb.m() << "x" << Mb.n()
                << abort(FatalError);
        }

        for (label i=0; i<mRows_; i++)
        {
            for (label j=0; j<nCols_; j++)
            {
                (*this)(i, j) = Mb(i, j);
            }
        }
    }
}


template<class MatrixType>
template<class MatrixType2>
void CML::MatrixBlock<MatrixType>::operator=
(
    const ConstMatrixBlock<MatrixType2>& Mb
)
{
    if (this != &Mb)
    {
        if (mRows_ != Mb.m() || nCols_ != Mb.n())
        {
            FatalErrorInFunction
                << "Attempt to assign blocks of different sizes: "
                << mRows_ << "x" << nCols_ << " != "
                << Mb.m() << "x" << Mb.n()
                << abort(FatalError);
        }

        for (label i=0; i<mRows_; i++)
        {
            for (label j=0; j<nCols_; j++)
            {
                (*this)(i, j) = Mb(i, j);
            }
        }
    }
}


template<class MatrixType>
template<class MatrixType2>
void CML::MatrixBlock<MatrixType>::operator=
(
    const MatrixBlock<MatrixType2>& Mb
)
{
    if (this != &Mb)
    {
        if (mRows_ != Mb.m() || nCols_ != Mb.n())
        {
            FatalErrorInFunction
                << "Attempt to assign blocks of different sizes: "
                << mRows_ << "x" << nCols_ << " != "
                << Mb.m() << "x" << Mb.n()
                << abort(FatalError);
        }

        for (label i=0; i<mRows_; i++)
        {
            for (label j=0; j<nCols_; j++)
            {
                (*this)(i, j) = Mb(i, j);
            }
        }
    }
}


template<class MatrixType>
template
<
    template<class, CML::direction, CML::direction> class MSBlock,
    class SubTensor,
    CML::direction BRowStart,
    CML::direction BColStart
>
void CML::MatrixBlock<MatrixType>::operator=
(
    const MSBlock<SubTensor, BRowStart, BColStart>& Mb
)
{
    if (mRows_ != Mb.mRows || nCols_ != Mb.nCols)
    {
        FatalErrorInFunction
            << "Attempt to assign blocks of different sizes: "
            << mRows_ << "x" << nCols_ << " != "
            << Mb.mRows << "x" << Mb.nCols
            << abort(FatalError);
    }

    for (direction i=0; i<mRows_; ++i)
    {
        for (direction j=0; j<nCols_; ++j)
        {
            operator()(i, j) = Mb(i, j);
        }
    }
}


template<class MatrixType>
template
<
    template<class, CML::direction> class VSBlock,
    class SubVector,
    CML::direction BStart
>
void CML::MatrixBlock<MatrixType>::operator=
(
    const VSBlock<SubVector, BStart>& Mb
)
{
    if (mRows_ != Mb.nComponents || nCols_ != 1)
    {
        FatalErrorInFunction
            << "Attempt to assign blocks of different sizes: "
            << mRows_ << "x" << nCols_ << " != "
            << Mb.nComponents << "x" << 1
            << abort(FatalError);
    }

    for (direction i=0; i<mRows_; ++i)
    {
        operator()(i, 0) = Mb[i];
    }
}


template<class MatrixType>
template<class MSForm, CML::direction Nrows, CML::direction Ncols>
void CML::MatrixBlock<MatrixType>::operator=
(
    const MatrixSpace<MSForm, cmptType, Nrows, Ncols>& ms
)
{
    if (mRows_ != Nrows || nCols_ != Ncols)
    {
        FatalErrorInFunction
            << "Attempt to assign blocks of different sizes: "
            << mRows_ << "x" << nCols_ << " != "
            << Nrows << "x" << Ncols
            << abort(FatalError);
    }

    for (label i=0; i<mRows_; i++)
    {
        for (label j=0; j<nCols_; j++)
        {
            (*this)(i, j) = ms(i, j);
        }
    }
}


template<class MatrixType>
template<class VSForm, CML::direction Ncmpts>
void CML::MatrixBlock<MatrixType>::operator=
(
    const VectorSpace<VSForm, cmptType, Ncmpts>& ms
)
{
    if (mRows_ != Ncmpts || nCols_ != 1)
    {
        FatalErrorInFunction
            << "Attempt to assign blocks of different sizes: "
            << mRows_ << "x" << nCols_ << " != "
            << Ncmpts << "x" << 1
            << abort(FatalError);
    }

    for (direction i=0; i<Ncmpts; ++i)
    {
        operator()(i, 0) = ms[i];
    }
}


template<class MatrixType>
void CML::MatrixBlock<MatrixType>::operator=(const Field<cmptType>& f)
{
    if (mRows_ != f.size() || nCols_ != 1)
    {
        FatalErrorInFunction
            << "Error: cannot assign blocks of different size (left is "
            << mRows_ << "x" << nCols_ << " != "
            << f.size() << "x" << 1
            << abort(FatalError);
    }

    forAll(f, i)
    {
        operator()(i, 0) = f[i];
    }
}


#endif
