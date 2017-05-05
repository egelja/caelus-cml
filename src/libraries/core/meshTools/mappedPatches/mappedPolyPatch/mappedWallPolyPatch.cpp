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

#include "mappedWallPolyPatch.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(mappedWallPolyPatch, 0);

    addToRunTimeSelectionTable(polyPatch, mappedWallPolyPatch, word);
    addToRunTimeSelectionTable
    (
        polyPatch,
        mappedWallPolyPatch,
        dictionary
    );
}


// * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * * * * //

CML::mappedWallPolyPatch::mappedWallPolyPatch
(
    const word& name,
    const label size,
    const label start,
    const label index,
    const polyBoundaryMesh& bm
)
:
    wallPolyPatch(name, size, start, index, bm),
    mappedPatchBase(static_cast<const polyPatch&>(*this))
{}


CML::mappedWallPolyPatch::mappedWallPolyPatch
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
    wallPolyPatch(name, size, start, index, bm),
    mappedPatchBase
    (
        static_cast<const polyPatch&>(*this),
        sampleRegion,
        mode,
        samplePatch,
        offset
    )
{}


CML::mappedWallPolyPatch::mappedWallPolyPatch
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
    wallPolyPatch(name, size, start, index, bm),
    mappedPatchBase
    (
        static_cast<const polyPatch&>(*this),
        sampleRegion,
        mode,
        samplePatch,
        offset
    )
{}


CML::mappedWallPolyPatch::mappedWallPolyPatch
(
    const word& name,
    const dictionary& dict,
    const label index,
    const polyBoundaryMesh& bm
)
:
    wallPolyPatch(name, dict, index, bm),
    mappedPatchBase(*this, dict)
{}


CML::mappedWallPolyPatch::mappedWallPolyPatch
(
    const mappedWallPolyPatch& pp,
    const polyBoundaryMesh& bm
)
:
    wallPolyPatch(pp, bm),
    mappedPatchBase(*this, pp)
{}


CML::mappedWallPolyPatch::mappedWallPolyPatch
(
    const mappedWallPolyPatch& pp,
    const polyBoundaryMesh& bm,
    const label index,
    const label newSize,
    const label newStart
)
:
    wallPolyPatch(pp, bm, index, newSize, newStart),
    mappedPatchBase(*this, pp)
{}


CML::mappedWallPolyPatch::mappedWallPolyPatch
(
    const mappedWallPolyPatch& pp,
    const polyBoundaryMesh& bm,
    const label index,
    const labelUList& mapAddressing,
    const label newStart
)
:
    wallPolyPatch(pp, bm, index, mapAddressing, newStart),
    mappedPatchBase(*this, pp, mapAddressing)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::mappedWallPolyPatch::~mappedWallPolyPatch()
{
    mappedPatchBase::clearOut();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::mappedWallPolyPatch::initGeometry(PstreamBuffers& pBufs)
{
    wallPolyPatch::initGeometry(pBufs);
}


void CML::mappedWallPolyPatch::calcGeometry(PstreamBuffers& pBufs)
{
    wallPolyPatch::calcGeometry(pBufs);
    mappedPatchBase::clearOut();
}


void CML::mappedWallPolyPatch::initMovePoints
(
    PstreamBuffers& pBufs,
    const pointField& p
)
{
    wallPolyPatch::initMovePoints(pBufs, p);
}


void CML::mappedWallPolyPatch::movePoints
(
    PstreamBuffers& pBufs,
    const pointField& p
)
{
    wallPolyPatch::movePoints(pBufs, p);
    mappedPatchBase::clearOut();
}


void CML::mappedWallPolyPatch::initUpdateMesh(PstreamBuffers& pBufs)
{
    wallPolyPatch::initUpdateMesh(pBufs);
}


void CML::mappedWallPolyPatch::updateMesh(PstreamBuffers& pBufs)
{
    wallPolyPatch::updateMesh(pBufs);
    mappedPatchBase::clearOut();
}


void CML::mappedWallPolyPatch::write(Ostream& os) const
{
    wallPolyPatch::write(os);
    mappedPatchBase::write(os);
}


// ************************************************************************* //
