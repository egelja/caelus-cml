/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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

Description
    Determine correspondence between points. See below.

SourceFiles
    matchPoints.cpp

\*---------------------------------------------------------------------------*/

#ifndef matchPoints_H
#define matchPoints_H

#include "scalarField.hpp"
#include "pointField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Function matchPoints Declaration
\*---------------------------------------------------------------------------*/

//- Determine correspondence between pointFields. Gets passed
//  local matching distance per point. Returns map from 0 to 1. Returns
//  true if found corresponding point in 1 for every point in 0; false
//  otherwise. Compares mag(pt - origin) and does proper comparison for points
//  with (almost) identical mag. pts0 can be subset of pts1.
bool matchPoints
(
    const UList<point>& pts0,
    const UList<point>& pts1,
    const UList<scalar>& matchDistance,
    const bool verbose,
    labelList& from0To1,
    const point& origin = point::zero
);

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
