/*---------------------------------------------------------------------------*\
Copyright (C) 2015 - 2018 Applied CCM
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

    cpbnsLTS

Description

    An LTS based steady state solver for compressible flows

Author

    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/
#include "fvCFD.hpp"
#include "psiThermo.hpp"
#include "compressibleTurbulenceModel.hpp"
#include "fvcSmooth.hpp"
#include "pimpleControl.hpp"
#include "fvIOoptionList.hpp"
#include "bound.hpp"
#include "localEulerDdtScheme.hpp"

int main(int argc, char *argv[])
{
    #include "setRootCase.hpp"
    #include "createTime.hpp"
    #include "createMesh.hpp"

    pimpleControl pimple(mesh);

    #include "createFields.hpp"
    #include "createMRF.hpp"
    #include "createFvOptions.hpp"
    #include "initContinuityErrs.hpp"

    Info<< "\nStarting time loop\n" << endl;

    while (runTime.run())
    {
        runTime++;

        Info<< "Time = " << runTime.timeName() << nl << endl;

        #include "setrDeltaT.hpp"

        #include "rhoEqn.hpp"

        while (pimple.loop())
        {

            #include "UEqn.hpp"
            #include "hEqn.hpp"

            // --- Pressure corrector loop
            while (pimple.correct())
            {
                #include "pEqn.hpp"
            }

            turbulence->correct();

        }

        runTime.write();

        #include "reportTimeStats.hpp"
    }

    Info<< "End\n" << endl;

    return 0;
}

