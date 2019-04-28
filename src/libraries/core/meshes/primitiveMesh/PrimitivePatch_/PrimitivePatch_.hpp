/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::PrimitivePatch

Description
    A list of faces which address into the list of points.

    The class is templated on the face type (e.g. triangle, polygon etc.)
    and on the list type of faces and points so that it can refer to
    existing lists using UList and const pointField& or hold the storage
    using List and pointField.


\*---------------------------------------------------------------------------*/

#ifndef PrimitivePatch__H
#define PrimitivePatch__H

#include "boolList.hpp"
#include "labelList.hpp"
#include "edgeList.hpp"
#include "point.hpp"
#include "intersection.hpp"
#include "HashSet.hpp"
#include "objectHit.hpp"
#include "DynamicList.hpp"
#include "demandDrivenData.hpp"
#include "SLList.hpp"
#include "Map.hpp"
#include "PointHit_.hpp"
#include "objectHit.hpp"
#include "bandCompression.hpp"
#include "ListOps.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class face;
template<class T> class Map;

/*---------------------------------------------------------------------------*\
                        Class PrimitivePatchName Declaration
\*---------------------------------------------------------------------------*/

TemplateName(PrimitivePatch);


/*---------------------------------------------------------------------------*\
                           Class PrimitivePatch Declaration
\*---------------------------------------------------------------------------*/

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType=point
>
class PrimitivePatch
:
    public PrimitivePatchName,
    public FaceList<Face>
{

public:

    // Public typedefs

        typedef Face FaceType;
        typedef FaceList<Face> FaceListType;
        typedef PointField PointFieldType;


    // Public data types

        //- Enumeration defining the surface type. Used in check routines.
        enum surfaceTopo
        {
            MANIFOLD,
            OPEN,
            ILLEGAL
        };

private:

    // Private data

        //- Reference to global list of points
        PointField points_;


    // Demand driven private data

        //- Edges of the patch; address into local point list;
        //  sorted with internal edges first in upper-triangular order
        //  and external edges last.
        mutable edgeList* edgesPtr_;

        //- Which part of edgesPtr_ is internal edges.
        mutable label nInternalEdges_;

        //- Boundary point labels, addressing into local point list
        mutable labelList* boundaryPointsPtr_;

        //- Face-face addressing
        mutable labelListList* faceFacesPtr_;

        //- Edge-face addressing
        mutable labelListList* edgeFacesPtr_;

        //- Face-edge addressing
        mutable labelListList* faceEdgesPtr_;

        //- Point-edge addressing
        mutable labelListList* pointEdgesPtr_;

        //- Point-face addressing
        mutable labelListList* pointFacesPtr_;

        //- Faces addressing into local point list
        mutable List<Face>* localFacesPtr_;

        //- Labels of mesh points
        mutable labelList* meshPointsPtr_;

        //- Mesh point map.  Given the global point index find its
        //location in the patch
        mutable Map<label>* meshPointMapPtr_;

        //- Outside edge loops
        mutable labelListList* edgeLoopsPtr_;

        //- Points local to patch
        mutable Field<PointType>* localPointsPtr_;

        //- Local point order for most efficient search
        mutable labelList* localPointOrderPtr_;

        //- Face centres
        mutable Field<PointType>* faceCentresPtr_;

        //- Face unit normals
        mutable Field<PointType>* faceNormalsPtr_;

        //- Point unit normals
        mutable Field<PointType>* pointNormalsPtr_;


    // Private Member Functions

        //- Calculate edges of the patch
        void calcIntBdryEdges() const;

        //- Calculated boundary points on a patch
        void calcBdryPoints() const;

        //- Calculate addressing
        void calcAddressing() const;

        //- Calculate point-edge addressing
        void calcPointEdges() const;

        //- Calculate point-face addressing
        void calcPointFaces() const;

        //- Calculate mesh addressing
        void calcMeshData() const;

        //- Calculate mesh point map
        void calcMeshPointMap() const;

        //- Calculate outside edge loops
        void calcEdgeLoops() const;

        //- Calculate local points
        void calcLocalPoints() const;

        //- Calculate local point order
        void calcLocalPointOrder() const;

        //- Calculate face centres
        void calcFaceCentres() const;

        //- Calculate unit face normals
        void calcFaceNormals() const;

        //- Calculate unit point normals
        void calcPointNormals() const;

        //- Calculate edge owner
        void calcEdgeOwner() const;


        //- Face-edge-face walk while remaining on a patch point.
        //  Used to determine if surface multiply connected through point.
        void visitPointRegion
        (
            const label pointI,
            const labelList& pFaces,
            const label startFaceI,
            const label startEdgeI,
            boolList& pFacesHad
        ) const;


public:

    // Constructors

        //- Construct from components
        PrimitivePatch
        (
            const FaceList<Face>& faces,
            const Field<PointType>& points
        );

        //- Construct from components, reuse storage
        PrimitivePatch
        (
            FaceList<Face>& faces,
            Field<PointType>& points,
            const bool reUse
        );

        //- Construct as copy
        PrimitivePatch
        (
            const PrimitivePatch<Face, FaceList, PointField, PointType>&
        );


    //- Destructor
    virtual ~PrimitivePatch();

        void clearOut();

        void clearGeom();

        void clearTopology();

        void clearPatchMeshAddr();


    // Member Functions

    // Access

        //- Return reference to global points
        const Field<PointType>& points() const
        {
            return points_;
        }


    // Access functions for demand driven data

        // Topological data; no mesh required.

            //- Return number of points supporting patch faces
            label nPoints() const
            {
                return meshPoints().size();
            }

            //- Return number of edges in patch
            label nEdges() const
            {
                return edges().size();
            }

            //- Return list of edges, address into LOCAL point list
            const edgeList& edges() const;

            //- Number of internal edges
            label nInternalEdges() const;

            //- Is internal edge?
            bool isInternalEdge(const label edgeI) const
            {
                return edgeI < nInternalEdges();
            }

            //- Return list of boundary points,
            //  address into LOCAL point list
            const labelList& boundaryPoints() const;

            //- Return face-face addressing
            const labelListList& faceFaces() const;

            //- Return edge-face addressing
            const labelListList& edgeFaces() const;

            //- Return face-edge addressing
            const labelListList& faceEdges() const;

            //- Return point-edge addressing
            const labelListList& pointEdges() const;

            //- Return point-face addressing
            const labelListList& pointFaces() const;

            //- Return patch faces addressing into local point list
            const List<Face>& localFaces() const;


        // Addressing into mesh

            //- Return labelList of mesh points in patch. They are constructed
            //  walking through the faces in incremental order and not sorted
            //  anymore.
            const labelList& meshPoints() const;

            //- Mesh point map.  Given the global point index find its
            //  location in the patch
            const Map<label>& meshPointMap() const;

            //- Return pointField of points in patch
            const Field<PointType>& localPoints() const;

            //- Return orders the local points for most efficient search
            const labelList& localPointOrder() const;

            //- Given a global point index, return the local point index.
            //  If the point is not found, return -1
            label whichPoint(const label gp) const;

            //- Given an edge in local point labels, return its
            //  index in the edge list.  If the edge is not found, return -1
            label whichEdge(const edge&) const;

            //- Return labels of patch edges in the global edge list using
            //  cell addressing
            labelList meshEdges
            (
                const edgeList& allEdges,
                const labelListList& cellEdges,
                const labelList& faceCells
            ) const;

            //- Return labels of patch edges in the global edge list using
            //  basic edge addressing.
            labelList meshEdges
            (
                const edgeList& allEdges,
                const labelListList& pointEdges
            ) const;

            //- Return face centres for patch
            const Field<PointType>& faceCentres() const;

            //- Return face normals for patch
            const Field<PointType>& faceNormals() const;

            //- Return point normals for patch
            const Field<PointType>& pointNormals() const;


        // Other patch operations

            //- Project vertices of patch onto another patch
            template <class ToPatch>
            List<objectHit> projectPoints
            (
                const ToPatch& targetPatch,
                const Field<PointType>& projectionDirection,
                const intersection::algorithm = intersection::FULL_RAY,
                const intersection::direction = intersection::VECTOR
            ) const;

            //- Project vertices of patch onto another patch
            template <class ToPatch>
            List<objectHit> projectFaceCentres
            (
                const ToPatch& targetPatch,
                const Field<PointType>& projectionDirection,
                const intersection::algorithm = intersection::FULL_RAY,
                const intersection::direction = intersection::VECTOR
            ) const;

            //- Return list of closed loops of boundary vertices.
            //  Edge loops are given as ordered lists of vertices
            //  in local addressing
            const labelListList& edgeLoops() const;


    // Check

        //- Calculate surface type formed by patch.
        //  Types:
        //  - all edges have two neighbours (manifold)
        //  - some edges have more than two neighbours (illegal)
        //  - other (open)
        surfaceTopo surfaceType() const;

        //- Check surface formed by patch for manifoldness (see above).
        //  Return true if any incorrect edges are found.
        //  Insert vertices of incorrect edges into set.
        bool checkTopology
        (
            const bool report = false,
            labelHashSet* setPtr = nullptr
        ) const;

        //- Checks primitivePatch for faces sharing point but not edge.
        //  This denotes a surface that is pinched at a single point
        //  (test for pinched at single edge is already in PrimitivePatch)
        //  Returns true if this situation found and puts conflicting
        //  (mesh)point in set. Based on all the checking routines in
        //  primitiveMesh.
        bool checkPointManifold
        (
            const bool report = false,
            labelHashSet* setPtr = nullptr
        ) const;


    // Edit

        //- Correct patch after moving points
        virtual void movePoints(const Field<PointType>&);


    // Member operators

        //- Assignment
        void operator=
        (
            const PrimitivePatch<Face, FaceList, PointField, PointType>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
PrimitivePatch
(
    const FaceList<Face>& faces,
    const Field<PointType>& points
)
:
    FaceList<Face>(faces),
    points_(points),
    edgesPtr_(nullptr),
    nInternalEdges_(-1),
    boundaryPointsPtr_(nullptr),
    faceFacesPtr_(nullptr),
    edgeFacesPtr_(nullptr),
    faceEdgesPtr_(nullptr),
    pointEdgesPtr_(nullptr),
    pointFacesPtr_(nullptr),
    localFacesPtr_(nullptr),
    meshPointsPtr_(nullptr),
    meshPointMapPtr_(nullptr),
    edgeLoopsPtr_(nullptr),
    localPointsPtr_(nullptr),
    localPointOrderPtr_(nullptr),
    faceCentresPtr_(nullptr),
    faceNormalsPtr_(nullptr),
    pointNormalsPtr_(nullptr)
{}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
PrimitivePatch
(
    FaceList<Face>& faces,
    Field<PointType>& points,
    const bool reUse
)
:
    FaceList<Face>(faces, reUse),
    points_(points, reUse),
    edgesPtr_(nullptr),
    nInternalEdges_(-1),
    boundaryPointsPtr_(nullptr),
    faceFacesPtr_(nullptr),
    edgeFacesPtr_(nullptr),
    faceEdgesPtr_(nullptr),
    pointEdgesPtr_(nullptr),
    pointFacesPtr_(nullptr),
    localFacesPtr_(nullptr),
    meshPointsPtr_(nullptr),
    meshPointMapPtr_(nullptr),
    edgeLoopsPtr_(nullptr),
    localPointsPtr_(nullptr),
    localPointOrderPtr_(nullptr),
    faceCentresPtr_(nullptr),
    faceNormalsPtr_(nullptr),
    pointNormalsPtr_(nullptr)
{}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
PrimitivePatch
(
    const PrimitivePatch<Face, FaceList, PointField, PointType>& pp
)
:
    PrimitivePatchName(),
    FaceList<Face>(pp),
    points_(pp.points_),
    edgesPtr_(nullptr),
    nInternalEdges_(-1),
    boundaryPointsPtr_(nullptr),
    faceFacesPtr_(nullptr),
    edgeFacesPtr_(nullptr),
    faceEdgesPtr_(nullptr),
    pointEdgesPtr_(nullptr),
    pointFacesPtr_(nullptr),
    localFacesPtr_(nullptr),
    meshPointsPtr_(nullptr),
    meshPointMapPtr_(nullptr),
    edgeLoopsPtr_(nullptr),
    localPointsPtr_(nullptr),
    localPointOrderPtr_(nullptr),
    faceCentresPtr_(nullptr),
    faceNormalsPtr_(nullptr),
    pointNormalsPtr_(nullptr)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
~PrimitivePatch()
{
    clearOut();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
movePoints
(
    const Field<PointType>&
)
{
    if (debug)
    {
        Pout<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
            << "movePoints() : "
            << "recalculating PrimitivePatch geometry following mesh motion"
            << endl;
    }

    clearGeom();
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
const CML::edgeList&
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
edges() const
{
    if (!edgesPtr_)
    {
        calcAddressing();
    }

    return *edgesPtr_;
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
CML::label
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
nInternalEdges() const
{
    if (!edgesPtr_)
    {
        calcAddressing();
    }

    return nInternalEdges_;
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
const CML::labelList&
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
boundaryPoints() const
{
    if (!boundaryPointsPtr_)
    {
        calcBdryPoints();
    }

    return *boundaryPointsPtr_;
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
const CML::labelListList&
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
faceFaces() const
{
    if (!faceFacesPtr_)
    {
        calcAddressing();
    }

    return *faceFacesPtr_;
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
const CML::labelListList&
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
edgeFaces() const
{
    if (!edgeFacesPtr_)
    {
        calcAddressing();
    }

    return *edgeFacesPtr_;
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
const CML::labelListList&
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
faceEdges() const
{
    if (!faceEdgesPtr_)
    {
        calcAddressing();
    }

    return *faceEdgesPtr_;
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
const CML::labelListList&
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
pointEdges() const
{
    if (!pointEdgesPtr_)
    {
        calcPointEdges();
    }

    return *pointEdgesPtr_;
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
const CML::labelListList&
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
pointFaces() const
{
    if (!pointFacesPtr_)
    {
        calcPointFaces();
    }

    return *pointFacesPtr_;
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
const CML::List<Face>&
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
localFaces() const
{
    if (!localFacesPtr_)
    {
        calcMeshData();
    }

    return *localFacesPtr_;
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
const CML::labelList&
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
meshPoints() const
{
    if (!meshPointsPtr_)
    {
        calcMeshData();
    }

    return *meshPointsPtr_;
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
const CML::Map<CML::label>&
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
meshPointMap() const
{
    if (!meshPointMapPtr_)
    {
        calcMeshPointMap();
    }

    return *meshPointMapPtr_;
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
const CML::Field<PointType>&
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
localPoints() const
{
    if (!localPointsPtr_)
    {
        calcLocalPoints();
    }

    return *localPointsPtr_;
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
const CML::labelList&
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
localPointOrder() const
{
    if (!localPointOrderPtr_)
    {
        calcLocalPointOrder();
    }

    return *localPointOrderPtr_;
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
CML::label
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
whichPoint
(
    const label gp
) const
{
    Map<label>::const_iterator fnd = meshPointMap().find(gp);

    if (fnd != meshPointMap().end())
    {
        return fnd();
    }
    else
    {
        // Not found
        return -1;
    }
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
const CML::Field<PointType>&
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
faceCentres() const
{
    if (!faceCentresPtr_)
    {
        calcFaceCentres();
    }

    return *faceCentresPtr_;
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
const CML::Field<PointType>&
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
faceNormals() const
{
    if (!faceNormalsPtr_)
    {
        calcFaceNormals();
    }

    return *faceNormalsPtr_;
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
const CML::Field<PointType>&
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
pointNormals() const
{
    if (!pointNormalsPtr_)
    {
        calcPointNormals();
    }

    return *pointNormalsPtr_;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
operator=
(
    const PrimitivePatch<Face, FaceList, PointField, PointType>& pp
)
{
    clearOut();

    FaceList<Face>::operator=(pp);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
calcAddressing() const
{
    if (debug)
    {
        Info<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
            << "calcAddressing() : calculating patch addressing"
            << endl;
    }

    if (edgesPtr_ || faceFacesPtr_ || edgeFacesPtr_ || faceEdgesPtr_)
    {
        // it is considered an error to attempt to recalculate
        // if already allocated
        FatalErrorInFunction
            << "addressing already calculated"
            << abort(FatalError);
    }

    // get reference to localFaces
    const List<Face>& locFcs = localFaces();

    // get reference to pointFaces
    const labelListList& pf = pointFaces();

    // Guess the max number of edges and neighbours for a face
    label maxEdges = 0;
    forAll(locFcs, faceI)
    {
        maxEdges += locFcs[faceI].size();
    }

    // create the lists for the various results. (resized on completion)
    edgesPtr_ = new edgeList(maxEdges);
    edgeList& edges = *edgesPtr_;

    edgeFacesPtr_ = new labelListList(maxEdges);
    labelListList& edgeFaces = *edgeFacesPtr_;

    // faceFaces created using a dynamic list.  Cannot guess size because
    // of multiple connections
    List<DynamicList<label> > ff(locFcs.size());

    faceEdgesPtr_ = new labelListList(locFcs.size());
    labelListList& faceEdges = *faceEdgesPtr_;

    // count the number of face neighbours
    labelList noFaceFaces(locFcs.size());

    // initialise the lists of subshapes for each face to avoid duplication
    edgeListList faceIntoEdges(locFcs.size());

    forAll(locFcs, faceI)
    {
        faceIntoEdges[faceI] = locFcs[faceI].edges();

        labelList& curFaceEdges = faceEdges[faceI];
        curFaceEdges.setSize(faceIntoEdges[faceI].size());

        forAll(curFaceEdges, faceEdgeI)
        {
            curFaceEdges[faceEdgeI] = -1;
        }
    }

    // This algorithm will produce a separated list of edges, internal edges
    // starting from 0 and boundary edges starting from the top and
    // growing down.

    label nEdges = 0;

    bool found = false;

    // Note that faceIntoEdges is sorted acc. to local vertex numbering
    // in face (i.e. curEdges[0] is edge between f[0] and f[1])

    // For all local faces ...
    forAll(locFcs, faceI)
    {
        // Get reference to vertices of current face and corresponding edges.
        const Face& curF = locFcs[faceI];
        const edgeList& curEdges = faceIntoEdges[faceI];

        // Record the neighbour face.  Multiple connectivity allowed
        List<DynamicList<label> > neiFaces(curF.size());
        List<DynamicList<label> > edgeOfNeiFace(curF.size());

        label nNeighbours = 0;

        // For all edges ...
        forAll(curEdges, edgeI)
        {
            // If the edge is already detected, skip
            if (faceEdges[faceI][edgeI] >= 0) continue;

            found = false;

            // Set reference to the current edge
            const edge& e = curEdges[edgeI];

            // Collect neighbours for the current face vertex.

            const labelList& nbrFaces = pf[e.start()];

            forAll(nbrFaces, nbrFaceI)
            {
                // set reference to the current neighbour
                label curNei = nbrFaces[nbrFaceI];

                // Reject neighbours with the lower label
                if (curNei > faceI)
                {
                    // get the reference to subshapes of the neighbour
                    const edgeList& searchEdges = faceIntoEdges[curNei];

                    forAll(searchEdges, neiEdgeI)
                    {
                        if (searchEdges[neiEdgeI] == e)
                        {
                            // Match
                            found = true;

                            neiFaces[edgeI].append(curNei);
                            edgeOfNeiFace[edgeI].append(neiEdgeI);

                            // Record faceFaces both ways
                            ff[faceI].append(curNei);
                            ff[curNei].append(faceI);

                            // Keep searching due to multiple connectivity
                        }
                    }
                }
            } // End of neighbouring faces

            if (found)
            {
                // Register another detected internal edge
                nNeighbours++;
            }
        } // End of current edges

        // Add the edges in increasing number of neighbours.
        // Note: for multiply connected surfaces, the lower index neighbour for
        // an edge will come first.

        // Add the faces in the increasing order of neighbours
        for (label neiSearch = 0; neiSearch < nNeighbours; neiSearch++)
        {
            // Find the lowest neighbour which is still valid
            label nextNei = -1;
            label minNei = locFcs.size();

            forAll(neiFaces, nfI)
            {
                if (neiFaces[nfI].size() && neiFaces[nfI][0] < minNei)
                {
                    nextNei = nfI;
                    minNei = neiFaces[nfI][0];
                }
            }

            if (nextNei > -1)
            {
                // Add the face to the list of faces
                edges[nEdges] = curEdges[nextNei];

                // Set face-edge and face-neighbour-edge to current face label
                faceEdges[faceI][nextNei] = nEdges;

                DynamicList<label>& cnf = neiFaces[nextNei];
                DynamicList<label>& eonf = edgeOfNeiFace[nextNei];

                // Set edge-face addressing
                labelList& curEf = edgeFaces[nEdges];
                curEf.setSize(cnf.size() + 1);
                curEf[0] = faceI;

                forAll(cnf, cnfI)
                {
                    faceEdges[cnf[cnfI]][eonf[cnfI]] = nEdges;

                    curEf[cnfI + 1] = cnf[cnfI];
                }

                // Stop the neighbour from being used again
                cnf.clear();
                eonf.clear();

                // Increment number of faces counter
                nEdges++;
            }
            else
            {
                FatalErrorInFunction
                    << "Error in internal edge insertion"
                    << abort(FatalError);
            }
        }
    }

    nInternalEdges_ = nEdges;

    // Do boundary faces

    forAll(faceEdges, faceI)
    {
        labelList& curEdges = faceEdges[faceI];

        forAll(curEdges, edgeI)
        {
            if (curEdges[edgeI] < 0)
            {
                // Grab edge and faceEdge
                edges[nEdges] = faceIntoEdges[faceI][edgeI];
                curEdges[edgeI] = nEdges;

                // Add edgeFace
                labelList& curEf = edgeFaces[nEdges];
                curEf.setSize(1);
                curEf[0] = faceI;

                nEdges++;
            }
        }
    }

    // edges
    edges.setSize(nEdges);

    // edgeFaces list
    edgeFaces.setSize(nEdges);

    // faceFaces list
    faceFacesPtr_ = new labelListList(locFcs.size());
    labelListList& faceFaces = *faceFacesPtr_;

    forAll(faceFaces, faceI)
    {
        faceFaces[faceI].transfer(ff[faceI]);
    }


    if (debug)
    {
        Info<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
            << "calcAddressing() : finished calculating patch addressing"
            << endl;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
calcEdgeLoops() const
{
    if (debug)
    {
        Info<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
            << "calcEdgeLoops() : "
            << "calculating boundary edge loops"
            << endl;
    }

    if (edgeLoopsPtr_)
    {
        // it is considered an error to attempt to recalculate
        // if already allocated
        FatalErrorInFunction
            << "edge loops already calculated"
            << abort(FatalError);
    }

    const edgeList& patchEdges = edges();
    label nIntEdges = nInternalEdges();
    label nBdryEdges = patchEdges.size() - nIntEdges;

    if (nBdryEdges == 0)
    {
        edgeLoopsPtr_ = new labelListList(0);
        return;
    }

    const labelListList& patchPointEdges = pointEdges();


    //
    // Walk point-edge-point and assign loop number
    //

    // Loop per (boundary) edge.
    labelList loopNumber(nBdryEdges, -1);

    // Size return list plenty big
    edgeLoopsPtr_ = new labelListList(nBdryEdges);
    labelListList& edgeLoops = *edgeLoopsPtr_;


    // Current loop number.
    label loopI = 0;

    while (true)
    {
        // Find edge not yet given a loop number.
        label currentEdgeI = -1;

        for (label edgeI = nIntEdges; edgeI < patchEdges.size(); edgeI++)
        {
            if (loopNumber[edgeI-nIntEdges] == -1)
            {
                currentEdgeI = edgeI;
                break;
            }
        }

        if (currentEdgeI == -1)
        {
            // Did not find edge not yet assigned a loop number so done all.
            break;
        }

        // Temporary storage for vertices of current loop
        DynamicList<label> loop(nBdryEdges);

        // Walk from first all the way round, assigning loops
        label currentVertI = patchEdges[currentEdgeI].start();

        do
        {
            loop.append(currentVertI);

            loopNumber[currentEdgeI - nIntEdges] = loopI;

            // Step to next vertex
            currentVertI = patchEdges[currentEdgeI].otherVertex(currentVertI);

            // Step to next (unmarked, boundary) edge.
            const labelList& curEdges = patchPointEdges[currentVertI];

            currentEdgeI = -1;

            forAll(curEdges, pI)
            {
                label edgeI = curEdges[pI];

                if (edgeI >= nIntEdges && (loopNumber[edgeI - nIntEdges] == -1))
                {
                    // Unassigned boundary edge.
                    currentEdgeI = edgeI;

                    break;
                }
            }
        }
        while (currentEdgeI != -1);

        // Done all for current loop. Transfer to edgeLoops.
        edgeLoops[loopI].transfer(loop);

        loopI++;
    }

    edgeLoops.setSize(loopI);

    if (debug)
    {
        Info<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
            << "calcEdgeLoops() : "
            << "finished calculating boundary edge loops"
            << endl;
    }
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
const CML::labelListList&
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
edgeLoops() const
{
    if (!edgeLoopsPtr_)
    {
        calcEdgeLoops();
    }

    return *edgeLoopsPtr_;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
clearGeom()
{
    if (debug)
    {
        Info<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
            << "clearGeom() : clearing geometric data"
            << endl;
    }

    deleteDemandDrivenData(localPointsPtr_);
    deleteDemandDrivenData(faceCentresPtr_);
    deleteDemandDrivenData(faceNormalsPtr_);
    deleteDemandDrivenData(pointNormalsPtr_);
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
clearTopology()
{
    if (debug)
    {
        Info<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
            << "clearTopology() : clearing patch addressing"
            << endl;
    }

    // group created and destroyed together
    if (edgesPtr_ && faceFacesPtr_ && edgeFacesPtr_ && faceEdgesPtr_)
    {
        delete edgesPtr_;
        edgesPtr_ = nullptr;

        delete faceFacesPtr_;
        faceFacesPtr_ = nullptr;

        delete edgeFacesPtr_;
        edgeFacesPtr_ = nullptr;

        delete faceEdgesPtr_;
        faceEdgesPtr_ = nullptr;
    }

    deleteDemandDrivenData(boundaryPointsPtr_);
    deleteDemandDrivenData(pointEdgesPtr_);
    deleteDemandDrivenData(pointFacesPtr_);
    deleteDemandDrivenData(edgeLoopsPtr_);
    deleteDemandDrivenData(localPointOrderPtr_);
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
clearPatchMeshAddr()
{
    if (debug)
    {
        Info<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
            << "clearPatchMeshAddr() : "
            << "clearing patch-mesh addressing"
            << endl;
    }

    deleteDemandDrivenData(meshPointsPtr_);
    deleteDemandDrivenData(meshPointMapPtr_);
    deleteDemandDrivenData(localFacesPtr_);
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
clearOut()
{
    clearGeom();
    clearTopology();
    clearPatchMeshAddr();
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
calcBdryPoints() const
{
    if (debug)
    {
        Info<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
            << "calcBdryPoints() : "
            << "calculating boundary points"
            << endl;
    }

    if (boundaryPointsPtr_)
    {
        // it is considered an error to attempt to recalculate
        // if already allocated
        FatalErrorInFunction
            << "edge types already calculated"
            << abort(FatalError);
    }

    const edgeList& e = edges();

    labelHashSet bp(2*e.size());

    for (label edgeI = nInternalEdges_; edgeI < e.size(); edgeI++)
    {
        const edge& curEdge = e[edgeI];

        bp.insert(curEdge.start());
        bp.insert(curEdge.end());
    }

    boundaryPointsPtr_ = new labelList(bp.toc());
    sort(*boundaryPointsPtr_);

    if (debug)
    {
        Info<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
            << "calcBdryPoints() : "
            << "finished calculating boundary points"
            << endl;
    }
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
calcLocalPointOrder() const
{
    // Note: Cannot use bandCompressing as point-point addressing does
    // not exist and is not considered generally useful.
    //

    if (debug)
    {
        Pout<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
            << "calcLocalPointOrder() : "
            << "calculating local point order"
            << endl;
    }

    if (localPointOrderPtr_)
    {
        // it is considered an error to attempt to recalculate
        // if already allocated
        FatalErrorInFunction
            << "local point order already calculated"
            << abort(FatalError);
    }

    const List<Face>& lf = localFaces();

    const labelListList& ff = faceFaces();

    boolList visitedFace(lf.size(), false);

    localPointOrderPtr_ = new labelList(meshPoints().size(), -1);

    labelList& pointOrder = *localPointOrderPtr_;

    boolList visitedPoint(pointOrder.size(), false);

    label nPoints = 0;

    forAll(lf, faceI)
    {
        if (!visitedFace[faceI])
        {
            SLList<label> faceOrder(faceI);

            do
            {
                const label curFace = faceOrder.first();

                faceOrder.removeHead();

                if (!visitedFace[curFace])
                {
                    visitedFace[curFace] = true;

                    const labelList& curPoints = lf[curFace];

                    // mark points
                    forAll(curPoints, pointI)
                    {
                        if (!visitedPoint[curPoints[pointI]])
                        {
                            visitedPoint[curPoints[pointI]] = true;

                            pointOrder[nPoints] = curPoints[pointI];

                            nPoints++;
                        }
                    }

                    // add face neighbours to the list
                    const labelList& nbrs = ff[curFace];

                    forAll(nbrs, nbrI)
                    {
                        if (!visitedFace[nbrs[nbrI]])
                        {
                            faceOrder.append(nbrs[nbrI]);
                        }
                    }
                }
            } while (faceOrder.size());
        }
    }

    if (debug)
    {
        Pout<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
            << "calcLocalPointOrder() "
            << "finished calculating local point order"
            << endl;
    }
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
calcMeshData() const
{
    if (debug)
    {
        Pout<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
               "calcMeshData() : "
               "calculating mesh data in PrimitivePatch"
            << endl;
    }

    // It is considered an error to attempt to recalculate meshPoints
    // if they have already been calculated.
    if (meshPointsPtr_ || localFacesPtr_)
    {
        FatalErrorInFunction
            << "meshPointsPtr_ or localFacesPtr_already allocated"
            << abort(FatalError);
    }

    // Create a map for marking points.  Estimated size is 4 times the
    // number of faces in the patch
    Map<label> markedPoints(4*this->size());


    // Important:
    // ~~~~~~~~~~
    // In <= 1.5 the meshPoints would be in increasing order but this gives
    // problems in processor point synchronisation where we have to find out
    // how the opposite side would have allocated points.

    ////- 1.5 code:
    //// if the point is used, set the mark to 1
    //forAll(*this, facei)
    //{
    //    const Face& curPoints = this->operator[](facei);
    //
    //    forAll(curPoints, pointi)
    //    {
    //        markedPoints.insert(curPoints[pointi], -1);
    //    }
    //}
    //
    //// Create the storage and store the meshPoints.  Mesh points are
    //// the ones marked by the usage loop above
    //meshPointsPtr_ = new labelList(markedPoints.toc());
    //labelList& pointPatch = *meshPointsPtr_;
    //
    //// Sort the list to preserve compatibility with the old ordering
    //sort(pointPatch);
    //
    //// For every point in map give it its label in mesh points
    //forAll(pointPatch, pointi)
    //{
    //    markedPoints.find(pointPatch[pointi])() = pointi;
    //}

    //- Unsorted version:
    DynamicList<label> meshPoints(2*this->size());
    forAll(*this, facei)
    {
        const Face& curPoints = this->operator[](facei);

        forAll(curPoints, pointi)
        {
            if (markedPoints.insert(curPoints[pointi], meshPoints.size()))
            {
                meshPoints.append(curPoints[pointi]);
            }
        }
    }
    // Transfer to straight list (reuses storage)
    meshPointsPtr_ = new labelList(meshPoints, true);


    // Create local faces. Note that we start off from copy of original face
    // list (even though vertices are overwritten below). This is done so
    // additional data gets copied (e.g. region number of labelledTri)
    localFacesPtr_ = new List<Face>(*this);
    List<Face>& lf = *localFacesPtr_;

    forAll(*this, facei)
    {
        const Face& curFace = this->operator[](facei);
        lf[facei].setSize(curFace.size());

        forAll(curFace, labelI)
        {
            lf[facei][labelI] = markedPoints.find(curFace[labelI])();
        }
    }

    if (debug)
    {
        Pout<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
               "calcMeshData() : "
               "finished calculating mesh data in PrimitivePatch"
            << endl;
    }
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
calcMeshPointMap() const
{
    if (debug)
    {
        Pout<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
               "calcMeshPointMap() : "
               "calculating mesh point map in PrimitivePatch"
            << endl;
    }

    // It is considered an error to attempt to recalculate meshPoints
    // if they have already been calculated.
    if (meshPointMapPtr_)
    {
        FatalErrorInFunction
            << "meshPointMapPtr_ already allocated"
            << abort(FatalError);
    }

    const labelList& mp = meshPoints();

    meshPointMapPtr_ = new Map<label>(2*mp.size());
    Map<label>& mpMap = *meshPointMapPtr_;

    forAll(mp, i)
    {
        mpMap.insert(mp[i], i);
    }

    if (debug)
    {
        Pout<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
               "calcMeshPointMap() : "
               "finished calculating mesh point map in PrimitivePatch"
            << endl;
    }
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
calcLocalPoints() const
{
    if (debug)
    {
        Pout<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
               "calcLocalPoints() : "
               "calculating localPoints in PrimitivePatch"
            << endl;
    }

    // It is considered an error to attempt to recalculate localPoints
    // if they have already been calculated.
    if (localPointsPtr_)
    {
        FatalErrorInFunction
            << "localPointsPtr_already allocated"
            << abort(FatalError);
    }

    const labelList& meshPts = meshPoints();

    localPointsPtr_ = new Field<PointType>(meshPts.size());

    Field<PointType>& locPts = *localPointsPtr_;

    forAll(meshPts, pointi)
    {
        locPts[pointi] = points_[meshPts[pointi]];
    }

    if (debug)
    {
        Pout<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
            << "calcLocalPoints() : "
            << "finished calculating localPoints in PrimitivePatch"
            << endl;
    }
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
calcPointNormals() const
{
    if (debug)
    {
        Pout<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
               "calcPointNormals() : "
               "calculating pointNormals in PrimitivePatch"
            << endl;
    }

    // It is considered an error to attempt to recalculate pointNormals
    // if they have already been calculated.
    if (pointNormalsPtr_)
    {
        FatalErrorInFunction
            << "pointNormalsPtr_already allocated"
            << abort(FatalError);
    }

    const Field<PointType>& faceUnitNormals = faceNormals();

    const labelListList& pf = pointFaces();

    pointNormalsPtr_ = new Field<PointType>
    (
        meshPoints().size(),
        PointType::zero
    );

    Field<PointType>& n = *pointNormalsPtr_;

    forAll(pf, pointi)
    {
        PointType& curNormal = n[pointi];

        const labelList& curFaces = pf[pointi];

        forAll(curFaces, facei)
        {
            curNormal += faceUnitNormals[curFaces[facei]];
        }

        curNormal /= mag(curNormal) + VSMALL;
    }

    if (debug)
    {
        Pout<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
               "calcPointNormals() : "
               "finished calculating pointNormals in PrimitivePatch"
            << endl;
    }
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
calcFaceCentres() const
{
    if (debug)
    {
        Pout<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
               "calcFaceCentres() : "
               "calculating faceCentres in PrimitivePatch"
            << endl;
    }

    // It is considered an error to attempt to recalculate faceCentres
    // if they have already been calculated.
    if (faceCentresPtr_)
    {
        FatalErrorInFunction
            << "faceCentresPtr_already allocated"
            << abort(FatalError);
    }

    faceCentresPtr_ = new Field<PointType>(this->size());

    Field<PointType>& c = *faceCentresPtr_;

    forAll(c, facei)
    {
        c[facei] = this->operator[](facei).centre(points_);
    }

    if (debug)
    {
        Pout<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
               "calcFaceCentres() : "
               "finished calculating faceCentres in PrimitivePatch"
            << endl;
    }
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
calcFaceNormals() const
{
    if (debug)
    {
        Pout<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
               "calcFaceNormals() : "
               "calculating faceNormals in PrimitivePatch"
            << endl;
    }

    // It is considered an error to attempt to recalculate faceNormals
    // if they have already been calculated.
    if (faceNormalsPtr_)
    {
        FatalErrorInFunction
            << "faceNormalsPtr_already allocated"
            << abort(FatalError);
    }

    faceNormalsPtr_ = new Field<PointType>(this->size());

    Field<PointType>& n = *faceNormalsPtr_;

    forAll(n, facei)
    {
        n[facei] = this->operator[](facei).normal(points_);
    }

    if (debug)
    {
        Pout<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
               "calcFaceNormals() : "
               "finished calculating faceNormals in PrimitivePatch"
            << endl;
    }
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
CML::labelList
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
meshEdges
(
    const edgeList& allEdges,
    const labelListList& cellEdges,
    const labelList& faceCells
) const
{
    if (debug)
    {
        Info<< "labelList PrimitivePatch<Face, FaceList, PointField, PointType>"
            << "::meshEdges() : "
            << "calculating labels of patch edges in mesh edge list"
            << endl;
    }

    // get reference to the list of edges on the patch
    const edgeList& PatchEdges = edges();

    const labelListList& EdgeFaces = edgeFaces();

    // create the storage
    labelList meshEdges(PatchEdges.size());

    register bool found = false;

    // get reference to the points on the patch
    const labelList& pp = meshPoints();

    // WARNING: Remember that local edges address into local point list;
    // local-to-global point label translation is necessary
    forAll(PatchEdges, edgeI)
    {
        const edge curEdge
            (pp[PatchEdges[edgeI].start()], pp[PatchEdges[edgeI].end()]);

        found = false;

        // get the patch faces sharing the edge
        const labelList& curFaces = EdgeFaces[edgeI];

        forAll(curFaces, faceI)
        {
            // get the cell next to the face
            label curCell = faceCells[curFaces[faceI]];

            // get reference to edges on the cell
            const labelList& ce = cellEdges[curCell];

            forAll(ce, cellEdgeI)
            {
                if (allEdges[ce[cellEdgeI]] == curEdge)
                {
                    found = true;

                    meshEdges[edgeI] = ce[cellEdgeI];

                    break;
                }
            }

            if (found) break;
        }
    }

    return meshEdges;
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
CML::labelList
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
meshEdges
(
    const edgeList& allEdges,
    const labelListList& pointEdges
) const
{
    if (debug)
    {
        Info<< "labelList PrimitivePatch<Face, FaceList, PointField, PointType>"
            << "::meshEdges() : "
            << "calculating labels of patch edges in mesh edge list"
            << endl;
    }

    // get reference to the list of edges on the patch
    const edgeList& PatchEdges = edges();

    // create the storage
    labelList meshEdges(PatchEdges.size());

    // get reference to the points on the patch
    const labelList& pp = meshPoints();

    // WARNING: Remember that local edges address into local point list;
    // local-to-global point label translation is necessary
    forAll(PatchEdges, edgeI)
    {
        const label globalPointI = pp[PatchEdges[edgeI].start()];
        const edge curEdge(globalPointI, pp[PatchEdges[edgeI].end()]);

        const labelList& pe = pointEdges[globalPointI];

        forAll(pe, i)
        {
            if (allEdges[pe[i]] == curEdge)
            {
                meshEdges[edgeI] = pe[i];
                break;
            }
        }
    }

    return meshEdges;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
CML::label
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
whichEdge
(
    const edge& e
) const
{
    // Get pointEdges from the starting point and search all the candidates
    const edgeList& Edges = edges();

    if (e.start() > -1 && e.start() < nPoints())
    {
        const labelList& pe = pointEdges()[e.start()];

        forAll(pe, peI)
        {
            if (e == Edges[pe[peI]])
            {
                return pe[peI];
            }
        }
    }

    // Edge not found.  Return -1
    return -1;
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
calcPointEdges() const
{
    if (debug)
    {
        Info<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
            << "calcPointEdges() : calculating pointEdges"
            << endl;
    }

    if (pointEdgesPtr_)
    {
        // it is considered an error to attempt to recalculate
        // if already allocated
        FatalErrorInFunction
            << "pointEdges already calculated"
            << abort(FatalError);
    }

    const edgeList& e = edges();

    // set up storage for pointEdges
    List<SLList<label> > pointEdges(meshPoints().size());

    forAll(e, edgeI)
    {
        pointEdges[e[edgeI].start()].append(edgeI);
        pointEdges[e[edgeI].end()].append(edgeI);
    }

    // sort out the list
    pointEdgesPtr_ = new labelListList(pointEdges.size());

    labelListList& pe = *pointEdgesPtr_;

    forAll(pointEdges, pointI)
    {
        const SLList<label>& pEdge = pointEdges[pointI];

        pe[pointI].setSize(pEdge.size());

        label i = 0;
        forAllConstIter(SLList<label>, pEdge, iter)
        {
            pe[pointI][i++] = iter();
        }
    }

    if (debug)
    {
        Info<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
            << "calcPointEdges() finished calculating pointEdges"
            << endl;
    }
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
calcPointFaces() const
{
    if (debug)
    {
        Info<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
            << "calcPointFaces() : calculating pointFaces"
            << endl;
    }

    if (pointFacesPtr_)
    {
        // it is considered an error to attempt to recalculate
        // if already allocated
        FatalErrorInFunction
            << "pointFaces already calculated"
            << abort(FatalError);
    }

    const List<Face>& f = localFaces();

    // set up storage for pointFaces
    List<SLList<label> > pointFcs(meshPoints().size());

    forAll(f, faceI)
    {
        const Face& curPoints = f[faceI];

        forAll(curPoints, pointI)
        {
            pointFcs[curPoints[pointI]].append(faceI);
        }
    }

    // sort out the list
    pointFacesPtr_ = new labelListList(pointFcs.size());

    labelListList& pf = *pointFacesPtr_;

    forAll(pointFcs, pointI)
    {
        pf[pointI].setSize(pointFcs[pointI].size());

        label i = 0;
        forAllIter(SLList<label>, pointFcs[pointI], curFacesIter)
        {
            pf[pointI][i++] = curFacesIter();
        }
    }

    if (debug)
    {
        Info<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
            << "calcPointFaces() finished calculating pointFaces"
            << endl;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
template <class ToPatch>
CML::List<CML::objectHit>
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
projectPoints
(
    const ToPatch& targetPatch,
    const Field<PointType>& projectionDirection,
    const intersection::algorithm alg,
    const intersection::direction dir
) const
{
    // The current patch is slave, i.e. it is being projected onto the target

    if (projectionDirection.size() != nPoints())
    {
        FatalErrorInFunction
           << "Projection direction field does not correspond to "
            << "patch points." << endl
            << "Size: " << projectionDirection.size()
            << " Number of points: " << nPoints()
            << abort(FatalError);
    }

    const labelList& slavePointOrder = localPointOrder();

    const labelList& slaveMeshPoints = meshPoints();

    // Result
    List<objectHit> result(nPoints());

    const labelListList& masterFaceFaces = targetPatch.faceFaces();

    const ToPatch& masterFaces = targetPatch;

    const Field<PointType>& masterPoints = targetPatch.points();

    // Estimate face centre of target side
    Field<PointType> masterFaceCentres(targetPatch.size());

    forAll(masterFaceCentres, faceI)
    {
        masterFaceCentres[faceI] =
            average(masterFaces[faceI].points(masterPoints));
    }

    // Algorithm:
    // Loop through all points of the slave side. For every point find the
    // radius for the current contact face. If the contact point falls inside
    // the face and the radius is smaller than for all neighbouring faces,
    // the contact is found. If not, visit the neighbour closest to the
    // calculated contact point. If a single master face is visited more than
    // twice, initiate n-squared search.

    label curFace = 0;
    label nNSquaredSearches = 0;

    forAll(slavePointOrder, pointI)
    {
        // Pick up slave point and direction
        const label curLocalPointLabel = slavePointOrder[pointI];

        const PointType& curPoint =
            points_[slaveMeshPoints[curLocalPointLabel]];

        const PointType& curProjectionDir =
            projectionDirection[curLocalPointLabel];

        bool closer;

        boolList visitedTargetFace(targetPatch.size(), false);
        bool doNSquaredSearch = false;

        bool foundEligible = false;

        scalar sqrDistance = GREAT;

        // Force the full search for the first point to ensure good
        // starting face
        if (pointI == 0)
        {
            doNSquaredSearch = true;
        }
        else
        {
            do
            {
                closer = false;
                doNSquaredSearch = false;

                // Calculate intersection with curFace
                PointHit<PointType> curHit =
                    masterFaces[curFace].ray
                    (
                        curPoint,
                        curProjectionDir,
                        masterPoints,
                        alg,
                        dir
                    );

                visitedTargetFace[curFace] = true;

                if (curHit.hit())
                {
                    result[curLocalPointLabel] = objectHit(true, curFace);

                    break;
                }
                else
                {
                    // If a new miss is eligible, it is closer than
                    // any previous eligible miss (due to surface walk)

                    // Only grab the miss if it is eligible
                    if (curHit.eligibleMiss())
                    {
                        foundEligible = true;
                        result[curLocalPointLabel] = objectHit(false, curFace);
                    }

                    // Find the next likely face for intersection

                    // Calculate the miss point on the plane of the
                    // face.  This is cooked (illogical!) for fastest
                    // surface walk.
                    //
                    PointType missPlanePoint =
                        curPoint + curProjectionDir*curHit.distance();

                    const labelList& masterNbrs = masterFaceFaces[curFace];

                    sqrDistance =
                        magSqr(missPlanePoint - masterFaceCentres[curFace]);

                    forAll(masterNbrs, nbrI)
                    {
                        if
                        (
                            magSqr
                            (
                                missPlanePoint
                              - masterFaceCentres[masterNbrs[nbrI]]
                            )
                         <= sqrDistance
                        )
                        {
                            closer = true;
                            curFace = masterNbrs[nbrI];
                        }
                    }

                    if (visitedTargetFace[curFace])
                    {
                        // This face has already been visited.
                        // Execute n-squared search
                        doNSquaredSearch = true;
                        break;
                    }
                }

                if (debug) Info<< ".";
            } while (closer);
        }

        if
        (
            doNSquaredSearch || !foundEligible
        )
        {
            nNSquaredSearches++;

            if (debug)
            {
                Info<< "p " << curLocalPointLabel << ": ";
            }

            result[curLocalPointLabel] = objectHit(false, -1);
            scalar minDistance = GREAT;

            forAll(masterFaces, faceI)
            {
                PointHit<PointType> curHit =
                    masterFaces[faceI].ray
                    (
                        curPoint,
                        curProjectionDir,
                        masterPoints,
                        alg,
                        dir
                    );

                if (curHit.hit())
                {
                    result[curLocalPointLabel] = objectHit(true, faceI);
                    curFace = faceI;

                    break;
                }
                else if (curHit.eligibleMiss())
                {
                    // Calculate min distance
                    scalar missDist =
                        CML::mag(curHit.missPoint() - curPoint);

                    if (missDist < minDistance)
                    {
                        minDistance = missDist;

                        result[curLocalPointLabel] = objectHit(false, faceI);
                        curFace = faceI;
                    }
                }
            }

            if (debug)
            {
                Info<< result[curLocalPointLabel] << nl;
            }
        }
        else
        {
            if (debug) Info<< "x";
        }
    }

    if (debug)
    {
        Info<< nl << "Executed " << nNSquaredSearches
            << " n-squared searches out of total of "
            << nPoints() << endl;
    }

    return result;
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
template <class ToPatch>
CML::List<CML::objectHit>
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
projectFaceCentres
(
    const ToPatch& targetPatch,
    const Field<PointType>& projectionDirection,
    const intersection::algorithm alg,
    const intersection::direction dir
) const
{
    // The current patch is slave, i.e. it is being projected onto the target

    if (projectionDirection.size() != this->size())
    {
        FatalErrorInFunction
            << "Projection direction field does not correspond to patch faces."
            << endl << "Size: " << projectionDirection.size()
            << " Number of points: " << this->size()
            << abort(FatalError);
    }

    labelList slaveFaceOrder = bandCompression(faceFaces());

    // calculate master face centres
    Field<PointType> masterFaceCentres(targetPatch.size());

    const labelListList& masterFaceFaces = targetPatch.faceFaces();

    const ToPatch& masterFaces = targetPatch;

    const typename ToPatch::PointFieldType& masterPoints = targetPatch.points();

    forAll(masterFaceCentres, faceI)
    {
        masterFaceCentres[faceI] =
            masterFaces[faceI].centre(masterPoints);
    }

    // Result
    List<objectHit> result(this->size());

    const PrimitivePatch<Face, FaceList, PointField, PointType>& slaveFaces =
        *this;

    const PointField& slaveGlobalPoints = points();

    // Algorithm:
    // Loop through all points of the slave side. For every point find the
    // radius for the current contact face. If the contact point falls inside
    // the face and the radius is smaller than for all neighbouring faces,
    // the contact is found. If not, visit the neighbour closest to the
    // calculated contact point. If a single master face is visited more than
    // twice, initiate n-squared search.

    label curFace = 0;
    label nNSquaredSearches = 0;

    forAll(slaveFaceOrder, faceI)
    {
        // pick up slave point and direction
        const label curLocalFaceLabel = slaveFaceOrder[faceI];

        const point& curFaceCentre =
            slaveFaces[curLocalFaceLabel].centre(slaveGlobalPoints);

        const vector& curProjectionDir =
            projectionDirection[curLocalFaceLabel];

        bool closer;

        boolList visitedTargetFace(targetPatch.size(), false);
        bool doNSquaredSearch = false;

        bool foundEligible = false;

        scalar sqrDistance = GREAT;

        // Force the full search for the first point to ensure good
        // starting face
        if (faceI == 0)
        {
            doNSquaredSearch = true;
        }
        else
        {
            do
            {
                closer = false;
                doNSquaredSearch = false;

                // Calculate intersection with curFace
                PointHit<PointType> curHit =
                    masterFaces[curFace].ray
                    (
                        curFaceCentre,
                        curProjectionDir,
                        masterPoints,
                        alg,
                        dir
                    );

                visitedTargetFace[curFace] = true;

                if (curHit.hit())
                {
                    result[curLocalFaceLabel] = objectHit(true, curFace);

                    break;
                }
                else
                {
                    // If a new miss is eligible, it is closer than
                    // any previous eligible miss (due to surface walk)

                    // Only grab the miss if it is eligible
                    if (curHit.eligibleMiss())
                    {
                        foundEligible = true;
                        result[curLocalFaceLabel] = objectHit(false, curFace);
                    }

                    // Find the next likely face for intersection

                    // Calculate the miss point.  This is
                    // cooked (illogical!) for fastest surface walk.
                    //
                    PointType missPlanePoint =
                        curFaceCentre + curProjectionDir*curHit.distance();

                    sqrDistance =
                        magSqr(missPlanePoint - masterFaceCentres[curFace]);

                    const labelList& masterNbrs = masterFaceFaces[curFace];

                    forAll(masterNbrs, nbrI)
                    {
                        if
                        (
                            magSqr
                            (
                                missPlanePoint
                              - masterFaceCentres[masterNbrs[nbrI]]
                            )
                         <= sqrDistance
                        )
                        {
                            closer = true;
                            curFace = masterNbrs[nbrI];
                        }
                    }

                    if (visitedTargetFace[curFace])
                    {
                        // This face has already been visited.
                        // Execute n-squared search
                        doNSquaredSearch = true;
                        break;
                    }
                }

                if (debug) Info<< ".";
            } while (closer);
        }

        if (doNSquaredSearch || !foundEligible)
        {
            nNSquaredSearches++;

            if (debug)
            {
                Info<< "p " << curLocalFaceLabel << ": ";
            }

            result[curLocalFaceLabel] = objectHit(false, -1);
            scalar minDistance = GREAT;

            forAll(masterFaces, faceI)
            {
                PointHit<PointType> curHit =
                    masterFaces[faceI].ray
                    (
                        curFaceCentre,
                        curProjectionDir,
                        masterPoints,
                        alg,
                        dir
                    );

                if (curHit.hit())
                {
                    result[curLocalFaceLabel] = objectHit(true, faceI);
                    curFace = faceI;

                    break;
                }
                else if (curHit.eligibleMiss())
                {
                    // Calculate min distance
                    scalar missDist =
                        CML::mag(curHit.missPoint() - curFaceCentre);

                    if (missDist < minDistance)
                    {
                        minDistance = missDist;

                        result[curLocalFaceLabel] = objectHit(false, faceI);
                        curFace = faceI;
                    }
                }
            }

            if (debug)
            {
                Info<< result[curLocalFaceLabel] << nl;
            }
        }
        else
        {
            if (debug) Info<< "x";
        }
    }

    if (debug)
    {
        Info<< nl << "Executed " << nNSquaredSearches
            << " n-squared searches out of total of "
            << this->size() << endl;
    }

    return result;
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
void
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
visitPointRegion
(
    const label pointI,
    const labelList& pFaces,
    const label startFaceI,
    const label startEdgeI,
    boolList& pFacesHad
) const
{
    label index = findIndex(pFaces, startFaceI);

    if (!pFacesHad[index])
    {
        // Mark face as been visited.
        pFacesHad[index] = true;

        // Step to next edge on face which is still using pointI
        const labelList& fEdges = faceEdges()[startFaceI];

        label nextEdgeI = -1;

        forAll(fEdges, i)
        {
            label edgeI = fEdges[i];

            const edge& e = edges()[edgeI];

            if (edgeI != startEdgeI && (e[0] == pointI || e[1] == pointI))
            {
                nextEdgeI = edgeI;

                break;
            }
        }

        if (nextEdgeI == -1)
        {
            FatalErrorInFunction
                << "Problem: cannot find edge out of " << fEdges
                << "on face " << startFaceI << " that uses point " << pointI
                << " and is not edge " << startEdgeI << abort(FatalError);
        }

        // Walk to next face(s) across edge.
        const labelList& eFaces = edgeFaces()[nextEdgeI];

        forAll(eFaces, i)
        {
            if (eFaces[i] != startFaceI)
            {
                visitPointRegion
                (
                    pointI,
                    pFaces,
                    eFaces[i],
                    nextEdgeI,
                    pFacesHad
                );
            }
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
typename
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::surfaceTopo
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
surfaceType() const
{
    if (debug)
    {
        Info<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
               "surfaceType() : "
               "calculating patch topology"
            << endl;
    }

    const labelListList& edgeFcs = edgeFaces();

    surfaceTopo pType = MANIFOLD;

    forAll(edgeFcs, edgeI)
    {
        label nNbrs = edgeFcs[edgeI].size();

        if (nNbrs < 1 || nNbrs > 2)
        {
            pType = ILLEGAL;

            // Can exit now. Surface is illegal.
            return pType;
        }
        else if (nNbrs == 1)
        {
            // Surface might be open or illegal so keep looping.
            pType = OPEN;
        }
    }

    if (debug)
    {
        Info<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
               "surfaceType() : "
               "finished calculating patch topology"
            << endl;
    }

    return pType;
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
bool
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
checkTopology
(
    const bool report,
    labelHashSet* setPtr
) const
{
    if (debug)
    {
        Info<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
               "checkTopology(const bool, labelHashSet&) : "
               "checking patch topology"
            << endl;
    }

    // Check edgeFaces

    const labelListList& edgeFcs = edgeFaces();

    surfaceTopo surfaceType = MANIFOLD;

    forAll(edgeFcs, edgeI)
    {
        label nNbrs = edgeFcs[edgeI].size();

        if (nNbrs < 1 || nNbrs > 2)
        {
            surfaceType = ILLEGAL;

            if (report)
            {
                Info<< "Edge " << edgeI << " with vertices:" << edges()[edgeI]
                    << " has " << nNbrs << " face neighbours"
                    << endl;
            }

            if (setPtr)
            {
                const edge& e = edges()[edgeI];

                setPtr->insert(meshPoints()[e.start()]);
                setPtr->insert(meshPoints()[e.end()]);
            }
        }
        else if (nNbrs == 1)
        {
            surfaceType = OPEN;
        }
    }

    if (debug)
    {
        Info<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
               "checkTopology(const bool, labelHashSet&) : "
               "finished checking patch topology"
            << endl;
    }

    return surfaceType == ILLEGAL;
}


template
<
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
>
bool
CML::PrimitivePatch<Face, FaceList, PointField, PointType>::
checkPointManifold
(
    const bool report,
    labelHashSet* setPtr
) const
{
    const labelListList& pf = pointFaces();
    const labelListList& pe = pointEdges();
    const labelListList& ef = edgeFaces();
    const labelList& mp = meshPoints();

    bool foundError = false;

    forAll(pf, pointI)
    {
        const labelList& pFaces = pf[pointI];

        // Visited faces (as indices into pFaces)
        boolList pFacesHad(pFaces.size(), false);

        // Starting edge
        const labelList& pEdges = pe[pointI];
        label startEdgeI = pEdges[0];

        const labelList& eFaces = ef[startEdgeI];

        forAll(eFaces, i)
        {
            // Visit all faces using pointI, starting from eFaces[i] and
            // startEdgeI. Mark off all faces visited in pFacesHad.
            this->visitPointRegion
            (
                pointI,
                pFaces,
                eFaces[i],  // starting face for walk
                startEdgeI, // starting edge for walk
                pFacesHad
            );
        }

        // After this all faces using pointI should have been visited and
        // marked off in pFacesHad.

        label unset = findIndex(pFacesHad, false);

        if (unset != -1)
        {
            foundError = true;

            label meshPointI = mp[pointI];

            if (setPtr)
            {
                setPtr->insert(meshPointI);
            }

            if (report)
            {
                Info<< "Point " << meshPointI
                    << " uses faces which are not connected through an edge"
                    << nl
                    << "This means that the surface formed by this patched"
                    << " is multiply connected at this point" << nl
                    << "Connected (patch) faces:" << nl;

                forAll(pFacesHad, i)
                {
                    if (pFacesHad[i])
                    {
                        Info<< "    " << pFaces[i] << endl;
                    }
                }

                Info<< nl << "Unconnected (patch) faces:" << nl;
                forAll(pFacesHad, i)
                {
                    if (!pFacesHad[i])
                    {
                        Info<< "    " << pFaces[i] << endl;
                    }
                }
            }
        }
    }

    return foundError;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
