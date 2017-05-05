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
    SRFSimpleFoam

Description
    Steady-state solver for incompressible, turbulent flow of non-Newtonian
    fluids in a single rotating frame.

\*---------------------------------------------------------------------------*/

#include "fvCFD.hpp"
#include "singlePhaseTransportModel.hpp"
#include "RASModel.hpp"
#include "SRFModel.hpp"
#include "simpleControl.hpp"
#include "IObasicSourceList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.hpp"
    #include "createTime.hpp"
    #include "createMesh.hpp"
    #include "createFields.hpp"
    #include "initContinuityErrs.hpp"

    simpleControl simple(mesh);

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    while (simple.loop())
    {
        Info<< "Time = " << runTime.timeName() << nl << endl;

        // --- Pressure-velocity SIMPLE corrector
        {
            #include "UrelEqn.hpp"
            #include "pEqn.hpp"
        }

        turbulence->correct();

        if (runTime.outputTime())
        {
            volVectorField Uabs
            (
                IOobject
                (
                    "Uabs",
                    runTime.timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                Urel + SRF->U()
            );

            runTime.write();
        }

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
