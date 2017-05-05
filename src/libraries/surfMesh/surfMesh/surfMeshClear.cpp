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

#include "surfMesh.hpp"
#include "globalMeshData.hpp"
#include "demandDrivenData.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::surfMesh::removeZones()
{
    if (debug)
    {
        Info<< "void surfMesh::removeZones(): "
            << "Removing surface zones."
            << endl;
    }

    // Remove the surface zones
    storedZones().clear();

    clearOut();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::surfMesh::clearGeom()
{
    if (debug)
    {
        Info<< "void surfMesh::clearGeom() : "
            << "clearing geometric data"
            << endl;
    }

    MeshReference::clearGeom();
}


void CML::surfMesh::clearAddressing()
{
    if (debug)
    {
        Info<< "void surfMesh::clearAddressing() : "
            << "clearing topology"
            << endl;
    }

    MeshReference::clearPatchMeshAddr();
}


void CML::surfMesh::clearOut()
{
    clearGeom();
    clearAddressing();
}


// ************************************************************************* //
