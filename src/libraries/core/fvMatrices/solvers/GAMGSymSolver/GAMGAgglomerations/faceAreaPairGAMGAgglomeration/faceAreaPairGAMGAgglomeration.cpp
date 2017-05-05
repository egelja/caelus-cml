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

#include "faceAreaPairGAMGAgglomeration.hpp"
#include "fvMesh.hpp"
#include "surfaceFields.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(faceAreaPairGAMGAgglomeration, 0);

    addToRunTimeSelectionTable
    (
        GAMGAgglomeration,
        faceAreaPairGAMGAgglomeration,
        lduMesh
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::faceAreaPairGAMGAgglomeration::faceAreaPairGAMGAgglomeration
(
    const lduMesh& mesh,
    const dictionary& controlDict
)
:
    pairGAMGAgglomeration(mesh, controlDict)
{
    const fvMesh& fvmesh = refCast<const fvMesh>(mesh);

    //agglomerate(mesh, sqrt(fvmesh.magSf().internalField()));
    agglomerate
    (
        mesh,
        mag
        (
            cmptMultiply
            (
                fvmesh.Sf().internalField()
               /sqrt(fvmesh.magSf().internalField()),
                vector(1, 1.01, 1.02)
                //vector::one
            )
        )
    );
}


// ************************************************************************* //
