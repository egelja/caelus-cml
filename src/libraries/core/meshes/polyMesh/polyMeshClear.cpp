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

\*---------------------------------------------------------------------------*/

#include "polyMesh.hpp"
#include "primitiveMesh.hpp"
#include "globalMeshData.hpp"
#include "pointMesh.hpp"
#include "Time.hpp"
#include "indexedOctree.hpp"
#include "treeDataCell.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::polyMesh::removeBoundary()
{
    if (debug)
    {
        Info<< "void polyMesh::removeBoundary(): "
            << "Removing boundary patches."
            << endl;
    }

    // Remove the point zones
    boundary_.clear();
    boundary_.setSize(0);

    clearOut();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::polyMesh::clearGeom()
{
    if (debug)
    {
        Info<< "void polyMesh::clearGeom() : "
            << "clearing geometric data"
            << endl;
    }

    primitiveMesh::clearGeom();

    forAll(boundary_, patchI)
    {
        boundary_[patchI].clearGeom();
    }

    // Reset valid directions (could change with rotation)
    geometricD_ = Vector<label>::zero;
    solutionD_ = Vector<label>::zero;

    // Remove the stored tet base points
    tetBasePtIsPtr_.clear();
    // Remove the cell tree
    cellTreePtr_.clear();
}


void CML::polyMesh::clearAdditionalGeom()
{
    if (debug)
    {
        Info<< "void polyMesh::clearAdditionalGeom() : "
            << "clearing additional geometric data"
            << endl;
    }

    // Remove the stored tet base points
    tetBasePtIsPtr_.clear();
    // Remove the cell tree
    cellTreePtr_.clear();
}


void CML::polyMesh::clearAddressing()
{
    if (debug)
    {
        Info<< "void polyMesh::clearAddressing() : "
            << "clearing topology"
            << endl;
    }

    primitiveMesh::clearAddressing();

    // parallelData depends on the processorPatch ordering so force
    // recalculation
    globalMeshDataPtr_.clear();

    // Reset valid directions
    geometricD_ = Vector<label>::zero;
    solutionD_ = Vector<label>::zero;

    // Update zones
    pointZones_.clearAddressing();
    faceZones_.clearAddressing();
    cellZones_.clearAddressing();

    // Remove the stored tet base points
    tetBasePtIsPtr_.clear();
    // Remove the cell tree
    cellTreePtr_.clear();
}


void CML::polyMesh::clearPrimitives()
{
    resetMotion();

    points_.setSize(0);
    faces_.setSize(0);
    owner_.setSize(0);
    neighbour_.setSize(0);

    pointMesh::Delete(*this);

    clearedPrimitives_ = true;
}


void CML::polyMesh::clearOut()
{
    clearGeom();
    clearAddressing();

    pointMesh::Delete(*this);
}


void CML::polyMesh::clearCellTree()
{
    if (debug)
    {
        Info<< "void polyMesh::clearCellTree() : "
            << "clearing cell tree"
            << endl;
    }

    cellTreePtr_.clear();
}


// ************************************************************************* //
