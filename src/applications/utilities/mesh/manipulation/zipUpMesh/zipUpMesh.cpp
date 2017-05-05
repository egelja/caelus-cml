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
    zipUpMesh

Description
    Reads in a mesh with hanging vertices and zips up the cells to guarantee
    that all polyhedral cells of valid shape are closed.

    Meshes with hanging vertices may occur as a result of split
    hex mesh conversion or integration or coupled math face pairs.

\*---------------------------------------------------------------------------*/

#include "argList.hpp"
#include "Time.hpp"
#include "polyMesh.hpp"
#include "polyMeshZipUpCells.hpp"
#include "boolList.hpp"

using namespace CML;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
#   include "addRegionOption.hpp"

#   include "setRootCase.hpp"
#   include "createTime.hpp"
#   include "createNamedPolyMesh.hpp"

    if (polyMeshZipUpCells(mesh))
    {
        Info<< "Writing zipped-up polyMesh to " << mesh.facesInstance() << endl;
        mesh.write();
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
