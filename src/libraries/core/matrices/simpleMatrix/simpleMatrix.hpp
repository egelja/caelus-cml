/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::simpleMatrix

Description
    A simple square matrix solver with scalar coefficients.


\*---------------------------------------------------------------------------*/

#ifndef simpleMatrix_H
#define simpleMatrix_H

#include "scalarMatrices.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Type>
class simpleMatrix;

template<class Type>
Ostream& operator<<
(
    Ostream&,
    const simpleMatrix<Type>&
);


/*---------------------------------------------------------------------------*\
                           Class simpleMatrix Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class simpleMatrix
:
    public scalarSquareMatrix
{
    // Private data

        Field<Type> source_;


public:

    // Constructors

        //- Construct given size
        //  Note: this does not initialise the coefficients or the source.
        simpleMatrix(const label);

        //- Construct given size and initial values for coefficients and source
        simpleMatrix(const label, const scalar, const Type&);

        //- Construct from components
        simpleMatrix(const scalarSquareMatrix&, const Field<Type>&);

        //- Construct from Istream
        simpleMatrix(Istream&);

        //- Construct as copy
        simpleMatrix(const simpleMatrix<Type>&);


    // Member Functions

        // Access

            //- Return access to the source
            Field<Type>& source()
            {
                return source_;
            }

            //- Return const-access to the source
            const Field<Type>& source() const
            {
                return source_;
            }


        //- Solve the matrix using Gaussian elimination with pivoting
        //  and return the solution
        Field<Type> solve() const;

        //- Solve the matrix using LU decomposition with pivoting
        //  and return the solution
        Field<Type> LUsolve() const;


    // Member Operators

        void operator=(const simpleMatrix<Type>&);


    // Ostream Operator

        friend Ostream& operator<< <Type>
        (
            Ostream&,
            const simpleMatrix<Type>&
        );
};


// Global operators

template<class Type>
simpleMatrix<Type> operator+
(
    const simpleMatrix<Type>&,
    const simpleMatrix<Type>&
);

template<class Type>
simpleMatrix<Type> operator-
(
    const simpleMatrix<Type>&,
    const simpleMatrix<Type>&
);

template<class Type>
simpleMatrix<Type> operator*
(
    const scalar,
    const simpleMatrix<Type>&
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::simpleMatrix<Type>::simpleMatrix(const label mSize)
:
    scalarSquareMatrix(mSize),
    source_(mSize)
{}


template<class Type>
CML::simpleMatrix<Type>::simpleMatrix
(
    const label mSize,
    const scalar coeffVal,
    const Type& sourceVal
)
:
    scalarSquareMatrix(mSize, coeffVal),
    source_(mSize, sourceVal)
{}


template<class Type>
CML::simpleMatrix<Type>::simpleMatrix
(
    const scalarSquareMatrix& matrix,
    const Field<Type>& source
)
:
    scalarSquareMatrix(matrix),
    source_(source)
{}


template<class Type>
CML::simpleMatrix<Type>::simpleMatrix(Istream& is)
:
    scalarSquareMatrix(is),
    source_(is)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::Field<Type> CML::simpleMatrix<Type>::solve() const
{
    scalarSquareMatrix tmpMatrix = *this;
    Field<Type> sourceSol = source_;

    CML::solve(tmpMatrix, sourceSol);

    return sourceSol;
}


template<class Type>
CML::Field<Type> CML::simpleMatrix<Type>::LUsolve() const
{
    scalarSquareMatrix luMatrix = *this;
    Field<Type> sourceSol = source_;

    CML::LUsolve(luMatrix, sourceSol);

    return sourceSol;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
void CML::simpleMatrix<Type>::operator=(const simpleMatrix<Type>& m)
{
    if (this == &m)
    {
        FatalErrorInFunction
            << "Attempted assignment to self"
            << abort(FatalError);
    }

    if (m() != m.m())
    {
        FatalErrorInFunction
            << "Different size matrices"
            << abort(FatalError);
    }

    if (source_.size() != m.source_.size())
    {
        FatalErrorInFunction
            << "Different size source vectors"
            << abort(FatalError);
    }

    scalarSquareMatrix::operator=(m);
    source_ = m.source_;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Type>
CML::simpleMatrix<Type> CML::operator+
(
    const simpleMatrix<Type>& m1,
    const simpleMatrix<Type>& m2
)
{
    return simpleMatrix<Type>
    (
        static_cast<const scalarSquareMatrix&>(m1)
      + static_cast<const scalarSquareMatrix&>(m2),
        m1.source_ + m2.source_
    );
}


template<class Type>
CML::simpleMatrix<Type> CML::operator-
(
    const simpleMatrix<Type>& m1,
    const simpleMatrix<Type>& m2
)
{
    return simpleMatrix<Type>
    (
        static_cast<const scalarSquareMatrix&>(m1)
      - static_cast<const scalarSquareMatrix&>(m2),
        m1.source_ - m2.source_
    );
}


template<class Type>
CML::simpleMatrix<Type> CML::operator*
(
    const scalar s,
    const simpleMatrix<Type>& m
)
{
    return simpleMatrix<Type>(s*m.matrix_, s*m.source_);
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const simpleMatrix<Type>& m
)
{
    os << static_cast<const scalarSquareMatrix&>(m) << nl << m.source_;
    return os;
}


#endif
