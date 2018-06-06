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

\*---------------------------------------------------------------------------*/

#include "scalarSquareMatrix.hpp"
#include "restrict.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::scalarSquareMatrix::scalarSquareMatrix()
{}


CML::scalarSquareMatrix::scalarSquareMatrix(const label mSize)
:
    SquareMatrix<scalar>(mSize, 0.0)
{}


CML::scalarSquareMatrix::scalarSquareMatrix
(
    const label mSize,
    const scalar v
)
:
    SquareMatrix<scalar>(mSize, v)
{}


CML::scalarSquareMatrix::scalarSquareMatrix(const scalarSquareMatrix& matrix)
:
    SquareMatrix<scalar>(matrix)
{}


CML::scalarSquareMatrix::scalarSquareMatrix(Istream& is)
:
    SquareMatrix<scalar>(is)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::scalarSquareMatrix::LUDecompose
(
 scalarSquareMatrix& matrix,
 labelList& pivotIndices
 )
{
    label n = matrix.n();
    scalar* vv = new scalar [n];
    
    for (register label i=0; i<n; i++)
    {
        scalar largestCoeff = 0.0;
        scalar temp;
        const scalar* RESTRICT matrixi = matrix[i];
        
        for (register label j=0; j<n; j++)
        {
            if ((temp = mag(matrixi[j])) > largestCoeff)
            {
                largestCoeff = temp;
            }
        }
        
        if (largestCoeff == 0.0)
        {
            FatalErrorIn
            (
             "LUdecompose"
             "(scalarSquareMatrix& matrix, labelList& rowIndices)"
             )   << "Singular matrix" << exit(FatalError);
        }
        
        vv[i] = 1.0/largestCoeff;
    }
    
    for (register label j=0; j<n; j++)
    {
        scalar* RESTRICT matrixj = matrix[j];
        
        for (register label i=0; i<j; i++)
        {
            scalar* RESTRICT matrixi = matrix[i];
            
            scalar sum = matrixi[j];
            for (register label k=0; k<i; k++)
            {
                sum -= matrixi[k]*matrix[k][j];
            }
            matrixi[j] = sum;
        }
        
        label iMax = 0;
        
        scalar largestCoeff = 0.0;
        for (register label i=j; i<n; i++)
        {
            scalar* RESTRICT matrixi = matrix[i];
            scalar sum = matrixi[j];
            
            for (register label k=0; k<j; k++)
            {
                sum -= matrixi[k]*matrix[k][j];
            }
            
            matrixi[j] = sum;
            
            scalar temp;
            if ((temp = vv[i]*mag(sum)) >= largestCoeff)
            {
                largestCoeff = temp;
                iMax = i;
            }
        }
        
        pivotIndices[j] = iMax;
        
        if (j != iMax)
        {
            scalar* RESTRICT matrixiMax = matrix[iMax];
            
            for (register label k=0; k<n; k++)
            {
                Swap(matrixj[k], matrixiMax[k]);
            }
            
            vv[iMax] = vv[j];
        }
        
        if (matrixj[j] == 0.0)
        {
            matrixj[j] = SMALL;
        }
        
        if (j != n-1)
        {
            scalar rDiag = 1.0/matrixj[j];
            
            for (register label i=j+1; i<n; i++)
            {
                matrix[i][j] *= rDiag;
            }
        }
    }
    delete [] vv;
}


CML::scalarSquareMatrix CML::scalarSquareMatrix::LUinvert() const
{
    scalarSquareMatrix luMatrix = *this;

    scalarSquareMatrix luInvert(luMatrix.n());
    scalarField column(luMatrix.n());

    labelList pivotIndices(luMatrix.n());

    LUDecompose(luMatrix, pivotIndices);

    for (label j = 0; j < luMatrix.n(); j++)
    {
        for (label i = 0; i < luMatrix.n(); i++)
        {
            column[i] = 0.0;
        }

        column[j] = 1.0;

        LUBacksubstitute(luMatrix, pivotIndices, column);

        for (label i = 0; i < luMatrix.n(); i++)
        {
            luInvert[i][j] = column[i];
        }
    }

    return luInvert;
}


// ************************************************************************* //