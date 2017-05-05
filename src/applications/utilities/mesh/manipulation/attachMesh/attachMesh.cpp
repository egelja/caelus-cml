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
    attachMesh

Description
    Attach topologically detached mesh using prescribed mesh modifiers.

\*---------------------------------------------------------------------------*/

#include "argList.hpp"
#include "polyMesh.hpp"
#include "Time.hpp"
#include "attachPolyTopoChanger.hpp"

using namespace CML;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
#   include "addOverwriteOption.hpp"
    argList::noParallel();

#   include "setRootCase.hpp"
#   include "createTime.hpp"
    runTime.functionObjects().off();
#   include "createPolyMesh.hpp"
    const word oldInstance = mesh.pointsInstance();

    const bool overwrite = args.optionFound("overwrite");

    if (!overwrite)
    {
        runTime++;
    }

    Info<< "Time = " << runTime.timeName() << nl
        << "Attaching sliding interface" << endl;

    attachPolyTopoChanger(mesh).attach();

    if (overwrite)
    {
        mesh.setInstance(oldInstance);
    }
    mesh.write();

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
