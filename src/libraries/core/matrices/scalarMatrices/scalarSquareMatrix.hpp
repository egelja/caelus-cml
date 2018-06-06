/*---------------------------------------------------------------------------*\
Copyright (C) 2015 Applied CCM
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    CAELUS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::scalarSquareMatrix

Description
    CML::scalarSquareMatrix

SourceFiles
    scalarSquareMatrix.cpp

\*---------------------------------------------------------------------------*/

#ifndef scalarSquareMatrix_H
#define scalarSquareMatrix_H

#include "SquareMatrix.hpp"
#include "scalarField.hpp"
#include "labelList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class scalarSquareMatrix Declaration
\*---------------------------------------------------------------------------*/

class scalarSquareMatrix
:
    public SquareMatrix<scalar>
{

public:

    // Constructors

        //- Construct null
        scalarSquareMatrix();

        //- Construct given size
        scalarSquareMatrix(const label);

        //- Construct given size and coefficient value
        scalarSquareMatrix(const label, const scalar v);

        //- Construct as copy
        scalarSquareMatrix(const scalarSquareMatrix&);

        //- Construct from Istream
        scalarSquareMatrix(Istream&);


    // Member Functions

    
      //- Solve the matrix using Gaussian elimination with pivoting,
      //  returning the solution in the source
      template<class Type>
      static void solve(scalarSquareMatrix& matrix, Field<Type>& source);
    
      //- Solve the matrix using Gaussian elimination with pivoting
      //  and return the solution
      template<class Type>
      void solve
      (
          Field<Type>& psi,
          const scalarSquareMatrix& matrix,
          const Field<Type>& source
      );
    
      //- LU decompose the matrix with pivoting
      static void LUDecompose
      (
          scalarSquareMatrix& matrix,
          labelList& pivotIndices
      );
    
      //- LU back-substitution with given source, returning the solution
      //  in the source
      template<class Type>
      static void LUBacksubstitute
      (
          const scalarSquareMatrix& luMmatrix,
          const labelList& pivotIndices,
          Field<Type>& source
      );

      //- Invert the matrix using LU decomposition with pivoting
      scalarSquareMatrix LUinvert() const;

      //- Solve the matrix using LU decomposition with pivoting
      //  returning the LU form of the matrix and the solution in the source
      template<class Type>
      static void LUsolve(scalarSquareMatrix& matrix, Field<Type>& source);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

#include "Swap.hpp"
#include "restrict.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::scalarSquareMatrix::solve
(
 scalarSquareMatrix& tmpMatrix,
 Field<Type>& sourceSol
 )
{
    label n = tmpMatrix.n();
    
    // Elimination
    for (register label i=0; i<n; i++)
    {
        label iMax = i;
        scalar largestCoeff = mag(tmpMatrix[iMax][i]);
        
        // Swap entries around to find a good pivot
        for (register label j=i+1; j<n; j++)
        {
            if (mag(tmpMatrix[j][i]) > largestCoeff)
            {
                iMax = j;
                largestCoeff = mag(tmpMatrix[iMax][i]);
            }
        }
        
        if (i != iMax)
        {
            //Info<< "Pivoted on " << i << " " << iMax << endl;
            
            for (register label k=i; k<n; k++)
            {
                Swap(tmpMatrix[i][k], tmpMatrix[iMax][k]);
            }
            Swap(sourceSol[i], sourceSol[iMax]);
        }
        
        // Check that the system of equations isn't singular
        if (mag(tmpMatrix[i][i]) < 1e-20)
        {
            FatalErrorIn("solve(scalarSquareMatrix&, Field<Type>& sourceSol)")
            << "Singular Matrix"
            << exit(FatalError);
        }
        
        // Reduce to upper triangular form
        for (register label j=i+1; j<n; j++)
        {
            sourceSol[j] -= sourceSol[i]*(tmpMatrix[j][i]/tmpMatrix[i][i]);
            
            for (register label k=n-1; k>=i; k--)
            {
                tmpMatrix[j][k] -=
                tmpMatrix[i][k]*tmpMatrix[j][i]/tmpMatrix[i][i];
            }
        }
    }
    
    // Back-substitution
    for (register label j=n-1; j>=0; j--)
    {
        Type ntempvec = pTraits<Type>::zero;
        
        for (register label k=j+1; k<n; k++)
        {
            ntempvec += tmpMatrix[j][k]*sourceSol[k];
        }
        
        sourceSol[j] = (sourceSol[j] - ntempvec)/tmpMatrix[j][j];
    }
}


template<class Type>
void CML::scalarSquareMatrix::solve
(
 Field<Type>& psi,
 const scalarSquareMatrix& matrix,
 const Field<Type>& source
 )
{
    scalarSquareMatrix tmpMatrix = matrix;
    psi = source;
    solve(tmpMatrix, psi);
}


template<class Type>
void CML::scalarSquareMatrix::LUBacksubstitute
(
 const scalarSquareMatrix& luMatrix,
 const labelList& pivotIndices,
 Field<Type>& sourceSol
 )
{
    label n = luMatrix.n();
    
    label ii = 0;
    
    for (register label i=0; i<n; i++)
    {
        label ip = pivotIndices[i];
        Type sum = sourceSol[ip];
        sourceSol[ip] = sourceSol[i];
        const scalar* RESTRICT luMatrixi = luMatrix[i];
        
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
    
    for (register label i=n-1; i>=0; i--)
    {
        Type sum = sourceSol[i];
        const scalar* RESTRICT luMatrixi = luMatrix[i];
        
        for (register label j=i+1; j<n; j++)
        {
            sum -= luMatrixi[j]*sourceSol[j];
        }
        
        sourceSol[i] = sum/luMatrixi[i];
    }
}


template<class Type>
void CML::scalarSquareMatrix::LUsolve
(
 scalarSquareMatrix& matrix,
 Field<Type>& sourceSol
 )
{
    labelList pivotIndices(matrix.n());
    LUDecompose(matrix, pivotIndices);
    LUBacksubstitute(matrix, pivotIndices, sourceSol);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //