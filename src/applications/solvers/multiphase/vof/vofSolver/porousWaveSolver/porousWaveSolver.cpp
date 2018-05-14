/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
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
    porousWaveSolver

Description
    Solver for 2 incompressible, isothermal immiscible fluids using a VOF
    (volume of fluid) phase-fraction based interface capturing approach.

    The momentum and other fluid properties are of the "mixture" and a single
    momentum equation is solved.

    Turbulence modelling is generic, i.e. laminar, RAS or LES may be selected.
---------------------------------------------------------------------------*/

#include "fvCFD.hpp"
#include "CMULES.hpp"
#include "EulerDdtScheme.hpp"
#include "localEulerDdtScheme.hpp"
#include "CrankNicolsonDdtScheme.hpp"
#include "subCycle.hpp"
#include "interfaceProperties.hpp"
#include "twoPhaseMixture.hpp"
#include "turbulenceModel.hpp"
#include "interpolationTable.hpp"
#include "pimpleControl.hpp"
#include "fvIOoptionList.hpp"
#include "fixedFluxPressureFvPatchScalarField.hpp"
#include "CorrectPhi_.hpp"
#include "fvcSmooth.hpp"

#include "relaxationZone.hpp"
#include "externalWaveForcing.hpp"
#include "wavesPorosityModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.hpp"
    #include "createTime.hpp"
    #include "createMesh.hpp"

    pimpleControl pimple(mesh);

    #include "initContinuityErrs.hpp"
    #include "createDyMControls.hpp"
    #include "createPorosityFields.hpp"
    #include "createFields.hpp"
    #include "createMRF.hpp"
    #include "createFvOptions.hpp"
    #include "initCorrectPhi.hpp"
    #include "createUfIfPresent.hpp"

    if (!LTS)
    {
        #include "readTimeControls.hpp"
        #include "CourantNo.hpp"
        #include "setInitialDeltaT.hpp"
    }

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    while (runTime.run())
    {
        #include "readTimeControls.hpp"

        if (LTS)
        {
            #include "setRDeltaTVoF.hpp"
        }
        else
        {
            #include "CourantNo.hpp"
            #include "alphaCourantNo.hpp"
            #include "setDeltaTVoF.hpp"
        }

        runTime++;

        Info<< "Time = " << runTime.timeName() << nl << endl;

        if (waves)
        {
            externalWave->step();
        }

        #include "calcPorosity.hpp"

        // --- Pressure-velocity PIMPLE corrector loop
        while (pimple.loop())
        {
            #include "alphaControls.hpp"
            #include "alphaEqnSubCycle.hpp"

            if (waves)
            {
                relaxing->correct();
            }

            mixture.correct();

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

    if (waves)
    {
        // Close down the external wave forcing in a nice manner
        externalWave->close();
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
