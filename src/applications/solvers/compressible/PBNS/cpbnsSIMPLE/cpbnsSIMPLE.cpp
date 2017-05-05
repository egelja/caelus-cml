/*---------------------------------------------------------------------------*\
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

Description
    Compressible solver based on SIMPLE algorithm. 

Author
    Aleksandar Jemcov
\*---------------------------------------------------------------------------*/


#include "fvCFD.hpp"
#include "basicPsiThermo.hpp"
#include "RASModel.hpp"
#include "simpleControl.hpp"
#include "fvIOoptionList.hpp"

int main(int argc, char *argv[])
{
    #include "setRootCase.hpp"
    #include "createTime.hpp"
    #include "createMesh.hpp"

    simpleControl simple(mesh);

    #include "createFields.hpp"
    #include "createFvOptions.hpp"

    Info<< "\nStarting time loop\n" << endl;

    while (simple.loop())
    {
        Info<< "Time = " << runTime.timeName() << nl << endl;

        // Predictor-corrector algorithm
        {
            #include "UEqn.hpp"
            #include "hEqn.hpp"
            #include "pEqn.hpp"
        }

        turbulence->correct();

        runTime.write();

        #include "reportTimeStats.hpp"
    }

    Info<< "End\n" << endl;

    return 0;
}


