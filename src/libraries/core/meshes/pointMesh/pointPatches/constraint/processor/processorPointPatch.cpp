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

#include "processorPointPatch.hpp"
#include "pointBoundaryMesh.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "pointMesh.hpp"
#include "faceList.hpp"
#include "primitiveFacePatch.hpp"
#include "emptyPolyPatch.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(processorPointPatch, 0);
    addToRunTimeSelectionTable
    (
        facePointPatch,
        processorPointPatch,
        polyPatch
    );
}


// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

void CML::processorPointPatch::initGeometry(PstreamBuffers& pBufs)
{
    // Algorithm:
    // Depending on whether the patch is a master or a slave, get the primitive
    // patch points and filter away the points from the global patch.

    // Create the reversed patch and pick up its points
    // so that the order is correct
    const polyPatch& pp = patch();

    faceList masterFaces(pp.size());

    forAll(pp, faceI)
    {
        masterFaces[faceI] = pp[faceI].reverseFace();
    }

    reverseMeshPoints_ = primitiveFacePatch
    (
        masterFaces,
        pp.points()
    ).meshPoints();
}


void CML::processorPointPatch::calcGeometry(PstreamBuffers& pBufs)
{}


void CML::processorPointPatch::initMovePoints
(
    PstreamBuffers&,
    const pointField&
)
{}


void CML::processorPointPatch::movePoints(PstreamBuffers&, const pointField&)
{}


void CML::processorPointPatch::initUpdateMesh(PstreamBuffers& pBufs)
{
    facePointPatch::initUpdateMesh(pBufs);
    processorPointPatch::initGeometry(pBufs);
}


void CML::processorPointPatch::updateMesh(PstreamBuffers& pBufs)
{
    facePointPatch::updateMesh(pBufs);
    processorPointPatch::calcGeometry(pBufs);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::processorPointPatch::processorPointPatch
(
    const polyPatch& patch,
    const pointBoundaryMesh& bm
)
:
    coupledFacePointPatch(patch, bm),
    procPolyPatch_(refCast<const processorPolyPatch>(patch))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::processorPointPatch::~processorPointPatch()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::labelList& CML::processorPointPatch::reverseMeshPoints() const
{
    return reverseMeshPoints_;
}


// ************************************************************************* //
