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

#include "error.hpp"
#include "polyLineEdge.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(polyLineEdge, 0);
    addToRunTimeSelectionTable(curvedEdge, polyLineEdge, Istream);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::polyLineEdge::polyLineEdge
(
    const pointField& ps,
    const label start,
    const label end,
    const pointField& otherPoints
)
:
    curvedEdge(ps, start, end),
    polyLine(appendEndPoints(ps, start_, end_, otherPoints))
{}


CML::polyLineEdge::polyLineEdge(const pointField& ps, Istream& is)
:
    curvedEdge(ps, is),
    polyLine(appendEndPoints(ps, start_, end_, pointField(is)))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::polyLineEdge::~polyLineEdge()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::point CML::polyLineEdge::position(const scalar lambda) const
{
    return polyLine::position(lambda);
}


CML::scalar CML::polyLineEdge::length() const
{
    return polyLine::lineLength_;
}


// ************************************************************************* //
