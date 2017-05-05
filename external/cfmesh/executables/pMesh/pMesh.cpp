/*---------------------------------------------------------------------------*\
Copyright (C) Creative Fields, Ltd.
-------------------------------------------------------------------------------
License
    This file is part of cfMesh.

    cfMesh is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    cfMesh is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with cfMesh.  If not, see <http://www.gnu.org/licenses/>.

Application
    Generates polyhedral mesh

Author: Franjo Juretic (franjo.juretic@c-fields.com)

Description
    - takes a triangulated surface and generates a body-fitted polyhedral mesh

\*---------------------------------------------------------------------------*/

#include "argList.hpp"
#include "voronoiMeshGenerator.hpp"

using namespace CML;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Main program:

int main(int argc, char *argv[])
{
#   include "setRootCase.hpp"
#   include "createTime.hpp"

    //- polyhedral mesher cannot be run in parallel yet
    argList::noParallel();

    voronoiMeshGenerator pmg(runTime);

    Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s\n"
        << "ClockTime = " << runTime.elapsedClockTime() << endl;

    pmg.writeMesh();

    Info << "End\n" << endl;
    return 0;
}

// ************************************************************************* //
