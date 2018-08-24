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
    Merge points. See below.

SourceFiles
    mergePoints.cpp

\*---------------------------------------------------------------------------*/

#ifndef mergePoints_H
#define mergePoints_H

#include "scalarField.hpp"
#include "pointField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Function mergePoints Declaration
\*---------------------------------------------------------------------------*/

//- Sorts and merges points. All points closer than/equal mergeTol get merged.
//  Returns the number of unique points and a map from old to new.
label mergePoints
(
    const UList<point>& points,
    const scalar mergeTol,
    const bool verbose,
    labelList& pointMap,
    const point& origin = point::zero
);

//- Sorts and merges points. All points closer than/equal mergeTol get merged.
//  Outputs the new unique points and a map from old to new. Returns
//  true if anything merged, false otherwise.
bool mergePoints
(
    const UList<point>& points,
    const scalar mergeTol,
    const bool verbose,
    labelList& pointMap,
    List<point>& newPoints,
    const point& origin = point::zero
);

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
