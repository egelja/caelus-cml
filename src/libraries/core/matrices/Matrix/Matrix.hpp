/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
Copyright (C) 2015 Applied CCM
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

Class
    CML::Matrix

Description
    A templated (m x n) matrix of objects of \<T\>.


\*---------------------------------------------------------------------------*/

#ifndef Matrix_H
#define Matrix_H

#include "bool.hpp"
#include "label.hpp"
#include "uLabel.hpp"
#include "Field.hpp"
#include "zero.hpp"
#include "autoPtr.hpp"
#include "restrict.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Form, class Type> class Matrix;

template<class Form, class Type> Istream& operator>>
(
    Istream&,
    Matrix<Form, Type>&
);

template<class Form, class Type> Ostream& operator<<
(
    Ostream&,
    const Matrix<Form, Type>&
);

template<class MatrixType>
class ConstMatrixBlock;

template<class MatrixType>
class MatrixBlock;


/*---------------------------------------------------------------------------*\
                           Class Matrix Declaration
\*---------------------------------------------------------------------------*/

template<class Form, class Type>
class Matrix
{
    // Private data

        //- Number of rows and columns in Matrix.
        label mRows_, nCols_;

        //- Row pointers
        Type* RESTRICT v_;

        //- Allocate the storage for the element vector
        void allocate();


public:

    //- Matrix type
    typedef Matrix<Form, Type> mType;

    //- Component type
    typedef Type cmptType;


    // Static Member Functions

        //- Return a null Matrix
        inline static const mType& null();


    // Constructors

        //- Null constructor.
        inline Matrix();

        //- Construct given number of rows and columns.
        Matrix(const label m, const label n);

        //- Construct with given number of rows and columns
        //  initializing all elements to zero
        Matrix(const label m, const label n, const zero);

        //- Construct with given number of rows and columns
        //  initializing all elements to the given value
        Matrix(const label m, const label n, const Type&);

        //- Copy constructor.
        Matrix(const mType&);

        //- Copy constructor from matrix of a different form
        template<class Form2>
        explicit Matrix(const Matrix<Form2, Type>&);

        //- Construct from a block of another matrix
        template<class MatrixType>
        Matrix(const ConstMatrixBlock<MatrixType>&);

        //- Construct from a block of another matrix
        template<class MatrixType>
        Matrix(const MatrixBlock<MatrixType>&);

        //- Construct from Istream.
        Matrix(Istream&);

        //- Clone
        inline autoPtr<mType> clone() const;


    //- Destructor
    ~Matrix();


    // Member Functions

        // Access

            //- Return the number of rows
            inline label m() const;

            //- Return the number of columns
            inline label n() const;

            //- Return the number of elements in matrix (m*n)
            inline label size() const;

            //- Return element vector of the constant Matrix
            inline const Type* v() const;

            //- Return element vector of the Matrix
            inline Type* v();


        // Block access

            inline ConstMatrixBlock<mType> block
            (
                const label m,
                const label n,
                const label mStart,
                const label nStart
            ) const;

            template<class VectorSpace>
            inline ConstMatrixBlock<mType> block
            (
                const label mStart,
                const label nStart
            ) const;

            inline ConstMatrixBlock<mType> col
            (
                const label m,
                const label rowStart
            ) const;

            inline ConstMatrixBlock<mType> col
            (
                const label m,
                const label mStart,
                const label nStart
            ) const;


            inline MatrixBlock<mType> block
            (
                const label m,
                const label n,
                const label mStart,
                const label nStart
            );

            template<class VectorSpace>
            inline MatrixBlock<mType> block
            (
                const label mStart,
                const label nStart
            );

            inline MatrixBlock<mType> col
            (
                const label m,
                const label rowStart
            );

            inline MatrixBlock<mType> col
            (
                const label m,
                const label mStart,
                const label nStart
            );


        // Check

            //- Check index i is within valid range (0 ... m-1).
            inline void checki(const label i) const;

            //- Check index j is within valid range (0 ... n-1).
            inline void checkj(const label j) const;


        // Edit

            //- Clear the Matrix, i.e. set sizes to zero.
            void clear();

            //- Transfer the contents of the argument Matrix into this Matrix
            //  and annul the argument Matrix.
            void transfer(mType&);

            //- Resize the matrix preserving the elements
            void setSize(const label m, const label n);

            //- Resize the matrix without reallocating storage (unsafe)
            inline void shallowResize(const label m, const label n);


        //- Return the transpose of the matrix
        Form T() const;


    // Member operators

        //- Return subscript-checked row of Matrix.
        inline Type* operator[](const label);

        //- Return subscript-checked row of constant Matrix.
        inline const Type* operator[](const label) const;

        //- (i, j) const element access operator
        inline const Type& operator()(const label i, const label j) const;

        //- (i, j) element access operator
        inline Type& operator()(const label i, const label j);

        //- Assignment operator. Takes linear time.
        void operator=(const mType&);

        //- Assignment to a block of another matrix
        template<class MatrixType>
        void operator=(const ConstMatrixBlock<MatrixType>&);

        //- Assignment to a block of another matrix
        template<class MatrixType>
        void operator=(const MatrixBlock<MatrixType>&);

        //- Assignment of all elements to zero
        void operator=(const zero);

        //- Assignment of all elements to the given value
        void operator=(const Type&);


    // IOstream operators

        //- Read Matrix from Istream, discarding contents of existing Matrix.
        friend Istream& operator>> <Form, Type>
        (
            Istream&,
            mType&
        );

        //- Write Matrix to Ostream.
        friend Ostream& operator<< <Form, Type>
        (
            Ostream&,
            const mType&
        );
};


// Global functions and operators

template<class Form, class Type>
const Type& max(const Matrix<Form, Type>&);

template<class Form, class Type>
const Type& min(const Matrix<Form, Type>&);

template<class Form, class Type>
Form operator-(const Matrix<Form, Type>&);

template<class Form, class Type>
Form operator+
(
    const Matrix<Form, Type>&,
    const Matrix<Form, Type>&
);

template<class Form, class Type>
Form operator-
(
    const Matrix<Form, Type>&,
    const Matrix<Form, Type>&
);

template<class Form, class Type>
Form operator*
(
    const scalar,
    const Matrix<Form, Type>&
);

template<class Form, class Type>
Form operator*
(
    const Matrix<Form, Type>&,
    const scalar
);

template<class Form, class Type>
Form operator/
(
    const Matrix<Form, Type>&,
    const scalar
);

template<class Form1, class Form2, class Type>
typename typeOfInnerProduct<Type, Form1, Form2>::type
operator*
(
    const Matrix<Form1, Type>& a,
    const Matrix<Form2, Type>& b
);

template<class Form, class Type>
tmp<Field<Type>> operator*
(
    const Matrix<Form, Type>&,
    const Field<Type>&
);


} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Form, class Type>
inline CML::Matrix<Form, Type>::Matrix()
:
    mRows_(0),
    nCols_(0),
    v_(nullptr)
{}


template<class Form, class Type>
inline CML::autoPtr<CML::Matrix<Form, Type> > CML::Matrix<Form, Type>::
clone() const
{
    return autoPtr<Matrix<Form, Type> >(new Matrix<Form, Type>(*this));
}

template<class Form, class Type>
template<class MatrixType>
inline CML::Matrix<Form, Type>::Matrix
(
    const ConstMatrixBlock<MatrixType>& Mb
)
:
    mRows_(Mb.m()),
    nCols_(Mb.n())
{
    allocate();

    for (label i=0; i<mRows_; i++)
    {
        for (label j=0; j<nCols_; j++)
        {
            (*this)(i,j) = Mb(i,j);
        }
    }
}


template<class Form, class Type>
template<class MatrixType>
inline CML::Matrix<Form, Type>::Matrix
(
    const MatrixBlock<MatrixType>& Mb
)
:
    mRows_(Mb.m()),
    nCols_(Mb.n())
{
    allocate();

    for (label i=0; i<mRows_; i++)
    {
        for (label j=0; j<nCols_; j++)
        {
            (*this)(i,j) = Mb(i,j);
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Form, class Type>
inline const CML::Matrix<Form, Type>& CML::Matrix<Form, Type>::null()
{
    return NullSingletonRef<Matrix<Form, Type> >();
}


template<class Form, class Type>
inline CML::label CML::Matrix<Form, Type>::m() const
{
    return mRows_;
}


template<class Form, class Type>
inline CML::label CML::Matrix<Form, Type>::n() const
{
    return nCols_;
}


template<class Form, class Type>
inline CML::label CML::Matrix<Form, Type>::size() const
{
    return mRows_*nCols_;
}


template<class Form, class Type>
inline void CML::Matrix<Form, Type>::checki(const label i) const
{
    #ifdef FULLDEBUG
    if (!mRows_ || !nCols_)
    {
        FatalErrorInFunction
            << "Attempt to access element from empty matrix"
            << abort(FatalError);
    }
    else if (i<0 || i>=mRows_)
    {
        FatalErrorInFunction
            << "Index " << i << " out of range 0 ... " << mRows_-1
            << abort(FatalError);
    }
    #endif
}


template<class Form, class Type>
inline void CML::Matrix<Form, Type>::checkj(const label j) const
{
    #ifdef FULLDEBUG
    if (!mRows_ || !nCols_)
    {
        FatalErrorInFunction
            << "Attempt to access element from empty matrix"
            << abort(FatalError);
    }
    else if (j<0 || j>=nCols_)
    {
        FatalErrorInFunction
            << "index " << j << " out of range 0 ... " << nCols_-1
            << abort(FatalError);
    }
    #endif
}


template<class Form, class Type>
inline const Type* CML::Matrix<Form, Type>::v() const
{
    return v_;
}


template<class Form, class Type>
inline Type* CML::Matrix<Form, Type>::v()
{
    return v_;
}


template<class Form, class Type>
inline CML::ConstMatrixBlock<CML::Matrix<Form, Type> >
CML::Matrix<Form, Type>::block
(
    const label m,
    const label n,
    const label mStart,
    const label nStart
) const
{
    return ConstMatrixBlock<mType>
    (
        *this,
        m,
        n,
        mStart,
        nStart
    );
}


template<class Form, class Type>
template<class VectorSpace>
inline CML::ConstMatrixBlock<CML::Matrix<Form, Type> >
CML::Matrix<Form, Type>::block
(
    const label mStart,
    const label nStart
) const
{
    return ConstMatrixBlock<mType>
    (
        *this,
        VectorSpace::mRows,
        VectorSpace::nCols,
        mStart,
        nStart
    );
}


template<class Form, class Type>
inline CML::ConstMatrixBlock<CML::Matrix<Form, Type> >
CML::Matrix<Form, Type>::col
(
    const label m,
    const label mStart
) const
{
    return ConstMatrixBlock<mType>
    (
        *this,
        m,
        1,
        mStart,
        0
    );
}


template<class Form, class Type>
inline CML::ConstMatrixBlock<CML::Matrix<Form, Type> >
CML::Matrix<Form, Type>::col
(
    const label m,
    const label mStart,
    const label nStart
) const
{
    return ConstMatrixBlock<mType>
    (
        *this,
        m,
        1,
        mStart,
        nStart
    );
}


template<class Form, class Type>
inline CML::MatrixBlock<CML::Matrix<Form, Type> >
CML::Matrix<Form, Type>::block
(
    const label m,
    const label n,
    const label mStart,
    const label nStart
)
{
    return MatrixBlock<mType>
    (
        *this,
        m,
        n,
        mStart,
        nStart
    );
}


template<class Form, class Type>
template<class VectorSpace>
inline CML::MatrixBlock<CML::Matrix<Form, Type> >
CML::Matrix<Form, Type>::block(const label mStart, const label nStart)
{
    return MatrixBlock<mType>
    (
        *this,
        VectorSpace::mRows,
        VectorSpace::nCols,
        mStart,
        nStart
    );
}


template<class Form, class Type>
inline CML::MatrixBlock<CML::Matrix<Form, Type> >
CML::Matrix<Form, Type>::col(const label m, const label mStart)
{
    return MatrixBlock<mType>
    (
        *this,
        m,
        1,
        mStart,
        0
    );
}


template<class Form, class Type>
inline CML::MatrixBlock<CML::Matrix<Form, Type> >
CML::Matrix<Form, Type>::col
(
    const label m,
    const label mStart,
    const label nStart
)
{
    return MatrixBlock<mType>
    (
        *this,
        m,
        1,
        mStart,
        nStart
    );
}


template<class Form, class Type>
void CML::Matrix<Form, Type>::shallowResize(const label m, const label n)
{
    mRows_ = m;
    nCols_ = n;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Form, class Type>
inline const Type& CML::Matrix<Form, Type>::operator()
(
    const label i,
    const label j
) const
{
    checki(i);
    checkj(j);
    return v_[i*nCols_ + j];
}


template<class Form, class Type>
inline Type& CML::Matrix<Form, Type>::operator()
(
    const label i,
    const label j
)
{
    checki(i);
    checkj(j);
    return v_[i*nCols_ + j];
}


template<class Form, class Type>
inline const Type* CML::Matrix<Form, Type>::operator[](const label i) const
{
    checki(i);
    return v_ + i*nCols_;
}


template<class Form, class Type>
inline Type* CML::Matrix<Form, Type>::operator[](const label i)
{
    checki(i);
    return v_ + i*nCols_;
}


// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

template<class Form, class Type>
void CML::Matrix<Form, Type>::allocate()
{
    if (mRows_ && nCols_)
    {
        v_ = new Type[size()];
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Form, class Type>
CML::Matrix<Form, Type>::Matrix(const label m, const label n)
:
    mRows_(m),
    nCols_(n),
    v_(nullptr)
{
    if (mRows_ < 0 || nCols_ < 0)
    {
        FatalErrorInFunction
            << "Incorrect m, n " << mRows_ << ", " << nCols_
            << abort(FatalError);
    }

    allocate();
}


template<class Form, class Type>
CML::Matrix<Form, Type>::Matrix(const label m, const label n, const zero)
:
    mRows_(m),
    nCols_(n),
    v_(nullptr)
{
    if (mRows_ < 0 || nCols_ < 0)
    {
        FatalErrorInFunction
            << "Incorrect m, n " << mRows_ << ", " << nCols_
            << abort(FatalError);
    }

    allocate();

    if (v_)
    {
        const label mn = size();
        for (label i=0; i<mn; i++)
        {
            v_[i] = Zero;
        }
    }
}


template<class Form, class Type>
CML::Matrix<Form, Type>::Matrix(const label m, const label n, const Type& s)
:
    mRows_(m),
    nCols_(n),
    v_(nullptr)
{
    if (mRows_ < 0 || nCols_ < 0)
    {
        FatalErrorInFunction
            << "Incorrect m, n " << mRows_ << ", " << nCols_
            << abort(FatalError);
    }

    allocate();

    if (v_)
    {
        const label mn = size();
        for (label i=0; i<mn; i++)
        {
            v_[i] = s;
        }
    }
}


template<class Form, class Type>
CML::Matrix<Form, Type>::Matrix(const Matrix<Form, Type>& M)
:
    mRows_(M.mRows_),
    nCols_(M.nCols_),
    v_(nullptr)
{
    if (M.v_)
    {
        allocate();

        const label mn = size();
        for (label i=0; i<mn; i++)
        {
            v_[i] = M.v_[i];
        }
    }
}


template<class Form, class Type>
template<class Form2>
CML::Matrix<Form, Type>::Matrix(const Matrix<Form2, Type>& M)
:
    mRows_(M.m()),
    nCols_(M.n()),
    v_(nullptr)
{
    if (M.v())
    {
        allocate();

        const label mn = size();
        for (label i=0; i<mn; i++)
        {
            v_[i] = M.v()[i];
        }
    }
}


// * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * * //

template<class Form, class Type>
CML::Matrix<Form, Type>::~Matrix()
{
    if (v_)
    {
        delete[] v_;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Form, class Type>
void CML::Matrix<Form, Type>::clear()
{
    if (v_)
    {
        delete[] v_;
        v_ = nullptr;
    }

    mRows_ = 0;
    nCols_ = 0;
}


template<class Form, class Type>
void CML::Matrix<Form, Type>::transfer(Matrix<Form, Type>& M)
{
    clear();

    mRows_ = M.mRows_;
    M.mRows_ = 0;

    nCols_ = M.nCols_;
    M.nCols_ = 0;

    v_ = M.v_;
    M.v_ = nullptr;
}


template<class Form, class Type>
void CML::Matrix<Form, Type>::setSize(const label m, const label n)
{
    mType newMatrix(m, n, Zero);

    label minM = min(m, mRows_);
    label minN = min(n, nCols_);

    for (label i=0; i<minM; i++)
    {
        for (label j=0; j<minN; j++)
        {
            newMatrix(i, j) = (*this)(i, j);
        }
    }

    transfer(newMatrix);
}


template<class Form, class Type>
Form CML::Matrix<Form, Type>::T() const
{
    const Matrix<Form, Type>& A = *this;
    Form At(n(), m());

    for (label i=0; i<m(); i++)
    {
        for (label j=0; j<n(); j++)
        {
            At(j, i) = A(i, j);
        }
    }

    return At;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Form, class Type>
void CML::Matrix<Form, Type>::operator=(const Matrix<Form, Type>& M)
{
    if (this == &M)
    {
        FatalErrorInFunction
            << "Attempted assignment to self"
            << abort(FatalError);
    }

    if (mRows_ != M.mRows_ || nCols_ != M.nCols_)
    {
        clear();
        mRows_ = M.mRows_;
        nCols_ = M.nCols_;
        allocate();
    }

    if (v_)
    {
        const label mn = size();
        for (label i=0; i<mn; i++)
        {
            v_[i] = M.v_[i];
        }
    }
}


template<class Form, class Type>
template<class MatrixType>
void CML::Matrix<Form, Type>::operator=
(
    const ConstMatrixBlock<MatrixType>& Mb
)
{
    for (label i=0; i<mRows_; i++)
    {
        for (label j=0; j<nCols_; j++)
        {
            (*this)(i,j) = Mb(i,j);
        }
    }
}


template<class Form, class Type>
template<class MatrixType>
void CML::Matrix<Form, Type>::operator=
(
    const MatrixBlock<MatrixType>& Mb
)
{
    for (label i=0; i<mRows_; i++)
    {
        for (label j=0; j<nCols_; j++)
        {
            (*this)(i,j) = Mb(i,j);
        }
    }
}


template<class Form, class Type>
void CML::Matrix<Form, Type>::operator=(const Type& s)
{
    if (v_)
    {
        const label mn = size();
        for (label i=0; i<mn; i++)
        {
            v_[i] = s;
        }
    }
}


template<class Form, class Type>
void CML::Matrix<Form, Type>::operator=(const zero)
{
    if (v_)
    {
        const label mn = size();
        for (label i=0; i<mn; i++)
        {
            v_[i] = Zero;
        }
    }
}


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

template<class Form, class Type>
const Type& CML::max(const Matrix<Form, Type>& M)
{
    const label mn = M.size();

    if (mn)
    {
        label curMaxI = 0;
        const Type* Mv = M.v();

        for (label i=1; i<mn; i++)
        {
            if (Mv[i] > Mv[curMaxI])
            {
                curMaxI = i;
            }
        }

        return Mv[curMaxI];
    }
    else
    {
        FatalErrorInFunction
            << "Matrix is empty"
            << abort(FatalError);

        // Return in error to keep compiler happy
        return M(0, 0);
    }
}


template<class Form, class Type>
const Type& CML::min(const Matrix<Form, Type>& M)
{
    const label mn = M.size();

    if (mn)
    {
        label curMinI = 0;
        const Type* Mv = M.v();

        for (label i=1; i<mn; i++)
        {
            if (Mv[i] < Mv[curMinI])
            {
                curMinI = i;
            }
        }

        return Mv[curMinI];
    }
    else
    {
        FatalErrorInFunction
            << "Matrix is empty"
            << abort(FatalError);

        // Return in error to keep compiler happy
        return M(0, 0);
    }
}


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

template<class Form, class Type>
Form CML::operator-(const Matrix<Form, Type>& M)
{
    Form nM(M.m(), M.n());

    if (M.m() && M.n())
    {
        Type* nMv = nM.v();
        const Type* Mv = M.v();

        const label mn = M.size();
        for (label i=0; i<mn; i++)
        {
            nMv[i] = -Mv[i];
        }
    }

    return nM;
}


template<class Form, class Type>
Form CML::operator+(const Matrix<Form, Type>& A, const Matrix<Form, Type>& B)
{
    if (A.m() != B.m())
    {
        FatalErrorInFunction
            << "Attempt to add matrices with different numbers of rows: "
            << A.m() << ", " << B.m()
            << abort(FatalError);
    }

    if (A.n() != B.n())
    {
        FatalErrorInFunction
            << "Attempt to add matrices with different numbers of columns: "
            << A.n() << ", " << B.n()
            << abort(FatalError);
    }

    Form AB(A.m(), A.n());

    Type* ABv = AB.v();
    const Type* Av = A.v();
    const Type* Bv = B.v();

    const label mn = A.size();
    for (label i=0; i<mn; i++)
    {
        ABv[i] = Av[i] + Bv[i];
    }

    return AB;
}


template<class Form, class Type>
Form CML::operator-(const Matrix<Form, Type>& A, const Matrix<Form, Type>& B)
{
    if (A.m() != B.m())
    {
        FatalErrorInFunction
            << "Attempt to add matrices with different numbers of rows: "
            << A.m() << ", " << B.m()
            << abort(FatalError);
    }

    if (A.n() != B.n())
    {
        FatalErrorInFunction
            << "Attempt to add matrices with different numbers of columns: "
            << A.n() << ", " << B.n()
            << abort(FatalError);
    }

    Form AB(A.m(), A.n());

    Type* ABv = AB.v();
    const Type* Av = A.v();
    const Type* Bv = B.v();

    const label mn = A.size();
    for (label i=0; i<mn; i++)
    {
        ABv[i] = Av[i] - Bv[i];
    }

    return AB;
}


template<class Form, class Type>
Form CML::operator*(const scalar s, const Matrix<Form, Type>& M)
{
    Form sM(M.m(), M.n());

    if (M.m() && M.n())
    {
        Type* sMv = sM.v();
        const Type* Mv = M.v();

        const label mn = M.size();
        for (label i=0; i<mn; i++)
        {
            sMv[i] = s*Mv[i];
        }
    }

    return sM;
}


template<class Form, class Type>
Form CML::operator*(const Matrix<Form, Type>& M, const scalar s)
{
    Form sM(M.m(), M.n());

    if (M.m() && M.n())
    {
        Type* sMv = sM.v();
        const Type* Mv = M.v();

        const label mn = M.size();
        for (label i=0; i<mn; i++)
        {
            sMv[i] = Mv[i]*s;
        }
    }

    return sM;
}


template<class Form, class Type>
Form CML::operator/(const Matrix<Form, Type>& M, const scalar s)
{
    Form sM(M.m(), M.n());

    if (M.m() && M.n())
    {
        Type* sMv = sM.v();
        const Type* Mv = M.v();

        const label mn = M.size();
        for (label i=0; i<mn; i++)
        {
            sMv[i] = Mv[i]/s;
        }
    }

    return sM;
}


template<class Form1, class Form2, class Type>
typename CML::typeOfInnerProduct<Type, Form1, Form2>::type
CML::operator*
(
    const Matrix<Form1, Type>& A,
    const Matrix<Form2, Type>& B
)
{
    if (A.n() != B.m())
    {
        FatalErrorInFunction
            << "Attempt to multiply incompatible matrices:" << nl
            << "Matrix A : " << A.m() << " x " << A.n() << nl
            << "Matrix B : " << B.m() << " x " << B.n() << nl
            << "In order to multiply matrices, columns of A must equal "
            << "rows of B"
            << abort(FatalError);
    }

    typename typeOfInnerProduct<Type, Form1, Form2>::type AB
    (
        A.m(),
        B.n(),
        Zero
    );

    for (label i=0; i<AB.m(); i++)
    {
        for (label j=0; j<AB.n(); j++)
        {
            for (label k=0; k<B.m(); k++)
            {
                AB(i, j) += A(i, k)*B(k, j);
            }
        }
    }

    return AB;
}


template<class Form, class Type>
inline CML::tmp<CML::Field<Type> > CML::operator*
(
    const Matrix<Form, Type>& M,
    const Field<Type>& f
)
{
    if (M.n() != f.size())
    {
        FatalErrorInFunction
            << "Attempt to multiply incompatible matrix and field:" << nl
            << "Matrix : " << M.m() << " x " << M.n() << nl
            << "Field : " << f.size() << " rows" << nl
            << "In order to multiply a matrix M and field f, "
               "columns of M must equal rows of f"
            << abort(FatalError);
    }

    tmp<Field<Type> > tMf(new Field<Type>(M.m(), Zero));
    Field<Type>& Mf = tMf.ref();

    for (label i=0; i<M.m(); i++)
    {
        for (label j=0; j<M.n(); j++)
        {
            Mf[i] += M(i, j)*f[j];
        }
    }

    return tMf;
}
// * * * * * * * * * * * * * * * *  IOStream operators * * * * * * * * * * * //

#include "Istream.hpp"
#include "Ostream.hpp"
#include "token.hpp"
#include "contiguous.hpp"

// * * * * * * * * * * * * * * * Ostream Operator *  * * * * * * * * * * * * //

template<class Form, class Type>
CML::Matrix<Form, Type>::Matrix(Istream& is)
:
    mRows_(0),
    nCols_(0),
    v_(nullptr)
{
    operator>>(is, *this);
}


template<class Form, class Type>
CML::Istream& CML::operator>>(Istream& is, Matrix<Form, Type>& M)
{
    // Anull matrix
    M.clear();

    is.fatalCheck("operator>>(Istream&, Matrix<Form, Type>&)");

    token firstToken(is);

    is.fatalCheck
    (
        "operator>>(Istream&, Matrix<Form, Type>&) : reading first token"
    );

    if (firstToken.isLabel())
    {
        M.mRows_ = firstToken.labelToken();
        M.nCols_ = readLabel(is);

        label mn = M.mRows_*M.nCols_;

        // Read list contents depending on data format
        if (is.format() == IOstream::ASCII || !contiguous<Type>())
        {
            // Read beginning of contents
            char listDelimiter = is.readBeginList("Matrix");

            if (mn)
            {
                M.allocate();
                Type* v = M.v_;

                if (listDelimiter == token::BEGIN_LIST)
                {
                    label k = 0;

                    // loop over rows
                    for (label i=0; i<M.m(); i++)
                    {
                        listDelimiter = is.readBeginList("MatrixRow");

                        for (label j=0; j<M.n(); j++)
                        {
                            is >> v[k++];

                            is.fatalCheck
                            (
                                "operator>>(Istream&, Matrix<Form, Type>&) : "
                                "reading entry"
                            );
                        }

                        is.readEndList("MatrixRow");
                    }
                }
                else
                {
                    Type element;
                    is >> element;

                    is.fatalCheck
                    (
                        "operator>>(Istream&, Matrix<Form, Type>&) : "
                        "reading the single entry"
                    );

                    for (label i=0; i<mn; i++)
                    {
                        v[i] = element;
                    }
                }
            }

            // Read end of contents
            is.readEndList("Matrix");
        }
        else
        {
            if (mn)
            {
                M.allocate();
                Type* v = M.v_;

                is.read(reinterpret_cast<char*>(v), mn*sizeof(Type));

                is.fatalCheck
                (
                    "operator>>(Istream&, Matrix<Form, Type>&) : "
                    "reading the binary block"
                );
            }
        }
    }
    else
    {
        FatalIOErrorInFunction(is)
            << "incorrect first token, expected <int>, found "
            << firstToken.info()
            << exit(FatalIOError);
    }

    return is;
}


template<class Form, class Type>
CML::Ostream& CML::operator<<(Ostream& os, const Matrix<Form, Type>& M)
{
    label mn = M.mRows_*M.nCols_;

    os  << M.m() << token::SPACE << M.n();

    // Write list contents depending on data format
    if (os.format() == IOstream::ASCII || !contiguous<Type>())
    {
        if (mn)
        {
            bool uniform = false;

            const Type* v = M.v_;

            if (mn > 1 && contiguous<Type>())
            {
                uniform = true;

                for (label i=0; i<mn; i++)
                {
                    if (v[i] != v[0])
                    {
                        uniform = false;
                        break;
                    }
                }
            }

            if (uniform)
            {
                // Write size of list and start contents delimiter
                os  << token::BEGIN_BLOCK;

                // Write list contents
                os << v[0];

                // Write end of contents delimiter
                os << token::END_BLOCK;
            }
            else if (mn < 10 && contiguous<Type>())
            {
                // Write size of list and start contents delimiter
                os  << token::BEGIN_LIST;

                label k = 0;

                // loop over rows
                for (label i=0; i<M.m(); i++)
                {
                    os  << token::BEGIN_LIST;

                    // Write row
                    for (label j=0; j< M.n(); j++)
                    {
                        if (j > 0) os << token::SPACE;
                        os << v[k++];
                    }

                    os << token::END_LIST;
                }

                // Write end of contents delimiter
                os << token::END_LIST;
            }
            else
            {
                // Write size of list and start contents delimiter
                os  << nl << token::BEGIN_LIST;

                label k = 0;

                // loop over rows
                for (label i=0; i<M.m(); i++)
                {
                    os  << nl << token::BEGIN_LIST;

                    // Write row
                    for (label j=0; j< M.n(); j++)
                    {
                        os << nl << v[k++];
                    }

                    os << nl << token::END_LIST;
                }

                // Write end of contents delimiter
                os << nl << token::END_LIST << nl;
            }
        }
        else
        {
            os  << token::BEGIN_LIST << token::END_LIST << nl;
        }
    }
    else
    {
        if (mn)
        {
            os.write(reinterpret_cast<const char*>(M.v_), mn*sizeof(Type));
        }
    }

    // Check state of IOstream
    os.check("Ostream& operator<<(Ostream&, const Matrix&)");

    return os;
}


#endif

// ************************************************************************* //
