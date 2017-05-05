/*---------------------------------------------------------------------------*\
Copyright (C) 2013 OpenFOAM Foundation
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

#include "cyclicACMIPointPatch.hpp"
#include "pointBoundaryMesh.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(cyclicACMIPointPatch, 0);
    addToRunTimeSelectionTable
    (
        facePointPatch,
        cyclicACMIPointPatch,
        polyPatch
    );
}


// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

void CML::cyclicACMIPointPatch::initGeometry(PstreamBuffers&)
{}


void CML::cyclicACMIPointPatch::calcGeometry(PstreamBuffers&)
{}


void CML::cyclicACMIPointPatch::initMovePoints
(
    PstreamBuffers&,
    const pointField&
)
{}


void CML::cyclicACMIPointPatch::movePoints(PstreamBuffers&, const pointField&)
{}


void CML::cyclicACMIPointPatch::initUpdateMesh(PstreamBuffers& pBufs)
{
    facePointPatch::initUpdateMesh(pBufs);
//    cyclicACMIPointPatch::initGeometry(pBufs);
}


void CML::cyclicACMIPointPatch::updateMesh(PstreamBuffers& pBufs)
{
    facePointPatch::updateMesh(pBufs);
//    cyclicACMIPointPatch::calcGeometry(pBufs);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::cyclicACMIPointPatch::cyclicACMIPointPatch
(
    const polyPatch& patch,
    const pointBoundaryMesh& bm
)
:
    coupledFacePointPatch(patch, bm),
    cyclicACMIPolyPatch_(refCast<const cyclicACMIPolyPatch>(patch))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::cyclicACMIPointPatch::~cyclicACMIPointPatch()
{}


// ************************************************************************* //
