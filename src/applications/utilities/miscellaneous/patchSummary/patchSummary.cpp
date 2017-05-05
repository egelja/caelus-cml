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
    patchSummary

Description
    Writes fields and boundary condition info for each patch at each requested
    time instance.

\*---------------------------------------------------------------------------*/

#include "fvCFD.hpp"
#include "volFields.hpp"
#include "pointFields.hpp"
#include "IOobjectList.hpp"
#include "patchSummaryTemplates.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    timeSelector::addOptions();

#   include "addRegionOption.hpp"
#   include "setRootCase.hpp"
#   include "createTime.hpp"

    instantList timeDirs = timeSelector::select0(runTime, args);

#   include "createNamedMesh.hpp"

    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);

        Info<< "Time = " << runTime.timeName() << nl << endl;

        const IOobjectList fieldObjs(mesh, runTime.timeName());
        const wordList objNames = fieldObjs.names();

        PtrList<volScalarField> vsf(objNames.size());
        PtrList<volVectorField> vvf(objNames.size());
        PtrList<volSphericalTensorField> vsptf(objNames.size());
        PtrList<volSymmTensorField> vsytf(objNames.size());
        PtrList<volTensorField> vtf(objNames.size());

        PtrList<pointScalarField> psf(objNames.size());
        PtrList<pointVectorField> pvf(objNames.size());
        PtrList<pointSphericalTensorField> psptf(objNames.size());
        PtrList<pointSymmTensorField> psytf(objNames.size());
        PtrList<pointTensorField> ptf(objNames.size());

        Info<< "Valid fields:" << endl;

        forAll(objNames, objI)
        {
            IOobject obj
            (
                objNames[objI],
                runTime.timeName(),
                mesh,
                IOobject::MUST_READ
            );

            if (obj.headerOk())
            {
                addToFieldList(vsf, obj, objI, mesh);
                addToFieldList(vvf, obj, objI, mesh);
                addToFieldList(vsptf, obj, objI, mesh);
                addToFieldList(vsytf, obj, objI, mesh);
                addToFieldList(vtf, obj, objI, mesh);

                addToFieldList(psf, obj, objI, pointMesh::New(mesh));
                addToFieldList(pvf, obj, objI, pointMesh::New(mesh));
                addToFieldList(psptf, obj, objI, pointMesh::New(mesh));
                addToFieldList(psytf, obj, objI, pointMesh::New(mesh));
                addToFieldList(ptf, obj, objI, pointMesh::New(mesh));
            }
        }

        Info<< endl;

        const polyBoundaryMesh& bm = mesh.boundaryMesh();
        forAll(bm, patchI)
        {
            Info<< bm[patchI].type() << ": " << bm[patchI].name() << nl;
                outputFieldList(vsf, patchI);
                outputFieldList(vvf, patchI);
                outputFieldList(vsptf, patchI);
                outputFieldList(vsytf, patchI);
                outputFieldList(vtf, patchI);

                outputFieldList(psf, patchI);
                outputFieldList(pvf, patchI);
                outputFieldList(psptf, patchI);
                outputFieldList(psytf, patchI);
                outputFieldList(ptf, patchI);
            Info<< endl;
        }
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
