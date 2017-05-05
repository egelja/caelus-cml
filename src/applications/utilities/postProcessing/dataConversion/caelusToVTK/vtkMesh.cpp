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

#include "vtkMesh.hpp"
#include "fvMeshSubset.hpp"
#include "Time.hpp"
#include "cellSet.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::vtkMesh::vtkMesh
(
    fvMesh& baseMesh,
    const word& setName
)
:
    baseMesh_(baseMesh),
    subsetter_(baseMesh),
    setName_(setName)
{
    if (setName.size())
    {
        // Read cellSet using whole mesh
        cellSet currentSet(baseMesh_, setName_);

        // Set current subset
        subsetter_.setLargeCellSubset(currentSet);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::polyMesh::readUpdateState CML::vtkMesh::readUpdate()
{
    polyMesh::readUpdateState meshState = baseMesh_.readUpdate();

    if (meshState != polyMesh::UNCHANGED)
    {
        // Note: since fvMeshSubset has no movePoints() functionality,
        // reconstruct the subset even if only movement.

        topoPtr_.clear();

        if (setName_.size())
        {
            Info<< "Subsetting mesh based on cellSet " << setName_ << endl;

            // Read cellSet using whole mesh
            cellSet currentSet(baseMesh_, setName_);

            subsetter_.setLargeCellSubset(currentSet);
        }
    }

    return meshState;
}


// ************************************************************************* //
