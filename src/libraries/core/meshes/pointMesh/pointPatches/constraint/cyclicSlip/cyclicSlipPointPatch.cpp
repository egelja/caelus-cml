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

#include "cyclicSlipPointPatch.hpp"
#include "pointConstraint.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(cyclicSlipPointPatch, 0);

// Add the patch constructor functions to the hash tables
addToRunTimeSelectionTable
(
    facePointPatch,
    cyclicSlipPointPatch,
    polyPatch
);


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const vectorField& cyclicSlipPointPatch::pointNormals() const
{
    // Use underlying patch normals
    return refCast<const facePointPatch>
    (
        *this
    ).facePointPatch::pointNormals();
}


void cyclicSlipPointPatch::applyConstraint
(
    const label pointi,
    pointConstraint& pc
) const
{
    pc.applyConstraint(pointNormals()[pointi]);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
