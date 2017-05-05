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

#include "processorCyclicPolyPatch.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "SubField.hpp"
#include "cyclicPolyPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(processorCyclicPolyPatch, 0);
    addToRunTimeSelectionTable(polyPatch, processorCyclicPolyPatch, dictionary);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::processorCyclicPolyPatch::processorCyclicPolyPatch
(
    const word& name,
    const label size,
    const label start,
    const label index,
    const polyBoundaryMesh& bm,
    const int myProcNo,
    const int neighbProcNo,
    const word& referPatchName,
    const transformType transform,
    const word& patchType
)
:
    processorPolyPatch
    (
        name,
        size,
        start,
        index,
        bm,
        myProcNo,
        neighbProcNo,
        transform,
        patchType
    ),
    tag_
    (
        Pstream::nProcs()*max(myProcNo, neighbProcNo)
      + min(myProcNo, neighbProcNo)
    ),
    referPatchName_(referPatchName),
    referPatchID_(-1)
{
    if (debug)
    {
        Pout<< "processorCyclicPolyPatch " << name << " uses tag " << tag_
            << endl;
    }
}


CML::processorCyclicPolyPatch::processorCyclicPolyPatch
(
    const word& name,
    const dictionary& dict,
    const label index,
    const polyBoundaryMesh& bm,
    const word& patchType
)
:
    processorPolyPatch(name, dict, index, bm, patchType),
    tag_
    (
        Pstream::nProcs()*max(myProcNo(), neighbProcNo())
      + min(myProcNo(), neighbProcNo())
    ),
    referPatchName_(dict.lookup("referPatch")),
    referPatchID_(-1)
{
    if (debug)
    {
        Pout<< "processorCyclicPolyPatch " << name << " uses tag " << tag_
            << endl;
    }
}


CML::processorCyclicPolyPatch::processorCyclicPolyPatch
(
    const processorCyclicPolyPatch& pp,
    const polyBoundaryMesh& bm
)
:
    processorPolyPatch(pp, bm),
    tag_(pp.tag_),
    referPatchName_(pp.referPatchName()),
    referPatchID_(-1)
{}


CML::processorCyclicPolyPatch::processorCyclicPolyPatch
(
    const processorCyclicPolyPatch& pp,
    const polyBoundaryMesh& bm,
    const label index,
    const label newSize,
    const label newStart
)
:
    processorPolyPatch(pp, bm, index, newSize, newStart),
    tag_(pp.tag_),
    referPatchName_(pp.referPatchName_),
    referPatchID_(-1)
{}


CML::processorCyclicPolyPatch::processorCyclicPolyPatch
(
    const processorCyclicPolyPatch& pp,
    const polyBoundaryMesh& bm,
    const label index,
    const label newSize,
    const label newStart,
    const word& referPatchName
)
:
    processorPolyPatch(pp, bm, index, newSize, newStart),
    tag_(pp.tag_),
    referPatchName_(referPatchName),
    referPatchID_(-1)
{}


CML::processorCyclicPolyPatch::processorCyclicPolyPatch
(
    const processorCyclicPolyPatch& pp,
    const polyBoundaryMesh& bm,
    const label index,
    const labelUList& mapAddressing,
    const label newStart
)
:
    processorPolyPatch(pp, bm, index, mapAddressing, newStart),
    tag_(pp.tag_),
    referPatchName_(pp.referPatchName()),
    referPatchID_(-1)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::processorCyclicPolyPatch::~processorCyclicPolyPatch()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void CML::processorCyclicPolyPatch::initGeometry(PstreamBuffers& pBufs)
{
    // Send over processorPolyPatch data
    processorPolyPatch::initGeometry(pBufs);
}


void CML::processorCyclicPolyPatch::calcGeometry(PstreamBuffers& pBufs)
{
    // Receive and initialise processorPolyPatch data
    processorPolyPatch::calcGeometry(pBufs);

    if (Pstream::parRun())
    {

        // Where do we store the calculated transformation?
        // - on the processor patch?
        // - on the underlying cyclic patch?
        // - or do we not auto-calculate the transformation but
        //   have option of reading it.

        // Update underlying cyclic halves. Need to do both since only one
        // half might be present as a processorCyclic.
        coupledPolyPatch& pp = const_cast<coupledPolyPatch&>(referPatch());
        pp.calcGeometry
        (
            *this,
            faceCentres(),
            faceAreas(),
            faceCellCentres(),
            neighbFaceCentres(),
            neighbFaceAreas(),
            neighbFaceCellCentres()
        );

        if (isA<cyclicPolyPatch>(pp))
        {
            const cyclicPolyPatch& cpp = refCast<const cyclicPolyPatch>(pp);
            const_cast<cyclicPolyPatch&>(cpp.neighbPatch()).calcGeometry
            (
                *this,
                neighbFaceCentres(),
                neighbFaceAreas(),
                neighbFaceCellCentres(),
                faceCentres(),
                faceAreas(),
                faceCellCentres()
            );
        }
    }
}


void CML::processorCyclicPolyPatch::initMovePoints
(
    PstreamBuffers& pBufs,
    const pointField& p
)
{
    // Recalculate geometry
    initGeometry(pBufs);
}


void CML::processorCyclicPolyPatch::movePoints
(
    PstreamBuffers& pBufs,
    const pointField&
)
{
    calcGeometry(pBufs);
}


void CML::processorCyclicPolyPatch::initUpdateMesh(PstreamBuffers& pBufs)
{
    processorPolyPatch::initUpdateMesh(pBufs);
}


void CML::processorCyclicPolyPatch::updateMesh(PstreamBuffers& pBufs)
{
     referPatchID_ = -1;
     processorPolyPatch::updateMesh(pBufs);
}


void CML::processorCyclicPolyPatch::initOrder
(
    PstreamBuffers& pBufs,
    const primitivePatch& pp
) const
{
    // For now use the same algorithm as processorPolyPatch
    processorPolyPatch::initOrder(pBufs, pp);
}


// Return new ordering. Ordering is -faceMap: for every face index
// the new face -rotation:for every new face the clockwise shift
// of the original face. Return false if nothing changes (faceMap
// is identity, rotation is 0)
bool CML::processorCyclicPolyPatch::order
(
    PstreamBuffers& pBufs,
    const primitivePatch& pp,
    labelList& faceMap,
    labelList& rotation
) const
{
    // For now use the same algorithm as processorPolyPatch
    return processorPolyPatch::order(pBufs, pp, faceMap, rotation);
}


void CML::processorCyclicPolyPatch::write(Ostream& os) const
{
    processorPolyPatch::write(os);
    os.writeKeyword("referPatch") << referPatchName_
        << token::END_STATEMENT << nl;
}


// ************************************************************************* //
