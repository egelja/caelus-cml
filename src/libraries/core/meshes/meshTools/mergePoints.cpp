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

#include "mergePoints.hpp"
#include "SortableList.hpp"
#include "ListOps.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::label CML::mergePoints
(
    const UList<point>& points,
    const scalar mergeTol,
    const bool verbose,
    labelList& pointMap,
    const point& origin
)
{
    point compareOrigin = origin;

    if (origin == point(VGREAT, VGREAT, VGREAT))
    {
        if (points.size())
        {
            compareOrigin = sum(points)/points.size();
        }
    }

    // Create a old to new point mapping array
    pointMap.setSize(points.size());
    pointMap = -1;

    if (points.empty())
    {
        return points.size();
    }

    // We're comparing distance squared to origin first.
    // Say if starting from two close points:
    //     x, y, z
    //     x+mergeTol, y+mergeTol, z+mergeTol
    // Then the magSqr of both will be
    //     x^2+y^2+z^2
    //     x^2+y^2+z^2 + 2*mergeTol*(x+z+y) + mergeTol^2*...
    // so the difference will be 2*mergeTol*(x+y+z)

    const scalar mergeTolSqr = CML::sqr(scalar(mergeTol));

    // Sort points by magSqr
    const pointField d(points - compareOrigin);

    List<scalar> magSqrD(d.size());
    forAll(d, pointi)
    {
        magSqrD[pointi] = magSqr(d[pointi]);
    }
    labelList order;
    sortedOrder(magSqrD, order);


    Field<scalar> sortedTol(points.size());
    forAll(order, sortI)
    {
        label pointi = order[sortI];

        // Convert to scalar precision
        const point pt
        (
            scalar(d[pointi].x()),
            scalar(d[pointi].y()),
            scalar(d[pointi].z())
        );
        sortedTol[sortI] = 2*mergeTol*(mag(pt.x())+mag(pt.y())+mag(pt.z()));
    }

    label newPointi = 0;


    // Handle 0th point separately (is always unique)
    label pointi = order[0];
    pointMap[pointi] = newPointi++;


    for (label sortI = 1; sortI < order.size(); sortI++)
    {
        // Get original point index
        label pointi = order[sortI];
        const scalar mag2 = magSqrD[order[sortI]];
        // Convert to scalar precision
        const point pt
        (
            scalar(points[pointi].x()),
            scalar(points[pointi].y()),
            scalar(points[pointi].z())
        );


        // Compare to previous points to find equal one.
        label equalPointi = -1;

        for
        (
            label prevSortI = sortI - 1;
            prevSortI >= 0
         && (mag(magSqrD[order[prevSortI]] - mag2) <= sortedTol[sortI]);
            prevSortI--
        )
        {
            label prevPointi = order[prevSortI];
            const point prevPt
            (
                scalar(points[prevPointi].x()),
                scalar(points[prevPointi].y()),
                scalar(points[prevPointi].z())
            );

            if (magSqr(pt - prevPt) <= mergeTolSqr)
            {
                // Found match.
                equalPointi = prevPointi;

                break;
            }
        }


        if (equalPointi != -1)
        {
            // Same coordinate as equalPointi. Map to same new point.
            pointMap[pointi] = pointMap[equalPointi];

            if (verbose)
            {
                Pout<< "CML::mergePoints : Merging points "
                    << pointi << " and " << equalPointi
                    << " with coordinates:" << points[pointi]
                    << " and " << points[equalPointi]
                    << endl;
            }
        }
        else
        {
            // Differs. Store new point.
            pointMap[pointi] = newPointi++;
        }
    }

    return newPointi;
}



bool CML::mergePoints
(
    const UList<point>& points,
    const scalar mergeTol,
    const bool verbose,
    labelList& pointMap,
    List<point>& newPoints,
    const point& origin
)
{
    label nUnique = mergePoints
    (
        points,
        mergeTol,
        verbose,
        pointMap,
        origin
    );

    newPoints.setSize(nUnique);
    forAll(pointMap, pointi)
    {
        newPoints[pointMap[pointi]] = points[pointi];
    }

    return (nUnique != points.size());
}


// ************************************************************************* //
