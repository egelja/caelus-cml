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

#include "splineEdge.hpp"
#include "addToRunTimeSelectionTable.hpp"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(splineEdge, 0);

    addToRunTimeSelectionTable
    (
        curvedEdge,
        splineEdge,
        Istream
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::splineEdge::splineEdge
(
    const pointField& points,
    const label start,
    const label end,
    const pointField& internalPoints
)
:
    curvedEdge(points, start, end),
    CatmullRomSpline(appendEndPoints(points, start, end, internalPoints))
{}


CML::splineEdge::splineEdge(const pointField& points, Istream& is)
:
    curvedEdge(points, is),
    CatmullRomSpline(appendEndPoints(points, start_, end_, pointField(is)))
{
    token t(is);
    is.putBack(t);

    // discard unused start/end tangents
    if (t == token::BEGIN_LIST)
    {
        vector tangent0Ignored(is);
        vector tangent1Ignored(is);
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::splineEdge::~splineEdge()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::point CML::splineEdge::position(const scalar mu) const
{
    return CatmullRomSpline::position(mu);
}


CML::scalar CML::splineEdge::length() const
{
    return CatmullRomSpline::length();
}


// ************************************************************************* //
