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

Class
    scalarMatrices

Description
    Scalar matrices

SourceFiles
    scalarMatrices.cpp

\*---------------------------------------------------------------------------*/

#ifndef scalarMatrices_H
#define scalarMatrices_H

#include "RectangularMatrix.hpp"
#include "SquareMatrix.hpp"
#include "SymmetricSquareMatrix.hpp"
#include "DiagonalMatrix.hpp"
#include "scalarField.hpp"
#include "labelList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

typedef RectangularMatrix<scalar> scalarRectangularMatrix;
typedef SquareMatrix<scalar> scalarSquareMatrix;
typedef SymmetricSquareMatrix<scalar> scalarSymmetricSquareMatrix;
typedef DiagonalMatrix<scalar> scalarDiagonalMatrix;

//- Solve the matrix using Gaussian elimination with pivoting,
//  returning the solution in the source
template<class Type>
void solve(scalarSquareMatrix& matrix, List<Type>& source);

//- Solve the matrix using Gaussian elimination with pivoting
//  and return the solution
template<class Type>
void solve
(
    List<Type>& psi,
    const scalarSquareMatrix& matrix,
    const List<Type>& source
);

//- LU decompose the matrix with pivoting
void LUDecompose
(
    scalarSquareMatrix& matrix,
    labelList& pivotIndices
);

//- LU decompose the matrix with pivoting.
//  sign is -1 for odd number of row interchanges and 1 for even number.
void LUDecompose
(
    scalarSquareMatrix& matrix,
    labelList& pivotIndices,
    label& sign
);

//- LU decompose the matrix into a lower (L) and upper (U) part. U = L.T()
void LUDecompose(scalarSymmetricSquareMatrix& matrix);

//- LU back-substitution with given source, returning the solution
//  in the source
template<class Type>
void LUBacksubstitute
(
    const scalarSquareMatrix& luMmatrix,
    const labelList& pivotIndices,
    List<Type>& source
);

//- LU back-substitution with given source, returning the solution
//  in the source. Specialised for symmetric square matrices that have been
//  decomposed into LU where U = L.T() as pivoting is not required
template<class Type>
void LUBacksubstitute
(
    const scalarSymmetricSquareMatrix& luMmatrix,
    List<Type>& source
);

//- Invert the matrix using LU decomposition with pivoting
scalarSquareMatrix LUinvert(scalarSquareMatrix& luMatrix);

//- Solve the matrix using LU decomposition with pivoting
//  returning the LU form of the matrix and the solution in the source
template<class Type>
void LUsolve(scalarSquareMatrix& matrix, List<Type>& source);

//- Solve the matrix using LU decomposition returning the LU form of the matrix
//  and the solution in the source, where U = L.T()
template<class Type>
void LUsolve(scalarSymmetricSquareMatrix& matrix, List<Type>& source);

template<class Form, class Type>
void multiply
(
    Matrix<Form, Type>& answer,         // value changed in return
    const Matrix<Form, Type>& A,
    const Matrix<Form, Type>& B
);

void multiply
(
    scalarRectangularMatrix& answer,         // value changed in return
    const scalarRectangularMatrix& A,
    const scalarRectangularMatrix& B,
    const scalarRectangularMatrix& C
);

void multiply
(
    scalarRectangularMatrix& answer,         // value changed in return
    const scalarRectangularMatrix& A,
    const DiagonalMatrix<scalar>& B,
    const scalarRectangularMatrix& C
);

void multiply
(
    scalarSquareMatrix& answer,         // value changed in return
    const scalarSquareMatrix& A,
    const DiagonalMatrix<scalar>& B,
    const scalarSquareMatrix& C
);

//- Return the inverse of matrix A using SVD
scalarRectangularMatrix SVDinv
(
    const scalarRectangularMatrix& A,
    scalar minCondition = 0
);


} // End namespace CML


#include "Swap.hpp"
#include "ListOps.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::solve
(
    scalarSquareMatrix& tmpMatrix,
    List<Type>& sourceSol
)
{
    label m = tmpMatrix.m();

    // Elimination
    for (label i=0; i<m; i++)
    {
        label iMax = i;
        scalar largestCoeff = mag(tmpMatrix(iMax, i));

        // Swap elements around to find a good pivot
        for (label j=i+1; j<m; j++)
        {
            if (mag(tmpMatrix(j, i)) > largestCoeff)
            {
                iMax = j;
                largestCoeff = mag(tmpMatrix(iMax, i));
            }
        }

        if (i != iMax)
        {
            for (label k=i; k<m; k++)
            {
                Swap(tmpMatrix(i, k), tmpMatrix(iMax, k));
            }
            Swap(sourceSol[i], sourceSol[iMax]);
        }

        // Check that the system of equations isn't singular
        if (mag(tmpMatrix(i, i)) < 1e-20)
        {
            FatalErrorInFunction
                << "Singular Matrix"
                << exit(FatalError);
        }

        // Reduce to upper triangular form
        for (label j=i+1; j<m; j++)
        {
            sourceSol[j] -= sourceSol[i]*(tmpMatrix(j, i)/tmpMatrix(i, i));

            for (label k=m-1; k>=i; k--)
            {
                tmpMatrix(j, k) -=
                    tmpMatrix(i, k)*tmpMatrix(j, i)/tmpMatrix(i, i);
            }
        }
    }

    // Back-substitution
    for (label j=m-1; j>=0; j--)
    {
        Type ntempvec = Zero;

        for (label k=j+1; k<m; k++)
        {
            ntempvec += tmpMatrix(j, k)*sourceSol[k];
        }

        sourceSol[j] = (sourceSol[j] - ntempvec)/tmpMatrix(j, j);
    }
}


template<class Type>
void CML::solve
(
    List<Type>& psi,
    const scalarSquareMatrix& matrix,
    const List<Type>& source
)
{
    scalarSquareMatrix tmpMatrix = matrix;
    psi = source;
    solve(tmpMatrix, psi);
}


template<class Type>
void CML::LUBacksubstitute
(
    const scalarSquareMatrix& luMatrix,
    const labelList& pivotIndices,
    List<Type>& sourceSol
)
{
    label m = luMatrix.m();

    label ii = 0;

    for (label i=0; i<m; i++)
    {
        label ip = pivotIndices[i];
        Type sum = sourceSol[ip];
        sourceSol[ip] = sourceSol[i];
        const scalar* __restrict__ luMatrixi = luMatrix[i];

        if (ii != 0)
        {
            for (label j=ii-1; j<i; j++)
            {
                sum -= luMatrixi[j]*sourceSol[j];
            }
        }
        else if (sum != pTraits<Type>::zero)
        {
            ii = i+1;
        }

        sourceSol[i] = sum;
    }

    for (label i=m-1; i>=0; i--)
    {
        Type sum = sourceSol[i];
        const scalar* __restrict__ luMatrixi = luMatrix[i];

        for (label j=i+1; j<m; j++)
        {
            sum -= luMatrixi[j]*sourceSol[j];
        }

        sourceSol[i] = sum/luMatrixi[i];
    }
}


template<class Type>
void CML::LUBacksubstitute
(
    const scalarSymmetricSquareMatrix& luMatrix,
    List<Type>& sourceSol
)
{
    label m = luMatrix.m();

    label ii = 0;

    for (label i=0; i<m; i++)
    {
        Type sum = sourceSol[i];
        const scalar* __restrict__ luMatrixi = luMatrix[i];

        if (ii != 0)
        {
            for (label j=ii-1; j<i; j++)
            {
                sum -= luMatrixi[j]*sourceSol[j];
            }
        }
        else if (sum != pTraits<Type>::zero)
        {
            ii = i+1;
        }

        sourceSol[i] = sum/luMatrixi[i];
    }

    for (label i=m-1; i>=0; i--)
    {
        Type sum = sourceSol[i];
        const scalar* __restrict__ luMatrixi = luMatrix[i];

        for (label j=i+1; j<m; j++)
        {
            sum -= luMatrixi[j]*sourceSol[j];
        }

        sourceSol[i] = sum/luMatrixi[i];
    }
}


template<class Type>
void CML::LUsolve
(
    scalarSquareMatrix& matrix,
    List<Type>& sourceSol
)
{
    labelList pivotIndices(matrix.m());
    LUDecompose(matrix, pivotIndices);
    LUBacksubstitute(matrix, pivotIndices, sourceSol);
}


template<class Type>
void CML::LUsolve
(
    scalarSymmetricSquareMatrix& matrix,
    List<Type>& sourceSol
)
{
    LUDecompose(matrix);
    LUBacksubstitute(matrix, sourceSol);
}


template<class Form, class Type>
void CML::multiply
(
    Matrix<Form, Type>& ans,         // value changed in return
    const Matrix<Form, Type>& A,
    const Matrix<Form, Type>& B
)
{
    if (A.n() != B.m())
    {
        FatalErrorInFunction
            << "A and B must have identical inner dimensions but A.n = "
            << A.n() << " and B.m = " << B.m()
            << abort(FatalError);
    }

    ans = Matrix<Form, Type>(A.m(), B.n(), scalar(0));

    for (label i=0; i<A.m(); i++)
    {
        for (label j=0; j<B.n(); j++)
        {
            for (label l=0; l<B.m(); l++)
            {
                ans(i, j) += A(i, l)*B(l, j);
            }
        }
    }
}


#endif
