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
    wdot

Description
    Calculates and writes wdot for each time.

\*---------------------------------------------------------------------------*/

#include "fvCFD.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    timeSelector::addOptions();

#   include "setRootCase.hpp"
#   include "createTime.hpp"

    instantList timeDirs = timeSelector::select0(runTime, args);

#   include "createMesh.hpp"

    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);

        mesh.readUpdate();

        volScalarField mgb
        (
            IOobject
            (
                "mgb",
                runTime.timeName(),
                mesh,
                IOobject::MUST_READ
            ),
            mesh
        );

        volScalarField Su
        (
            IOobject
            (
                "Su",
                runTime.timeName(),
                mesh,
                IOobject::MUST_READ
            ),
            mesh
        );

        volScalarField Xi
        (
            IOobject
            (
                "Xi",
                runTime.timeName(),
                mesh,
                IOobject::MUST_READ
            ),
            mesh
        );

        volScalarField St
        (
            IOobject
            (
                "St",
                runTime.timeName(),
                mesh,
                IOobject::NO_READ
            ),
            Xi*Su
        );

        St.write();

        volScalarField wdot
        (
            IOobject
            (
                "wdot",
                runTime.timeName(),
                mesh,
                IOobject::NO_READ
            ),
           St*mgb
        );

        wdot.write();
    }

    return 0;
}


// ************************************************************************* //
