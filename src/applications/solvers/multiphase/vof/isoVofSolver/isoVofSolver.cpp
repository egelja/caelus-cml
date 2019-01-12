/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2017 OpenFOAM Foundation
Copyright (C) 2017 OpenCFD Ltd
Copyright (C) 2016 DHI
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
    interIsoFoam

Group
    grpMultiphaseSolvers

Description
    Solver derived from interFoam for 2 incompressible, isothermal immiscible
    fluids using the iso-advector phase-fraction based interface capturing
    approach.

    The momentum and other fluid properties are of the "mixture" and a single
    momentum equation is solved.

    Turbulence modelling is generic, i.e. laminar, RAS or LES may be selected.

    For a two-fluid approach see twoPhaseEulerFoam.

    Reference:
        \verbatim
            Roenby, J., Bredmose, H. and Jasak, H. (2016).
            A computational method for sharp interface advection
            Royal Society Open Science, 3
            doi 10.1098/rsos.160405
        \endverbatim

    isoAdvector code supplied by Johan Roenby, DHI (2016)

\*---------------------------------------------------------------------------*/

#include "isoAdvection.hpp"
#include "fvCFD.hpp"
#include "localEulerDdtScheme.hpp"
#include "subCycle.hpp"
#include "interfaceProperties.hpp"
#include "twoPhaseMixture.hpp"
#include "turbulenceModel.hpp"
#include "interpolationTable.hpp"
#include "pimpleControl.hpp"
#include "fvIOoptionList.hpp"
#include "fvcSmooth.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.hpp"
    #include "createTime.hpp"
    #include "createMesh.hpp"

    pimpleControl pimple(mesh);

    #include "createTimeControls.hpp"
    #include "initContinuityErrs.hpp"
    #include "createFields.hpp"
    #include "createMRF.hpp"
    #include "correctPhi.hpp"

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

        // --- Pressure-velocity PIMPLE corrector loop
        while (pimple.loop())
        {
            #include "alphaControls.hpp"
            #include "alphaEqnSubCycle_isoVoF.hpp"

            mixture.correct();
            interface.correct();

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
