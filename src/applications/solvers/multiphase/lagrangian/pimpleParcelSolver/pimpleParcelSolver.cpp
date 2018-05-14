/*----------------------------------------------------------------------------- 

Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2017-2018 Applied CCM
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
    pimpleParcelSolver

Description
    Large time-step transient solver for incompressible, flow using the PIMPLE
    (merged PISO-SIMPLE) algorithm.

    Turbulence modelling is generic, i.e. laminar, RAS or LES may be selected.

Author(s)
    Aleksandar Jemcov
    Darrin Stephens
\*---------------------------------------------------------------------------*/

#include "fvCFD.hpp"
#include "singlePhaseTransportModel.hpp"
#include "turbulenceModel.hpp"
#include "basicKinematicCloud.hpp"
#include "pimpleControl.hpp"
#include "fvIOoptionList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addOption
    (
        "cloudName",
        "name",
        "specify alternative cloud name prefix. default is 'kinematicCloud'"
    );

    #include "setRootCase.hpp"
    #include "createTime.hpp"
    #include "createMesh.hpp"
    #include "createTimeControls.hpp"
    #include "readGravitationalAcceleration.hpp"
    #include "createFields.hpp"
    #include "createMRF.hpp"
    #include "createClouds.hpp"
    #include "createFvOptions.hpp"
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

	    for (label i = 0; i < nClouds; ++i)
        {
            kinematicClouds[i].evolve();
        }

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

	    // Update molecular viscosity
	    mu = rhoInf*turbulence->nu();

        runTime.write();

        #include "reportTimeStats.hpp"
    }

    Info<< "End\n" << endl;

    return 0;
}


