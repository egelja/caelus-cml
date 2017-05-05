/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2013 OpenFOAM Foundation
Copyright (C) 2015 Applied CCM
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
    vofSolverLPT

Description
    Solver for 2 incompressible, isothermal immiscible fluids using a VOF
    (volume of fluid) phase-fraction based interface capturing approach.

    The momentum and other fluid properties are of the "mixture" and a single
    momentum equation is solved.

    Turbulence modelling is generic, i.e. laminar, RAS or LES may be selected.
    Coupled with lagrangian particle tracking
---------------------------------------------------------------------------*/

#include "fvCFD.hpp"
#include "MULES.hpp"
#include "subCycle.hpp"
#include "interfaceProperties.hpp"
#include "twoPhaseMixture.hpp"
#include "turbulenceModel.hpp"
#include "interpolationTable.hpp"
#include "pimpleControl.hpp"
#include "fvIOoptionList.hpp"
#include "fixedFluxPressureFvPatchScalarField.hpp"
#include "basicKinematicCloud.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.hpp"
    #include "createTime.hpp"
    #include "createMesh.hpp"

    pimpleControl pimple(mesh);

    #include "initContinuityErrs.hpp"
    #include "createFields.hpp"
    #include "createClouds.hpp"
    #include "readTimeControls.hpp"
    #include "correctPhi.hpp"
    #include "CourantNo.hpp"
    #include "setInitialDeltaT.hpp"


    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    while (runTime.run())
    {
        #include "readTimeControls.hpp"
        #include "CourantNo.hpp"
        #include "alphaCourantNo.hpp"
        #include "setDeltaT.hpp"

        runTime++;

        Info<< "Time = " << runTime.timeName() << nl << endl;

        Info<< "Evolving bubbles" << nl << endl;
	    bubbleCloud.evolve();

        // Update alphaD from the particle locations
        alphaD = bubbleCloud.theta();
        alphaD.correctBoundaryConditions();

        // Update Ud velocity on mesh
        Ud = bubbleCloud.U();
        Ud.correctBoundaryConditions();

        twoPhaseProperties.correct();

        #include "alphaEqnSubCycle.hpp"

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

   	    // Update molecular viscosity used in bubble tracking
	    mu = twoPhaseProperties.mu();

        runTime.write();

        #include "reportTimeStats.hpp" 
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
