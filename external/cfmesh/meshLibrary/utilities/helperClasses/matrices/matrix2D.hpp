/*---------------------------------------------------------------------------*\
Copyright (C) Creative Fields, Ltd.
-------------------------------------------------------------------------------
License
    This file is part of cfMesh.

    cfMesh is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    cfMesh is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with cfMesh.  If not, see <http://www.gnu.org/licenses/>.

Class
    matrix2D

Description
    Implementation of 2 x 2 matrix

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef matrix2D_HPP
#define matrix2D_HPP

#include "scalar.hpp"
#include "FixedList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class matrix2D Declaration
\*---------------------------------------------------------------------------*/

class matrix2D
: public FixedList<FixedList<scalar, 2>, 2>
{
    // Private members
        scalar det_;
        bool calculatedDet_;

    // Private member functions
        //- calculate matrix determinant
        inline void calculateDeterminant();

public:

    // Constructors
        //- Null constructor
        explicit inline matrix2D();

        //- Copy constructor
        inline matrix2D(const matrix2D&);

    // Destructor
        inline ~matrix2D();

    // Member functions
        //- return matrix determinant
        inline scalar determinant();

        //- return inverse matrix
        inline matrix2D inverse();

        //- find the solution of the system with the given rhs
        inline FixedList<scalar, 2> solve
        (
            const FixedList<scalar, 2>& source
        );

        //- return the first component of the solution vector
        inline scalar solveFirst(const FixedList<scalar, 2>& source);

        //- return the second component of the solution vector
        inline scalar solveSecond(const FixedList<scalar, 2>& source);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline void matrix2D::calculateDeterminant()
{
    if( calculatedDet_ )
        return;

    const FixedList<FixedList<scalar, 2>, 2>& mat = *this;
    det_ = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

    calculatedDet_ = true;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline matrix2D::matrix2D()
:
    det_(),
    calculatedDet_(false)
{}

inline matrix2D::matrix2D(const matrix2D& m)
:
    FixedList<FixedList<scalar, 2>, 2>(m),
    det_(m.det_),
    calculatedDet_(m.calculatedDet_)
{}

inline matrix2D::~matrix2D()
{}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline scalar matrix2D::determinant()
{
    calculateDeterminant();

    return det_;
}

inline matrix2D matrix2D::inverse()
{
    calculateDeterminant();

    const FixedList<FixedList<scalar, 2>, 2>& mat = *this;

    matrix2D imat;
    imat[0][0] = mat[1][1] / det_;
    imat[0][1] = -1.0 * mat[0][1] / det_;
    imat[1][0] = -1.0 * mat[1][0] / det_;
    imat[1][1] = mat[0][0] / det_;

    return matrix2D(imat);
}

inline FixedList<scalar, 2> matrix2D::solve
(
    const FixedList<scalar, 2>& source
)
{
    FixedList<scalar, 2> result;
    result[0] = solveFirst(source);
    result[1] = solveSecond(source);

    return result;
}

inline scalar matrix2D::solveFirst(const FixedList<scalar, 2>& source)
{
    calculateDeterminant();

    const FixedList<FixedList<scalar, 2>, 2>& mat = *this;

    return
    (
        mat[1][1] * source[0] -
        mat[0][1] * source[1]
    ) / det_;
}

inline scalar matrix2D::solveSecond(const FixedList<scalar, 2>& source)
{
    calculateDeterminant();

    const FixedList<FixedList<scalar, 2>, 2>& mat = *this;

    return
    (
        -1.0 * mat[1][0] * source[0] +
        mat[0][0] * source[1]
    ) / det_;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
