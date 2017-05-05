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
    postCalc

Description
    Generic wrapper for calculating a quantity at each time

\*---------------------------------------------------------------------------*/

#include "calc.hpp"
#include "timeSelector.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    void tryCalc(const argList& args, const Time& runTime, const fvMesh& mesh)
    {
        FatalIOError.throwExceptions();

        try
        {
            calc(args, runTime, mesh);
        }
        catch(IOerror& err)
        {
            Warning<< err << endl;
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    CML::timeSelector::addOptions();
    #include "addRegionOption.hpp"
    CML::argList::addBoolOption
    (
        "noWrite",
        "suppress writing results"
    );
    #include "addDictOption.hpp"

    #include "setRootCase.hpp"
    #include "createTime.hpp"
    CML::instantList timeDirs = CML::timeSelector::select0(runTime, args);
    #include "createNamedMesh.hpp"

    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);

        CML::Info<< "Time = " << runTime.timeName() << CML::endl;

        mesh.readUpdate();

        CML::tryCalc(args, runTime, mesh);

        CML::Info<< CML::endl;
    }

    return 0;
}


// ************************************************************************* //
