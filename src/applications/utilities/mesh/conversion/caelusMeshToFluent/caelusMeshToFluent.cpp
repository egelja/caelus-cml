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

Description
    Writes out the CAELUS mesh in Fluent mesh format.

\*---------------------------------------------------------------------------*/

#include "argList.hpp"
#include "Time.hpp"
#include "fluentFvMesh.hpp"

using namespace CML;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:

int main(int argc, char *argv[])
{
    argList::noParallel();
#   include "setRootCase.hpp"
#   include "createTime.hpp"

    Info<< "Create mesh for time = "
        << runTime.timeName() << nl << endl;

    fluentFvMesh mesh
    (
        IOobject
        (
            fluentFvMesh::defaultRegion,
            runTime.constant(),
            runTime
        )
    );

    mesh.writeFluentMesh();

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
