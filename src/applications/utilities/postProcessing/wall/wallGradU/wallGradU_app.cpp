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
    wallGradU

Description
    Calculates and writes the gradient of U at the wall

\*---------------------------------------------------------------------------*/

#include "fvCFD.hpp"
#include "wallFvPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    timeSelector::addOptions();
    #include "setRootCase.hpp"
    #include "createTime.hpp"
    instantList timeDirs = timeSelector::select0(runTime, args);
    #include "createMesh.hpp"

    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);
        Info<< "Time = " << runTime.timeName() << endl;

        IOobject Uheader
        (
            "U",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ
        );

        // Check U exists
        if (Uheader.headerOk())
        {
            mesh.readUpdate();

            Info<< "    Reading U" << endl;
            volVectorField U(Uheader, mesh);

            Info<< "    Calculating wallGradU" << endl;

            volVectorField wallGradU
            (
                IOobject
                (
                    "wallGradU",
                    runTime.timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                mesh,
                dimensionedVector
                (
                    "wallGradU",
                    U.dimensions()/dimLength,
                    vector::zero
                )
            );

            const fvPatchList& patches = mesh.boundary();

            forAll(wallGradU.boundaryField(), patchi)
            {
                const fvPatch& currPatch = patches[patchi];

                if (isA<wallFvPatch>(currPatch))
                {
                    wallGradU.boundaryField()[patchi] =
                        -U.boundaryField()[patchi].snGrad();
                }
            }

            wallGradU.write();
        }
        else
        {
            Info<< "    No U" << endl;
        }
    }

    Info<< "End" << endl;

    return 0;
}


// ************************************************************************* //
