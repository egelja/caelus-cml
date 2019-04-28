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
    CML::DiagonalMatrix

Description
    DiagonalMatrix<Type> is a 2D diagonal matrix of objects
    of type Type, size nxn


\*---------------------------------------------------------------------------*/

#ifndef DiagonalMatrix_H
#define DiagonalMatrix_H

#include "List.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * *  * * * * * * Class Forward declaration  * * * * * * * * * * * //

template<class Form, class Type> class Matrix;

/*---------------------------------------------------------------------------*\
                           Class DiagonalMatrix Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class DiagonalMatrix
:
    public List<Type>
{
public:

    // Constructors

        //- Null constructor.
        DiagonalMatrix<Type>();

        //- Construct from diagonal component of a Matrix
        template<class Form>
        DiagonalMatrix<Type>(const Matrix<Form, Type>&);

        //- Construct empty from size
        DiagonalMatrix<Type>(const label size);

        //- Construct from size and a value
        DiagonalMatrix<Type>(const label, const Type&);


    // Member functions

        //- Invert the diagonal matrix and return itself
        DiagonalMatrix<Type>& invert();
};


// Global functions

//- Return the diagonal Matrix inverse
template<class Type>
DiagonalMatrix<Type> inv(const DiagonalMatrix<Type>&);


} // End namespace CML


template<class Type>
inline CML::DiagonalMatrix<Type>::DiagonalMatrix()
:
    List<Type>()
{}


template<class Type>
template<class Form>
CML::DiagonalMatrix<Type>::DiagonalMatrix(const Matrix<Form, Type>& a)
:
    List<Type>(min(a.m(), a.n()))
{
    forAll(*this, i)
    {
        this->operator[](i) = a(i, i);
    }
}


template<class Type>
CML::DiagonalMatrix<Type>::DiagonalMatrix(const label size)
:
    List<Type>(size)
{}


template<class Type>
CML::DiagonalMatrix<Type>::DiagonalMatrix(const label size, const Type& val)
:
    List<Type>(size, val)
{}


template<class Type>
CML::DiagonalMatrix<Type>& CML::DiagonalMatrix<Type>::invert()
{
    forAll(*this, i)
    {
        Type x = this->operator[](i);
        if (mag(x) < VSMALL)
        {
            this->operator[](i) = Type(0);
        }
        else
        {
            this->operator[](i) = Type(1)/x;
        }
    }

    return this;
}


template<class Type>
CML::DiagonalMatrix<Type> CML::inv(const DiagonalMatrix<Type>& A)
{
    DiagonalMatrix<Type> Ainv = A;

    forAll(A, i)
    {
        Type x = A[i];
        if (mag(x) < VSMALL)
        {
            Ainv[i] = Type(0);
        }
        else
        {
            Ainv[i] = Type(1)/x;
        }
    }

    return Ainv;
}


#endif
