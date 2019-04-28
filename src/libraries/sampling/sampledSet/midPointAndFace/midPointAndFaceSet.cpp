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

#include "midPointAndFaceSet.hpp"
#include "polyMesh.hpp"
#include "addToRunTimeSelectionTable.hpp"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(midPointAndFaceSet, 0);
    addToRunTimeSelectionTable(sampledSet, midPointAndFaceSet, word);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// Rework faceOnlySet samples.
// Take two consecutive samples
void CML::midPointAndFaceSet::genSamples()
{
    // Generate midpoints and add to face points

    List<point> newSamplePoints(3*size());
    labelList newSampleCells(3*size());
    labelList newSampleFaces(3*size());
    labelList newSampleSegments(3*size());
    scalarList newSampleCurveDist(3*size());

    label newSampleI = 0;

    label sampleI = 0;

    while(true && size()>0)
    {
        // sampleI is start of segment

        // Add sampleI
        newSamplePoints[newSampleI] = operator[](sampleI);
        newSampleCells[newSampleI] = cells_[sampleI];
        newSampleFaces[newSampleI] = faces_[sampleI];
        newSampleSegments[newSampleI] = segments_[sampleI];
        newSampleCurveDist[newSampleI] = curveDist_[sampleI];
        newSampleI++;

        while
        (
            (sampleI < size() - 1)
         && (segments_[sampleI] == segments_[sampleI+1])
        )
        {
            // Add mid point
            const point mid = 0.5*(operator[](sampleI) + operator[](sampleI+1));

            label cell1 = getCell(faces_[sampleI], mid);
            label cell2 = getCell(faces_[sampleI+1], mid);

            if (cell1 != cell2)
            {
                FatalErrorInFunction
                    << "  sampleI:" << sampleI
                    << "  newSampleI:" << newSampleI
                    << "  pts[sampleI]:" << operator[](sampleI)
                    << "  face[sampleI]:" << faces_[sampleI]
                    << "  pts[sampleI+1]:" << operator[](sampleI+1)
                    << "  face[sampleI+1]:" << faces_[sampleI+1]
                    << "  cell1:" << cell1
                    << "  cell2:" << cell2
                    << abort(FatalError);
            }

            newSamplePoints[newSampleI] = mid;
            newSampleCells[newSampleI] = cell1;
            newSampleFaces[newSampleI] = -1;
            newSampleSegments[newSampleI] = segments_[sampleI];
            newSampleCurveDist[newSampleI] =
                mag(newSamplePoints[newSampleI] - start());

            newSampleI++;

            // Add sampleI+1
            newSamplePoints[newSampleI] = operator[](sampleI+1);
            newSampleCells[newSampleI] = cells_[sampleI+1];
            newSampleFaces[newSampleI] = faces_[sampleI+1];
            newSampleSegments[newSampleI] = segments_[sampleI+1];
            newSampleCurveDist[newSampleI] =
                mag(newSamplePoints[newSampleI] - start());

            newSampleI++;

            sampleI++;
        }

        if (sampleI == size() - 1)
        {
            break;
        }
        sampleI++;
    }

    newSamplePoints.setSize(newSampleI);
    newSampleCells.setSize(newSampleI);
    newSampleFaces.setSize(newSampleI);
    newSampleSegments.setSize(newSampleI);
    newSampleCurveDist.setSize(newSampleI);

    setSamples
    (
        newSamplePoints,
        newSampleCells,
        newSampleFaces,
        newSampleSegments,
        newSampleCurveDist
    );
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::midPointAndFaceSet::midPointAndFaceSet
(
    const word& name,
    const polyMesh& mesh,
    const meshSearch& searchEngine,
    const word& axis,
    const point& start,
    const point& end
)
:
    faceOnlySet(name, mesh, searchEngine, axis, start, end)
{
    genSamples();

    if (debug)
    {
        write(Info);
    }
}


CML::midPointAndFaceSet::midPointAndFaceSet
(
    const word& name,
    const polyMesh& mesh,
    const meshSearch& searchEngine,
    const dictionary& dict
)
:
    faceOnlySet(name, mesh, searchEngine, dict)
{
    genSamples();

    if (debug)
    {
        write(Info);
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::midPointAndFaceSet::~midPointAndFaceSet()
{}


// ************************************************************************* //
