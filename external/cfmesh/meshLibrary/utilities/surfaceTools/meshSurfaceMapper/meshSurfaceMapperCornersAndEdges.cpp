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

Author: Franjo Juretic (franjo.juretic@c-fields.com)

\*---------------------------------------------------------------------------*/

#include "meshOctree.hpp"
#include "triSurf.hpp"
#include "triSurfacePartitioner.hpp"
#include "meshSurfaceMapper.hpp"
#include "meshSurfaceEngine.hpp"
#include "meshSurfaceEngineModifier.hpp"
#include "meshSurfacePartitioner.hpp"
#include "labelledScalar.hpp"

#include "helperFunctions.hpp"

# ifdef USE_OMP
#include <omp.h>
# endif

//#define DEBUGMapping

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void meshSurfaceMapper::findMappingDistance
(
    const labelLongList& nodesToMap,
    scalarList& mappingDistance
) const
{
    const vectorField& faceCentres = surfaceEngine_.faceCentres();
    const VRWGraph& pFaces = surfaceEngine_.pointFaces();
    const labelList& bPoints = surfaceEngine_.boundaryPoints();
    const pointFieldPMG& points = surfaceEngine_.points();

    //- generate search distance for corner nodes
    mappingDistance.setSize(nodesToMap.size());
    # ifdef USE_OMP
    # pragma omp parallel for schedule(dynamic, 50)
    # endif
    forAll(nodesToMap, i)
    {
        const label bpI = nodesToMap[i];

        mappingDistance[i] = 0.0;

        const point& p = points[bPoints[bpI]];
        forAllRow(pFaces, bpI, pfI)
        {
            const scalar d = magSqr(faceCentres[pFaces(bpI, pfI)] - p);
            mappingDistance[i] = CML::max(mappingDistance[i], d);
        }

        //- safety factor
        mappingDistance[i] *= 4.0;
    }

    if( Pstream::parRun() )
    {
        //- make sure that corner nodesd at parallel boundaries
        //- have the same range in which they accept the corners
        const VRWGraph& bpAtProcs = surfaceEngine_.bpAtProcs();
        const labelList& globalPointLabel =
            surfaceEngine_.globalBoundaryPointLabel();

        //- create the map for exchanging data
        std::map<label, DynList<labelledScalar> > exchangeData;
        const DynList<label>& neiProcs = surfaceEngine_.bpNeiProcs();
        forAll(neiProcs, i)
            exchangeData.insert
            (
                std::make_pair(neiProcs[i], DynList<labelledScalar>())
            );

        Map<label> globalToLocal;

        forAll(nodesToMap, nI)
        {
            const label bpI = nodesToMap[nI];

            if( bpAtProcs.sizeOfRow(bpI) != 0 )
                globalToLocal.insert(globalPointLabel[bpI], nI);

            forAllRow(bpAtProcs, bpI, i)
            {
                const label neiProc = bpAtProcs(bpI, i);
                if( neiProc == Pstream::myProcNo() )
                    continue;

                exchangeData[neiProc].append
                (
                    labelledScalar(globalPointLabel[bpI], mappingDistance[nI])
                );
            }
        }

        //- exchange data between processors
        LongList<labelledScalar> receivedData;
        help::exchangeMap(exchangeData, receivedData);

        //- select the maximum mapping distance for processor points
        forAll(receivedData, i)
        {
            const labelledScalar& ls = receivedData[i];

            const label nI = globalToLocal[ls.scalarLabel()];

            //- choose the maximum value for the mapping distance
            mappingDistance[nI] = CML::max(mappingDistance[nI], ls.value());
        }
    }
}

scalar meshSurfaceMapper::faceMetricInPatch
(
    const label bfI,
    const label patchI
) const
{
    const face& bf = surfaceEngine_.boundaryFaces()[bfI];

    const pointFieldPMG& points = surfaceEngine_.points();

    const point centre = bf.centre(points);
    const vector area = bf.normal(points);

    point projCentre;
    scalar dSq;
    label nt;

    meshOctree_.findNearestSurfacePointInRegion
    (
        projCentre,
        dSq,
        nt,
        patchI,
        centre
    );

    DynList<point> projPoints(bf.size());
    forAll(bf, pI)
    {
        meshOctree_.findNearestSurfacePointInRegion
        (
            projPoints[pI],
            dSq,
            nt,
            patchI,
            points[bf[pI]]
        );
    }

    vector projArea(vector::zero);
    forAll(bf, pI)
    {
        projArea +=
            triPointRef
            (
                projPoints[pI],
                projPoints[bf.fcIndex(pI)],
                projCentre
            ).normal();
    }

    return magSqr(centre - projCentre) + mag(mag(projArea) - mag(area));
}

void meshSurfaceMapper::mapCorners(const labelLongList& nodesToMap)
{
    const triSurfacePartitioner& sPartitioner = surfacePartitioner();
    const labelList& surfCorners = sPartitioner.corners();
    const List<DynList<label> >& cornerPatches = sPartitioner.cornerPatches();

    const meshSurfacePartitioner& mPart = meshPartitioner();
    const labelHashSet& corners = mPart.corners();
    const VRWGraph& pPatches = mPart.pointPatches();

    const pointFieldPMG& points = surfaceEngine_.points();
    const labelList& bPoints = surfaceEngine_.boundaryPoints();

    const triSurf& surf = meshOctree_.surface();
    const pointField& sPoints = surf.points();

    //std::map<label, scalar> mappingDistance;
    scalarList mappingDistance;
    findMappingDistance(nodesToMap, mappingDistance);

    //- for every corner in the mesh surface find the nearest corner in the
    //- triSurface
    meshSurfaceEngineModifier sMod(surfaceEngine_);

    # ifdef USE_OMP
    # pragma omp parallel for schedule(dynamic, 50)
    # endif
    forAll(nodesToMap, cornerI)
    {
        const label bpI = nodesToMap[cornerI];
        if( !corners.found(bpI) )
            FatalErrorIn
            (
                "meshSurfaceMapper::mapCorners(const labelLongList&)"
            ) << "Trying to map a point that is not a corner"
                << abort(FatalError);

        const point& p = points[bPoints[bpI]];
        const scalar maxDist = mappingDistance[cornerI];

        //- find the nearest position to the given point patches
        const DynList<label> patches = pPatches[bpI];

        point mapPointApprox(p);
        scalar distSqApprox;

        label iter(0);
        while( iter++ < 20 )
        {
            point newP(vector::zero);
            forAll(patches, patchI)
            {
                point np;
                label nt;
                meshOctree_.findNearestSurfacePointInRegion
                (
                    np,
                    distSqApprox,
                    nt,
                    patches[patchI],
                    mapPointApprox
                );

                newP += np;
            }

            newP /= patches.size();

            if( magSqr(newP - mapPointApprox) < 1e-8 * maxDist )
                break;

            mapPointApprox = newP;
        }
        distSqApprox = magSqr(mapPointApprox - p);

        //- find the nearest triSurface corner for the given corner
        scalar distSq(mappingDistance[cornerI]);
        point mapPoint(p);
        forAll(surfCorners, scI)
        {
            const label cornerID = surfCorners[scI];
            const point& sp = sPoints[cornerID];

            if( CML::magSqr(sp - p) < distSq )
            {
                bool store(true);
                const DynList<label>& cPatches = cornerPatches[scI];
                forAll(patches, i)
                {
                    if( !cPatches.contains(patches[i]) )
                    {
                        store = false;
                        break;
                    }
                }

                if( store )
                {
                    mapPoint = sp;
                    distSq = CML::magSqr(sp - p);
                }
            }
        }

        if( distSq > 1.2 * distSqApprox )
        {
            mapPoint = mapPointApprox;
        }

        //- move the point to the nearest corner
        sMod.moveBoundaryVertexNoUpdate(bpI, mapPoint);
    }

    sMod.updateGeometry(nodesToMap);
}

void meshSurfaceMapper::mapEdgeNodes(const labelLongList& nodesToMap)
{
    const pointFieldPMG& points = surfaceEngine_.points();
    const labelList& bPoints = surfaceEngine_.boundaryPoints();

    const meshSurfacePartitioner& mPart = meshPartitioner();
    const VRWGraph& pPatches = mPart.pointPatches();

    //const triSurf& surf = meshOctree_.surface();
    //const pointField& sPoints = surf.points();

    //- find mapping distance for selected vertices
    scalarList mappingDistance;
    findMappingDistance(nodesToMap, mappingDistance);

    const VRWGraph* bpAtProcsPtr(NULL);
    if( Pstream::parRun() )
        bpAtProcsPtr = &surfaceEngine_.bpAtProcs();

    LongList<parMapperHelper> parallelBndNodes;

    meshSurfaceEngineModifier sMod(surfaceEngine_);

    //- map point to the nearest vertex on the surface mesh
    # ifdef USE_OMP
    # pragma omp parallel for schedule(dynamic, 50)
    # endif
    forAll(nodesToMap, i)
    {
        const label bpI = nodesToMap[i];
        const point& p = points[bPoints[bpI]];

        //- find patches at this edge point
        const DynList<label> patches = pPatches[bpI];

        const scalar maxDist = mappingDistance[i];

        //- find approximate position of the vertex on the edge
        point mapPointApprox(p);
        scalar distSqApprox;
        label iter(0);
        while( iter++ < 20 )
        {
            point newP(vector::zero);

            forAll(patches, patchI)
            {
                point np;
                label nt;
                meshOctree_.findNearestSurfacePointInRegion
                (
                    np,
                    distSqApprox,
                    nt,
                    patches[patchI],
                    mapPointApprox
                );

                newP += np;
            }

            newP /= patches.size();

            if( magSqr(newP - mapPointApprox) < 1e-8 * maxDist )
                break;

            mapPointApprox = newP;
        }
        distSqApprox = magSqr(mapPointApprox - p);

        //- find the nearest vertex on the triSurface feature edge
        point mapPoint(mapPointApprox);
        scalar distSq(distSqApprox);
        label nse;
        meshOctree_.findNearestEdgePoint(mapPoint, distSq, nse, p, patches);

        //- use the vertex with the smallest mapping distance
        if( distSq > 1.2 * distSqApprox )
        {
            mapPoint = mapPointApprox;
            distSq = distSqApprox;
        }

        //- check if the mapping distance is within the given tolerances
        if( distSq > maxDist )
        {
            //- this indicates possible problems
            //- reduce the mapping distance
            const scalar f = CML::sqrt(maxDist / distSq);
            distSq = mappingDistance[i];
            mapPoint = f * (mapPoint - p) + p;
        }

        //- move the point to the nearest edge vertex
        sMod.moveBoundaryVertexNoUpdate(bpI, mapPoint);

        if( bpAtProcsPtr && bpAtProcsPtr->sizeOfRow(bpI) )
        {
            # ifdef USE_OMP
            # pragma omp critical
            # endif
            {
                parallelBndNodes.append
                (
                    parMapperHelper
                    (
                        mapPoint,
                        distSq,
                        bpI,
                        -1
                    )
                );
            }
        }
    }

    sMod.updateGeometry(nodesToMap);

    mapToSmallestDistance(parallelBndNodes);
}

void meshSurfaceMapper::mapCornersAndEdges()
{
    const meshSurfacePartitioner& mPart = meshPartitioner();
    const labelHashSet& cornerPoints = mPart.corners();
    labelLongList selectedPoints;
    forAllConstIter(labelHashSet, cornerPoints, it)
        selectedPoints.append(it.key());

    mapCorners(selectedPoints);

    selectedPoints.clear();
    const labelHashSet& edgePoints = mPart.edgePoints();
    forAllConstIter(labelHashSet, edgePoints, it)
        selectedPoints.append(it.key());

    mapEdgeNodes(selectedPoints);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
