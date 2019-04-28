/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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
    Update the polyMesh corresponding to the given map.

\*---------------------------------------------------------------------------*/

#include "polyMesh.hpp"
#include "mapPolyMesh.hpp"
#include "Time.hpp"
#include "globalMeshData.hpp"
#include "pointMesh.hpp"
#include "indexedOctree.hpp"
#include "treeDataCell.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::polyMesh::updateMesh(const mapPolyMesh& mpm)
{
    if (debug)
    {
        Info<< "void polyMesh::updateMesh(const mapPolyMesh&) : "
            << "updating addressing and (optional) pointMesh/pointFields"
            << endl;
    }

    // Update boundaryMesh (note that patches themselves already ok)
    boundary_.updateMesh();

    // Update zones
    pointZones_.clearAddressing();
    faceZones_.clearAddressing();
    cellZones_.clearAddressing();

    // Remove the stored tet base points
    tetBasePtIsPtr_.clear();
    // Remove the cell tree
    cellTreePtr_.clear();

    // Update parallel data
    if (globalMeshDataPtr_.valid())
    {
        globalMeshDataPtr_->updateMesh();
    }

    setInstance(time().timeName());

    // Map the old motion points if present
    if (oldPointsPtr_.valid())
    {
        // Make a copy of the original points
        pointField oldMotionPoints = oldPointsPtr_();

        pointField& newMotionPoints = oldPointsPtr_();

        // Resize the list to new size
        newMotionPoints.setSize(points_.size());

        // Map the list
        newMotionPoints.map(oldMotionPoints, mpm.pointMap());

        // Any points created out-of-nothing get set to the current coordinate
        // for lack of anything better.
        forAll(mpm.pointMap(), newPointi)
        {
            if (mpm.pointMap()[newPointi] == -1)
            {
                newMotionPoints[newPointi] = points_[newPointi];
            }
        }
    }

    if (oldCellCentresPtr_.valid())
    {
        // Make a copy of the original cell-centres
        pointField oldMotionCellCentres = oldCellCentresPtr_();

        pointField& newMotionCellCentres = oldCellCentresPtr_();

        // Resize the list to new size
        newMotionCellCentres.setSize(cellCentres().size());

        // Map the list
        newMotionCellCentres.map(oldMotionCellCentres, mpm.cellMap());

        // Any points created out-of-nothing get set to the current coordinate
        // for lack of anything better.
        forAll(mpm.cellMap(), newCelli)
        {
            if (mpm.cellMap()[newCelli] == -1)
            {
                newMotionCellCentres[newCelli] = cellCentres()[newCelli];
            }
        }
    }

    // Reset valid directions (could change by faces put into empty patches)
    geometricD_ = Vector<label>::zero;
    solutionD_ = Vector<label>::zero;


    // Hack until proper callbacks. Below are all the polyMesh-MeshObjects.

    // pointMesh
    if (thisDb().foundObject<pointMesh>(pointMesh::typeName))
    {
        const_cast<pointMesh&>
        (
            thisDb().lookupObject<pointMesh>
            (
                pointMesh::typeName
            )
        ).updateMesh(mpm);
    }
}


// ************************************************************************* //
