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
    rotateMesh

Description
    Rotates the mesh and fields from the direcion n1 to the direction n2.

\*---------------------------------------------------------------------------*/

#include "argList.hpp"
#include "timeSelector.hpp"
#include "Time.hpp"
#include "fvMesh.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "transformGeometricField.hpp"
#include "IOobjectList.hpp"

using namespace CML;

template<class GeometricField>
void RotateFields
(
    const fvMesh& mesh,
    const IOobjectList& objects,
    const tensor& T
)
{
    // Search list of objects for volScalarFields
    IOobjectList fields(objects.lookupClass(GeometricField::typeName));

    forAllIter(IOobjectList, fields, fieldIter)
    {
        Info<< "    Rotating " << fieldIter()->name() << endl;

        GeometricField theta(*fieldIter(), mesh);
        transform(theta, dimensionedTensor(T), theta);
        theta.write();
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    timeSelector::addOptions();

    argList::validArgs.append("n1");
    argList::validArgs.append("n2");

#   include "setRootCase.hpp"
#   include "createTime.hpp"

    vector n1 = args.argRead<vector>(1);
    n1 /= mag(n1);

    vector n2 = args.argRead<vector>(2);
    n2 /= mag(n2);

    tensor T = rotationTensor(n1, n2);

    {
        pointIOField points
        (
            IOobject
            (
                "points",
                runTime.findInstance(polyMesh::meshSubDir, "points"),
                polyMesh::meshSubDir,
                runTime,
                IOobject::MUST_READ,
                IOobject::NO_WRITE,
                false
            )
        );

        points = transform(T, points);

        // Set the precision of the points data to 10
        IOstream::defaultPrecision(10);

        Info<< "Writing points into directory " << points.path() << nl << endl;
        points.write();
    }


    instantList timeDirs = timeSelector::select0(runTime, args);

#   include "createMesh.hpp"


    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);

        Info<< "Time = " << runTime.timeName() << endl;

        // Search for list of objects for this time
        IOobjectList objects(mesh, runTime.timeName());

        RotateFields<volVectorField>(mesh, objects, T);
        RotateFields<volSphericalTensorField>(mesh, objects, T);
        RotateFields<volSymmTensorField>(mesh, objects, T);
        RotateFields<volTensorField>(mesh, objects, T);

        RotateFields<surfaceVectorField>(mesh, objects, T);
        RotateFields<surfaceSphericalTensorField>(mesh, objects, T);
        RotateFields<surfaceSymmTensorField>(mesh, objects, T);
        RotateFields<surfaceTensorField>(mesh, objects, T);
    }

    Info<< "\nEnd.\n" << endl;

    return 0;
}


// ************************************************************************* //
