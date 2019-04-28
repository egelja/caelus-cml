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

Description

\*---------------------------------------------------------------------------*/

#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::tetCell::tetCell()
{}


inline CML::tetCell::tetCell
(
    const label a,
    const label b,
    const label c,
    const label d
)
{
    operator[](0) = a;
    operator[](1) = b;
    operator[](2) = c;
    operator[](3) = d;
}


inline CML::tetCell::tetCell(const FixedList<label, 4>& lst)
:
    FixedList<label, 4>(lst)
{}


inline CML::tetCell::tetCell(Istream& is)
:
    FixedList<label, 4>(is)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::triFace CML::tetCell::face(const label faceI) const
{
    // Warning. Ordering of faces needs to be the same for a tetrahedron
    // class, a tetrahedron cell shape model and a tetCell
    static const label a[] = {1, 0, 0, 0};
    static const label b[] = {2, 3, 1, 2};
    static const label c[] = {3, 2, 3, 1};

#   ifdef FULLDEBUG
    if (faceI >= 4)
    {
        FatalErrorInFunction
            << "index out of range 0 -> 3. faceI = " << faceI
            << abort(FatalError);
    }
#   endif

    return triFace
    (
        operator[](a[faceI]),
        operator[](b[faceI]),
        operator[](c[faceI])
    );
}


inline CML::label CML::tetCell::edgeFace(const label edgeI) const
{
    // Warning. Ordering of faces needs to be the same for a tetrahedron
    // class, a tetrahedron cell shape model and a tetCell
    //static const label edgeFaces[6] = {2, 1, 1, 0, 0, 0};
    static const label edgeFaces[6] = {2, 3, 1, 0, 0, 1};

#   ifdef FULLDEBUG
    if (edgeI >= 6)
    {
        FatalErrorInFunction
            << "edge index out of range 0 -> 5. edgeI = " << edgeI
            << abort(FatalError);
    }
#   endif

    return edgeFaces[edgeI];
}


inline CML::label CML::tetCell::edgeAdjacentFace
(
    const label edgeI,
    const label faceI
) const
{
    // Warning. Ordering of faces needs to be the same for a tetrahedron
    // class, a tetrahedron cell shape model and a tetCell
    static const label adjacentFace[6][4] =
    {
        {-1, -1, 3, 2},
        {-1, 3, -1, 1},
        {-1, 2, 1, -1},
        {2, -1, 0, -1},
        {3, -1, -1, 0},
        {1, 0, -1, -1}
    };

#   ifdef FULLDEBUG
    if (faceI >= 4)
    {
        FatalErrorInFunction
            << "face index out of range 0 -> 3. faceI = " << faceI
            << abort(FatalError);
    }

    if (edgeI >= 6)
    {
        FatalErrorInFunction
            << "edge index out of range 0 -> 5. edgeI = " << edgeI
            << abort(FatalError);
    }
#   endif

    return adjacentFace[edgeI][faceI];
}


inline CML::edge CML::tetCell::tetEdge(const label edgeI) const
{
    // Warning. Ordering of edges needs to be the same for a tetrahedron
    // class, a tetrahedron cell shape model and a tetCell
    //
    static const label start[] = {0, 0, 0, 3, 1, 3};
    static const label end[] = {1, 2, 3, 1, 2, 2};

#   ifdef FULLDEBUG
    if (edgeI >= 6)
    {
        FatalErrorInFunction
            << "index out of range 0 -> 5. edgeI = " << edgeI
            << abort(FatalError);
    }
#   endif

    return edge(operator[](start[edgeI]), operator[](end[edgeI]));
}


inline CML::tetPointRef CML::tetCell::tet(const pointField& points) const
{
    return tetPointRef
    (
        points[operator[](0)],
        points[operator[](1)],
        points[operator[](2)],
        points[operator[](3)]
    );
}


// ************************************************************************* //
