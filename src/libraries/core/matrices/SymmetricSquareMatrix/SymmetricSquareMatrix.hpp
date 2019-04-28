/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::SymmetricSquareMatrix

Description
    A templated 2D square symmetric matrix of objects of \<T\>, where the
    n x n matrix dimension is known and used for subscript bounds checking, etc.


\*---------------------------------------------------------------------------*/

#ifndef SymmetricSquareMatrix_HPP
#define SymmetricSquareMatrix_HPP

#include "SquareMatrix.hpp"
#include "Identity.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                   Class SymmetricSquareMatrix Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class SymmetricSquareMatrix
:
    public Matrix<SymmetricSquareMatrix<Type>, Type>
{

public:

    //- Null constructor.
    inline SymmetricSquareMatrix()
    :
        Matrix<SymmetricSquareMatrix<Type>, Type>()
    {}


    //- Construct given number of rows/columns.
    inline SymmetricSquareMatrix(const label n)
    :
        Matrix<SymmetricSquareMatrix<Type>, Type>(n, n)
    {}

    //- Construct given number of rows/columns, initializing to zero
    inline SymmetricSquareMatrix(const label n, const zero)
    :
        Matrix<SymmetricSquareMatrix<Type>, Type>(n, n, Zero)
    {}

    //- Construct given number of rows/columns,
    inline SymmetricSquareMatrix(const label n, const Identity<Type>)
    :
        Matrix<SymmetricSquareMatrix<Type>, Type>(n, n, Zero)
    {
        for (label i=0; i<n; i++)
        {
            this->operator()(i, i) = pTraits<Type>::one;
        }
    }

    //- Construct with given number of rows/columns
    //  initializing all elements to the given value
    inline SymmetricSquareMatrix(const label n, const Type& t)
    :
        Matrix<SymmetricSquareMatrix<Type>, Type>(n, n, t)
    {}

    //- Construct from Istream.
    inline SymmetricSquareMatrix(Istream& is)
    :
        Matrix<SymmetricSquareMatrix<Type>, Type>(is)
    {}

    //- Clone
    inline autoPtr<SymmetricSquareMatrix<Type> > clone() const
    {
        return autoPtr<SymmetricSquareMatrix<Type> >
        (
            new SymmetricSquareMatrix<Type>(*this)
        );
    }

};


// Global functions

//- Return the LU decomposed SymmetricSquareMatrix inverse
template<class Type>
SymmetricSquareMatrix<Type> invDecomposed(const SymmetricSquareMatrix<Type>&);

//- Return the SymmetricSquareMatrix inverse
template<class Type>
SymmetricSquareMatrix<Type> inv(const SymmetricSquareMatrix<Type>&);

//- Return the LU decomposed SymmetricSquareMatrix det
template<class Type>
Type detDecomposed(const SymmetricSquareMatrix<Type>&);

//- Return the SymmetricSquareMatrix det
template<class Type>
Type det(const SymmetricSquareMatrix<Type>&);


} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::SymmetricSquareMatrix<Type> CML::invDecomposed
(
    const SymmetricSquareMatrix<Type>& matrix
)
{
    const label n = matrix.n();

    SymmetricSquareMatrix<Type> inv(n, Zero);

    for (label i=0; i<n; i++)
    {
        inv(i, i) = 1.0/matrix(i, i);

        for (label j=0; j<i; j++)
        {
            Type sum = Zero;

            for (label k=j; k<i; k++)
            {
                sum -= matrix(i, k)*inv(k, j);
            }

            inv(i, j) = sum/matrix(i, i);
        }
    }

    SymmetricSquareMatrix<Type> result(n, Zero);

    for (label k=0; k<n; k++)
    {
        for (label i=0; i <= k; i++)
        {
            for (label j=0; j <= k; j++)
            {
                result(i, j) += inv(k, i)*inv(k, j);
            }
        }
    }

    return result;
}


template<class Type>
CML::SymmetricSquareMatrix<Type> CML::inv
(
    const SymmetricSquareMatrix<Type>& matrix
)
{
    SymmetricSquareMatrix<Type> matrixTmp(matrix);
    LUDecompose(matrixTmp);

    return invDecomposed(matrixTmp);
}


template<class Type>
Type CML::detDecomposed(const SymmetricSquareMatrix<Type>& matrix)
{
    Type diagProduct = pTraits<Type>::one;

    for (label i=0; i<matrix.m(); i++)
    {
        diagProduct *= matrix(i, i);
    }

    return sqr(diagProduct);
}


template<class Type>
Type CML::det(const SymmetricSquareMatrix<Type>& matrix)
{
    SymmetricSquareMatrix<Type> matrixTmp(matrix);
    LUDecompose(matrixTmp);

    return detDecomposed(matrixTmp);
}


#endif
