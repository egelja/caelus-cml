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

#ifndef tetPoints_HPP
#define tetPoints_HPP

#include "tetrahedron.hpp"
#include "FixedList.hpp"
#include "treeBoundBox.hpp"

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class tetPoints Declaration
\*---------------------------------------------------------------------------*/

class tetPoints
:
    public FixedList<point, 4>
{
public:

    // Constructors

        //- Construct null
        inline tetPoints()
        {}

        //- Construct from four points
        inline tetPoints
        (
            const point& a,
            const point& b,
            const point& c,
            const point& d
        )
        {
            operator[](0) = a;
            operator[](1) = b;
            operator[](2) = c;
            operator[](3) = d;
        }

    // Member Functions

        //- Return the tetrahedron
        inline tetPointRef tet() const
        {
            return tetPointRef
            (
                operator[](0),
                operator[](1),
                operator[](2),
                operator[](3)
            );
        }

        //- Calculate the bounding box
        inline treeBoundBox bounds() const
        {
            treeBoundBox bb(operator[](0), operator[](0));
            for (label i = 1; i < size(); i++)
            {
                bb.min() = min(bb.min(), operator[](i));
                bb.max() = max(bb.max(), operator[](i));
            }
            return bb;
        }
};


} // End namespace CML

#endif

