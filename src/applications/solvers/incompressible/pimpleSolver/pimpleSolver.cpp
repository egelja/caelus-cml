/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Application
    pimpleSolver

Description
    Large time-step transient solver for incompressible, flow using the PIMPLE
    (merged PISO-SIMPLE) algorithm.

    Turbulence modelling is generic, i.e. laminar, RAS or LES may be selected.

\*---------------------------------------------------------------------------*/

#include "fvCFD.hpp"
#include "singlePhaseTransportModel.hpp"
#include "turbulenceModel.hpp"
#include "pimpleControl.hpp"
#include "IObasicSourceList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.hpp"
    #include "createTime.hpp"
    #include "createMesh.hpp"
    #include "createFields.hpp"
    #include "initContinuityErrs.hpp"

    pimpleControl pimple(mesh);

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    while (runTime.run())
    {
        #include "readTimeControls.hpp"
        #include "CourantNo.hpp"
        #include "setDeltaT.hpp"

        runTime++;

        Info<< "Time = " << runTime.timeName() << nl << endl;

        // --- Pressure-velocity PIMPLE corrector loop
        while (pimple.loop())
        {
            #include "UEqn.hpp"

            // --- Pressure corrector loop
            while (pimple.correct())
            {
                #include "pEqn.hpp"
            }

            if (pimple.turbCorr())
            {
                turbulence->correct();
            }
        }

        runTime.write();

#ifdef windows
        Info<< "ExecutionTime = " << static_cast<double>(runTime.elapsedCpuTime()) << " s"
            << "  ClockTime = " << static_cast<double>(runTime.elapsedClockTime()) << " s"
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
