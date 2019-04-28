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
    CML::PatchEdgeFaceWave

Description
    Wave propagation of information along patch. Every iteration
    information goes through one layer of faces. Templated on information
    that is transferred.

SourceFiles
    PatchEdgeFaceWave.cpp

\*---------------------------------------------------------------------------*/

#ifndef PatchEdgeFaceWave_H
#define PatchEdgeFaceWave_H

#include "scalarField.hpp"
#include "PackedBoolList.hpp"
#include "PrimitivePatch_.hpp"
#include "vectorTensorTransform.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class polyMesh;

/*---------------------------------------------------------------------------*\
                    Class PatchEdgeFaceWaveName Declaration
\*---------------------------------------------------------------------------*/

TemplateName(PatchEdgeFaceWave);


/*---------------------------------------------------------------------------*\
                           Class PatchEdgeFaceWave Declaration
\*---------------------------------------------------------------------------*/

template
<
    class PrimitivePatchType,
    class Type,
    class TrackingData = label
>
class PatchEdgeFaceWave
:
    public PatchEdgeFaceWaveName
{
  // Private static data

        //- Relative tolerance. Stop propagation if relative changes
        //  less than this tolerance (responsibility for checking this is
        //  up to Type implementation)
        static scalar propagationTol_;

        //- Used as default trackdata value to satisfy default template
        //  argument.
        static label dummyTrackData_;


    // Private data

        //- Reference to mesh
        const polyMesh& mesh_;

        //- Reference to patch
        const PrimitivePatchType& patch_;

        //- Wall information for all edges
        UList<Type>& allEdgeInfo_;

        //- Information on all patch faces
        UList<Type>& allFaceInfo_;

        //- Additional data to be passed into container
        TrackingData& td_;

        //- Has edge changed
        PackedBoolList changedEdge_;

        //- List of changed edges
        DynamicList<label> changedEdges_;

        //- Has face changed
        PackedBoolList changedFace_;

        //- List of changed faces
        DynamicList<label> changedFaces_;

        //- Number of evaluations
        label nEvals_;

        //- Number of unvisited faces/edges
        label nUnvisitedEdges_;
        label nUnvisitedFaces_;


        // Addressing between edges of patch_ and globalData.coupledPatch()
        labelList patchEdges_;
        labelList coupledEdges_;
        PackedBoolList sameEdgeOrientation_;


    // Private Member Functions

        //- Updates edgeInfo with information from neighbour. Updates all
        //  statistics.
        bool updateEdge
        (
            const label edgeI,
            const label neighbourFaceI,
            const Type& neighbourInfo,
            Type& edgeInfo
        );

        //- Updates faceInfo with information from neighbour. Updates all
        //  statistics.
        bool updateFace
        (
            const label faceI,
            const label neighbourEdgeI,
            const Type& neighbourInfo,
            Type& faceInfo
        );

        //- Update coupled edges
        void syncEdges();

        //- Disallow default bitwise copy construct
        PatchEdgeFaceWave(const PatchEdgeFaceWave&);

        //- Disallow default bitwise assignment
        void operator=(const PatchEdgeFaceWave&);


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

        //- Construct from patch, list of changed edges with the Type
        //  for these edges. Gets work arrays to operate on, one of size
        //  number of patch edges, the other number of patch faces.
        //  Iterates until nothing changes or maxIter reached.
        //  (maxIter can be 0)
        PatchEdgeFaceWave
        (
            const polyMesh& mesh,
            const PrimitivePatchType& patch,
            const labelList& initialEdges,
            const List<Type>& initialEdgesInfo,
            UList<Type>& allEdgeInfo,
            UList<Type>& allFaceInfo,
            const label maxIter,
            TrackingData& td = dummyTrackData_
        );

        //- Construct from patch. Use setEdgeInfo and iterate() to do
        //  actual calculation
        PatchEdgeFaceWave
        (
            const polyMesh& mesh,
            const PrimitivePatchType& patch,
            UList<Type>& allEdgeInfo,
            UList<Type>& allFaceInfo,
            TrackingData& td = dummyTrackData_
        );


    // Member Functions

        //- Access allEdgeInfo
        UList<Type>& allEdgeInfo() const
        {
            return allEdgeInfo_;
        }

        //- Access allFaceInfo
        UList<Type>& allFaceInfo() const
        {
            return allFaceInfo_;
        }

        //- Additional data to be passed into container
        const TrackingData& data() const
        {
            return td_;
        }

        //- Get number of unvisited faces, i.e. faces that were not (yet)
        //  reached from walking across patch. This can happen from
        //  - not enough iterations done
        //  - a disconnected patch
        //  - a patch without walls in it
        label getUnsetFaces() const;

        label getUnsetEdges() const;

        //- Copy initial data into allEdgeInfo_
        void setEdgeInfo
        (
            const labelList& changedEdges,
            const List<Type>& changedEdgesInfo
        );

        //- Propagate from edge to face. Returns total number of faces
        //  (over all processors) changed.
        label edgeToFace();

        //- Propagate from face to edge. Returns total number of edges
        //  (over all processors) changed.
        label faceToEdge();

        //- Iterate until no changes or maxIter reached. Returns actual
        //  number of iterations.
        label iterate(const label maxIter);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Update operation
template
<
    class PrimitivePatchType,
    class Type,
    class TrackingData = label
>
class updateOp
{
    //- Additional data to be passed into container
    const polyMesh& mesh_;
    const PrimitivePatchType& patch_;
    const scalar tol_;
    TrackingData& td_;

public:
    updateOp
    (
        const polyMesh& mesh,
        const PrimitivePatchType& patch,
        const scalar tol,
        TrackingData& td
    )
    :
        mesh_(mesh),
        patch_(patch),
        tol_(tol),
        td_(td)
    {}

    void operator()(Type& x, const Type& y) const
    {
        if (y.valid(td_))
        {
            x.updateEdge(mesh_, patch_, y, true, tol_, td_);
        }
    }
};


//- Transform operation
template
<
    class PrimitivePatchType,
    class Type,
    class TrackingData = label
>
class transformOp
{
    //- Additional data to be passed into container
    const polyMesh& mesh_;
    const PrimitivePatchType& patch_;
    const scalar tol_;
    TrackingData& td_;

public:
    transformOp
    (
        const polyMesh& mesh,
        const PrimitivePatchType& patch,
        const scalar tol,
        TrackingData& td
    )
    :
        mesh_(mesh),
        patch_(patch),
        tol_(tol),
        td_(td)
    {}

    void operator()
    (
        const vectorTensorTransform& vt,
        const bool forward,
        List<Type>& fld
    ) const
    {
        if (forward)
        {
            forAll(fld, i)
            {
                fld[i].transform(mesh_, patch_, vt.R(), tol_, td_);
            }
        }
        else
        {
            forAll(fld, i)
            {
                fld[i].transform(mesh_, patch_, vt.R().T(), tol_, td_);
            }
        }
    }
};

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "polyMesh.hpp"
#include "globalMeshData.hpp"
#include "PatchTools.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template
<
    class PrimitivePatchType,
    class Type,
    class TrackingData
>
CML::scalar CML::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
propagationTol_ = 0.01;

template
<
    class PrimitivePatchType,
    class Type,
    class TrackingData
>
CML::label
CML::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
dummyTrackData_ = 12345;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// Update info for edgeI, at position pt, with information from
// neighbouring face.
// Updates:
//      - changedEdge_, changedEdges_,
//      - statistics: nEvals_, nUnvisitedEdges_
template
<
    class PrimitivePatchType,
    class Type,
    class TrackingData
>
bool CML::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
updateEdge
(
    const label edgeI,
    const label neighbourFaceI,
    const Type& neighbourInfo,
    Type& edgeInfo
)
{
    nEvals_++;

    bool wasValid = edgeInfo.valid(td_);

    bool propagate =
        edgeInfo.updateEdge
        (
            mesh_,
            patch_,
            edgeI,
            neighbourFaceI,
            neighbourInfo,
            propagationTol_,
            td_
        );

    if (propagate)
    {
        if (!changedEdge_[edgeI])
        {
            changedEdge_[edgeI] = true;
            changedEdges_.append(edgeI);
        }
    }

    if (!wasValid && edgeInfo.valid(td_))
    {
        --nUnvisitedEdges_;
    }

    return propagate;
}


// Update info for faceI, at position pt, with information from
// neighbouring edge.
// Updates:
//      - changedFace_, changedFaces_,
//      - statistics: nEvals_, nUnvisitedFace_
template
<
    class PrimitivePatchType,
    class Type,
    class TrackingData
>
bool CML::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
updateFace
(
    const label faceI,
    const label neighbourEdgeI,
    const Type& neighbourInfo,
    Type& faceInfo
)
{
    nEvals_++;

    bool wasValid = faceInfo.valid(td_);

    bool propagate =
        faceInfo.updateFace
        (
            mesh_,
            patch_,
            faceI,
            neighbourEdgeI,
            neighbourInfo,
            propagationTol_,
            td_
        );

    if (propagate)
    {
        if (!changedFace_[faceI])
        {
            changedFace_[faceI] = true;
            changedFaces_.append(faceI);
        }
    }

    if (!wasValid && faceInfo.valid(td_))
    {
        --nUnvisitedFaces_;
    }

    return propagate;
}


template
<
    class PrimitivePatchType,
    class Type,
    class TrackingData
>
void CML::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
syncEdges()
{
    const globalMeshData& globalData = mesh_.globalData();
    const mapDistribute& map = globalData.globalEdgeSlavesMap();
    const PackedBoolList& cppOrientation = globalData.globalEdgeOrientation();

    // Convert patch-edge data into cpp-edge data
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    //- Construct with all data in consistent orientation
    List<Type> cppEdgeData(map.constructSize());

    forAll(patchEdges_, i)
    {
        label patchEdgeI = patchEdges_[i];
        label coupledEdgeI = coupledEdges_[i];

        if (changedEdge_[patchEdgeI])
        {
            const Type& data = allEdgeInfo_[patchEdgeI];

            // Patch-edge data needs to be converted into coupled-edge data
            // (optionally flipped) and consistent in orientation with
            // master of coupled edge (optionally flipped)
            bool sameOrientation =
            (
                sameEdgeOrientation_[i]
             == cppOrientation[coupledEdgeI]
            );

            cppEdgeData[coupledEdgeI].updateEdge
            (
                mesh_,
                patch_,
                data,
                sameOrientation,
                propagationTol_,
                td_
            );
        }
    }


    // Synchronise
    // ~~~~~~~~~~~

    globalData.syncData
    (
        cppEdgeData,
        globalData.globalEdgeSlaves(),
        globalData.globalEdgeTransformedSlaves(),
        map,
        globalData.globalTransforms(),
        updateOp<PrimitivePatchType, Type, TrackingData>
        (
            mesh_,
            patch_,
            propagationTol_,
            td_
        ),
        transformOp<PrimitivePatchType, Type, TrackingData>
        (
            mesh_,
            patch_,
            propagationTol_,
            td_
        )
    );


    // Back from cpp-edge to patch-edge data
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    forAll(patchEdges_, i)
    {
        label patchEdgeI = patchEdges_[i];
        label coupledEdgeI = coupledEdges_[i];

        const Type& data = cppEdgeData[coupledEdgeI];

        if (data.valid(td_))
        {
            bool sameOrientation =
            (
                sameEdgeOrientation_[i]
             == cppOrientation[coupledEdgeI]
            );

            allEdgeInfo_[patchEdgeI].updateEdge
            (
                mesh_,
                patch_,
                data,
                sameOrientation,
                propagationTol_,
                td_
            );

            if (!changedEdge_[patchEdgeI])
            {
                changedEdges_.append(patchEdgeI);
                changedEdge_[patchEdgeI] = true;
            }
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Iterate, propagating changedEdgesInfo across patch, until no change (or
// maxIter reached). Initial edge values specified.
template
<
    class PrimitivePatchType,
    class Type,
    class TrackingData
>
CML::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
PatchEdgeFaceWave
(
    const polyMesh& mesh,
    const PrimitivePatchType& patch,
    const labelList& changedEdges,
    const List<Type>& changedEdgesInfo,

    UList<Type>& allEdgeInfo,
    UList<Type>& allFaceInfo,

    const label maxIter,
    TrackingData& td
)
:
    mesh_(mesh),
    patch_(patch),
    allEdgeInfo_(allEdgeInfo),
    allFaceInfo_(allFaceInfo),
    td_(td),
    changedEdge_(patch_.nEdges()),
    changedEdges_(patch_.size()),
    changedFace_(patch_.size()),
    changedFaces_(patch_.size()),
    nEvals_(0),
    nUnvisitedEdges_(patch_.nEdges()),
    nUnvisitedFaces_(patch_.size())
{
    // Calculate addressing between patch_ and mesh.globalData().coupledPatch()
    // for ease of synchronisation
    PatchTools::matchEdges
    (
        patch_,
        mesh_.globalData().coupledPatch(),

        patchEdges_,
        coupledEdges_,
        sameEdgeOrientation_
    );


    if (allEdgeInfo_.size() != patch_.nEdges())
    {
        FatalErrorInFunction
            << "size of edgeInfo work array is not equal to the number"
            << " of edges in the patch" << endl
            << "    edgeInfo   :" << allEdgeInfo_.size() << endl
            << "    patch.nEdges:" << patch_.nEdges()
            << exit(FatalError);
    }
    if (allFaceInfo_.size() != patch_.size())
    {
        FatalErrorInFunction
            << "size of edgeInfo work array is not equal to the number"
            << " of faces in the patch" << endl
            << "    faceInfo   :" << allFaceInfo_.size() << endl
            << "    patch.size:" << patch_.size()
            << exit(FatalError);
    }


    // Set from initial changed edges data
    setEdgeInfo(changedEdges, changedEdgesInfo);

    if (debug)
    {
        Pout<< "Seed edges                : " << changedEdges_.size() << endl;
    }

    // Iterate until nothing changes
    label iter = iterate(maxIter);

    if ((maxIter > 0) && (iter >= maxIter))
    {
        FatalErrorInFunction
            << "Maximum number of iterations reached. Increase maxIter." << endl
            << "    maxIter:" << maxIter << endl
            << "    changedEdges:" << changedEdges_.size() << endl
            << "    changedFaces:" << changedFaces_.size() << endl
            << exit(FatalError);
    }
}


template
<
    class PrimitivePatchType,
    class Type,
    class TrackingData
>
CML::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
PatchEdgeFaceWave
(
    const polyMesh& mesh,
    const PrimitivePatchType& patch,
    UList<Type>& allEdgeInfo,
    UList<Type>& allFaceInfo,
    TrackingData& td
)
:
    mesh_(mesh),
    patch_(patch),
    allEdgeInfo_(allEdgeInfo),
    allFaceInfo_(allFaceInfo),
    td_(td),
    changedEdge_(patch_.nEdges()),
    changedEdges_(patch_.nEdges()),
    changedFace_(patch_.size()),
    changedFaces_(patch_.size()),
    nEvals_(0),
    nUnvisitedEdges_(patch_.nEdges()),
    nUnvisitedFaces_(patch_.size())
{
    // Calculate addressing between patch_ and mesh.globalData().coupledPatch()
    // for ease of synchronisation
    PatchTools::matchEdges
    (
        patch_,
        mesh_.globalData().coupledPatch(),

        patchEdges_,
        coupledEdges_,
        sameEdgeOrientation_
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template
<
    class PrimitivePatchType,
    class Type,
    class TrackingData
>
CML::label CML::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
getUnsetEdges() const
{
    return nUnvisitedEdges_;
}


template
<
    class PrimitivePatchType,
    class Type,
    class TrackingData
>
CML::label CML::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
getUnsetFaces() const
{
    return nUnvisitedFaces_;
}


// Copy edge information into member data
template
<
    class PrimitivePatchType,
    class Type,
    class TrackingData
>
void CML::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
setEdgeInfo
(
    const labelList& changedEdges,
    const List<Type>& changedEdgesInfo
)
{
    forAll(changedEdges, changedEdgeI)
    {
        label edgeI = changedEdges[changedEdgeI];

        bool wasValid = allEdgeInfo_[edgeI].valid(td_);

        // Copy info for edgeI
        allEdgeInfo_[edgeI] = changedEdgesInfo[changedEdgeI];

        // Maintain count of unset edges
        if (!wasValid && allEdgeInfo_[edgeI].valid(td_))
        {
            --nUnvisitedEdges_;
        }

        // Mark edgeI as changed, both on list and on edge itself.

        if (!changedEdge_[edgeI])
        {
            changedEdge_[edgeI] = true;
            changedEdges_.append(edgeI);
        }
    }
}


// Propagate information from face to edge. Return number of edges changed.
template
<
    class PrimitivePatchType,
    class Type,
    class TrackingData
>
CML::label CML::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
faceToEdge()
{
    changedEdges_.clear();
    changedEdge_ = false;

    forAll(changedFaces_, changedFaceI)
    {
        label faceI = changedFaces_[changedFaceI];

        if (!changedFace_[faceI])
        {
            FatalErrorInFunction
                << "face " << faceI
                << " not marked as having been changed" << nl
                << "This might be caused by multiple occurrences of the same"
                << " seed edge." << abort(FatalError);
        }

        const Type& neighbourWallInfo = allFaceInfo_[faceI];

        // Evaluate all connected edges
        const labelList& fEdges = patch_.faceEdges()[faceI];

        forAll(fEdges, fEdgeI)
        {
            label edgeI = fEdges[fEdgeI];

            Type& currentWallInfo = allEdgeInfo_[edgeI];

            if (!currentWallInfo.equal(neighbourWallInfo, td_))
            {
                updateEdge
                (
                    edgeI,
                    faceI,
                    neighbourWallInfo,
                    currentWallInfo
                );
            }
        }
    }


    syncEdges();


    if (debug)
    {
        Pout<< "Changed edges             : " << changedEdges_.size() << endl;
    }

    return returnReduce(changedEdges_.size(), sumOp<label>());
}


// Propagate information from edge to face. Return number of faces changed.
template
<
    class PrimitivePatchType,
    class Type,
    class TrackingData
>
CML::label CML::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
edgeToFace()
{
    changedFaces_.clear();
    changedFace_ = false;

    const labelListList& edgeFaces = patch_.edgeFaces();

    forAll(changedEdges_, changedEdgeI)
    {
        label edgeI = changedEdges_[changedEdgeI];

        if (!changedEdge_[edgeI])
        {
            FatalErrorInFunction
                << "edge " << edgeI
                << " not marked as having been changed" << nl
                << "This might be caused by multiple occurrences of the same"
                << " seed edge." << abort(FatalError);
        }

        const Type& neighbourWallInfo = allEdgeInfo_[edgeI];

        // Evaluate all connected faces

        const labelList& eFaces = edgeFaces[edgeI];
        forAll(eFaces, eFaceI)
        {
            label faceI = eFaces[eFaceI];

            Type& currentWallInfo = allFaceInfo_[faceI];

            if (!currentWallInfo.equal(neighbourWallInfo, td_))
            {
                updateFace
                (
                    faceI,
                    edgeI,
                    neighbourWallInfo,
                    currentWallInfo
                );
            }
        }
    }

    if (debug)
    {
        Pout<< "Changed faces             : " << changedFaces_.size() << endl;
    }

    return returnReduce(changedFaces_.size(), sumOp<label>());
}


// Iterate
template
<
    class PrimitivePatchType,
    class Type,
    class TrackingData
>
CML::label CML::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
iterate
(
    const label maxIter
)
{
    // Make sure coupled edges contain same info
    syncEdges();

    nEvals_ = 0;

    label iter = 0;

    while (iter < maxIter)
    {
        if (debug)
        {
            Pout<< "Iteration " << iter << endl;
        }

        label nFaces = edgeToFace();

        if (debug)
        {
            Pout<< "Total changed faces       : " << nFaces << endl;
        }

        if (nFaces == 0)
        {
            break;
        }

        label nEdges = faceToEdge();

        if (debug)
        {
            Pout<< "Total changed edges       : " << nEdges << nl
                << "Total evaluations         : " << nEvals_ << nl
                << "Remaining unvisited edges : " << nUnvisitedEdges_ << nl
                << "Remaining unvisited faces : " << nUnvisitedFaces_ << nl
                << endl;
        }

        if (nEdges == 0)
        {
            break;
        }

        iter++;
    }

    return iter;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
