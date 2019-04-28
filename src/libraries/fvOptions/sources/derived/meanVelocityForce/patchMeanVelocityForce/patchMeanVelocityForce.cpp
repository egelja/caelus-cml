/*---------------------------------------------------------------------------*\
Copyright (C) 2015 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "patchMeanVelocityForce.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

namespace CML
{
namespace fv
{
    defineTypeNameAndDebug(patchMeanVelocityForce, 0);

    addToRunTimeSelectionTable
    (
        option,
        patchMeanVelocityForce,
        dictionary
    );
}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::fv::patchMeanVelocityForce::patchMeanVelocityForce
(
    const word& sourceName,
    const word& modelType,
    const dictionary& dict,
    const fvMesh& mesh
)
:
    meanVelocityForce(sourceName, modelType, dict, mesh),
    patch_(coeffs_.lookup("patch")),
    patchi_(mesh.boundaryMesh().findPatchID(patch_))
{
    if (patchi_ < 0)
    {
        FatalErrorInFunction
            << "Cannot find patch " << patch_
            << exit(FatalError);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::scalar CML::fv::patchMeanVelocityForce::magUbarAve
(
    const volVectorField& U
) const
{
    return
        gSum
        (
            (flowDir_ & U.boundaryField()[patchi_])
           *mesh_.boundary()[patchi_].magSf()
        )/gSum(mesh_.boundary()[patchi_].magSf());
}
