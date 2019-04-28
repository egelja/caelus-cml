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

\*---------------------------------------------------------------------------*/

#include "error.hpp"
#include "lineEdge.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(lineEdge, 0);
    addToRunTimeSelectionTable(curvedEdge, lineEdge, Istream);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::lineEdge::lineEdge
(
    const pointField& points,
    const label start,
    const label end
)
:
    curvedEdge(points, start, end)
{}


CML::lineEdge::lineEdge(const pointField& points, Istream& is)
:
    curvedEdge(points, is)
{}

// * * * * * * * * * * * * * * * * Destructor * * * * * * * * * * * * * * * * //

CML::lineEdge::~lineEdge()
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::point CML::lineEdge::position(const scalar lambda) const
{
    if (lambda < 0 || lambda > 1)
    {
        FatalErrorInFunction
            << "Parameter out of range, lambda = " << lambda
            << abort(FatalError);
    }

    return points_[start_] + lambda * (points_[end_] - points_[start_]);
}


CML::scalar CML::lineEdge::length() const
{
    return mag(points_[end_] - points_[start_]);
}


// ************************************************************************* //
