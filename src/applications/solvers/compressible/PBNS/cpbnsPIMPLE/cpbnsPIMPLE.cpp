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

    cpbnsPIMPLE

Description

    A fractional step based transient solver for compressible flows

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

int main(int argc, char *argv[])
{
    #include "setRootCase.hpp"
    #include "createTime.hpp"
    #include "createMesh.hpp"

    pimpleControl pimple(mesh);

    #include "createTimeControls.hpp"
    #include "createFields.hpp"
    #include "createMRF.hpp"
    #include "createFvOptions.hpp"
    #include "initContinuityErrs.hpp"

    Info<< "\nStarting time loop\n" << endl;

    while (runTime.run())
    {
        #include "readTimeControls.hpp"
        #include "cfl.hpp"
        #include "setDeltaT.hpp"

        runTime++;

        Info<< "Time = " << runTime.timeName() << nl << endl;

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
        }

        turbulence->correct();

        Ek = 0.5*magSqr(U);
        EkMatDer = fvc::ddt(rho,Ek)+fvc::div(phi,Ek);

        runTime.write();

        #include "reportTimeStats.hpp"
    }

    Info<< "End\n" << endl;

    return 0;
}

