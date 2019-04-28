/*---------------------------------------------------------------------------*\
Copyright (C) Creative Fields, Ltd.
-------------------------------------------------------------------------------
License
    This file is part of cfMesh.

    cfMesh is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    cfMesh is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with cfMesh.  If not, see <http://www.gnu.org/licenses/>.

Class
    meshSurfaceOptimizer

Description
    Surface smoothing without any topological changes

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    meshSurfaceOptimizer.C

\*---------------------------------------------------------------------------*/

#ifndef meshSurfaceOptimizer_HPP
#define meshSurfaceOptimizer_HPP

#include "polyMeshGen.hpp"
#include "boolList.hpp"
#include "labelLongList.hpp"
#include "DynList.hpp"
#include "triFace.hpp"
#include "meshSurfaceEngine.hpp"
#include "parTriFace.hpp"
#include "meshSurfacePartitioner.hpp"
#include "partTriMesh.hpp"

#include <map>

#include "demandDrivenData.hpp"
#include "plane.hpp"
#include "Map.hpp"
#include "surfaceOptimizer.hpp"
#include "helperFunctions.hpp"
#include "partTriMeshSimplex.hpp"

//#define DEBUGSmooth

# ifdef DEBUGSmooth
#include "triSurf.hpp"
#include "triSurfModifier.hpp"
#include "helperFunctions.hpp"
# endif


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declarations
class meshOctree;
class meshSurfaceMapper;
class plane;

/*---------------------------------------------------------------------------*\
                    Class meshSurfaceOptimizer Declaration
\*---------------------------------------------------------------------------*/

class meshSurfaceOptimizer
{
    // Private data
        //- const reference to the mesh surface
        const meshSurfaceEngine& surfaceEngine_;

        //- type of surface vertex
        List<direction> vertexType_;

        //- locked faces which shall not be changed
        labelLongList lockedSurfaceFaces_;

        //- surface partitioner
        const meshSurfacePartitioner* partitionerPtr_;
        const bool deletePartitioner_;

        //- pointer to mesh octree
        const meshOctree* octreePtr_;

        //- mesh of surface triangles needed for some smoothers
        mutable partTriMesh* triMeshPtr_;

        //- enforce constraints
        bool enforceConstraints_;

        //- name of the subset contaning tangled points
        word badPointsSubsetName_;

    // Private member functions
        //- classify surface vertices as PARTITION, EDGE, CORNER
        void classifySurfaceVertices();

        //- calculate surface triangulation
        void calculateTrianglesAndAddressing() const;
        inline const partTriMesh& triMesh() const;
        inline void updateTriMesh(const labelLongList&);
        inline void updateTriMesh();

        //- find vertices which are inverted
        //- mark additional layers of vertices near the inverted ones
        label findInvertedVertices
        (
            boolList& smoothVertex,
            const label nAdditionalLayers = 2
        ) const;

        //- transform into a 2D space in plane
        inline bool transformIntoPlane
        (
            const label bpI,
            const plane& pl,
            vector& vecX,
            vector& vecY,
            DynList<point>& pts,
            DynList<triFace>& trias
        ) const;

        //- new position of a node after laplacian smoothing
        //- the position is the average of neighbouring vertex positions
        inline point newPositionLaplacian
        (
            const label bpI,
            const bool transformIntoPlane = true
        ) const;

        //- new position of a node after laplacian smoothing
        //- the position is the average of neighbouring face centres
        inline point newPositionLaplacianFC
        (
            const label bpI,
            const bool transformIntoPlane = true
        ) const;

        //- new position of a node after laplacian smoothing
        //- the position is the weighted average of neighbouring face centres
        //- the weights are the magnitudes of neighbouring face area vectors
        inline point newPositionLaplacianWFC
        (
            const label bpI,
            const bool transformIntoPlane = true
        ) const;

        //- new position of a node after using surfaceOptimizer
        inline point newPositionSurfaceOptimizer
        (
            const label bpI,
            const scalar tol = 0.001
        ) const;

        //- new position of edge vertex
        //- the position is the average of neighbouring edge vertices
        inline point newEdgePositionLaplacian(const label bpI) const;

        //- smooth the node using the laplacian smoother
        //- new position is the average of the neighbouring vertices
        void nodeDisplacementLaplacian
        (
            const label bpI,
            const bool transformIntoPlane = true
        ) const;

        //- smooth the node using the laplacian smoother
        //- new position is the average of the centres of faces attached
        //- to the vertex
        void nodeDisplacementLaplacianFC
        (
            const label bpI,
            const bool transformIntoPlane = true
        ) const;

        //- smooth the node using surface optimizer
        void nodeDisplacementSurfaceOptimizer
        (
            const label bpI,
            const scalar tol = 1.0
        );

        //- smooth edge node
        void edgeNodeDisplacement(const label bpI) const;

        //- smooth selected edge points
        void smoothEdgePoints
        (
            const labelLongList& edgePoints,
            const labelLongList& procEdgePoints
        );

        //- smooth selected points using laplaceFC
        void smoothLaplacianFC
        (
            const labelLongList& selectedPoints,
            const labelLongList& selectedProcPoints,
            const bool transform = true
        );

        //- smooth selected points using surface optimizer
        void smoothSurfaceOptimizer
        (
            const labelLongList& selectedPoints,
            const labelLongList& selectedProcPoints
        );

    // Functions needed for parallel runs

        //- transfer data between processors
        void exchangeData
        (
            const labelLongList& nodesToSmooth,
            std::map<label, DynList<parTriFace> >& m
        ) const;

        //- laplacian smoothing of points at processor boundaries
        //- new position is the average of the neighbouring vertices
        void nodeDisplacementLaplacianParallel
        (
            const labelLongList& nodesToSmooth,
            const bool transformIntoPlane = true
        );

        //- laplacian smoothing of points at processor boundaries
        void nodeDisplacementLaplacianFCParallel
        (
            const labelLongList& nodesToSmooth,
            const bool transformIntoPlane = true
        );

        //- smooth edge nodes at processor boundaries
        void edgeNodeDisplacementParallel
        (
            const labelLongList& nodesToSmooth
        );

        //- Disallow default bitwise copy construct
        meshSurfaceOptimizer(const meshSurfaceOptimizer&);

        //- Disallow default bitwise assignment
        void operator=(const meshSurfaceOptimizer&);

    // Enumerators
        enum vertexTypes
        {
            PARTITION = 1,
            EDGE = 2,
            CORNER = 4,
            PROCBND = 8,
            LOCKED = 16
        };

public:

    // Constructors

        //- Construct from mesh surface
        meshSurfaceOptimizer(const meshSurfaceEngine&);

        //- Construct from partitioner
        meshSurfaceOptimizer(const meshSurfacePartitioner&);

        //- Construct from mesh surface and octree
        meshSurfaceOptimizer(const meshSurfaceEngine&, const meshOctree&);

        //- Construct from partitioner and octree
        meshSurfaceOptimizer
        (
            const meshSurfacePartitioner&,
            const meshOctree&
        );


    // Destructor

        ~meshSurfaceOptimizer();

    // Member Functions
        //- lock the boundary faces which shall not be modified
        template<class labelListType>
        inline void lockBoundaryFaces(const labelListType&);

        //- lock boundary points. They are not be moved.
        template<class labelListType>
        inline void lockBoundaryPoints(const labelListType&);

        //- lock edge points
        inline void lockFeatureEdges();

        //- reset to default constraints
        void removeUserConstraints();

        //- Flag stopping the meshing process if it is not possible
        //- to untangle the surface without sacrificing geometry constraints
        //- Points which must be moved away from the required position are
        //- stored into a point subset
        void enforceConstraints(const word subsetName="badPoints");

        //- runs a surface smoother on the selected boundary points
        bool untangleSurface
        (
            const labelLongList& activeBoundaryPoints,
            const label nAdditionalLayers = 0
        );

        //- checks for invertex surface elements and tries to untangle them
        //- it tries to keep the points on the surface for a couple
        //- of iteration and gives up the final iterations
        //- by default, it smooths two additional layers of elements
        //- around the inverted ones
        bool untangleSurface(const label nAdditionalLayers = 2);

        //- optimize boundary nodes after boundary regions are created
        void optimizeSurface(const label nIterations = 5);

        //- optimize the surface of a 2D mesh
        void optimizeSurface2D(const label nIterations = 5);

        //- untangle the surface of a 2D mesh
        void untangleSurface2D();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline const partTriMesh& meshSurfaceOptimizer::triMesh() const
{
    if( !triMeshPtr_ )
        calculateTrianglesAndAddressing();

    return *triMeshPtr_;
}

inline void meshSurfaceOptimizer::updateTriMesh(const labelLongList& selPoints)
{
    if( !triMeshPtr_ )
        FatalErrorInFunction
          << "triMeshPtr_ is not allocated " << abort(FatalError);

    triMeshPtr_->updateVertices(selPoints);
}

inline void meshSurfaceOptimizer::updateTriMesh()
{
    if( !triMeshPtr_ )
        FatalErrorInFunction
          << "triMeshPtr_ is not allocated " << abort(FatalError);

    triMeshPtr_->updateVertices();
}

inline bool meshSurfaceOptimizer::transformIntoPlane
(
    const label bpI,
    const plane& pl,
    vector& vecX,
    vector& vecY,
    DynList<point>& pts,
    DynList<triFace>& trias
) const
{
    # ifdef DEBUGSmooth
    Pout << "Transforming boundary node " << bpI << endl;
    # endif

    const partTriMesh& triMesh = this->triMesh();
    const label triPointI = triMesh.meshSurfacePointLabelInTriMesh()[bpI];
    if( triPointI < 0 )
        return false;

    partTriMeshSimplex simplex(triMesh, triPointI);

    const DynList<point, 32>& sPts = simplex.pts();

    //- create vecX and vecY
    const point& p = simplex.centrePoint();
    pts.setSize(0);
    bool found(false);
    forAll(sPts, pI)
    {
        const point sp = pl.nearestPoint(sPts[pI]);
        const scalar d = mag(sp - p);
        if( d > VSMALL )
        {
            vecX = sp - p;
            vecX /= d;
            vecY = pl.normal() ^ vecX;
            vecY /= mag(vecY);
            found = true;
            break;
        }
    }

    if( !found )
        return false;

    trias = simplex.triangles();

    # ifdef DEBUGSmooth
    Pout << "VecX " << vecX << endl;
    Pout << "vecY " << vecY << endl;
    # endif

    //- transform the vertices
    pts.setSize(sPts.size());

    forAll(sPts, pI)
    {
        const point pt
        (
            ((sPts[pI] - p) & vecX),
            ((sPts[pI] - p) & vecY),
            0.0
        );

        pts[pI] = pt;
    }

    # ifdef DEBUGSmooth
    Info << "Original triangles " << endl;
    forAll(simplex.triangles(), triI)
        Info << "Tri " << triI << " is " << simplex.triangles()[triI] << endl;
    Info << "Transformed triangles are " << trias << endl;
    Info << "Transformed vertices " << pts << endl;

    triSurf surf;
    triSurfModifier sMod(surf);
    pointField& sPoints = sMod.pointsAccess();
    sPoints.setSize(pts.size());
    forAll(sPoints, i)
        sPoints[i] = pts[i];
    LongList<labelledTri>& sTrias = sMod.facetsAccess();
    sTrias.setSize(trias.size());
    forAll(sTrias, i)
    {
        labelledTri& tf = sTrias[i];
        tf[0] = trias[i][0];
        tf[1] = trias[i][1];
        tf[2] = trias[i][2];

        tf.region() = 0;
    }
    sMod.patchesAccess().setSize(1);
    sMod.patchesAccess()[0].name() = "bnd";
    sMod.patchesAccess()[0].geometricType() = "patch";

    fileName sName("bndSimplex_");
    sName += help::scalarToText(bpI);
    sName += ".stl";
    surf.writeSurface(sName);
    # endif

    return found;
}

inline point meshSurfaceOptimizer::newPositionLaplacian
(
    const label bpI,
    const bool transformIntoPlane
) const
{
    const VRWGraph& pPoints = surfaceEngine_.pointPoints();
    const pointFieldPMG& points = surfaceEngine_.points();
    const labelList& bPoints = surfaceEngine_.boundaryPoints();

    if( vertexType_[bpI] & LOCKED )
        return points[bPoints[bpI]];

    vector newP(vector::zero);
    if( transformIntoPlane )
    {
        const vector& pNormal = surfaceEngine_.pointNormals()[bpI];

        if( magSqr(pNormal) < VSMALL )
            return points[bPoints[bpI]];

        plane pl(points[bPoints[bpI]], pNormal);

        DynList<point> projectedPoints;
        projectedPoints.setSize(pPoints.sizeOfRow(bpI));
        forAllRow(pPoints, bpI, pI)
            projectedPoints[pI] =
                pl.nearestPoint(points[bPoints[pPoints(bpI, pI)]]);

        forAll(projectedPoints, pI)
            newP += projectedPoints[pI];

        newP /= projectedPoints.size();
    }
    else
    {
        forAllRow(pPoints, bpI, pI)
            newP += points[bPoints[pPoints(bpI, pI)]];

        newP /= pPoints.sizeOfRow(bpI);
    }

    return newP;
}

inline point meshSurfaceOptimizer::newPositionLaplacianFC
(
    const label bpI,
    const bool transformIntoPlane
) const
{
    const VRWGraph& pointFaces = surfaceEngine_.pointFaces();
    const pointFieldPMG& points = surfaceEngine_.points();
    const vectorField& faceCentres = surfaceEngine_.faceCentres();
    const labelList& bPoints = surfaceEngine_.boundaryPoints();

    if( vertexType_[bpI] & LOCKED )
        return points[bPoints[bpI]];

    vector newP(vector::zero);

    if( transformIntoPlane )
    {
        const vector& pNormal = surfaceEngine_.pointNormals()[bpI];

        if( magSqr(pNormal) < VSMALL )
            return points[bPoints[bpI]];

        plane pl(points[bPoints[bpI]], pNormal);

        DynList<point> projectedPoints;
        projectedPoints.setSize(pointFaces.sizeOfRow(bpI));
        forAllRow(pointFaces, bpI, pfI)
            projectedPoints[pfI] =
                pl.nearestPoint(faceCentres[pointFaces(bpI, pfI)]);

        forAll(projectedPoints, pI)
            newP += projectedPoints[pI];

        newP /= projectedPoints.size();
    }
    else
    {
        forAllRow(pointFaces, bpI, pfI)
            newP += faceCentres[pointFaces(bpI, pfI)];

        newP /= pointFaces.sizeOfRow(bpI);
    }

    return newP;
}

inline point meshSurfaceOptimizer::newPositionLaplacianWFC
(
    const label bpI,
    const bool transformIntoPlane
) const
{
    const VRWGraph& pointFaces = surfaceEngine_.pointFaces();
    const pointFieldPMG& points = surfaceEngine_.points();
    const vectorField& faceAreas = surfaceEngine_.faceNormals();
    const vectorField& faceCentres = surfaceEngine_.faceCentres();
    const labelList& bPoints = surfaceEngine_.boundaryPoints();

    if( vertexType_[bpI] & LOCKED )
        return points[bPoints[bpI]];

    vector newP(vector::zero);

    if( transformIntoPlane )
    {
        const vector& pNormal = surfaceEngine_.pointNormals()[bpI];

        if( magSqr(pNormal) < VSMALL )
            return points[bPoints[bpI]];

        plane pl(points[bPoints[bpI]], pNormal);

        DynList<point> projectedPoints;
        projectedPoints.setSize(pointFaces.sizeOfRow(bpI));
        forAllRow(pointFaces, bpI, pfI)
            projectedPoints[pfI] =
                pl.nearestPoint(faceCentres[pointFaces(bpI, pfI)]);

        scalar sumW(0.0);
        forAll(projectedPoints, pI)
        {
            const label bfI = pointFaces(bpI, pI);
            const scalar w = (faceAreas[bfI] & faceAreas[bfI]);
            newP += w * projectedPoints[pI];
            sumW += w;
        }

        newP /= CML::max(sumW, VSMALL);
    }
    else
    {
        scalar sumW(0.0);
        forAllRow(pointFaces, bpI, pfI)
        {
            const label bfI = pointFaces(bpI, pfI);
            const scalar w = (faceAreas[bfI] & faceAreas[bfI]);
            newP += w * faceCentres[pointFaces(bpI, pfI)];
            sumW += w;
        }

        newP /= CML::max(sumW, VSMALL);
    }

    return newP;
}

inline point meshSurfaceOptimizer::newPositionSurfaceOptimizer
(
    const label bpI,
    const scalar tol
) const
{
    const pointFieldPMG& points = surfaceEngine_.points();
    const labelList& bPoints = surfaceEngine_.boundaryPoints();

    if( vertexType_[bpI] & LOCKED )
        return points[bPoints[bpI]];

    # ifdef DEBUGSmooth
    Pout << "Smoothing boundary node " << bpI << endl;
    Pout << "Node label in the mesh is " << bPoints[bpI] << endl;
    Pout << "Point coordinates " << points[bPoints[bpI]] << endl;
    # endif

    //- project vertices onto the plane
    const vector& pNormal = surfaceEngine_.pointNormals()[bpI];
    if( magSqr(pNormal) < VSMALL )
        return points[bPoints[bpI]];

    const plane pl(points[bPoints[bpI]], pNormal);

    DynList<point> pts;
    DynList<triFace> trias;
    vector vecX, vecY;
    bool success = this->transformIntoPlane(bpI, pl, vecX, vecY, pts, trias);
    if( !success )
    {
        //Warning << "Cannot transform to plane" << endl;
        return points[bPoints[bpI]];
    }

    surfaceOptimizer so(pts, trias);
    const point newPoint = so.optimizePoint(tol);

    const point newP
    (
        points[bPoints[bpI]] +
        vecX * newPoint.x() +
        vecY * newPoint.y()
    );

    if( help::isnan(newP) || help::isinf(newP) )
    {
        WarningIn
        (
            "inline point meshSurfaceOptimizer::newPositionSurfaceOptimizer"
            "(const label, const scalar) const"
        ) << "Cannot move point " << bpI << endl;

        return points[bPoints[bpI]];
    }

    return newP;
}

inline point meshSurfaceOptimizer::newEdgePositionLaplacian
(
    const label bpI
) const
{
    const labelList& bPoints = surfaceEngine_.boundaryPoints();
    const edgeList& edges = surfaceEngine_.edges();
    const VRWGraph& bpEdges = surfaceEngine_.boundaryPointEdges();
    const pointFieldPMG& points = surfaceEngine_.points();

    if( vertexType_[bpI] & LOCKED )
        return points[bPoints[bpI]];

    const labelHashSet& featureEdges = partitionerPtr_->featureEdges();

    DynList<label> edgePoints;

    forAllRow(bpEdges, bpI, i)
    {
        const label beI = bpEdges(bpI, i);

        if( featureEdges.found(beI) )
        {
            edgePoints.append(edges[beI].otherVertex(bPoints[bpI]));
        }
    }

    if( edgePoints.size() != 2 )
        return points[bPoints[bpI]];

    # ifdef DEBUGSearch
    Info << "Edge points " << edgePoints << endl;
    # endif

    vector pos(vector::zero);
    forAll(edgePoints, epI)
        pos += points[edgePoints[epI]];

    pos /= edgePoints.size();

    return pos;
}

template<class labelListType>
inline void meshSurfaceOptimizer::lockBoundaryFaces(const labelListType& l)
{
    lockedSurfaceFaces_ = l;

    const faceList::subList& bFaces = surfaceEngine_.boundaryFaces();
    const labelList& bp = surfaceEngine_.bp();

    # ifdef USE_OMP
    # pragma omp parallel for schedule(dynamic, 20)
    # endif
    forAll(lockedSurfaceFaces_, lfI)
    {
        const face& bf = bFaces[lockedSurfaceFaces_[lfI]];

        forAll(bf, pI)
            vertexType_[bp[bf[pI]]] |= LOCKED;
    }

    if( Pstream::parRun() )
    {
        const Map<label>& globalToLocal =
            surfaceEngine_.globalToLocalBndPointAddressing();
        const VRWGraph& bpAtProcs = surfaceEngine_.bpAtProcs();
        const DynList<label>& bpNeiProcs = surfaceEngine_.bpNeiProcs();

        std::map<label, labelLongList> exchangeData;
        forAll(bpNeiProcs, i)
            exchangeData[bpNeiProcs[i]].clear();

        //- prepare data which will be sent to other processors
        forAllConstIter(Map<label>, globalToLocal, it)
        {
            const label bpI = it();

            if( vertexType_[bpI] & LOCKED )
            {
                forAllRow(bpAtProcs, bpI, i)
                {
                    const label neiProc = bpAtProcs(bpI, i);

                    if( neiProc == Pstream::myProcNo() )
                        continue;

                    exchangeData[neiProc].append(it.key());
                }
            }
        }

        labelLongList receivedData;
        help::exchangeMap(exchangeData, receivedData);

        forAll(receivedData, i)
        {
            const label bpI = globalToLocal[receivedData[i]];

            vertexType_[bpI] |= LOCKED;
        }
    }
}

template<class labelListType>
inline void meshSurfaceOptimizer::lockBoundaryPoints(const labelListType& l)
{
    # ifdef USE_OMP
    # pragma omp parallel for schedule(dynamic, 50)
    # endif
    forAll(l, i)
    {
        const label bpI = l[i];

        vertexType_[bpI] |= LOCKED;
    }

    if( Pstream::parRun() )
    {
        const Map<label>& globalToLocal =
            surfaceEngine_.globalToLocalBndPointAddressing();
        const VRWGraph& bpAtProcs = surfaceEngine_.bpAtProcs();
        const DynList<label>& bpNeiProcs = surfaceEngine_.bpNeiProcs();

        std::map<label, labelLongList> exchangeData;
        forAll(bpNeiProcs, i)
            exchangeData[bpNeiProcs[i]].clear();

        //- prepare data which will be sent to other processors
        forAllConstIter(Map<label>, globalToLocal, it)
        {
            const label bpI = it();

            if( vertexType_[bpI] & LOCKED )
            {
                forAllRow(bpAtProcs, bpI, i)
                {
                    const label neiProc = bpAtProcs(bpI, i);

                    if( neiProc == Pstream::myProcNo() )
                        continue;

                    exchangeData[neiProc].append(it.key());
                }
            }
        }

        labelLongList receivedData;
        help::exchangeMap(exchangeData, receivedData);

        forAll(receivedData, i)
        {
            const label bpI = globalToLocal[receivedData[i]];

            vertexType_[bpI] |= LOCKED;
        }
    }
}

//- lock edge points
inline void meshSurfaceOptimizer::lockFeatureEdges()
{
    # ifdef USE_OMP
    # pragma omp parallel for schedule(dynamic, 50)
    # endif
    forAll(vertexType_, bpI)
        if( vertexType_[bpI] & (EDGE | CORNER) )
            vertexType_[bpI] |= LOCKED;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
