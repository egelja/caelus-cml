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
    Probe locations.

\*---------------------------------------------------------------------------*/

#include "argList.hpp"
#include "timeSelector.hpp"
#include "IOprobes.hpp"

using namespace CML;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:

int main(int argc, char *argv[])
{
    timeSelector::addOptions();
#   include "addRegionOption.hpp"

#   include "setRootCase.hpp"
#   include "createTime.hpp"
    instantList timeDirs = timeSelector::select0(runTime, args);
#   include "createNamedMesh.hpp"

    IOprobes sniff
    (
        probes::typeName,
        mesh,
        "probesDict",
        IOobject::MUST_READ,
        true
    );

    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);
        Info<< "Time = " << runTime.timeName() << endl;

        // Handle geometry/topology changes
        polyMesh::readUpdateState state = mesh.readUpdate();

        if
        (
            state == polyMesh::POINTS_MOVED
         || state == polyMesh::TOPO_CHANGE
        )
        {
            sniff.read();
        }

        sniff.write();

        Info<< endl;
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
