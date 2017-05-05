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

Application
    flattenMesh

Description
    Flattens the front and back planes of a 2D cartesian mesh.

\*---------------------------------------------------------------------------*/

#include "argList.hpp"
#include "Time.hpp"
#include "polyMesh.hpp"
#include "emptyPolyPatch.hpp"
#include "twoDPointCorrector.hpp"

using namespace CML;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//  Main program:

int main(int argc, char *argv[])
{
#   include "setRootCase.hpp"
#   include "createTime.hpp"
#   include "createPolyMesh.hpp"

    pointIOField points
    (
        IOobject
        (
            "points",
            runTime.findInstance(polyMesh::meshSubDir, "points"),
            polyMesh::meshSubDir,
            runTime,
            IOobject::MUST_READ,
            IOobject::NO_WRITE,
            false
        )
    );

    boundBox bb(points);

    Info<< "bounding box: min = " << bb.min()
        << " max = " << bb.max() << " meters."
        << endl;


    point midPoint = gAverage(points);

    twoDPointCorrector twoDCorr(mesh);

    direction planeNormalCmpt = twoDCorr.normalDir();

    scalar midCmptVal = midPoint[planeNormalCmpt];
    scalar minCmptVal = bb.min()[planeNormalCmpt];
    scalar maxCmptVal = bb.max()[planeNormalCmpt];

    forAll(points, pointI)
    {
        if (points[pointI][planeNormalCmpt] < midCmptVal)
        {
            points[pointI][planeNormalCmpt] = minCmptVal;
        }
        else
        {
            points[pointI][planeNormalCmpt] = maxCmptVal;
        }
    }

    twoDCorr.correctPoints(points);

    // Set the precision of the points data to 10
    IOstream::defaultPrecision(10);

    Info<< "Writing points into directory " << points.path() << nl << endl;
    points.write();

    return 0;
}


// ************************************************************************* //
