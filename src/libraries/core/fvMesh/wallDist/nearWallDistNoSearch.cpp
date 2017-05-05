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

\*---------------------------------------------------------------------------*/

#include "nearWallDistNoSearch.hpp"
#include "fvMesh.hpp"
#include "wallFvPatch.hpp"
#include "surfaceFields.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::nearWallDistNoSearch::doAll()
{
    const volVectorField& cellCentres = mesh_.C();
    const fvPatchList& patches = mesh_.boundary();

    forAll(patches, patchI)
    {
        fvPatchScalarField& ypatch = operator[](patchI);

        if (isA<wallFvPatch>(patches[patchI]))
        {
            const labelUList& faceCells = patches[patchI].faceCells();

            const fvPatchVectorField& patchCentres
                = cellCentres.boundaryField()[patchI];

            const fvsPatchVectorField& Apatch
                = mesh_.Sf().boundaryField()[patchI];

            const fvsPatchScalarField& magApatch
                = mesh_.magSf().boundaryField()[patchI];

            forAll(patchCentres, facei)
            {
                ypatch[facei] =
                (
                    Apatch[facei] &
                    (
                        patchCentres[facei]
                      - cellCentres[faceCells[facei]]
                    )
                )/magApatch[facei];
            }
        }
        else
        {
            ypatch = 0.0;
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::nearWallDistNoSearch::nearWallDistNoSearch(const CML::fvMesh& mesh)
:
    volScalarField::GeometricBoundaryField
    (
        mesh.boundary(),
        mesh.V(),           // Dummy internal field
        calculatedFvPatchScalarField::typeName
    ),
    mesh_(mesh)
{
    doAll();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::nearWallDistNoSearch::~nearWallDistNoSearch()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::nearWallDistNoSearch::correct()
{
    if (mesh_.changing())
    {
        // Update size of GeometricBoundaryField
        forAll(mesh_.boundary(), patchI)
        {
            operator[](patchI).setSize(mesh_.boundary()[patchI].size());
        }
    }

    doAll();
}


// ************************************************************************* //
