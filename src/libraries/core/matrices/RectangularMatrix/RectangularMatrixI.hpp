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
inline CML::RectangularMatrix<Type>::RectangularMatrix()
:
    Matrix<RectangularMatrix<Type>, Type>()
{}

template<class Type>
inline CML::RectangularMatrix<Type>::RectangularMatrix
(
    const label m,
    const label n
)
:
    Matrix<RectangularMatrix<Type>, Type>(m, n)
{}

template<class Type>
inline CML::RectangularMatrix<Type>::RectangularMatrix
(
    const label m,
    const label n,
    const Type& t
)
:
    Matrix<RectangularMatrix<Type>, Type>(m, n, t)
{}

template<class Type>
inline CML::RectangularMatrix<Type>::RectangularMatrix(Istream& is)
:
    Matrix<RectangularMatrix<Type>, Type>(is)
{}

template<class Type>
inline CML::autoPtr<CML::RectangularMatrix<Type> >
CML::RectangularMatrix<Type>::clone() const
{
    return autoPtr<RectangularMatrix<Type> >
    (
        new RectangularMatrix<Type>(*this)
    );
}


// ************************************************************************* //
