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
    caelusCalc

Description
    Generic wrapper for calculating a quantity at each time.

    Split into four phases:
        1. Initialise
        2. Pre-time calculation loop
        3. Calculation loop
        4. Post-calculation loop

\*---------------------------------------------------------------------------*/

#include "timeSelector.hpp"
#include "calcType.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    CML::timeSelector::addOptions();
#   include "addRegionOption.hpp"
    CML::argList::addBoolOption
    (
        "noWrite",
        "suppress writing results"
    );
    CML::argList::addOption
    (
        "dict",
        "name",
        "dictionary to use"
    );

    if (argc < 2)
    {
        FatalError
            << "No utility has been supplied" << nl
            << exit(FatalError);
    }

    const word utilityName = argv[1];

    CML::autoPtr<CML::calcType> utility
    (
        calcType::New(utilityName)
    );

    utility().tryInit();

#   include "setRootCase.hpp"
#   include "createTime.hpp"
    CML::instantList timeDirs = CML::timeSelector::select0(runTime, args);
#   include "createNamedMesh.hpp"

    utility().tryPreCalc(args, runTime, mesh);

    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);

        CML::Info<< "Time = " << runTime.timeName() << CML::endl;

        mesh.readUpdate();

        utility().tryCalc(args, runTime, mesh);

        CML::Info<< CML::endl;
    }

    utility().tryPostCalc(args, runTime, mesh);

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
