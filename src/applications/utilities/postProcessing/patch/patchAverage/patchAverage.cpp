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
    patchAverage

Description
    Calculates the average of the specified field over the specified patch.

\*---------------------------------------------------------------------------*/

#include "fvCFD.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class FieldType>
void printAverage
(
    const fvMesh& mesh,
    const IOobject& fieldHeader,
    const scalar area,
    const label patchI,
    bool& done
)
{
    if (!done && fieldHeader.headerClassName() == FieldType::typeName)
    {
        Info<< "    Reading " << fieldHeader.headerClassName() << " "
            << fieldHeader.name() << endl;

        FieldType field(fieldHeader, mesh);

        typename FieldType::value_type sumField =
            pTraits<typename FieldType::value_type>::zero;

        if (area > 0)
        {
            sumField = gSum
            (
                mesh.magSf().boundaryField()[patchI]
              * field.boundaryField()[patchI]
            ) / area;
        }

        Info<< "    Average of " << fieldHeader.headerClassName()
            << " over patch "
            << mesh.boundary()[patchI].name()
            << '[' << patchI << ']' << " = "
            << sumField << endl;

        done = true;
    }
}


// Main program:

int main(int argc, char *argv[])
{
    timeSelector::addOptions();
    #include "addRegionOption.hpp"
    argList::validArgs.append("fieldName");
    argList::validArgs.append("patchName");
#   include "setRootCase.hpp"
#   include "createTime.hpp"
    instantList timeDirs = timeSelector::select0(runTime, args);
#   include "createNamedMesh.hpp"

    const word fieldName = args[1];
    const word patchName = args[2];

    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);
        Info<< "Time = " << runTime.timeName() << endl;

        IOobject io
        (
            fieldName,
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ
        );

        // Check field exists
        if (io.headerOk())
        {
            mesh.readUpdate();

            const label patchI = mesh.boundaryMesh().findPatchID(patchName);
            if (patchI < 0)
            {
                FatalError
                    << "Unable to find patch " << patchName << nl
                    << exit(FatalError);
            }
            scalar area = gSum(mesh.magSf().boundaryField()[patchI]);

            bool done = false;
            printAverage<volScalarField>(mesh, io, area, patchI, done);
            printAverage<volVectorField>(mesh, io, area, patchI, done);
            printAverage<volSphericalTensorField>(mesh, io, area, patchI, done);
            printAverage<volSymmTensorField>(mesh, io, area, patchI, done);
            printAverage<volTensorField>(mesh, io, area, patchI, done);

            if (!done)
            {
                FatalError
                    << "Only possible to average volFields."
                    << " Field " << fieldName << " is of type "
                    << io.headerClassName()
                    << nl << exit(FatalError);
            }
        }
        else
        {
            Info<< "    No field " << fieldName << endl;
        }

        Info<< endl;
    }

    Info<< "End\n" << endl;

    return 0;
}

// ************************************************************************* //
