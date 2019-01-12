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

#include "enrichedPatch.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::enrichedPatch::completePointMap() const
{
    if (pointMapComplete_)
    {
        FatalErrorInFunction
            << "Point map already completed"
            << abort(FatalError);
    }

    pointMapComplete_ = true;

    const Map<label>& pmm = pointMergeMap();

    // Get the mesh points for both patches.  If the point has not been
    // merged away, add it to the map

    // Do master patch
    const labelList& masterMeshPoints = masterPatch_.meshPoints();
    const pointField& masterLocalPoints = masterPatch_.localPoints();

    forAll(masterMeshPoints, pointI)
    {
        if (!pmm.found(masterMeshPoints[pointI]))
        {
            pointMap_.insert
            (
                masterMeshPoints[pointI],
                masterLocalPoints[pointI]
            );
        }
    }

    // Do slave patch
    const labelList& slaveMeshPoints = slavePatch_.meshPoints();
    const pointField& slaveLocalPoints = slavePatch_.localPoints();

    forAll(slaveMeshPoints, pointI)
    {
        if (!pmm.found(slaveMeshPoints[pointI]))
        {
            pointMap_.insert
            (
                slaveMeshPoints[pointI],
                slaveLocalPoints[pointI]
            );
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


CML::Map<CML::point>& CML::enrichedPatch::pointMap()
{
    if (!pointMapComplete_)
    {
        completePointMap();
    }

    return pointMap_;
}


const CML::Map<CML::point>& CML::enrichedPatch::pointMap() const
{
    if (!pointMapComplete_)
    {
        completePointMap();
    }

    return pointMap_;
}


// ************************************************************************* //
