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

#include "mappedVariableThicknessWallPolyPatch.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(mappedVariableThicknessWallPolyPatch, 0);

    addToRunTimeSelectionTable
    (
        polyPatch,
        mappedVariableThicknessWallPolyPatch,
        word
    );

    addToRunTimeSelectionTable
    (
        polyPatch,
        mappedVariableThicknessWallPolyPatch,
        dictionary
    );
}


// * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * * * * //

CML::mappedVariableThicknessWallPolyPatch::mappedVariableThicknessWallPolyPatch
(
    const word& name,
    const label size,
    const label start,
    const label index,
    const polyBoundaryMesh& bm
)
:
    mappedWallPolyPatch(name, size, start, index, bm),
    thickness_(size)
{}


CML::mappedVariableThicknessWallPolyPatch::mappedVariableThicknessWallPolyPatch
(
    const word& name,
    const label size,
    const label start,
    const label index,
    const word& sampleRegion,
    const mappedPatchBase::sampleMode mode,
    const word& samplePatch,
    const vectorField& offset,
    const polyBoundaryMesh& bm
)
:
    mappedWallPolyPatch(name, size, start, index, bm),
    thickness_(size)
{}


CML::mappedVariableThicknessWallPolyPatch::mappedVariableThicknessWallPolyPatch
(
    const word& name,
    const label size,
    const label start,
    const label index,
    const word& sampleRegion,
    const mappedPatchBase::sampleMode mode,
    const word& samplePatch,
    const vector& offset,
    const polyBoundaryMesh& bm
)
:
    mappedWallPolyPatch(name, size, start, index, bm),
    thickness_(size)
{}


CML::mappedVariableThicknessWallPolyPatch::mappedVariableThicknessWallPolyPatch
(
    const word& name,
    const dictionary& dict,
    const label index,
    const polyBoundaryMesh& bm
)
:
    mappedWallPolyPatch(name, dict, index, bm),
    thickness_(scalarField("thickness", dict, this->size()))
{}


CML::mappedVariableThicknessWallPolyPatch::
mappedVariableThicknessWallPolyPatch
(
    const mappedVariableThicknessWallPolyPatch& pp,
    const polyBoundaryMesh& bm
)
:
    mappedWallPolyPatch(pp, bm),
    thickness_(pp.thickness_)
{}


CML::mappedVariableThicknessWallPolyPatch::mappedVariableThicknessWallPolyPatch
(
    const mappedVariableThicknessWallPolyPatch& pp,
    const polyBoundaryMesh& bm,
    const label index,
    const label newSize,
    const label newStart
)
:
    mappedWallPolyPatch(pp, bm, index, newSize, newStart),
    thickness_(newSize)
{}


CML::mappedVariableThicknessWallPolyPatch::mappedVariableThicknessWallPolyPatch
(
    const mappedVariableThicknessWallPolyPatch& pp,
    const polyBoundaryMesh& bm,
    const label index,
    const labelUList& mapAddressing,
    const label newStart
)
:
    mappedWallPolyPatch(pp, bm, index, mapAddressing, newStart),
    thickness_(pp.size())
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::mappedVariableThicknessWallPolyPatch::
~mappedVariableThicknessWallPolyPatch()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::mappedVariableThicknessWallPolyPatch::
write(CML::Ostream& os) const
{
    os.writeKeyword("thickness") << thickness_ << token::END_STATEMENT << nl;
}


// ************************************************************************* //
