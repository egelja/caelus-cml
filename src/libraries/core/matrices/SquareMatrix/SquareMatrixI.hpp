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

\*---------------------------------------------------------------------------*/


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
inline CML::SquareMatrix<Type>::SquareMatrix()
:
    Matrix<SquareMatrix<Type>, Type>()
{}


template<class Type>
inline CML::SquareMatrix<Type>::SquareMatrix(const label n)
:
    Matrix<SquareMatrix<Type>, Type>(n, n)
{}


template<class Type>
inline CML::SquareMatrix<Type>::SquareMatrix
(
    const label n,
    const Type& t
)
:
    Matrix<SquareMatrix<Type>, Type>(n, n, t)
{}


template<class Type>
inline CML::SquareMatrix<Type>::SquareMatrix(Istream& is)
:
    Matrix<SquareMatrix<Type>, Type>(is)
{}


template<class Type>
inline CML::autoPtr<CML::SquareMatrix<Type> >
CML::SquareMatrix<Type>::clone() const
{
    return autoPtr<SquareMatrix<Type> >(new SquareMatrix<Type>(*this));
}


// ************************************************************************* //
