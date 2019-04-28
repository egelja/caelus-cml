/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

Class
    CML::FaceCellWave

Description
    Wave propagation of information through grid. Every iteration
    information goes through one layer of cells. Templated on information
    that is transferred.

    Handles parallel and cyclics and non-parallel cyclics.

    Note: whether to propagate depends on the return value of Type::update
    which returns true (i.e. propagate) if the value changes by more than a
    certain tolerance.
    This tolerance can be very strict for normal face-cell and parallel
    cyclics (we use a value of 0.01 just to limit propagation of small changes)
    but for non-parallel cyclics this tolerance can be critical and if chosen
    too small can lead to non-convergence.

SourceFiles
    FaceCellWave.cpp

\*---------------------------------------------------------------------------*/

#ifndef FaceCellWave_H
#define FaceCellWave_H

#include "boolList.hpp"
#include "labelList.hpp"
#include "primitiveFieldsFwd.hpp"
#include "labelPair.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class polyMesh;
class polyPatch;

/*---------------------------------------------------------------------------*\
                        Class FaceCellWaveName Declaration
\*---------------------------------------------------------------------------*/

TemplateName(FaceCellWave);


/*---------------------------------------------------------------------------*\
                           Class FaceCellWave Declaration
\*---------------------------------------------------------------------------*/

template<class Type, class TrackingData = int>
class FaceCellWave
:
    public FaceCellWaveName
{
    // Private data

        //- Reference to mesh
        const polyMesh& mesh_;

        //- Optional boundary faces that information should travel through
        const List<labelPair> explicitConnections_;

        //- Information for all faces
        UList<Type>& allFaceInfo_;

        //- Information for all cells
        UList<Type>& allCellInfo_;

        //- Additional data to be passed into container
        TrackingData& td_;

        //- Has face changed
        boolList changedFace_;

        //- List of changed faces
        labelList changedFaces_;

        //- Number of changed faces
        label nChangedFaces_;

        // Cells that have changed
        boolList changedCell_;
        labelList changedCells_;
        label nChangedCells_;

        //- Contains cyclics
        const bool hasCyclicPatches_;

        //- Contains cyclicAMI
        const bool hasCyclicAMIPatches_;

        //- Number of evaluations
        label nEvals_;

        //- Number of unvisited cells/faces
        label nUnvisitedCells_;
        label nUnvisitedFaces_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        FaceCellWave(const FaceCellWave&);

        //- Disallow default bitwise assignment
        void operator=(const FaceCellWave&);


        //- Updates cellInfo with information from neighbour. Updates all
        //  statistics.
        bool updateCell
        (
            const label cellI,
            const label neighbourFaceI,
            const Type& neighbourInfo,
            const scalar tol,
            Type& cellInfo
        );

        //- Updates faceInfo with information from neighbour. Updates all
        //  statistics.
        bool updateFace
        (
            const label faceI,
            const label neighbourCellI,
            const Type& neighbourInfo,
            const scalar tol,
            Type& faceInfo
        );

        //- Updates faceInfo with information from same face. Updates all
        //  statistics.
        bool updateFace
        (
            const label faceI,
            const Type& neighbourInfo,
            const scalar tol,
            Type& faceInfo
        );


        // Parallel, cyclic

            //- Debugging: check info on both sides of cyclic
            void checkCyclic(const polyPatch& pPatch) const;

            //- Has cyclic patch?
            template <class PatchType>
            bool hasPatch() const;

            //- Merge received patch data into global data
            void mergeFaceInfo
            (
                const polyPatch& patch,
                const label nFaces,
                const labelList&,
                const List<Type>&
            );

            //- Extract info for single patch only
            label getChangedPatchFaces
            (
                const polyPatch& patch,
                const label startFaceI,
                const label nFaces,
                labelList& changedPatchFaces,
                List<Type>& changedPatchFacesInfo
            ) const;

            //- Handle leaving domain. Implementation referred to Type
            void leaveDomain
            (
                const polyPatch& patch,
                const label nFaces,
                const labelList& faceLabels,
                List<Type>& faceInfo
            ) const;

            //- Handle leaving domain. Implementation referred to Type
            void enterDomain
            (
                const polyPatch& patch,
                const label nFaces,
                const labelList& faceLabels,
                List<Type>& faceInfo
            ) const;

            //- Offset face labels by constant value
            static void offset
            (
                const polyPatch& patch,
                const label off,
                const label nFaces,
                labelList& faces
            );

            //- Apply transformation to Type
            void transform
            (
                const tensorField& rotTensor,
                const label nFaces,
                List<Type>& faceInfo
            );

            //- Merge data from across processor boundaries
            void handleProcPatches();

            //- Merge data from across cyclics
            void handleCyclicPatches();

            //- Merge data from across AMI cyclics
            void handleAMICyclicPatches();

            //- Merge data across explicitly provided local connections (usually
            //  baffles)
            void handleExplicitConnections();


      // Private static data

            static const scalar geomTol_;
            static scalar propagationTol_;

            //- Used as default trackdata value to satisfy default template
            //  argument.
            static int dummyTrackData_;


public:

    // Static Functions

        //- Access to tolerance
        static scalar propagationTol()
        {
            return propagationTol_;
        }

        //- Change tolerance
        static void setPropagationTol(const scalar tol)
        {
            propagationTol_ = tol;
        }


    // Constructors

        // Construct from mesh. Use setFaceInfo and iterate() to do actual
        // calculation.
        FaceCellWave
        (
            const polyMesh&,
            UList<Type>& allFaceInfo,
            UList<Type>& allCellInfo,
            TrackingData& td = dummyTrackData_
        );

        //- Construct from mesh and list of changed faces with the Type
        //  for these faces. Iterates until nothing changes or maxIter reached.
        //  (maxIter can be 0)
        FaceCellWave
        (
            const polyMesh&,
            const labelList& initialChangedFaces,
            const List<Type>& changedFacesInfo,
            UList<Type>& allFaceInfo,
            UList<Type>& allCellInfo,
            const label maxIter,
            TrackingData& td = dummyTrackData_
        );

        //- Construct from mesh and explicitly connected boundary faces
        //  and list of changed faces with the Type
        //  for these faces. Iterates until nothing changes or maxIter reached.
        //  (maxIter can be 0)
        FaceCellWave
        (
            const polyMesh&,
            const List<labelPair>& explicitConnections,
            const bool handleCyclicAMI,
            const labelList& initialChangedFaces,
            const List<Type>& changedFacesInfo,
            UList<Type>& allFaceInfo,
            UList<Type>& allCellInfo,
            const label maxIter,
            TrackingData& td = dummyTrackData_
        );


    // Member Functions

        // Access

            //- Access allFaceInfo
            UList<Type>& allFaceInfo()
            {
                return allFaceInfo_;
            }

            //- Access allCellInfo
            UList<Type>& allCellInfo()
            {
                return allCellInfo_;
            }

            //- Additional data to be passed into container
            const TrackingData& data() const
            {
                return td_;
            }

            //- Access mesh
            const polyMesh& mesh() const
            {
                return mesh_;
            }

            //- Get number of unvisited cells, i.e. cells that were not (yet)
            //  reached from walking across mesh. This can happen from
            //  - not enough iterations done
            //  - a disconnected mesh
            //  - a mesh without walls in it
            label getUnsetCells() const;

            //- Get number of unvisited faces
            label getUnsetFaces() const;


        // Edit

            //- Set initial changed faces
            void setFaceInfo
            (
                const labelList& changedFaces,
                const List<Type>& changedFacesInfo
            );

            //- Propagate from face to cell. Returns total number of cells
            //  (over all processors) changed.
            label faceToCell();

            //- Propagate from cell to face. Returns total number of faces
            //  (over all processors) changed. (Faces on processorpatches are
            //  counted double)
            label cellToFace();

            //- Iterate until no changes or maxIter reached.  Returns actual
            //  number of iterations.
            label iterate(const label maxIter);

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "polyMesh.hpp"
#include "processorPolyPatch.hpp"
#include "cyclicPolyPatch.hpp"
#include "cyclicAMIPolyPatch.hpp"
#include "OPstream.hpp"
#include "IPstream.hpp"
#include "PstreamReduceOps.hpp"
#include "debug.hpp"
#include "typeInfo.hpp"
#include "SubField.hpp"
#include "globalMeshData.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template <class Type, class TrackingData>
const CML::scalar CML::FaceCellWave<Type, TrackingData>::geomTol_ = 1e-6;

template <class Type, class TrackingData>
CML::scalar CML::FaceCellWave<Type, TrackingData>::propagationTol_ = 0.01;

template <class Type, class TrackingData>
int CML::FaceCellWave<Type, TrackingData>::dummyTrackData_ = 12345;

namespace CML
{
    //- Combine operator for AMIInterpolation
    template<class Type, class TrackingData>
    class combine
    {
        FaceCellWave<Type, TrackingData>& solver_;

        const cyclicAMIPolyPatch& patch_;

        public:

            combine
            (
                FaceCellWave<Type, TrackingData>& solver,
                const cyclicAMIPolyPatch& patch
            )
            :
                solver_(solver),
                patch_(patch)
            {}


            void operator()
            (
                Type& x,
                const label faceI,
                const Type& y,
                const scalar weight
            ) const
            {
                if (y.valid(solver_.data()))
                {
                    label meshFaceI = -1;
                    if (patch_.owner())
                    {
                        meshFaceI = patch_.start() + faceI;
                    }
                    else
                    {
                        meshFaceI = patch_.neighbPatch().start() + faceI;
                    }
                    x.updateFace
                    (
                        solver_.mesh(),
                        meshFaceI,
                        y,
                        solver_.propagationTol(),
                        solver_.data()
                    );
                }
            }
    };
}



// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// Update info for cellI, at position pt, with information from
// neighbouring face/cell.
// Updates:
//      - changedCell_, changedCells_, nChangedCells_,
//      - statistics: nEvals_, nUnvisitedCells_
template <class Type, class TrackingData>
bool CML::FaceCellWave<Type, TrackingData>::updateCell
(
    const label cellI,
    const label neighbourFaceI,
    const Type& neighbourInfo,
    const scalar tol,
    Type& cellInfo
)
{
    nEvals_++;

    bool wasValid = cellInfo.valid(td_);

    bool propagate =
        cellInfo.updateCell
        (
            mesh_,
            cellI,
            neighbourFaceI,
            neighbourInfo,
            tol,
            td_
        );

    if (propagate)
    {
        if (!changedCell_[cellI])
        {
            changedCell_[cellI] = true;
            changedCells_[nChangedCells_++] = cellI;
        }
    }

    if (!wasValid && cellInfo.valid(td_))
    {
        --nUnvisitedCells_;
    }

    return propagate;
}


// Update info for faceI, at position pt, with information from
// neighbouring face/cell.
// Updates:
//      - changedFace_, changedFaces_, nChangedFaces_,
//      - statistics: nEvals_, nUnvisitedFaces_
template <class Type, class TrackingData>
bool CML::FaceCellWave<Type, TrackingData>::updateFace
(
    const label faceI,
    const label neighbourCellI,
    const Type& neighbourInfo,
    const scalar tol,
    Type& faceInfo
)
{
    nEvals_++;

    bool wasValid = faceInfo.valid(td_);

    bool propagate =
        faceInfo.updateFace
        (
            mesh_,
            faceI,
            neighbourCellI,
            neighbourInfo,
            tol,
            td_
        );

    if (propagate)
    {
        if (!changedFace_[faceI])
        {
            changedFace_[faceI] = true;
            changedFaces_[nChangedFaces_++] = faceI;
        }
    }

    if (!wasValid && faceInfo.valid(td_))
    {
        --nUnvisitedFaces_;
    }

    return propagate;
}


// Update info for faceI, at position pt, with information from
// same face.
// Updates:
//      - changedFace_, changedFaces_, nChangedFaces_,
//      - statistics: nEvals_, nUnvisitedFaces_
template <class Type, class TrackingData>
bool CML::FaceCellWave<Type, TrackingData>::updateFace
(
    const label faceI,
    const Type& neighbourInfo,
    const scalar tol,
    Type& faceInfo
)
{
    nEvals_++;

    bool wasValid = faceInfo.valid(td_);

    bool propagate =
        faceInfo.updateFace
        (
            mesh_,
            faceI,
            neighbourInfo,
            tol,
            td_
        );

    if (propagate)
    {
        if (!changedFace_[faceI])
        {
            changedFace_[faceI] = true;
            changedFaces_[nChangedFaces_++] = faceI;
        }
    }

    if (!wasValid && faceInfo.valid(td_))
    {
        --nUnvisitedFaces_;
    }

    return propagate;
}


// For debugging: check status on both sides of cyclic
template <class Type, class TrackingData>
void CML::FaceCellWave<Type, TrackingData>::checkCyclic
(
    const polyPatch& patch
) const
{
    const cyclicPolyPatch& nbrPatch =
        refCast<const cyclicPolyPatch>(patch).neighbPatch();

    forAll(patch, patchFaceI)
    {
        label i1 = patch.start() + patchFaceI;
        label i2 = nbrPatch.start() + patchFaceI;

        if
        (
           !allFaceInfo_[i1].sameGeometry
            (
                mesh_,
                allFaceInfo_[i2],
                geomTol_,
                td_
            )
        )
        {
            FatalErrorInFunction
                << "problem: i:" << i1 << "  otheri:" << i2
                << "   faceInfo:" << allFaceInfo_[i1]
                << "   otherfaceInfo:" << allFaceInfo_[i2]
                << abort(FatalError);
        }

        if (changedFace_[i1] != changedFace_[i2])
        {
            FatalErrorInFunction
                << " problem: i:" << i1 << "  otheri:" << i2
                << "   faceInfo:" << allFaceInfo_[i1]
                << "   otherfaceInfo:" << allFaceInfo_[i2]
                << "   changedFace:" << changedFace_[i1]
                << "   otherchangedFace:" << changedFace_[i2]
                << abort(FatalError);
        }
    }
}


// Check if has cyclic patches
template <class Type, class TrackingData>
template <class PatchType>
bool CML::FaceCellWave<Type, TrackingData>::hasPatch() const
{
    forAll(mesh_.boundaryMesh(), patchI)
    {
        if (isA<PatchType>(mesh_.boundaryMesh()[patchI]))
        {
            return true;
        }
    }
    return false;
}


// Copy face information into member data
template <class Type, class TrackingData>
void CML::FaceCellWave<Type, TrackingData>::setFaceInfo
(
    const labelList& changedFaces,
    const List<Type>& changedFacesInfo
)
{
    forAll(changedFaces, changedFaceI)
    {
        label faceI = changedFaces[changedFaceI];

        bool wasValid = allFaceInfo_[faceI].valid(td_);

        // Copy info for faceI
        allFaceInfo_[faceI] = changedFacesInfo[changedFaceI];

        // Maintain count of unset faces
        if (!wasValid && allFaceInfo_[faceI].valid(td_))
        {
            --nUnvisitedFaces_;
        }

        // Mark faceI as changed, both on list and on face itself.

        changedFace_[faceI] = true;
        changedFaces_[nChangedFaces_++] = faceI;
    }
}


// Merge face information into member data
template <class Type, class TrackingData>
void CML::FaceCellWave<Type, TrackingData>::mergeFaceInfo
(
    const polyPatch& patch,
    const label nFaces,
    const labelList& changedFaces,
    const List<Type>& changedFacesInfo
)
{
    for (label changedFaceI = 0; changedFaceI < nFaces; changedFaceI++)
    {
        const Type& neighbourWallInfo = changedFacesInfo[changedFaceI];
        label patchFaceI = changedFaces[changedFaceI];

        label meshFaceI = patch.start() + patchFaceI;

        Type& currentWallInfo = allFaceInfo_[meshFaceI];

        if (!currentWallInfo.equal(neighbourWallInfo, td_))
        {
            updateFace
            (
                meshFaceI,
                neighbourWallInfo,
                propagationTol_,
                currentWallInfo
            );
        }
    }
}


// Construct compact patchFace change arrays for a (slice of a) single patch.
// changedPatchFaces in local patch numbering.
// Return length of arrays.
template <class Type, class TrackingData>
CML::label CML::FaceCellWave<Type, TrackingData>::getChangedPatchFaces
(
    const polyPatch& patch,
    const label startFaceI,
    const label nFaces,
    labelList& changedPatchFaces,
    List<Type>& changedPatchFacesInfo
) const
{
    label nChangedPatchFaces = 0;

    for (label i = 0; i < nFaces; i++)
    {
        label patchFaceI = i + startFaceI;

        label meshFaceI = patch.start() + patchFaceI;

        if (changedFace_[meshFaceI])
        {
            changedPatchFaces[nChangedPatchFaces] = patchFaceI;
            changedPatchFacesInfo[nChangedPatchFaces] = allFaceInfo_[meshFaceI];
            nChangedPatchFaces++;
        }
    }
    return nChangedPatchFaces;
}


// Handle leaving domain. Implementation referred to Type
template <class Type, class TrackingData>
void CML::FaceCellWave<Type, TrackingData>::leaveDomain
(
    const polyPatch& patch,
    const label nFaces,
    const labelList& faceLabels,
    List<Type>& faceInfo
) const
{
    const vectorField& fc = mesh_.faceCentres();

    for (label i = 0; i < nFaces; i++)
    {
        label patchFaceI = faceLabels[i];

        label meshFaceI = patch.start() + patchFaceI;
        faceInfo[i].leaveDomain(mesh_, patch, patchFaceI, fc[meshFaceI], td_);
    }
}


// Handle entering domain. Implementation referred to Type
template <class Type, class TrackingData>
void CML::FaceCellWave<Type, TrackingData>::enterDomain
(
    const polyPatch& patch,
    const label nFaces,
    const labelList& faceLabels,
    List<Type>& faceInfo
) const
{
    const vectorField& fc = mesh_.faceCentres();

    for (label i = 0; i < nFaces; i++)
    {
        label patchFaceI = faceLabels[i];

        label meshFaceI = patch.start() + patchFaceI;
        faceInfo[i].enterDomain(mesh_, patch, patchFaceI, fc[meshFaceI], td_);
    }
}


// Transform. Implementation referred to Type
template <class Type, class TrackingData>
void CML::FaceCellWave<Type, TrackingData>::transform
(
    const tensorField& rotTensor,
    const label nFaces,
    List<Type>& faceInfo
)
{
    if (rotTensor.size() == 1)
    {
        const tensor& T = rotTensor[0];

        for (label faceI = 0; faceI < nFaces; faceI++)
        {
            faceInfo[faceI].transform(mesh_, T, td_);
        }
    }
    else
    {
        for (label faceI = 0; faceI < nFaces; faceI++)
        {
            faceInfo[faceI].transform(mesh_, rotTensor[faceI], td_);
        }
    }
}


// Offset mesh face. Used for transferring from one cyclic half to the other.
template <class Type, class TrackingData>
void CML::FaceCellWave<Type, TrackingData>::offset
(
    const polyPatch&,
    const label cycOffset,
    const label nFaces,
    labelList& faces
)
{
    for (label faceI = 0; faceI < nFaces; faceI++)
    {
        faces[faceI] += cycOffset;
    }
}


// Transfer all the information to/from neighbouring processors
template <class Type, class TrackingData>
void CML::FaceCellWave<Type, TrackingData>::handleProcPatches()
{
    const globalMeshData& pData = mesh_.globalData();

    // Which patches are processor patches
    const labelList& procPatches = pData.processorPatches();

    // Send all

    PstreamBuffers pBufs(Pstream::nonBlocking);

    forAll(procPatches, i)
    {
        label patchI = procPatches[i];

        const processorPolyPatch& procPatch =
            refCast<const processorPolyPatch>(mesh_.boundaryMesh()[patchI]);

        // Allocate buffers
        label nSendFaces;
        labelList sendFaces(procPatch.size());
        List<Type> sendFacesInfo(procPatch.size());

        // Determine which faces changed on current patch
        nSendFaces = getChangedPatchFaces
        (
            procPatch,
            0,
            procPatch.size(),
            sendFaces,
            sendFacesInfo
        );

        // Adapt wallInfo for leaving domain
        leaveDomain
        (
            procPatch,
            nSendFaces,
            sendFaces,
            sendFacesInfo
        );

        if (debug)
        {
            Pout<< " Processor patch " << patchI << ' ' << procPatch.name()
                << " communicating with " << procPatch.neighbProcNo()
                << "  Sending:" << nSendFaces
                << endl;
        }

        UOPstream toNeighbour(procPatch.neighbProcNo(), pBufs);
        //writeFaces(nSendFaces, sendFaces, sendFacesInfo, toNeighbour);
        toNeighbour
            << SubList<label>(sendFaces, nSendFaces)
            << SubList<Type>(sendFacesInfo, nSendFaces);
    }

    pBufs.finishedSends();

    // Receive all

    forAll(procPatches, i)
    {
        label patchI = procPatches[i];

        const processorPolyPatch& procPatch =
            refCast<const processorPolyPatch>(mesh_.boundaryMesh()[patchI]);

        // Allocate buffers
        labelList receiveFaces;
        List<Type> receiveFacesInfo;

        {
            UIPstream fromNeighbour(procPatch.neighbProcNo(), pBufs);
            fromNeighbour >> receiveFaces >> receiveFacesInfo;
        }

        if (debug)
        {
            Pout<< " Processor patch " << patchI << ' ' << procPatch.name()
                << " communicating with " << procPatch.neighbProcNo()
                << "  Receiving:" << receiveFaces.size()
                << endl;
        }

        // Apply transform to received data for non-parallel planes
        if (!procPatch.parallel())
        {
            transform
            (
                procPatch.forwardT(),
                receiveFaces.size(),
                receiveFacesInfo
            );
        }

        // Adapt wallInfo for entering domain
        enterDomain
        (
            procPatch,
            receiveFaces.size(),
            receiveFaces,
            receiveFacesInfo
        );

        // Merge received info
        mergeFaceInfo
        (
            procPatch,
            receiveFaces.size(),
            receiveFaces,
            receiveFacesInfo
        );
    }
}


// Transfer information across cyclic halves.
template <class Type, class TrackingData>
void CML::FaceCellWave<Type, TrackingData>::handleCyclicPatches()
{
    forAll(mesh_.boundaryMesh(), patchI)
    {
        const polyPatch& patch = mesh_.boundaryMesh()[patchI];

        if (isA<cyclicPolyPatch>(patch))
        {
            const cyclicPolyPatch& nbrPatch =
                refCast<const cyclicPolyPatch>(patch).neighbPatch();

            // Allocate buffers
            label nReceiveFaces;
            labelList receiveFaces(patch.size());
            List<Type> receiveFacesInfo(patch.size());

            // Determine which faces changed
            nReceiveFaces = getChangedPatchFaces
            (
                nbrPatch,
                0,
                nbrPatch.size(),
                receiveFaces,
                receiveFacesInfo
            );

            // Adapt wallInfo for leaving domain
            leaveDomain
            (
                nbrPatch,
                nReceiveFaces,
                receiveFaces,
                receiveFacesInfo
            );

            const cyclicPolyPatch& cycPatch =
                refCast<const cyclicPolyPatch>(patch);

            if (!cycPatch.parallel())
            {
                // received data from other half
                transform
                (
                    cycPatch.forwardT(),
                    nReceiveFaces,
                    receiveFacesInfo
                );
            }

            if (debug)
            {
                Pout<< " Cyclic patch " << patchI << ' ' << cycPatch.name()
                    << "  Changed : " << nReceiveFaces
                    << endl;
            }

            // Half2: Adapt wallInfo for entering domain
            enterDomain
            (
                cycPatch,
                nReceiveFaces,
                receiveFaces,
                receiveFacesInfo
            );

            // Merge into global storage
            mergeFaceInfo
            (
                cycPatch,
                nReceiveFaces,
                receiveFaces,
                receiveFacesInfo
            );

            if (debug)
            {
                checkCyclic(cycPatch);
            }
        }
    }
}


// Transfer information across cyclic halves.
template <class Type, class TrackingData>
void CML::FaceCellWave<Type, TrackingData>::handleAMICyclicPatches()
{
    forAll(mesh_.boundaryMesh(), patchI)
    {
        const polyPatch& patch = mesh_.boundaryMesh()[patchI];

        if (isA<cyclicAMIPolyPatch>(patch))
        {
            const cyclicAMIPolyPatch& cycPatch =
                refCast<const cyclicAMIPolyPatch>(patch);

            List<Type> receiveInfo;

            {
                const cyclicAMIPolyPatch& nbrPatch =
                    refCast<const cyclicAMIPolyPatch>(patch).neighbPatch();

                // Get nbrPatch data (so not just changed faces)
                typename List<Type>::subList sendInfo
                (
                    nbrPatch.patchSlice
                    (
                        allFaceInfo_
                    )
                );

                if (!nbrPatch.parallel() || nbrPatch.separated())
                {
                    // Adapt sendInfo for leaving domain
                    const vectorField::subField fc = nbrPatch.faceCentres();
                    forAll(sendInfo, i)
                    {
                        sendInfo[i].leaveDomain(mesh_, nbrPatch, i, fc[i], td_);
                    }
                }

                // Transfer sendInfo to cycPatch
                combine<Type, TrackingData> cmb(*this, cycPatch);

                if (cycPatch.applyLowWeightCorrection())
                {
                    List<Type> defVals
                    (
                        cycPatch.patchInternalList(allCellInfo_)
                    );

                    cycPatch.interpolate(sendInfo, cmb, receiveInfo, defVals);
                }
                else
                {
                    cycPatch.interpolate(sendInfo, cmb, receiveInfo);
                }
            }

            // Apply transform to received data for non-parallel planes
            if (!cycPatch.parallel())
            {
                transform
                (
                    cycPatch.forwardT(),
                    receiveInfo.size(),
                    receiveInfo
                );
            }

            if (!cycPatch.parallel() || cycPatch.separated())
            {
                // Adapt receiveInfo for entering domain
                const vectorField::subField fc = cycPatch.faceCentres();
                forAll(receiveInfo, i)
                {
                    receiveInfo[i].enterDomain(mesh_, cycPatch, i, fc[i], td_);
                }
            }

            // Merge into global storage
            forAll(receiveInfo, i)
            {
                label meshFaceI = cycPatch.start()+i;

                Type& currentWallInfo = allFaceInfo_[meshFaceI];

                if
                (
                    receiveInfo[i].valid(td_)
                && !currentWallInfo.equal(receiveInfo[i], td_)
                )
                {
                    updateFace
                    (
                        meshFaceI,
                        receiveInfo[i],
                        propagationTol_,
                        currentWallInfo
                    );
                }
            }
        }
    }
}


template<class Type, class TrackingData>
void CML::FaceCellWave<Type, TrackingData>::handleExplicitConnections()
{
    // Collect changed information

    DynamicList<label> f0Baffle(explicitConnections_.size());
    DynamicList<Type> f0Info(explicitConnections_.size());

    DynamicList<label> f1Baffle(explicitConnections_.size());
    DynamicList<Type> f1Info(explicitConnections_.size());

    forAll(explicitConnections_, connI)
    {
        const labelPair& baffle = explicitConnections_[connI];

        label f0 = baffle[0];
        if (changedFace_[f0])
        {
            f0Baffle.append(connI);
            f0Info.append(allFaceInfo_[f0]);
        }

        label f1 = baffle[1];
        if (changedFace_[f1])
        {
            f1Baffle.append(connI);
            f1Info.append(allFaceInfo_[f1]);
        }
    }


    // Update other side with changed information

    forAll(f1Info, index)
    {
        const labelPair& baffle = explicitConnections_[f1Baffle[index]];

        label f0 = baffle[0];
        Type& currentWallInfo = allFaceInfo_[f0];

        if (!currentWallInfo.equal(f1Info[index], td_))
        {
            updateFace
            (
                f0,
                f1Info[index],
                propagationTol_,
                currentWallInfo
            );
        }
    }

    forAll(f0Info, index)
    {
        const labelPair& baffle = explicitConnections_[f0Baffle[index]];

        label f1 = baffle[1];
        Type& currentWallInfo = allFaceInfo_[f1];

        if (!currentWallInfo.equal(f0Info[index], td_))
        {
            updateFace
            (
                f1,
                f0Info[index],
                propagationTol_,
                currentWallInfo
            );
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Set up only. Use setFaceInfo and iterate() to do actual calculation.
template <class Type, class TrackingData>
CML::FaceCellWave<Type, TrackingData>::FaceCellWave
(
    const polyMesh& mesh,
    UList<Type>& allFaceInfo,
    UList<Type>& allCellInfo,
    TrackingData& td
)
:
    mesh_(mesh),
    explicitConnections_(0),
    allFaceInfo_(allFaceInfo),
    allCellInfo_(allCellInfo),
    td_(td),
    changedFace_(mesh_.nFaces(), false),
    changedFaces_(mesh_.nFaces()),
    nChangedFaces_(0),
    changedCell_(mesh_.nCells(), false),
    changedCells_(mesh_.nCells()),
    nChangedCells_(0),
    hasCyclicPatches_(hasPatch<cyclicPolyPatch>()),
    hasCyclicAMIPatches_
    (
        returnReduce(hasPatch<cyclicAMIPolyPatch>(), orOp<bool>())
    ),
    nEvals_(0),
    nUnvisitedCells_(mesh_.nCells()),
    nUnvisitedFaces_(mesh_.nFaces())
{
    if
    (
        allFaceInfo.size() != mesh_.nFaces()
     || allCellInfo.size() != mesh_.nCells()
    )
    {
        FatalErrorInFunction
            << "face and cell storage not the size of mesh faces, cells:"
            << endl
            << "    allFaceInfo   :" << allFaceInfo.size() << endl
            << "    mesh_.nFaces():" << mesh_.nFaces() << endl
            << "    allCellInfo   :" << allCellInfo.size() << endl
            << "    mesh_.nCells():" << mesh_.nCells()
            << exit(FatalError);
    }
}


// Iterate, propagating changedFacesInfo across mesh, until no change (or
// maxIter reached). Initial cell values specified.
template <class Type, class TrackingData>
CML::FaceCellWave<Type, TrackingData>::FaceCellWave
(
    const polyMesh& mesh,
    const labelList& changedFaces,
    const List<Type>& changedFacesInfo,
    UList<Type>& allFaceInfo,
    UList<Type>& allCellInfo,
    const label maxIter,
    TrackingData& td
)
:
    mesh_(mesh),
    explicitConnections_(0),
    allFaceInfo_(allFaceInfo),
    allCellInfo_(allCellInfo),
    td_(td),
    changedFace_(mesh_.nFaces(), false),
    changedFaces_(mesh_.nFaces()),
    nChangedFaces_(0),
    changedCell_(mesh_.nCells(), false),
    changedCells_(mesh_.nCells()),
    nChangedCells_(0),
    hasCyclicPatches_(hasPatch<cyclicPolyPatch>()),
    hasCyclicAMIPatches_
    (
        returnReduce(hasPatch<cyclicAMIPolyPatch>(), orOp<bool>())
    ),
    nEvals_(0),
    nUnvisitedCells_(mesh_.nCells()),
    nUnvisitedFaces_(mesh_.nFaces())
{
    if
    (
        allFaceInfo.size() != mesh_.nFaces()
     || allCellInfo.size() != mesh_.nCells()
    )
    {
        FatalErrorInFunction
            << "face and cell storage not the size of mesh faces, cells:"
            << endl
            << "    allFaceInfo   :" << allFaceInfo.size() << endl
            << "    mesh_.nFaces():" << mesh_.nFaces() << endl
            << "    allCellInfo   :" << allCellInfo.size() << endl
            << "    mesh_.nCells():" << mesh_.nCells()
            << exit(FatalError);
    }

    // Copy initial changed faces data
    setFaceInfo(changedFaces, changedFacesInfo);

    // Iterate until nothing changes
    label iter = iterate(maxIter);

    if ((maxIter > 0) && (iter >= maxIter))
    {
        FatalErrorInFunction
            << "Maximum number of iterations reached. Increase maxIter." << endl
            << "    maxIter:" << maxIter << endl
            << "    nChangedCells:" << nChangedCells_ << endl
            << "    nChangedFaces:" << nChangedFaces_ << endl
            << exit(FatalError);
    }
}


// Iterate, propagating changedFacesInfo across mesh, until no change (or
// maxIter reached). Initial cell values specified.
template<class Type, class TrackingData>
CML::FaceCellWave<Type, TrackingData>::FaceCellWave
(
    const polyMesh& mesh,
    const List<labelPair>& explicitConnections,
    const bool handleCyclicAMI,
    const labelList& changedFaces,
    const List<Type>& changedFacesInfo,
    UList<Type>& allFaceInfo,
    UList<Type>& allCellInfo,
    const label maxIter,
    TrackingData& td
)
:
    mesh_(mesh),
    explicitConnections_(explicitConnections),
    allFaceInfo_(allFaceInfo),
    allCellInfo_(allCellInfo),
    td_(td),
    changedFace_(mesh_.nFaces(), false),
    changedFaces_(mesh_.nFaces()),
    nChangedFaces_(0),
    changedCell_(mesh_.nCells(), false),
    changedCells_(mesh_.nCells()),
    nChangedCells_(0),
    hasCyclicPatches_(hasPatch<cyclicPolyPatch>()),
    hasCyclicAMIPatches_
    (
        handleCyclicAMI
     && returnReduce(hasPatch<cyclicAMIPolyPatch>(), orOp<bool>())
    ),
    nEvals_(0),
    nUnvisitedCells_(mesh_.nCells()),
    nUnvisitedFaces_(mesh_.nFaces())
{
    if
    (
        allFaceInfo.size() != mesh_.nFaces()
     || allCellInfo.size() != mesh_.nCells()
    )
    {
        FatalErrorInFunction
            << "face and cell storage not the size of mesh faces, cells:"
            << endl
            << "    allFaceInfo   :" << allFaceInfo.size() << endl
            << "    mesh_.nFaces():" << mesh_.nFaces() << endl
            << "    allCellInfo   :" << allCellInfo.size() << endl
            << "    mesh_.nCells():" << mesh_.nCells()
            << exit(FatalError);
    }

    // Copy initial changed faces data
    setFaceInfo(changedFaces, changedFacesInfo);

    // Iterate until nothing changes
    label iter = iterate(maxIter);

    if ((maxIter > 0) && (iter >= maxIter))
    {
        FatalErrorInFunction
            << "Maximum number of iterations reached. Increase maxIter." << endl
            << "    maxIter:" << maxIter << endl
            << "    nChangedCells:" << nChangedCells_ << endl
            << "    nChangedFaces:" << nChangedFaces_ << endl
            << exit(FatalError);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


template <class Type, class TrackingData>
CML::label CML::FaceCellWave<Type, TrackingData>::getUnsetCells() const
{
    return nUnvisitedCells_;
}


template <class Type, class TrackingData>
CML::label CML::FaceCellWave<Type, TrackingData>::getUnsetFaces() const
{
    return nUnvisitedFaces_;
}



// Propagate cell to face
template <class Type, class TrackingData>
CML::label CML::FaceCellWave<Type, TrackingData>::faceToCell()
{
    const labelList& owner = mesh_.faceOwner();
    const labelList& neighbour = mesh_.faceNeighbour();
    label nInternalFaces = mesh_.nInternalFaces();

    for
    (
        label changedFaceI = 0;
        changedFaceI < nChangedFaces_;
        changedFaceI++
    )
    {
        label faceI = changedFaces_[changedFaceI];
        if (!changedFace_[faceI])
        {
            FatalErrorInFunction
                << "Face " << faceI
                << " not marked as having been changed"
                << abort(FatalError);
        }


        const Type& neighbourWallInfo = allFaceInfo_[faceI];

        // Evaluate all connected cells

        // Owner
        label cellI = owner[faceI];
        Type& currentWallInfo = allCellInfo_[cellI];

        if (!currentWallInfo.equal(neighbourWallInfo, td_))
        {
            updateCell
            (
                cellI,
                faceI,
                neighbourWallInfo,
                propagationTol_,
                currentWallInfo
            );
        }

        // Neighbour.
        if (faceI < nInternalFaces)
        {
            cellI = neighbour[faceI];
            Type& currentWallInfo2 = allCellInfo_[cellI];

            if (!currentWallInfo2.equal(neighbourWallInfo, td_))
            {
                updateCell
                (
                    cellI,
                    faceI,
                    neighbourWallInfo,
                    propagationTol_,
                    currentWallInfo2
                );
            }
        }

        // Reset status of face
        changedFace_[faceI] = false;
    }

    // Handled all changed faces by now
    nChangedFaces_ = 0;

    if (debug)
    {
        Pout<< " Changed cells            : " << nChangedCells_ << endl;
    }

    // Sum nChangedCells over all procs
    label totNChanged = nChangedCells_;

    reduce(totNChanged, sumOp<label>());

    return totNChanged;
}


// Propagate cell to face
template <class Type, class TrackingData>
CML::label CML::FaceCellWave<Type, TrackingData>::cellToFace()
{
    const cellList& cells = mesh_.cells();

    for
    (
        label changedCellI = 0;
        changedCellI < nChangedCells_;
        changedCellI++
    )
    {
        label cellI = changedCells_[changedCellI];
        if (!changedCell_[cellI])
        {
            FatalErrorInFunction
                << "Cell " << cellI << " not marked as having been changed"
                << abort(FatalError);
        }

        const Type& neighbourWallInfo = allCellInfo_[cellI];

        // Evaluate all connected faces

        const labelList& faceLabels = cells[cellI];
        forAll(faceLabels, faceLabelI)
        {
            label faceI = faceLabels[faceLabelI];
            Type& currentWallInfo = allFaceInfo_[faceI];

            if (!currentWallInfo.equal(neighbourWallInfo, td_))
            {
                updateFace
                (
                    faceI,
                    cellI,
                    neighbourWallInfo,
                    propagationTol_,
                    currentWallInfo
                );
            }
        }

        // Reset status of cell
        changedCell_[cellI] = false;
    }

    // Handled all changed cells by now
    nChangedCells_ = 0;


    // Transfer across any explicitly provided internal connections
    handleExplicitConnections();

    if (hasCyclicPatches_)
    {
        // Transfer changed faces across cyclic halves
        handleCyclicPatches();
    }

    if (hasCyclicAMIPatches_)
    {
        handleAMICyclicPatches();
    }

    if (Pstream::parRun())
    {
        // Transfer changed faces from neighbouring processors.
        handleProcPatches();
    }

    if (debug)
    {
        Pout<< " Changed faces            : " << nChangedFaces_ << endl;
    }

    // Sum nChangedFaces over all procs
    label totNChanged = nChangedFaces_;

    reduce(totNChanged, sumOp<label>());

    return totNChanged;
}


// Iterate
template <class Type, class TrackingData>
CML::label CML::FaceCellWave<Type, TrackingData>::iterate(const label maxIter)
{
    if (hasCyclicPatches_)
    {
        // Transfer changed faces across cyclic halves
        handleCyclicPatches();
    }

    if (hasCyclicAMIPatches_)
    {
        handleAMICyclicPatches();
    }

    if (Pstream::parRun())
    {
        // Transfer changed faces from neighbouring processors.
        handleProcPatches();
    }

    label iter = 0;

    while (iter < maxIter)
    {
        if (debug)
        {
            Info<< " Iteration " << iter << endl;
        }

        nEvals_ = 0;

        label nCells = faceToCell();

        if (debug)
        {
            Info<< " Total changed cells      : " << nCells << endl;
        }

        if (nCells == 0)
        {
            break;
        }

        label nFaces = cellToFace();

        if (debug)
        {
            Info<< " Total changed faces      : " << nFaces << nl
                << " Total evaluations        : " << nEvals_ << nl
                << " Remaining unvisited cells: " << nUnvisitedCells_ << nl
                << " Remaining unvisited faces: " << nUnvisitedFaces_ << endl;
        }

        if (nFaces == 0)
        {
            break;
        }

        ++iter;
    }

    return iter;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
