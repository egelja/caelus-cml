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
    CML::RectangularMatrix

Description
    A templated 2D rectangular m x n matrix of objects of \<Type\>.

    The matrix dimensions are used for subscript bounds checking etc.

SourceFiles
    RectangularMatrixI.hpp
    RectangularMatrix.cpp

\*---------------------------------------------------------------------------*/

#ifndef RectangularMatrix_H
#define RectangularMatrix_H

#include "Matrix.hpp"
#include "SquareMatrix.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class Matrix Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class RectangularMatrix
:
    public Matrix<RectangularMatrix<Type>, Type>
{

public:

    // Constructors

        //- Null constructor.
        inline RectangularMatrix();

        //- Construct given number of rows and columns,
        inline RectangularMatrix(const label m, const label n);

        //- Construct from a block of another matrix
        template<class MatrixType>
        inline RectangularMatrix(const ConstMatrixBlock<MatrixType>&);

        //- Construct from a block of another matrix
        template<class MatrixType>
        inline RectangularMatrix(const MatrixBlock<MatrixType>&);

        //- Construct with given number of rows and columns
        //  initializing all elements to zero
        inline RectangularMatrix(const label m, const label n, const zero);

        //- Construct with given number of rows and columns
        //  and value for all elements.
        inline RectangularMatrix(const label m, const label n, const Type&);

        //- Construct as copy of a square matrix
        inline RectangularMatrix(const SquareMatrix<Type>&);

        //- Construct from Istream.
        inline RectangularMatrix(Istream&);

        //- Clone
        inline autoPtr<RectangularMatrix<Type> > clone() const;


    // Member operators

        //- Assignment of all elements to zero
        void operator=(const zero);
};


// Global functions and operators

template<class Type>
class typeOfInnerProduct<Type, RectangularMatrix<Type>, RectangularMatrix<Type>>
{
public:

    typedef RectangularMatrix<Type> type;
};

template<class Type>
class typeOfInnerProduct<Type, RectangularMatrix<Type>, SquareMatrix<Type>>
{
public:

    typedef RectangularMatrix<Type> type;
};

template<class Type>
class typeOfInnerProduct<Type, SquareMatrix<Type>, RectangularMatrix<Type>>
{
public:

    typedef RectangularMatrix<Type> type;
};


template<class Type>
RectangularMatrix<Type> outer(const Field<Type>& f1, const Field<Type>& f2);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "RectangularMatrixI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
