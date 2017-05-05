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
    deformedGeom

Description
    Deforms a polyMesh using a displacement field U and a scaling factor
    supplied as an argument.

\*---------------------------------------------------------------------------*/

#include "argList.hpp"
#include "fvMesh.hpp"
#include "pointFields.hpp"
#include "IStringStream.hpp"
#include "volPointInterpolation.hpp"

using namespace CML;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::validArgs.append("scaling factor");

#   include "setRootCase.hpp"

    const scalar scaleFactor = args.argRead<scalar>(1);

#   include "createTime.hpp"
#   include "createMesh.hpp"

    volPointInterpolation pInterp(mesh);

    // Get times list
    instantList Times = runTime.times();

    pointField zeroPoints(mesh.points());

    // skip "constant" time
    for (label timeI = 1; timeI < Times.size(); ++timeI)
    {
        runTime.setTime(Times[timeI], timeI);

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
            Info<< "    Reading U" << endl;
            volVectorField U(Uheader, mesh);

            pointField newPoints
            (
                zeroPoints
              + scaleFactor*pInterp.interpolate(U)().internalField()
            );

            mesh.polyMesh::movePoints(newPoints);
            mesh.write();
        }
        else
        {
            Info<< "    No U" << endl;
        }

        Info<< endl;
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
