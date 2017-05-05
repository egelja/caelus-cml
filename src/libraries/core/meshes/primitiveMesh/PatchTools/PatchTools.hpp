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

Class
    CML::PatchTools

Description
    A collection of tools for searching, sorting PrimitivePatch information.

    The class could also be extended to include more that just static methods.

SourceFiles
    PatchTools.cpp
    PatchToolsCheck.cpp
    PatchToolsEdgeOwner.cpp
    PatchToolsGatherAndMerge.cpp
    PatchToolsMatch.cpp
    PatchToolsNormals.cpp
    PatchToolsSearch.cpp
    PatchToolsSortEdges.cpp
    PatchToolsSortPoints.cpp

\*---------------------------------------------------------------------------*/

#ifndef PatchTools_H
#define PatchTools_H

#include "PrimitivePatch_.hpp"
#include "HashSet.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class polyMesh;
class PackedBoolList;
class boundBox;

/*---------------------------------------------------------------------------*\
                         Class PatchTools Declaration
\*---------------------------------------------------------------------------*/

class PatchTools
{
public:

    //- Check for orientation issues.
    //  Returns true if problems were found.
    //  If a normal flips across an edge, places it in the HashSet
    template
    <
        class Face,
        template<class> class FaceList,
        class PointField,
        class PointType
    >
    static bool checkOrientation
    (
        const PrimitivePatch<Face, FaceList, PointField, PointType>&,
        const bool report = false,
        labelHashSet* marked = 0
    );


    //- Fill faceZone with currentZone for every face reachable
    //  from faceI without crossing edge marked in borderEdge.
    //  Note: faceZone has to be sized nFaces before calling.
    template
    <
        class BoolListType,
        class Face,
        template<class> class FaceList,
        class PointField,
        class PointType
    >
    static void markZone
    (
        const PrimitivePatch<Face, FaceList, PointField, PointType>&,
        const BoolListType& borderEdge,
        const label faceI,
        const label currentZone,
        labelList& faceZone
    );

    //- Size and fills faceZone with zone of face.
    //  Zone is area reachable by edge crossing without crossing borderEdge.
    //  Returns number of zones.
    template
    <
        class BoolListType,
        class Face,
        template<class> class FaceList,
        class PointField,
        class PointType
    >
    static label markZones
    (
        const PrimitivePatch<Face, FaceList, PointField, PointType>&,
        const BoolListType& borderEdge,
        labelList& faceZone
    );

    //- Determine the mapping for a sub-patch.
    //  Only include faces for which bool-list entry is true.
    //  \param[in]  includeFaces faces to include
    //  \param[out] pointMap mapping new to old localPoints
    //  \param[out] faceMap  mapping new to old faces
    template
    <
        class BoolListType,
        class Face,
        template<class> class FaceList,
        class PointField,
        class PointType
    >
    static void subsetMap
    (
        const PrimitivePatch<Face, FaceList, PointField, PointType>&,
        const BoolListType& includeFaces,
        labelList& pointMap,
        labelList& faceMap
    );

    //-
    template
    <
        class Face,
        template<class> class FaceList,
        class PointField,
        class PointType
    >
    static void calcBounds
    (
        const PrimitivePatch<Face, FaceList, PointField, PointType>& p,
        boundBox& bb,
        label& nPoints
    );

    //- Return edge-face addressing sorted by angle around the edge.
    //  Orientation is anticlockwise looking from edge.vec(localPoints())
    template
    <
        class Face,
        template<class> class FaceList,
        class PointField,
        class PointType
    >
    static labelListList sortedEdgeFaces
    (
        const PrimitivePatch<Face, FaceList, PointField, PointType>&
    );

    //- Return point-edge addressing sorted by order around the point.
    template
    <
        class Face,
        template<class> class FaceList,
        class PointField,
        class PointType
    >
    static labelListList sortedPointEdges
    (
        const PrimitivePatch<Face, FaceList, PointField, PointType>&
    );

    //- If 2 face neighbours: label of face where ordering of edge
    //  is consistent with righthand walk.
    //  If 1 neighbour: label of only face.
    //  If >2 neighbours: undetermined.
    template
    <
        class Face,
        template<class> class FaceList,
        class PointField,
        class PointType
    >
    static labelList edgeOwner
    (
        const PrimitivePatch<Face, FaceList, PointField, PointType>&
    );


    //- Find corresponding points on patches sharing the same points
    //  p1PointLabels : points on p1 that were matched
    //  p2PointLabels : corresponding points on p2
    template
    <
        class Face1,
        template<class> class FaceList1,
        class PointField1,
        class PointType1,
        class Face2,
        template<class> class FaceList2,
        class PointField2,
        class PointType2
    >
    static void matchPoints
    (
        const PrimitivePatch<Face1, FaceList1, PointField1, PointType1>& p1,
        const PrimitivePatch<Face2, FaceList2, PointField2, PointType2>& p2,

        labelList& p1PointLabels,
        labelList& p2PointLabels
    );

    //- Find corresponding edges on patches sharing the same points
    //  p1EdgeLabels    : edges on p1 that were matched
    //  p2EdgeLabels    : corresponding edges on p2
    //  sameOrientation : same orientation?
    template
    <
        class Face1,
        template<class> class FaceList1,
        class PointField1,
        class PointType1,
        class Face2,
        template<class> class FaceList2,
        class PointField2,
        class PointType2
    >
    static void matchEdges
    (
        const PrimitivePatch<Face1, FaceList1, PointField1, PointType1>& p1,
        const PrimitivePatch<Face2, FaceList2, PointField2, PointType2>& p2,

        labelList& p1EdgeLabels,
        labelList& p2EdgeLabels,
        PackedBoolList& sameOrientation
    );


    //- Return parallel consistent point normals for patches using mesh points.
    template
    <
        class Face,
        template<class> class FaceList,
        class PointField,
        class PointType
    >
    static tmp<pointField> pointNormals
    (
        const polyMesh&,
        const PrimitivePatch<Face, FaceList, PointField, PointType>&
    );


    //- Return parallel consistent edge normals for patches using mesh points.
    //  Supply with patch matching info from matchEdges.
    template
    <
        class Face,
        template<class> class FaceList,
        class PointField,
        class PointType
    >
    static tmp<pointField> edgeNormals
    (
        const polyMesh&,
        const PrimitivePatch<Face, FaceList, PointField, PointType>&,
        const labelList& patchEdges,
        const labelList& coupledEdges
    );


    //- Gather points and faces onto master and merge into single patch.
    //  Note: uses faces/points, not localFaces/localPoints.
    template
    <
        class Face,
        template<class> class FaceList,
        class PointField,
        class PointType
    >
    static void gatherAndMerge
    (
        const scalar mergeDist,
        const PrimitivePatch<Face, FaceList, PointField, PointType>& p,
        Field<PointType>& mergedPoints,
        List<Face>& mergedFaces,
        labelList& pointMergeMap
    );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// PatchToolsCheck
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>

bool
CML::PatchTools::checkOrientation
(
    const PrimitivePatch<Face, FaceList, PointField, PointType>& p,
    const bool report,
    labelHashSet* setPtr
)
{
    bool foundError = false;

    // Check edge normals, face normals, point normals.
    forAll(p.faceEdges(), faceI)
    {
        const labelList& edgeLabels = p.faceEdges()[faceI];
        bool valid = true;

        if (edgeLabels.size() < 3)
        {
            if (report)
            {
                Info<< "Face[" << faceI << "] " << p[faceI]
                    << " has fewer than 3 edges. Edges: " << edgeLabels
                    << endl;
            }
            valid = false;
        }
        else
        {
            forAll(edgeLabels, i)
            {
                if (edgeLabels[i] < 0 || edgeLabels[i] >= p.nEdges())
                {
                    if (report)
                    {
                        Info<< "edge number " << edgeLabels[i]
                            << " on face " << faceI
                            << " out-of-range\n"
                            << "This usually means the input surface has "
                            << "edges with more than 2 faces connected."
                            << endl;
                    }
                    valid = false;
                }
            }
        }

        if (!valid)
        {
            foundError = true;
            continue;
        }


        //
        //- Compute normal from 3 points, use the first as the origin
        // minor warpage should not be a problem
        const Face& f = p[faceI];
        const point& p0 = p.points()[f[0]];
        const point& p1 = p.points()[f[1]];
        const point& p2 = p.points()[f.last()];

        const vector pointNormal((p1 - p0) ^ (p2 - p0));
        if ((pointNormal & p.faceNormals()[faceI]) < 0)
        {
            foundError = false;

            if (report)
            {
                Info
                    << "Normal calculated from points inconsistent"
                    << " with faceNormal" << nl
                    << "face: " << f << nl
                    << "points: " << p0 << ' ' << p1 << ' ' << p2 << nl
                    << "pointNormal:" << pointNormal << nl
                    << "faceNormal:" << p.faceNormals()[faceI] << endl;
            }
        }
    }


    forAll(p.edges(), edgeI)
    {
        const edge& e = p.edges()[edgeI];
        const labelList& neighbouringFaces = p.edgeFaces()[edgeI];

        if (neighbouringFaces.size() == 2)
        {
            // we use localFaces() since edges() are LOCAL
            // these are both already available
            const Face& faceA = p.localFaces()[neighbouringFaces[0]];
            const Face& faceB = p.localFaces()[neighbouringFaces[1]];

            // If the faces are correctly oriented, the edges must go in
            // different directions on connected faces.
            if (faceA.edgeDirection(e) == faceB.edgeDirection(e))
            {
                if (report)
                {
                    Info<< "face orientation incorrect." << nl
                        << "localEdge[" << edgeI << "] " << e
                        << " between faces:" << nl
                        << "  face[" << neighbouringFaces[0] << "] "
                        << p[neighbouringFaces[0]]
                        << "   localFace: " << faceA
                        << nl
                        << "  face[" << neighbouringFaces[1] << "] "
                        << p[neighbouringFaces[1]]
                        << "   localFace: " << faceB
                        << endl;
                }
                if (setPtr)
                {
                    setPtr->insert(edgeI);
                }

                foundError = true;
            }
        }
        else if (neighbouringFaces.size() != 1)
        {
            if (report)
            {
                Info
                    << "Wrong number of edge neighbours." << nl
                    << "edge[" << edgeI << "] " << e
                    << " with points:" << p.localPoints()[e.start()]
                    << ' ' << p.localPoints()[e.end()]
                    << " has neighbouringFaces:" << neighbouringFaces << endl;
            }

            if (setPtr)
            {
                setPtr->insert(edgeI);
            }

            foundError = true;
        }
    }

    return foundError;
}


// PatchToolsEdgeOwner
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>

CML::labelList
CML::PatchTools::edgeOwner
(
    const PrimitivePatch<Face, FaceList, PointField, PointType>& p
)
{
    const edgeList& edges = p.edges();
    const labelListList& edgeFaces = p.edgeFaces();
    const List<Face>& localFaces = p.localFaces();

    // create the owner list
    labelList edgeOwner(edges.size(), -1);

    forAll(edges, edgeI)
    {
        const labelList& nbrFaces = edgeFaces[edgeI];

        if (nbrFaces.size() == 1)
        {
            edgeOwner[edgeI] = nbrFaces[0];
        }
        else
        {
            // Find the first face whose vertices are aligned with the edge.
            // with multiply connected edges, this is the best we can do
            forAll(nbrFaces, i)
            {
                const Face& f = localFaces[nbrFaces[i]];

                if (f.edgeDirection(edges[edgeI]) > 0)
                {
                    edgeOwner[edgeI] = nbrFaces[i];
                    break;
                }
            }

            if (edgeOwner[edgeI] == -1)
            {
                FatalErrorIn
                (
                    "PatchTools::edgeOwner()"
                )
                    << "Edge " << edgeI << " vertices:" << edges[edgeI]
                    << " is used by faces " << nbrFaces
                    << " vertices:"
                    << UIndirectList<Face>(localFaces, nbrFaces)()
                    << " none of which use the edge vertices in the same order"
                    << nl << "I give up" << abort(FatalError);
            }
        }
    }

    return edgeOwner;
}


// PatchToolsGatherAndMerge
#include "ListListOps.hpp"
#include "mergePoints.hpp"
#include "face.hpp"
#include "triFace.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void CML::PatchTools::gatherAndMerge
(
    const scalar mergeDist,
    const PrimitivePatch<Face, FaceList, PointField, PointType>& p,
    Field<PointType>& mergedPoints,
    List<Face>& mergedFaces,
    labelList& pointMergeMap
)
{
    // Collect points from all processors
    labelList pointSizes;
    {
        List<Field<PointType> > gatheredPoints(Pstream::nProcs());
        gatheredPoints[Pstream::myProcNo()] = p.points();

        Pstream::gatherList(gatheredPoints);

        if (Pstream::master())
        {
            pointSizes = ListListOps::subSizes
            (
                gatheredPoints,
                accessOp<Field<PointType> >()
            );

            mergedPoints = ListListOps::combine<Field<PointType> >
            (
                gatheredPoints,
                accessOp<Field<PointType> >()
            );
        }
    }

    // Collect faces from all processors and renumber using sizes of
    // gathered points
    {
        List<List<Face> > gatheredFaces(Pstream::nProcs());
        gatheredFaces[Pstream::myProcNo()] = p;
        Pstream::gatherList(gatheredFaces);

        if (Pstream::master())
        {
            mergedFaces = static_cast<const List<Face>&>
            (
                ListListOps::combineOffset<List<Face> >
                (
                    gatheredFaces,
                    pointSizes,
                    accessOp<List<Face> >(),
                    offsetOp<Face>()
                )
            );
        }
    }

    if (Pstream::master())
    {
        Field<PointType> newPoints;
        labelList oldToNew;

        bool hasMerged = mergePoints
        (
            mergedPoints,
            mergeDist,
            false,                  // verbosity
            oldToNew,
            newPoints
        );

        if (hasMerged)
        {
            // Store point mapping
            pointMergeMap.transfer(oldToNew);

            // Copy points
            mergedPoints.transfer(newPoints);

            // Relabel faces
            List<Face>& faces = mergedFaces;

            forAll(faces, faceI)
            {
                inplaceRenumber(pointMergeMap, faces[faceI]);
            }
        }
    }
}


// PatchToolsSearch
#include "PackedBoolList.hpp"
#include "boundBox.hpp"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Finds area, starting at faceI, delimited by borderEdge.
// Marks all visited faces (from face-edge-face walk) with currentZone.
template
<
    class BoolListType,
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>

void
CML::PatchTools::markZone
(
    const PrimitivePatch<Face, FaceList, PointField, PointType>& p,
    const BoolListType& borderEdge,
    const label faceI,
    const label currentZone,
    labelList&  faceZone
)
{
    const labelListList& faceEdges = p.faceEdges();
    const labelListList& edgeFaces = p.edgeFaces();

    // List of faces whose faceZone has been set.
    labelList changedFaces(1, faceI);

    while (true)
    {
        // Pick up neighbours of changedFaces
        DynamicList<label> newChangedFaces(2*changedFaces.size());

        forAll(changedFaces, i)
        {
            label faceI = changedFaces[i];

            const labelList& fEdges = faceEdges[faceI];

            forAll(fEdges, fEdgeI)
            {
                label edgeI = fEdges[fEdgeI];

                if (!borderEdge[edgeI])
                {
                    const labelList& eFaceLst = edgeFaces[edgeI];

                    forAll(eFaceLst, j)
                    {
                        label nbrFaceI = eFaceLst[j];

                        if (faceZone[nbrFaceI] == -1)
                        {
                            faceZone[nbrFaceI] = currentZone;
                            newChangedFaces.append(nbrFaceI);
                        }
                        else if (faceZone[nbrFaceI] != currentZone)
                        {
                            FatalErrorIn
                            (
                                "PatchTools::markZone"
                                "("
                                    "const PrimitivePatch<Face, FaceList, "
                                        "PointField, PointType>& p,"
                                    "const BoolListType& borderEdge,"
                                    "const label faceI,"
                                    "const label currentZone,"
                                    "labelList&  faceZone"
                                ")"
                            )
                                << "Zones " << faceZone[nbrFaceI]
                                << " at face " << nbrFaceI
                                << " connects to zone " << currentZone
                                << " at face " << faceI
                                << abort(FatalError);
                        }
                    }
                }
            }
        }

        if (newChangedFaces.empty())
        {
            break;
        }

        // transfer from dynamic to normal list
        changedFaces.transfer(newChangedFaces);
    }
}


// Finds areas delimited by borderEdge (or 'real' edges).
// Fills faceZone accordingly
template
<
    class BoolListType,
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>

CML::label
CML::PatchTools::markZones
(
    const PrimitivePatch<Face, FaceList, PointField, PointType>& p,
    const BoolListType& borderEdge,
    labelList& faceZone
)
{
    faceZone.setSize(p.size());
    faceZone = -1;

    label zoneI = 0;
    for (label startFaceI = 0; startFaceI < faceZone.size();)
    {
        // Find next non-visited face
        for (; startFaceI < faceZone.size(); ++startFaceI)
        {
            if (faceZone[startFaceI] == -1)
            {
                faceZone[startFaceI] = zoneI;
                markZone(p, borderEdge, startFaceI, zoneI, faceZone);
                zoneI++;
                break;
            }
        }
    }

    return zoneI;
}



// Finds areas delimited by borderEdge (or 'real' edges).
// Fills faceZone accordingly
template
<
    class BoolListType,
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>

void
CML::PatchTools::subsetMap
(
    const PrimitivePatch<Face, FaceList, PointField, PointType>& p,
    const BoolListType& includeFaces,
    labelList& pointMap,
    labelList& faceMap
)
{
    label faceI  = 0;
    label pointI = 0;

    const List<Face>& localFaces = p.localFaces();

    faceMap.setSize(localFaces.size());
    pointMap.setSize(p.nPoints());

    boolList pointHad(pointMap.size(), false);

    forAll(p, oldFaceI)
    {
        if (includeFaces[oldFaceI])
        {
            // Store new faces compact
            faceMap[faceI++] = oldFaceI;

            // Renumber labels for face
            const Face& f = localFaces[oldFaceI];

            forAll(f, fp)
            {
                const label ptLabel = f[fp];
                if (!pointHad[ptLabel])
                {
                    pointHad[ptLabel]  = true;
                    pointMap[pointI++] = ptLabel;
                }
            }
        }
    }

    // Trim
    faceMap.setSize(faceI);
    pointMap.setSize(pointI);
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void CML::PatchTools::calcBounds
(
    const PrimitivePatch<Face, FaceList, PointField, PointType>& p,
    boundBox& bb,
    label& nPoints
)
{
    // Unfortunately nPoints constructs meshPoints() so do compact version
    // ourselves
    const PointField& points = p.points();

    PackedBoolList pointIsUsed(points.size());

    nPoints = 0;
    bb = boundBox::invertedBox;

    forAll(p, faceI)
    {
        const Face& f = p[faceI];

        forAll(f, fp)
        {
            label pointI = f[fp];
            if (pointIsUsed.set(pointI, 1u))
            {
                bb.min() = ::CML::min(bb.min(), points[pointI]);
                bb.max() = ::CML::max(bb.max(), points[pointI]);
                nPoints++;
            }
        }
    }
}

// PatchToolsSortEdges
#include "SortableList.hpp"
#include "transform.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>

CML::labelListList
CML::PatchTools::sortedEdgeFaces
(
    const PrimitivePatch<Face, FaceList, PointField, PointType>& p
)
{
    const edgeList& edges = p.edges();
    const labelListList& edgeFaces = p.edgeFaces();
    const List<Face>& localFaces = p.localFaces();
    const Field<PointType>& localPoints = p.localPoints();

    // create the lists for the various results. (resized on completion)
    labelListList sortedEdgeFaces(edgeFaces.size());

    forAll(edgeFaces, edgeI)
    {
        const labelList& faceNbs = edgeFaces[edgeI];

        if (faceNbs.size() > 2)
        {
            // Get point on edge and normalized direction of edge (= e2 base
            // of our coordinate system)
            const edge& e = edges[edgeI];

            const point& edgePt = localPoints[e.start()];

            vector e2 = e.vec(localPoints);
            e2 /= mag(e2) + VSMALL;

            // Get the vertex on 0th face that forms a vector with the first
            // edge point that has the largest angle with the edge
            const Face& f0 = localFaces[faceNbs[0]];

            scalar maxAngle = GREAT;
            vector maxAngleEdgeDir(vector::max);

            forAll(f0, fpI)
            {
                if (f0[fpI] != e.start())
                {
                    vector faceEdgeDir = localPoints[f0[fpI]] - edgePt;
                    faceEdgeDir /= mag(faceEdgeDir) + VSMALL;

                    const scalar angle = e2 & faceEdgeDir;

                    if (mag(angle) < maxAngle)
                    {
                        maxAngle = angle;
                        maxAngleEdgeDir = faceEdgeDir;
                    }
                }
            }

            // Get vector normal both to e2 and to edge from opposite vertex
            // to edge (will be x-axis of our coordinate system)
            vector e0 = e2 ^ maxAngleEdgeDir;
            e0 /= mag(e0) + VSMALL;

            // Get y-axis of coordinate system
            vector e1 = e2 ^ e0;

            SortableList<scalar> faceAngles(faceNbs.size());

            // e0 is reference so angle is 0
            faceAngles[0] = 0;

            for (label nbI = 1; nbI < faceNbs.size(); nbI++)
            {
                // Get the vertex on face that forms a vector with the first
                // edge point that has the largest angle with the edge
                const Face& f = localFaces[faceNbs[nbI]];

                maxAngle = GREAT;
                maxAngleEdgeDir = vector::max;

                forAll(f, fpI)
                {
                    if (f[fpI] != e.start())
                    {
                        vector faceEdgeDir = localPoints[f[fpI]] - edgePt;
                        faceEdgeDir /= mag(faceEdgeDir) + VSMALL;

                        const scalar angle = e2 & faceEdgeDir;

                        if (mag(angle) < maxAngle)
                        {
                            maxAngle = angle;
                            maxAngleEdgeDir = faceEdgeDir;
                        }
                    }
                }

                vector vec = e2 ^ maxAngleEdgeDir;
                vec /= mag(vec) + VSMALL;

                faceAngles[nbI] = pseudoAngle
                (
                    e0,
                    e1,
                    vec
                );
            }

            faceAngles.sort();

            sortedEdgeFaces[edgeI] = UIndirectList<label>
            (
                faceNbs,
                faceAngles.indices()
            );
        }
        else
        {
            // No need to sort. Just copy.
            sortedEdgeFaces[edgeI] = faceNbs;
        }
    }

    return sortedEdgeFaces;
}

// PatchToolsSortPoints
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>

CML::labelListList
CML::PatchTools::sortedPointEdges
(
    const PrimitivePatch<Face, FaceList, PointField, PointType>& p
)
{
    // Now order the edges of each point according to whether they share a
    // face
    const labelListList& pointEdges = p.pointEdges();
    const edgeList& edges = p.edges();
    const labelListList& edgeFaces = p.edgeFaces();
    const labelListList& faceEdges = p.faceEdges();

    // create the lists for the various results. (resized on completion)
    labelListList sortedPointEdges(pointEdges);

    DynamicList<label> newEdgeList;

    forAll(pointEdges, pointI)
    {
        const labelList& pEdges = pointEdges[pointI];

        label nPointEdges = pEdges.size();

        label edgeI = pEdges[0];

        label prevFaceI = edgeFaces[edgeI][0];

        newEdgeList.clear();
        newEdgeList.setCapacity(nPointEdges);

        label nVisitedEdges = 0;

        do
        {
            newEdgeList.append(edgeI);

            // Cross edge to next face
            const labelList& eFaces = edgeFaces[edgeI];

            if (eFaces.size() != 2)
            {
                break;
            }

            label faceI = eFaces[0];
            if (faceI == prevFaceI)
            {
                faceI = eFaces[1];
            }

            // Cross face to next edge
            const labelList& fEdges = faceEdges[faceI];

            forAll(fEdges, feI)
            {
                const label nextEdgeI = fEdges[feI];
                const edge& nextEdge = edges[nextEdgeI];

                if
                (
                    nextEdgeI != edgeI
                 && (nextEdge.start() == pointI || nextEdge.end() == pointI)
                )
                {
                    edgeI = nextEdgeI;
                    break;
                }
            }

            prevFaceI = faceI;

            nVisitedEdges++;
            if (nVisitedEdges > nPointEdges)
            {
                WarningIn("CML::PatchTools::sortedPointEdges()")
                    << "Unable to order pointEdges as the face connections "
                    << "are not circular" << nl
                    << "    Original pointEdges = " << pEdges << nl
                    << "    New pointEdges = " << newEdgeList
                    << endl;

                newEdgeList = pEdges;

                break;
            }

        } while (edgeI != pEdges[0]);

        if (newEdgeList.size() == nPointEdges)
        {
            forAll(pEdges, eI)
            {
                if (findIndex(newEdgeList, pEdges[eI]) == -1)
                {
                    WarningIn("CML::PatchTools::sortedPointEdges()")
                        << "Cannot find all original edges in the new list"
                        << nl
                        << "    Original pointEdges = " << pEdges << nl
                        << "    New pointEdges = " << newEdgeList
                        << endl;

                    newEdgeList = pEdges;

                    break;
                }
            }

            sortedPointEdges[pointI] = newEdgeList;
        }
    }

    return sortedPointEdges;
}


// PatchToolsNormals
#include "polyMesh.hpp"
#include "indirectPrimitivePatch.hpp"
#include "globalMeshData.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>

CML::tmp<CML::pointField>
CML::PatchTools::pointNormals
(
    const polyMesh& mesh,
    const PrimitivePatch<Face, FaceList, PointField, PointType>& p
)
{
    const globalMeshData& globalData = mesh.globalData();
    const indirectPrimitivePatch& coupledPatch = globalData.coupledPatch();
    const Map<label>& coupledPatchMP = coupledPatch.meshPointMap();
    const mapDistribute& map = globalData.globalPointSlavesMap();
    const globalIndexAndTransform& transforms =
        globalData.globalTransforms();




    // Combine normals. Note: do on all master points. Cannot just use
    // patch points since the master point does not have to be on the
    // patch!

    pointField coupledPointNormals(map.constructSize(), vector::zero);

    {
        // Collect local pointFaces (sized on patch points only)
        List<List<point> > pointFaceNormals(map.constructSize());
        forAll(p.meshPoints(), patchPointI)
        {
            label meshPointI = p.meshPoints()[patchPointI];
            Map<label>::const_iterator fnd = coupledPatchMP.find(meshPointI);
            if (fnd != coupledPatchMP.end())
            {
                label coupledPointI = fnd();

                List<point>& pNormals = pointFaceNormals[coupledPointI];
                const labelList& pFaces = p.pointFaces()[patchPointI];
                pNormals.setSize(pFaces.size());
                forAll(pFaces, i)
                {
                    pNormals[i] = p.faceNormals()[pFaces[i]];
                }
            }
        }


        // Pull remote data into local slots
        map.distribute
        (
            transforms,
            pointFaceNormals,
            mapDistribute::transform()
        );


        // Combine all face normals (-local, -remote,untransformed,
        //  -remote,transformed)

        const labelListList& slaves = globalData.globalPointSlaves();
        const labelListList& transformedSlaves =
            globalData.globalPointTransformedSlaves();

        forAll(slaves, coupledPointI)
        {
            const labelList& slaveSlots = slaves[coupledPointI];
            const labelList& transformedSlaveSlots =
                transformedSlaves[coupledPointI];

            point& n = coupledPointNormals[coupledPointI];

            // Local entries
            const List<point>& local = pointFaceNormals[coupledPointI];

            label nFaces =
                local.size()
              + slaveSlots.size()
              + transformedSlaveSlots.size();

            n = sum(local);

            // Add any remote face normals
            forAll(slaveSlots, i)
            {
                n += sum(pointFaceNormals[slaveSlots[i]]);
            }
            forAll(transformedSlaveSlots, i)
            {
                n += sum(pointFaceNormals[transformedSlaveSlots[i]]);
            }

            if (nFaces >= 1)
            {
                n /= mag(n)+VSMALL;
            }

            // Put back into slave slots
            forAll(slaveSlots, i)
            {
                coupledPointNormals[slaveSlots[i]] = n;
            }
            forAll(transformedSlaveSlots, i)
            {
                coupledPointNormals[transformedSlaveSlots[i]] = n;
            }
        }


        // Send back
        map.reverseDistribute
        (
            transforms,
            coupledPointNormals.size(),
            coupledPointNormals,
            mapDistribute::transform()
        );
    }


    // 1. Start off with local normals (note:without calculating pointNormals
    //    to avoid them being stored)

    tmp<pointField> textrudeN(new pointField(p.nPoints(), vector::zero));
    pointField& extrudeN = textrudeN();
    {
        const faceList& localFaces = p.localFaces();
        const vectorField& faceNormals = p.faceNormals();

        forAll(localFaces, faceI)
        {
            const face& f = localFaces[faceI];
            const vector& n = faceNormals[faceI];
            forAll(f, fp)
            {
                extrudeN[f[fp]] += n;
            }
        }
        extrudeN /= mag(extrudeN)+VSMALL;
    }


    // 2. Override patch normals on coupled points
    forAll(p.meshPoints(), patchPointI)
    {
        label meshPointI = p.meshPoints()[patchPointI];
        Map<label>::const_iterator fnd = coupledPatchMP.find(meshPointI);
        if (fnd != coupledPatchMP.end())
        {
            label coupledPointI = fnd();
            extrudeN[patchPointI] = coupledPointNormals[coupledPointI];
        }
    }

    return textrudeN;
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>

CML::tmp<CML::pointField>
CML::PatchTools::edgeNormals
(
    const polyMesh& mesh,
    const PrimitivePatch<Face, FaceList, PointField, PointType>& p,
    const labelList& patchEdges,
    const labelList& coupledEdges
)
{
    // 1. Start off with local normals

    tmp<pointField> tedgeNormals(new pointField(p.nEdges(), vector::zero));
    pointField& edgeNormals = tedgeNormals();
    {
        const labelListList& edgeFaces = p.edgeFaces();
        const vectorField& faceNormals = p.faceNormals();

        forAll(edgeFaces, edgeI)
        {
            const labelList& eFaces = edgeFaces[edgeI];
            forAll(eFaces, i)
            {
                edgeNormals[edgeI] += faceNormals[eFaces[i]];
            }
        }
        edgeNormals /= mag(edgeNormals)+VSMALL;
    }



    const globalMeshData& globalData = mesh.globalData();
    const mapDistribute& map = globalData.globalEdgeSlavesMap();


    // Convert patch-edge data into cpp-edge data
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    //- Construct with all data in consistent orientation
    pointField cppEdgeData(map.constructSize(), vector::zero);

    forAll(patchEdges, i)
    {
        label patchEdgeI = patchEdges[i];
        label coupledEdgeI = coupledEdges[i];
        cppEdgeData[coupledEdgeI] = edgeNormals[patchEdgeI];
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
        plusEqOp<point>(),              // add since normalised later on
        mapDistribute::transform()
    );
    cppEdgeData /= mag(cppEdgeData)+VSMALL;


    // Back from cpp-edge to patch-edge data
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    forAll(patchEdges, i)
    {
        label patchEdgeI = patchEdges[i];
        label coupledEdgeI = coupledEdges[i];
        edgeNormals[patchEdgeI] = cppEdgeData[coupledEdgeI];
    }

    return tedgeNormals;
}


// ************************************************************************* //// PatchToolsMatch
template
<
    class Face1,
    template<class> class FaceList1,
    class PointField1,
    class PointType1,
    class Face2,
    template<class> class FaceList2,
    class PointField2,
    class PointType2
>
void CML::PatchTools::matchPoints
(
    const PrimitivePatch<Face1, FaceList1, PointField1, PointType1>& p1,
    const PrimitivePatch<Face2, FaceList2, PointField2, PointType2>& p2,

    labelList& p1PointLabels,
    labelList& p2PointLabels
)
{
    p1PointLabels.setSize(p1.nPoints());
    p2PointLabels.setSize(p1.nPoints());

    label nMatches = 0;

    forAll(p1.meshPoints(), pointI)
    {
        label meshPointI = p1.meshPoints()[pointI];

        Map<label>::const_iterator iter = p2.meshPointMap().find
        (
            meshPointI
        );

        if (iter != p2.meshPointMap().end())
        {
            p1PointLabels[nMatches] = pointI;
            p2PointLabels[nMatches] = iter();
            nMatches++;
        }
    }
    p1PointLabels.setSize(nMatches);
    p2PointLabels.setSize(nMatches);
}


template
<
    class Face1,
    template<class> class FaceList1,
    class PointField1,
    class PointType1,
    class Face2,
    template<class> class FaceList2,
    class PointField2,
    class PointType2
>
void CML::PatchTools::matchEdges
(
    const PrimitivePatch<Face1, FaceList1, PointField1, PointType1>& p1,
    const PrimitivePatch<Face2, FaceList2, PointField2, PointType2>& p2,

    labelList& p1EdgeLabels,
    labelList& p2EdgeLabels,
    PackedBoolList& sameOrientation
)
{
    p1EdgeLabels.setSize(p1.nEdges());
    p2EdgeLabels.setSize(p1.nEdges());
    sameOrientation.setSize(p1.nEdges());
    sameOrientation = 0;

    label nMatches = 0;

    EdgeMap<label> edgeToIndex(2*p1.nEdges());
    forAll(p1.edges(), edgeI)
    {
        const edge& e = p1.edges()[edgeI];
        const edge meshE
        (
            p1.meshPoints()[e[0]],
            p1.meshPoints()[e[1]]
        );
        edgeToIndex.insert(meshE, edgeI);
    }

    forAll(p2.edges(), edgeI)
    {
        const edge& e = p2.edges()[edgeI];
        const edge meshE(p2.meshPoints()[e[0]], p2.meshPoints()[e[1]]);

        EdgeMap<label>::const_iterator iter = edgeToIndex.find(meshE);

        if (iter != edgeToIndex.end())
        {
            p1EdgeLabels[nMatches] = iter();
            p2EdgeLabels[nMatches] = edgeI;
            sameOrientation[nMatches] = (meshE[0] == iter.key()[0]);
            nMatches++;
        }
    }
    p1EdgeLabels.setSize(nMatches);
    p2EdgeLabels.setSize(nMatches);
    sameOrientation.setSize(nMatches);
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
