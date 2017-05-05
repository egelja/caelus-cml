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

#include "attachPolyTopoChanger.hpp"
#include "polyMesh.hpp"
#include "polyTopoChange.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::attachPolyTopoChanger::attachPolyTopoChanger
(
    const IOobject& io,
    polyMesh& mesh
)
:
    polyTopoChanger(io, mesh)
{}


CML::attachPolyTopoChanger::attachPolyTopoChanger
(
    polyMesh& mesh
)
:
    polyTopoChanger(mesh)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Attach mesh
void CML::attachPolyTopoChanger::attach(const bool removeEmptyPatches)
{
    if (debug)
    {
        Pout<< "void attachPolyTopoChanger::attach(): "
            << "Attaching mesh" << endl;
    }

    // Save current file instance
    const fileName oldInst = mesh_.facesInstance();

    // Execute all polyMeshModifiers
    changeMesh(false);  // no inflation

    const pointField p = mesh_.oldPoints();

    mesh_.movePoints(p);

    if (debug)
    {
        Pout<< "Clearing mesh." << endl;
    }

    if (removeEmptyPatches)
    {
        // Re-do the boundary patches, removing the ones with zero size
        const polyBoundaryMesh& oldPatches = mesh_.boundaryMesh();

        List<polyPatch*> newPatches(oldPatches.size());
        label nNewPatches = 0;

        forAll(oldPatches, patchI)
        {
            if (oldPatches[patchI].size())
            {
                newPatches[nNewPatches] = oldPatches[patchI].clone
                (
                    mesh_.boundaryMesh(),
                    nNewPatches,
                    oldPatches[patchI].size(),
                    oldPatches[patchI].start()
                ).ptr();

                nNewPatches++;
            }
            else
            {
                if (debug)
                {
                    Pout<< "Removing zero-sized patch " << patchI
                        << " named " << oldPatches[patchI].name() << endl;
                }
            }
        }

        newPatches.setSize(nNewPatches);

        mesh_.removeBoundary();
        mesh_.addPatches(newPatches);
    }

    // Reset the file instance to overwrite the original mesh
    mesh_.setInstance(oldInst);

    if (debug)
    {
        Pout<< "void attachPolyTopoChanger::attach(): "
            << "Finished attaching mesh" << endl;
    }

    mesh_.checkMesh();
}


// ************************************************************************* //
