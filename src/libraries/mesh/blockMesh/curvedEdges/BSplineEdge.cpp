/*---------------------------------------------------------------------------*\
Copyright (C) 2014 OpenFOAM Foundation
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

#include "BSplineEdge.hpp"
#include "addToRunTimeSelectionTable.hpp"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(BSplineEdge, 0);

    addToRunTimeSelectionTable
    (
        curvedEdge,
        BSplineEdge,
        Istream
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::BSplineEdge::BSplineEdge
(
    const pointField& points,
    const label start,
    const label end,
    const pointField& internalPoints
)
:
    curvedEdge(points, start, end),
    BSpline(appendEndPoints(points, start, end, internalPoints))
{}


CML::BSplineEdge::BSplineEdge(const pointField& points, Istream& is)
:
    curvedEdge(points, is),
    BSpline(appendEndPoints(points, start_, end_, pointField(is)))
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

CML::BSplineEdge::~BSplineEdge()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::point CML::BSplineEdge::position(const scalar mu) const
{
    return BSpline::position(mu);
}


CML::scalar CML::BSplineEdge::length() const
{
    return BSpline::length();
}


// ************************************************************************* //
