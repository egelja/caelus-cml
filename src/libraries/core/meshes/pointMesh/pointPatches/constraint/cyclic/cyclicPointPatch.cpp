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

#include "cyclicPointPatch.hpp"
#include "pointBoundaryMesh.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "pointMesh.hpp"
#include "edgeList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(cyclicPointPatch, 0);
    addToRunTimeSelectionTable
    (
        facePointPatch,
        cyclicPointPatch,
        polyPatch
    );
}

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

void CML::cyclicPointPatch::initGeometry(PstreamBuffers&)
{}


void CML::cyclicPointPatch::calcGeometry(PstreamBuffers&)
{}


void CML::cyclicPointPatch::initMovePoints(PstreamBuffers&, const pointField&)
{}


void CML::cyclicPointPatch::movePoints(PstreamBuffers&, const pointField&)
{}


void CML::cyclicPointPatch::initUpdateMesh(PstreamBuffers& pBufs)
{
    facePointPatch::initUpdateMesh(pBufs);
    cyclicPointPatch::initGeometry(pBufs);
}


void CML::cyclicPointPatch::updateMesh(PstreamBuffers& pBufs)
{
    facePointPatch::updateMesh(pBufs);
    cyclicPointPatch::calcGeometry(pBufs);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::cyclicPointPatch::cyclicPointPatch
(
    const polyPatch& patch,
    const pointBoundaryMesh& bm
)
:
    coupledFacePointPatch(patch, bm),
    cyclicPolyPatch_(refCast<const cyclicPolyPatch>(patch))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::cyclicPointPatch::~cyclicPointPatch()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::edgeList& CML::cyclicPointPatch::transformPairs() const
{
    return cyclicPolyPatch_.coupledPoints();
}


// ************************************************************************* //
