/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2017 OpenFOAM Foundation
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
    pimpleSolver.cpp

Description
    Large time-step transient solver for incompressible, flow of Newtonian fluids
    on a moving or static mesh using the PIMPLE (merged PISO-SIMPLE) algorithm.

    Turbulence modelling is generic, i.e. laminar, RAS or LES may be selected.

\*---------------------------------------------------------------------------*/

#include "fvCFD.hpp"
#include "dynamicFvMesh.hpp"
#include "singlePhaseTransportModel.hpp"
#include "turbulenceModel.hpp"
#include "pimpleControl.hpp"
#include "fvIOoptionList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.hpp"
    #include "createTime.hpp"
    #include "createDynamicFvMesh.hpp"

    #include "createFields.hpp"
    #include "createMRF.hpp"
    #include "createUfIfPresent.hpp"
    #include "createFvOptions.hpp"
    #include "createPcorrTypes.hpp"
    #include "initContinuityErrs.hpp"

    pimpleControl pimple(mesh);
    #include "createDyMControls.hpp"
    #include "CourantNo.hpp"
    #include "setInitialDeltaT.hpp"

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    while (runTime.run())
    {
        #include "readDyMControls.hpp"
        #include "CourantNo.hpp"
        #include "setDeltaT.hpp"

        runTime++;

        Info<< "Time = " << runTime.timeName() << nl << endl;

        // --- Pressure-velocity PIMPLE corrector loop
        while (pimple.loop())
        {
            if (pimple.firstIter() || moveMeshOuterCorrectors)
            {
                mesh.update();

                if (mesh.changing())
                {
                    if (correctPhi)
                    {
                        // Calculate absolute flux 
                        // from the mapped surface velocity
                        phi = mesh.Sf() & UfPtr();

                        #include "correctPhi.hpp"

                        // Make the flux relative to the mesh motion
                        fvc::makeRelative(phi, U);
                    }

                    if (checkMeshCourantNo)
                    {
                        #include "meshCourantNo.hpp"
                    }
                }
            }

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

        #include "reportTimeStats.hpp"
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
