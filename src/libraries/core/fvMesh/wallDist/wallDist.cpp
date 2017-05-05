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

#include "wallDist.hpp"
#include "patchWave.hpp"
#include "fvMesh.hpp"
#include "wallPolyPatch.hpp"
#include "fvPatchField.hpp"
#include "Field.hpp"
#include "emptyFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::wallDist::wallDist(const fvMesh& mesh, const bool correctWalls)
:
    volScalarField
    (
        IOobject
        (
            "y",
            mesh.time().timeName(),
            mesh
        ),
        mesh,
        dimensionedScalar("y", dimLength, GREAT)
    ),
    cellDistFuncs(mesh),
    correctWalls_(correctWalls),
    nUnset_(0)
{
    wallDist::correct();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::wallDist::~wallDist()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Correct for mesh geom/topo changes. Might be more intelligent in the
// future (if only small topology change)
void CML::wallDist::correct()
{
    // Get patchids of walls
    labelHashSet wallPatchIDs(getPatchIDs<wallPolyPatch>());

    // Calculate distance starting from wallPatch faces.
    patchWave wave(cellDistFuncs::mesh(), wallPatchIDs, correctWalls_);

    // Transfer cell values from wave into *this
    transfer(wave.distance());

    // Transfer values on patches into boundaryField of *this
    forAll(boundaryField(), patchI)
    {
        if (!isA<emptyFvPatchScalarField>(boundaryField()[patchI]))
        {
            scalarField& waveFld = wave.patchDistance()[patchI];

            boundaryField()[patchI].transfer(waveFld);
        }
    }

    // Transfer number of unset values
    nUnset_ = wave.nUnset();
}


// ************************************************************************* //
