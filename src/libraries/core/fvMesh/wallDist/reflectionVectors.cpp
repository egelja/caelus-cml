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

#include "reflectionVectors.hpp"
#include "wallFvPatch.hpp"
#include "surfaceFields.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::reflectionVectors::reflectionVectors(const CML::fvMesh& mesh)
:
    n_
    (
        IOobject
        (
            "reflectionVectors",
            mesh.time().timeName(),
            mesh
        ),
        mesh,
        dimensionedVector("n", dimless, vector::zero)
    )
{
    correct();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void CML::reflectionVectors::correct()
{
    const fvMesh& mesh = n_.mesh();
    const fvPatchList& patches = mesh.boundary();

    forAll(patches, patchi)
    {
        // find the nearest face for every cell
        if (isA<wallFvPatch>(patches[patchi]))
        {
            n_.boundaryField()[patchi] =
                mesh.Sf().boundaryField()[patchi]
               /mesh.magSf().boundaryField()[patchi];
        }
    }
}


// ************************************************************************* //
