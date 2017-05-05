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
    matrix3D

Description
    Implementation of 3 x 3 matrix

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef matrix3D_HPP
#define matrix3D_HPP

#include "scalar.hpp"
#include "FixedList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class matrix3D Declaration
\*---------------------------------------------------------------------------*/

class matrix3D
: public FixedList<FixedList<scalar, 3>, 3>
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
        explicit inline matrix3D();

        //- Copy constructor
        inline matrix3D(const matrix3D&);

    // Destructor
        inline ~matrix3D();

    // Member functions
        //- return matrix determinant
        inline scalar determinant();

        //- return inverse matrix
        inline matrix3D inverse();

        //- find the solution of the system with the given rhs
        inline FixedList<scalar, 3> solve
        (
            const FixedList<scalar, 3>& source
        );

        //- return the first component of the solution vector
        inline scalar solveFirst(const FixedList<scalar, 3>& source);

        //- return the second component of the solution vector
        inline scalar solveSecond(const FixedList<scalar, 3>& source);

        //- return the third component of the solution vector
        inline scalar solveThird(const FixedList<scalar, 3>& source);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline void matrix3D::calculateDeterminant()
{
    if( calculatedDet_ )
        return;

    const FixedList<FixedList<scalar, 3>, 3>& mat = *this;

    det_ =
        mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) -
        mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]) +
        mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);

    calculatedDet_ = true;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline matrix3D::matrix3D()
:
    det_(),
    calculatedDet_(false)
{}

inline matrix3D::matrix3D(const matrix3D& m)
:
    FixedList<FixedList<scalar, 3>, 3>(m),
    det_(m.det_),
    calculatedDet_(m.calculatedDet_)
{}

inline matrix3D::~matrix3D()
{}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline scalar matrix3D::determinant()
{
    calculateDeterminant();

    return det_;
}

inline matrix3D matrix3D::inverse()
{
    calculateDeterminant();

    const FixedList<FixedList<scalar, 3>, 3>& mat = *this;

    matrix3D imat;
    imat[0][0] = (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) / det_;
    imat[0][1] = (mat[0][2] * mat[2][1] - mat[0][1] * mat[2][2]) / det_;
    imat[0][2] = (mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]) / det_;
    imat[1][0] = (mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2]) / det_;
    imat[1][1] = (mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0]) / det_;
    imat[1][2] = (mat[0][2] * mat[1][0] - mat[0][0] * mat[1][2]) / det_;
    imat[2][0] = (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]) / det_;
    imat[2][1] = (mat[0][1] * mat[2][0] - mat[0][0] * mat[2][1]) / det_;
    imat[2][2] = (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]) / det_;

    return imat;
}

inline FixedList<scalar, 3> matrix3D::solve
(
    const FixedList<scalar, 3>& source
)
{
    FixedList<scalar, 3> result;
    result[0] = solveFirst(source);
    result[1] = solveSecond(source);
    result[2] = solveThird(source);

    return result;
}

inline scalar matrix3D::solveFirst(const FixedList<scalar, 3>& source)
{
    calculateDeterminant();

    const FixedList<FixedList<scalar, 3>, 3>& mat = *this;

    return
    (
        (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) * source[0] +
        (mat[0][2] * mat[2][1] - mat[0][1] * mat[2][2]) * source[1] +
        (mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]) * source[2]
    ) / det_;
}

inline scalar matrix3D::solveSecond(const FixedList<scalar, 3>& source)
{
    calculateDeterminant();

    const FixedList<FixedList<scalar, 3>, 3>& mat = *this;

    return
    (
        (mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2]) * source[0] +
        (mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0]) * source[1] +
        (mat[0][2] * mat[1][0] - mat[0][0] * mat[1][2]) * source[2]
    ) / det_;
}

inline scalar matrix3D::solveThird(const FixedList<scalar, 3>& source)
{
    calculateDeterminant();

    const FixedList<FixedList<scalar, 3>, 3>& mat = *this;

    return
    (
        (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]) * source[0] +
        (mat[0][1] * mat[2][0] - mat[0][0] * mat[2][1]) * source[1] +
        (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]) * source[2]
    ) / det_;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
