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
    Solver for moving meshes

\*---------------------------------------------------------------------------*/

#include "argList.hpp"
#include "Time.hpp"
#include "fvMesh.hpp"
#include "motionSolver.hpp"

using namespace CML;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:

int main(int argc, char *argv[])
{

#   include "setRootCase.hpp"
#   include "createTime.hpp"
#   include "createMesh.hpp"

    autoPtr<motionSolver> motionPtr = motionSolver::New(mesh);

    while (runTime.loop())
    {
        Info<< "Time = " << runTime.timeName() << endl;

        mesh.movePoints(motionPtr->newPoints());

        runTime.write();

#ifdef _WIN32
        Info<< "ExecutionTime = " << static_cast<scalar>(runTime.elapsedCpuTime()) << " s"
            << "  ClockTime = " << static_cast<scalar>(runTime.elapsedClockTime()) << " s"
#else
        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
#endif
            << nl << endl;
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
