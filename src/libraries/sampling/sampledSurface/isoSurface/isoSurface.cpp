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

\*---------------------------------------------------------------------------*/

#include "isoSurface.hpp"
#include "fvMesh.hpp"
#include "mergePoints.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "slicedVolFields.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "OFstream.hpp"
#include "meshTools.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(isoSurface, 0);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

bool CML::isoSurface::noTransform(const tensor& tt) const
{
    return
        (mag(tt.xx()-1) < mergeDistance_)
     && (mag(tt.yy()-1) < mergeDistance_)
     && (mag(tt.zz()-1) < mergeDistance_)
     && (mag(tt.xy()) < mergeDistance_)
     && (mag(tt.xz()) < mergeDistance_)
     && (mag(tt.yx()) < mergeDistance_)
     && (mag(tt.yz()) < mergeDistance_)
     && (mag(tt.zx()) < mergeDistance_)
     && (mag(tt.zy()) < mergeDistance_);
}


// Calculates per face whether couple is collocated.
bool CML::isoSurface::collocatedPatch(const polyPatch& pp)
{
    const coupledPolyPatch& cpp = refCast<const coupledPolyPatch>(pp);
    return cpp.parallel() && !cpp.separated();
}


// Calculates per face whether couple is collocated.
CML::PackedBoolList CML::isoSurface::collocatedFaces
(
    const coupledPolyPatch& pp
) const
{
    // Initialise to false
    PackedBoolList collocated(pp.size());

    if (isA<processorPolyPatch>(pp))
    {
        if (collocatedPatch(pp))
        {
            forAll(pp, i)
            {
                collocated[i] = 1u;
            }
        }
    }
    else if (isA<cyclicPolyPatch>(pp))
    {
        if (collocatedPatch(pp))
        {
            forAll(pp, i)
            {
                collocated[i] = 1u;
            }
        }
    }
    else
    {
        FatalErrorInFunction
            << "Unhandled coupledPolyPatch type " << pp.type()
            << abort(FatalError);
    }
    return collocated;
}


void CML::isoSurface::syncUnseparatedPoints
(
    pointField& pointValues,
    const point& nullValue
) const
{
    // Until syncPointList handles separated coupled patches with multiple
    // transforms do our own synchronisation of non-separated patches only
    const polyBoundaryMesh& patches = mesh_.boundaryMesh();

    if (Pstream::parRun())
    {
        // Send
        forAll(patches, patchi)
        {
            if
            (
                isA<processorPolyPatch>(patches[patchi])
             && patches[patchi].nPoints() > 0
             && collocatedPatch(patches[patchi])
            )
            {
                const processorPolyPatch& pp =
                    refCast<const processorPolyPatch>(patches[patchi]);

                const labelList& meshPts = pp.meshPoints();
                const labelList& nbrPts = pp.neighbPoints();

                pointField patchInfo(meshPts.size());

                forAll(nbrPts, pointi)
                {
                    label nbrPointi = nbrPts[pointi];
                    patchInfo[nbrPointi] = pointValues[meshPts[pointi]];
                }

                OPstream toNbr(Pstream::blocking, pp.neighbProcNo());
                toNbr << patchInfo;
            }
        }

        // Receive and combine.

        forAll(patches, patchi)
        {
            if
            (
                isA<processorPolyPatch>(patches[patchi])
             && patches[patchi].nPoints() > 0
             && collocatedPatch(patches[patchi])
            )
            {
                const processorPolyPatch& pp =
                    refCast<const processorPolyPatch>(patches[patchi]);

                pointField nbrPatchInfo(pp.nPoints());
                {
                    // We do not know the number of points on the other side
                    // so cannot use Pstream::read.
                    IPstream fromNbr(Pstream::blocking, pp.neighbProcNo());
                    fromNbr >> nbrPatchInfo;
                }

                const labelList& meshPts = pp.meshPoints();

                forAll(meshPts, pointi)
                {
                    label meshPointi = meshPts[pointi];
                    minEqOp<point>()
                    (
                        pointValues[meshPointi],
                        nbrPatchInfo[pointi]
                    );
                }
            }
        }
    }

    // Do the cyclics.
    forAll(patches, patchi)
    {
        if (isA<cyclicPolyPatch>(patches[patchi]))
        {
            const cyclicPolyPatch& cycPatch =
                refCast<const cyclicPolyPatch>(patches[patchi]);

            if (cycPatch.owner() && collocatedPatch(cycPatch))
            {
                // Owner does all.

                const edgeList& coupledPoints = cycPatch.coupledPoints();
                const labelList& meshPts = cycPatch.meshPoints();
                const cyclicPolyPatch& nbrPatch = cycPatch.neighbPatch();
                const labelList& nbrMeshPoints = nbrPatch.meshPoints();

                pointField half0Values(coupledPoints.size());
                pointField half1Values(coupledPoints.size());

                forAll(coupledPoints, i)
                {
                    const edge& e = coupledPoints[i];
                    half0Values[i] = pointValues[meshPts[e[0]]];
                    half1Values[i] = pointValues[nbrMeshPoints[e[1]]];
                }

                forAll(coupledPoints, i)
                {
                    const edge& e = coupledPoints[i];
                    label p0 = meshPts[e[0]];
                    label p1 = nbrMeshPoints[e[1]];

                    minEqOp<point>()(pointValues[p0], half1Values[i]);
                    minEqOp<point>()(pointValues[p1], half0Values[i]);
                }
            }
        }
    }

    // Synchronize multiple shared points.
    const globalMeshData& pd = mesh_.globalData();

    if (pd.nGlobalPoints() > 0)
    {
        // Values on shared points.
        pointField sharedPts(pd.nGlobalPoints(), nullValue);

        forAll(pd.sharedPointLabels(), i)
        {
            label meshPointi = pd.sharedPointLabels()[i];
            // Fill my entries in the shared points
            sharedPts[pd.sharedPointAddr()[i]] = pointValues[meshPointi];
        }

        // Combine on master.
        Pstream::listCombineGather(sharedPts, minEqOp<point>());
        Pstream::listCombineScatter(sharedPts);

        // Now we will all have the same information. Merge it back with
        // my local information.
        forAll(pd.sharedPointLabels(), i)
        {
            label meshPointi = pd.sharedPointLabels()[i];
            pointValues[meshPointi] = sharedPts[pd.sharedPointAddr()[i]];
        }
    }
}


CML::scalar CML::isoSurface::isoFraction
(
    const scalar s0,
    const scalar s1
) const
{
    scalar d = s1-s0;

    if (mag(d) > VSMALL)
    {
        return (iso_-s0)/d;
    }
    else
    {
        return -1.0;
    }
}


bool CML::isoSurface::isEdgeOfFaceCut
(
    const scalarField& pVals,
    const face& f,
    const bool ownLower,
    const bool neiLower
) const
{
    forAll(f, fp)
    {
        bool fpLower = (pVals[f[fp]] < iso_);
        if
        (
            (fpLower != ownLower)
         || (fpLower != neiLower)
         || (fpLower != (pVals[f[f.fcIndex(fp)]] < iso_))
        )
        {
            return true;
        }
    }
    return false;
}


// Get neighbour value and position.
void CML::isoSurface::getNeighbour
(
    const labelList& boundaryRegion,
    const volVectorField& meshC,
    const volScalarField& cVals,
    const label celli,
    const label facei,
    scalar& nbrValue,
    point& nbrPoint
) const
{
    const labelList& own = mesh_.faceOwner();
    const labelList& nei = mesh_.faceNeighbour();

    if (mesh_.isInternalFace(facei))
    {
        label nbr = (own[facei] == celli ? nei[facei] : own[facei]);
        nbrValue = cVals[nbr];
        nbrPoint = meshC[nbr];
    }
    else
    {
        label bFacei = facei-mesh_.nInternalFaces();
        label patchi = boundaryRegion[bFacei];
        const polyPatch& pp = mesh_.boundaryMesh()[patchi];
        label patchFacei = facei-pp.start();

        nbrValue = cVals.boundaryField()[patchi][patchFacei];
        nbrPoint = meshC.boundaryField()[patchi][patchFacei];
    }
}


// Determine for every face/cell whether it (possibly) generates triangles.
void CML::isoSurface::calcCutTypes
(
    const labelList& boundaryRegion,
    const volVectorField& meshC,
    const volScalarField& cVals,
    const scalarField& pVals
)
{
    const polyBoundaryMesh& patches = mesh_.boundaryMesh();
    const labelList& own = mesh_.faceOwner();
    const labelList& nei = mesh_.faceNeighbour();

    faceCutType_.setSize(mesh_.nFaces());
    faceCutType_ = NOTCUT;

    for (label facei = 0; facei < mesh_.nInternalFaces(); facei++)
    {
        // CC edge.
        bool ownLower = (cVals[own[facei]] < iso_);

        scalar nbrValue;
        point nbrPoint;
        getNeighbour
        (
            boundaryRegion,
            meshC,
            cVals,
            own[facei],
            facei,
            nbrValue,
            nbrPoint
        );

        bool neiLower = (nbrValue < iso_);

        if (ownLower != neiLower)
        {
            faceCutType_[facei] = CUT;
        }
        else
        {
            // See if any mesh edge is cut by looping over all the edges of the
            // face.
            const face f = mesh_.faces()[facei];

            if (isEdgeOfFaceCut(pVals, f, ownLower, neiLower))
            {
                faceCutType_[facei] = CUT;
            }
        }
    }

    forAll(patches, patchi)
    {
        const polyPatch& pp = patches[patchi];

        label facei = pp.start();

        forAll(pp, i)
        {
            bool ownLower = (cVals[own[facei]] < iso_);

            scalar nbrValue;
            point nbrPoint;
            getNeighbour
            (
                boundaryRegion,
                meshC,
                cVals,
                own[facei],
                facei,
                nbrValue,
                nbrPoint
            );

            bool neiLower = (nbrValue < iso_);

            if (ownLower != neiLower)
            {
                faceCutType_[facei] = CUT;
            }
            else
            {
                // Mesh edge.
                const face f = mesh_.faces()[facei];

                if (isEdgeOfFaceCut(pVals, f, ownLower, neiLower))
                {
                    faceCutType_[facei] = CUT;
                }
            }

            facei++;
        }
    }



    nCutCells_ = 0;
    cellCutType_.setSize(mesh_.nCells());
    cellCutType_ = NOTCUT;

    for (label facei = 0; facei < mesh_.nInternalFaces(); facei++)
    {
        if (faceCutType_[facei] != NOTCUT)
        {
            if (cellCutType_[own[facei]] == NOTCUT)
            {
                cellCutType_[own[facei]] = CUT;
                nCutCells_++;
            }
            if (cellCutType_[nei[facei]] == NOTCUT)
            {
                cellCutType_[nei[facei]] = CUT;
                nCutCells_++;
            }
        }
    }
    for (label facei = mesh_.nInternalFaces(); facei < mesh_.nFaces(); facei++)
    {
        if (faceCutType_[facei] != NOTCUT)
        {
            if (cellCutType_[own[facei]] == NOTCUT)
            {
                cellCutType_[own[facei]] = CUT;
                nCutCells_++;
            }
        }
    }

    if (debug)
    {
        Pout<< "isoSurface : detected " << nCutCells_
            << " candidate cut cells (out of " << mesh_.nCells()
            << ")." << endl;
    }
}


// Return the two common points between two triangles
CML::labelPair CML::isoSurface::findCommonPoints
(
    const labelledTri& tri0,
    const labelledTri& tri1
)
{
    labelPair common(-1, -1);

    label fp0 = 0;
    label fp1 = findIndex(tri1, tri0[fp0]);

    if (fp1 == -1)
    {
        fp0 = 1;
        fp1 = findIndex(tri1, tri0[fp0]);
    }

    if (fp1 != -1)
    {
        // So tri0[fp0] is tri1[fp1]

        // Find next common point
        label fp0p1 = tri0.fcIndex(fp0);
        label fp1p1 = tri1.fcIndex(fp1);
        label fp1m1 = tri1.rcIndex(fp1);

        if (tri0[fp0p1] == tri1[fp1p1] || tri0[fp0p1] == tri1[fp1m1])
        {
            common[0] = tri0[fp0];
            common[1] = tri0[fp0p1];
        }
    }
    return common;
}


// Caculate centre of surface.
CML::point CML::isoSurface::calcCentre(const triSurface& s)
{
    vector sum = vector::zero;

    forAll(s, i)
    {
        sum += s[i].centre(s.points());
    }
    return sum/s.size();
}


// Replace surface (localPoints, localTris) with single point. Returns
// point. Destructs arguments.
CML::pointIndexHit CML::isoSurface::collapseSurface
(
    pointField& localPoints,
    DynamicList<labelledTri, 64>& localTris
)
{
    pointIndexHit info(false, vector::zero, localTris.size());

    if (localTris.size() == 1)
    {
        const labelledTri& tri = localTris[0];
        info.setPoint(tri.centre(localPoints));
        info.setHit();
    }
    else if (localTris.size() == 2)
    {
        // Check if the two triangles share an edge.
        const labelledTri& tri0 = localTris[0];
        const labelledTri& tri1 = localTris[0];

        labelPair shared = findCommonPoints(tri0, tri1);

        if (shared[0] != -1)
        {
            info.setPoint
            (
                0.5
              * (
                    tri0.centre(localPoints)
                  + tri1.centre(localPoints)
                )
            );
            info.setHit();
        }
    }
    else if (localTris.size())
    {
        // Check if single region. Rare situation.
        triSurface surf
        (
            localTris,
            geometricSurfacePatchList(0),
            localPoints,
            true
        );
        localTris.clearStorage();

        labelList faceZone;
        label nZones = surf.markZones
        (
            boolList(surf.nEdges(), false),
            faceZone
        );

        if (nZones == 1)
        {
            info.setPoint(calcCentre(surf));
            info.setHit();
        }
    }

    return info;
}


// Determine per cell centre whether all the intersections get collapsed
// to a single point
void CML::isoSurface::calcSnappedCc
(
    const labelList& boundaryRegion,
    const volVectorField& meshC,
    const volScalarField& cVals,
    const scalarField& pVals,

    DynamicList<point>& snappedPoints,
    labelList& snappedCc
) const
{
    const pointField& pts = mesh_.points();
    const pointField& cc = mesh_.cellCentres();

    snappedCc.setSize(mesh_.nCells());
    snappedCc = -1;

    // Work arrays
    DynamicList<point, 64> localTriPoints(64);

    forAll(mesh_.cells(), celli)
    {
        if (cellCutType_[celli] == CUT)
        {
            scalar cVal = cVals[celli];

            const cell& cFaces = mesh_.cells()[celli];

            localTriPoints.clear();
            label nOther = 0;
            point otherPointSum = Zero;

            // Create points for all intersections close to cell centre
            // (i.e. from pyramid edges)

            forAll(cFaces, cFacei)
            {
                label facei = cFaces[cFacei];

                scalar nbrValue;
                point nbrPoint;
                getNeighbour
                (
                    boundaryRegion,
                    meshC,
                    cVals,
                    celli,
                    facei,
                    nbrValue,
                    nbrPoint
                );

                // Calculate intersection points of edges to cell centre
                FixedList<scalar, 3> s;
                FixedList<point, 3> pt;

                // From cc to neighbour cc.
                s[2] = isoFraction(cVal, nbrValue);
                pt[2] = (1.0-s[2])*cc[celli] + s[2]*nbrPoint;

                const face& f = mesh_.faces()[cFaces[cFacei]];

                forAll(f, fp)
                {
                    // From cc to fp
                    label p0 = f[fp];
                    s[0] = isoFraction(cVal, pVals[p0]);
                    pt[0] = (1.0-s[0])*cc[celli] + s[0]*pts[p0];

                    // From cc to fp+1
                    label p1 = f[f.fcIndex(fp)];
                    s[1] = isoFraction(cVal, pVals[p1]);
                    pt[1] = (1.0-s[1])*cc[celli] + s[1]*pts[p1];

                    if
                    (
                        (s[0] >= 0.0 && s[0] <= 0.5)
                     && (s[1] >= 0.0 && s[1] <= 0.5)
                     && (s[2] >= 0.0 && s[2] <= 0.5)
                    )
                    {
                        localTriPoints.append(pt[0]);
                        localTriPoints.append(pt[1]);
                        localTriPoints.append(pt[2]);
                    }
                    else
                    {
                        // Get average of all other points
                        forAll(s, i)
                        {
                            if (s[i] >= 0.0 && s[i] <= 0.5)
                            {
                                otherPointSum += pt[i];
                                nOther++;
                            }
                        }
                    }
                }
            }

            if (localTriPoints.size() == 0)
            {
                // No complete triangles. Get average of all intersection
                // points.
                if (nOther > 0)
                {
                    snappedCc[celli] = snappedPoints.size();
                    snappedPoints.append(otherPointSum/nOther);

                    //Pout<< "    point:" << pointi
                    //    << " replacing coord:" << mesh_.points()[pointi]
                    //    << " by average:" << collapsedPoint[pointi] << endl;
                }
            }
            else if (localTriPoints.size() == 3)
            {
                // Single triangle. No need for any analysis. Average points.
                pointField points;
                points.transfer(localTriPoints);
                snappedCc[celli] = snappedPoints.size();
                snappedPoints.append(sum(points)/points.size());

                //Pout<< "    point:" << pointi
                //    << " replacing coord:" << mesh_.points()[pointi]
                //    << " by average:" << collapsedPoint[pointi] << endl;
            }
            else
            {
                // Convert points into triSurface.

                // Merge points and compact out non-valid triangles
                labelList triMap;               // merged to unmerged triangle
                labelList triPointReverseMap;   // unmerged to merged point
                triSurface surf
                (
                    stitchTriPoints
                    (
                        false,              // do not check for duplicate tris
                        localTriPoints,
                        triPointReverseMap,
                        triMap
                    )
                );

                labelList faceZone;
                label nZones = surf.markZones
                (
                    boolList(surf.nEdges(), false),
                    faceZone
                );

                if (nZones == 1)
                {
                    snappedCc[celli] = snappedPoints.size();
                    snappedPoints.append(calcCentre(surf));
                    //Pout<< "    point:" << pointi << " nZones:" << nZones
                    //    << " replacing coord:" << mesh_.points()[pointi]
                    //    << " by average:" << collapsedPoint[pointi] << endl;
                }
            }
        }
    }
}


// Determine per meshpoint whether all the intersections get collapsed
// to a single point
void CML::isoSurface::calcSnappedPoint
(
    const PackedBoolList& isBoundaryPoint,
    const labelList& boundaryRegion,
    const volVectorField& meshC,
    const volScalarField& cVals,
    const scalarField& pVals,

    DynamicList<point>& snappedPoints,
    labelList& snappedPoint
) const
{
    const pointField& pts = mesh_.points();
    const pointField& cc = mesh_.cellCentres();

    pointField collapsedPoint(mesh_.nPoints(), point::max);

    // Work arrays
    DynamicList<point, 64> localTriPoints(100);

    forAll(mesh_.pointFaces(), pointi)
    {
        if (isBoundaryPoint.get(pointi) == 1)
        {
            continue;
        }

        const labelList& pFaces = mesh_.pointFaces()[pointi];

        bool anyCut = false;

        forAll(pFaces, i)
        {
            label facei = pFaces[i];

            if (faceCutType_[facei] == CUT)
            {
                anyCut = true;
                break;
            }
        }

        if (!anyCut)
        {
            continue;
        }


        localTriPoints.clear();
        label nOther = 0;
        point otherPointSum = Zero;

        forAll(pFaces, pFacei)
        {
            // Create points for all intersections close to point
            // (i.e. from pyramid edges)

            label facei = pFaces[pFacei];
            const face& f = mesh_.faces()[facei];
            label own = mesh_.faceOwner()[facei];

            // Get neighbour value
            scalar nbrValue;
            point nbrPoint;
            getNeighbour
            (
                boundaryRegion,
                meshC,
                cVals,
                own,
                facei,
                nbrValue,
                nbrPoint
            );

            // Calculate intersection points of edges emanating from point
            FixedList<scalar, 4> s;
            FixedList<point, 4> pt;

            label fp = findIndex(f, pointi);
            s[0] = isoFraction(pVals[pointi], cVals[own]);
            pt[0] = (1.0-s[0])*pts[pointi] + s[0]*cc[own];

            s[1] = isoFraction(pVals[pointi], nbrValue);
            pt[1] = (1.0-s[1])*pts[pointi] + s[1]*nbrPoint;

            label nextPointi = f[f.fcIndex(fp)];
            s[2] = isoFraction(pVals[pointi], pVals[nextPointi]);
            pt[2] = (1.0-s[2])*pts[pointi] + s[2]*pts[nextPointi];

            label prevPointi = f[f.rcIndex(fp)];
            s[3] = isoFraction(pVals[pointi], pVals[prevPointi]);
            pt[3] = (1.0-s[3])*pts[pointi] + s[3]*pts[prevPointi];

            if
            (
                (s[0] >= 0.0 && s[0] <= 0.5)
             && (s[1] >= 0.0 && s[1] <= 0.5)
             && (s[2] >= 0.0 && s[2] <= 0.5)
            )
            {
                localTriPoints.append(pt[0]);
                localTriPoints.append(pt[1]);
                localTriPoints.append(pt[2]);
            }
            if
            (
                (s[0] >= 0.0 && s[0] <= 0.5)
             && (s[1] >= 0.0 && s[1] <= 0.5)
             && (s[3] >= 0.0 && s[3] <= 0.5)
            )
            {
                localTriPoints.append(pt[3]);
                localTriPoints.append(pt[0]);
                localTriPoints.append(pt[1]);
            }

            // Get average of points as fallback
            forAll(s, i)
            {
                if (s[i] >= 0.0 && s[i] <= 0.5)
                {
                    otherPointSum += pt[i];
                    nOther++;
                }
            }
        }

        if (localTriPoints.size() == 0)
        {
            // No complete triangles. Get average of all intersection
            // points.
            if (nOther > 0)
            {
                collapsedPoint[pointi] = otherPointSum/nOther;
            }
        }
        else if (localTriPoints.size() == 3)
        {
            // Single triangle. No need for any analysis. Average points.
            pointField points;
            points.transfer(localTriPoints);
            collapsedPoint[pointi] = sum(points)/points.size();
        }
        else
        {
            // Convert points into triSurface.

            // Merge points and compact out non-valid triangles
            labelList triMap;               // merged to unmerged triangle
            labelList triPointReverseMap;   // unmerged to merged point
            triSurface surf
            (
                stitchTriPoints
                (
                    false,                  // do not check for duplicate tris
                    localTriPoints,
                    triPointReverseMap,
                    triMap
                )
            );

            labelList faceZone;
            label nZones = surf.markZones
            (
                boolList(surf.nEdges(), false),
                faceZone
            );

            if (nZones == 1)
            {
                collapsedPoint[pointi] = calcCentre(surf);
            }
        }
    }


    // Synchronise snap location
    syncUnseparatedPoints(collapsedPoint, point::max);


    snappedPoint.setSize(mesh_.nPoints());
    snappedPoint = -1;

    forAll(collapsedPoint, pointi)
    {
        if (collapsedPoint[pointi] != point::max)
        {
            snappedPoint[pointi] = snappedPoints.size();
            snappedPoints.append(collapsedPoint[pointi]);
        }
    }
}


CML::triSurface CML::isoSurface::stitchTriPoints
(
    const bool checkDuplicates,
    const List<point>& triPoints,
    labelList& triPointReverseMap,  // unmerged to merged point
    labelList& triMap               // merged to unmerged triangle
) const
{
    label nTris = triPoints.size()/3;

    if ((triPoints.size() % 3) != 0)
    {
        FatalErrorInFunction
            << "Problem: number of points " << triPoints.size()
            << " not a multiple of 3." << abort(FatalError);
    }

    pointField newPoints;
    mergePoints
    (
        triPoints,
        mergeDistance_,
        false,
        triPointReverseMap,
        newPoints
    );

    // Check that enough merged.
    if (debug)
    {
        pointField newNewPoints;
        labelList oldToNew;
        bool hasMerged = mergePoints
        (
            newPoints,
            mergeDistance_,
            true,
            oldToNew,
            newNewPoints
        );

        if (hasMerged)
        {
            FatalErrorInFunction
                << "Merged points contain duplicates"
                << " when merging with distance " << mergeDistance_ << endl
                << "merged:" << newPoints.size() << " re-merged:"
                << newNewPoints.size()
                << abort(FatalError);
        }
    }


    List<labelledTri> tris;
    {
        DynamicList<labelledTri> dynTris(nTris);
        label rawPointi = 0;
        DynamicList<label> newToOldTri(nTris);

        for (label oldTriI = 0; oldTriI < nTris; oldTriI++)
        {
            labelledTri tri
            (
                triPointReverseMap[rawPointi],
                triPointReverseMap[rawPointi+1],
                triPointReverseMap[rawPointi+2],
                0
            );
            rawPointi += 3;

            if ((tri[0] != tri[1]) && (tri[0] != tri[2]) && (tri[1] != tri[2]))
            {
                newToOldTri.append(oldTriI);
                dynTris.append(tri);
            }
        }

        triMap.transfer(newToOldTri);
        tris.transfer(dynTris);
    }



    // Determine 'flat hole' situation (see RMT paper).
    // Two unconnected triangles get connected because (some of) the edges
    // separating them get collapsed. Below only checks for duplicate triangles,
    // not non-manifold edge connectivity.
    if (checkDuplicates)
    {
        labelListList pointFaces;
        invertManyToMany(newPoints.size(), tris, pointFaces);

        // Filter out duplicates.
        DynamicList<label> newToOldTri(tris.size());

        forAll(tris, triI)
        {
            const labelledTri& tri = tris[triI];
            const labelList& pFaces = pointFaces[tri[0]];

            // Find the maximum of any duplicates. Maximum since the tris
            // below triI
            // get overwritten so we cannot use them in a comparison.
            label dupTriI = -1;
            forAll(pFaces, i)
            {
                label nbrTriI = pFaces[i];

                if (nbrTriI > triI && (tris[nbrTriI] == tri))
                {
                    //Pout<< "Duplicate : " << triI << " verts:" << tri
                    //    << " to " << nbrTriI << " verts:" << tris[nbrTriI]
                    //    << endl;
                    dupTriI = nbrTriI;
                    break;
                }
            }

            if (dupTriI == -1)
            {
                // There is no (higher numbered) duplicate triangle
                label newTriI = newToOldTri.size();
                newToOldTri.append(triMap[triI]);
                tris[newTriI] = tris[triI];
            }
        }

        triMap.transfer(newToOldTri);
        tris.setSize(triMap.size());

        if (debug)
        {
            Pout<< "isoSurface : merged from " << nTris
                << " down to " << tris.size() << " unique triangles." << endl;
        }


        if (debug)
        {
            triSurface surf(tris, geometricSurfacePatchList(0), newPoints);

            forAll(surf, facei)
            {
                const labelledTri& f = surf[facei];
                const labelList& fFaces = surf.faceFaces()[facei];

                forAll(fFaces, i)
                {
                    label nbrFacei = fFaces[i];

                    if (nbrFacei <= facei)
                    {
                        // lower numbered faces already checked
                        continue;
                    }

                    const labelledTri& nbrF = surf[nbrFacei];

                    if (f == nbrF)
                    {
                        FatalErrorInFunction
                            << "Check : "
                            << " triangle " << facei << " vertices " << f
                            << " fc:" << f.centre(surf.points())
                            << " has the same vertices as triangle " << nbrFacei
                            << " vertices " << nbrF
                            << " fc:" << nbrF.centre(surf.points())
                            << abort(FatalError);
                    }
                }
            }
        }
    }

    return triSurface(tris, geometricSurfacePatchList(0), newPoints, true);
}


// Does face use valid vertices?
bool CML::isoSurface::validTri(const triSurface& surf, const label faceI)
{
    // Simple check on indices ok.

    const labelledTri& f = surf[faceI];

    if
    (
        (f[0] < 0) || (f[0] >= surf.points().size())
     || (f[1] < 0) || (f[1] >= surf.points().size())
     || (f[2] < 0) || (f[2] >= surf.points().size())
    )
    {
        WarningInFunction
            << "triangle " << faceI << " vertices " << f
            << " uses point indices outside point range 0.."
            << surf.points().size()-1 << endl;

        return false;
    }

    if ((f[0] == f[1]) || (f[0] == f[2]) || (f[1] == f[2]))
    {
        WarningInFunction
            << "triangle " << faceI
            << " uses non-unique vertices " << f
            << endl;
        return false;
    }

    // duplicate triangle check

    const labelList& fFaces = surf.faceFaces()[faceI];

    // Check if faceNeighbours use same points as this face.
    // Note: discards normal information - sides of baffle are merged.
    forAll(fFaces, i)
    {
        label nbrFaceI = fFaces[i];

        if (nbrFaceI <= faceI)
        {
            // lower numbered faces already checked
            continue;
        }

        const labelledTri& nbrF = surf[nbrFaceI];

        if
        (
            ((f[0] == nbrF[0]) || (f[0] == nbrF[1]) || (f[0] == nbrF[2]))
         && ((f[1] == nbrF[0]) || (f[1] == nbrF[1]) || (f[1] == nbrF[2]))
         && ((f[2] == nbrF[0]) || (f[2] == nbrF[1]) || (f[2] == nbrF[2]))
        )
        {
            WarningInFunction
                << "triangle " << faceI << " vertices " << f
                << " fc:" << f.centre(surf.points())
                << " has the same vertices as triangle " << nbrFaceI
                << " vertices " << nbrF
                << " fc:" << nbrF.centre(surf.points())
                << endl;

            return false;
        }
    }
    return true;
}


void CML::isoSurface::calcAddressing
(
    const triSurface& surf,
    List<FixedList<label, 3> >& faceEdges,
    labelList& edgeFace0,
    labelList& edgeFace1,
    Map<labelList>& edgeFacesRest
) const
{
    const pointField& points = surf.points();

    pointField edgeCentres(3*surf.size());
    label edgeI = 0;
    forAll(surf, triI)
    {
        const labelledTri& tri = surf[triI];
        edgeCentres[edgeI++] = 0.5*(points[tri[0]]+points[tri[1]]);
        edgeCentres[edgeI++] = 0.5*(points[tri[1]]+points[tri[2]]);
        edgeCentres[edgeI++] = 0.5*(points[tri[2]]+points[tri[0]]);
    }

    pointField mergedCentres;
    labelList oldToMerged;
    bool hasMerged = mergePoints
    (
        edgeCentres,
        mergeDistance_,
        false,
        oldToMerged,
        mergedCentres
    );

    if (debug)
    {
        Pout<< "isoSurface : detected "
            << mergedCentres.size()
            << " geometric edges on " << surf.size() << " triangles." << endl;
    }

    if (!hasMerged)
    {
        return;
    }


    // Determine faceEdges
    faceEdges.setSize(surf.size());
    edgeI = 0;
    forAll(surf, triI)
    {
        faceEdges[triI][0] = oldToMerged[edgeI++];
        faceEdges[triI][1] = oldToMerged[edgeI++];
        faceEdges[triI][2] = oldToMerged[edgeI++];
    }


    // Determine edgeFaces
    edgeFace0.setSize(mergedCentres.size());
    edgeFace0 = -1;
    edgeFace1.setSize(mergedCentres.size());
    edgeFace1 = -1;
    edgeFacesRest.clear();

    // Overflow edge faces for geometric shared edges that turned
    // out to be different anyway.
    EdgeMap<labelList> extraEdgeFaces(mergedCentres.size()/100);

    forAll(oldToMerged, oldEdgeI)
    {
        label triI = oldEdgeI / 3;
        label edgeI = oldToMerged[oldEdgeI];

        if (edgeFace0[edgeI] == -1)
        {
            // First triangle for edge
            edgeFace0[edgeI] = triI;
        }
        else
        {
            //- Check that the two triangles actually topologically
            //  share an edge
            const labelledTri& prevTri = surf[edgeFace0[edgeI]];
            const labelledTri& tri = surf[triI];

            label fp = oldEdgeI % 3;

            edge e(tri[fp], tri[tri.fcIndex(fp)]);

            label prevTriIndex = -1;

            forAll(prevTri, i)
            {
                if (edge(prevTri[i], prevTri[prevTri.fcIndex(i)]) == e)
                {
                    prevTriIndex = i;
                    break;
                }
            }

            if (prevTriIndex == -1)
            {
                // Different edge. Store for later.
                EdgeMap<labelList>::iterator iter = extraEdgeFaces.find(e);

                if (iter != extraEdgeFaces.end())
                {
                    labelList& eFaces = iter();
                    label sz = eFaces.size();
                    eFaces.setSize(sz+1);
                    eFaces[sz] = triI;
                }
                else
                {
                    extraEdgeFaces.insert(e, labelList(1, triI));
                }
            }
            else if (edgeFace1[edgeI] == -1)
            {
                edgeFace1[edgeI] = triI;
            }
            else
            {
                //WarningInFunction
                //    << "Edge " << edgeI << " with centre "
                //    << mergedCentres[edgeI]
                //    << " used by more than two triangles: "
                //    << edgeFace0[edgeI] << ", "
                //    << edgeFace1[edgeI] << " and " << triI << endl;
                Map<labelList>::iterator iter = edgeFacesRest.find(edgeI);

                if (iter != edgeFacesRest.end())
                {
                    labelList& eFaces = iter();
                    label sz = eFaces.size();
                    eFaces.setSize(sz+1);
                    eFaces[sz] = triI;
                }
                else
                {
                    edgeFacesRest.insert(edgeI, labelList(1, triI));
                }
            }
        }
    }

    // Add extraEdgeFaces
    edgeI = edgeFace0.size();

    edgeFace0.setSize(edgeI + extraEdgeFaces.size());
    edgeFace1.setSize(edgeI + extraEdgeFaces.size(), -1);

    forAllConstIter(EdgeMap<labelList>, extraEdgeFaces, iter)
    {
        const labelList& eFaces = iter();

        // The current edge will become edgeI. Replace all occurrences in
        // faceEdges
        forAll(eFaces, i)
        {
            label triI = eFaces[i];
            const labelledTri& tri = surf[triI];

            FixedList<label, 3>& fEdges = faceEdges[triI];
            forAll(tri, fp)
            {
                edge e(tri[fp], tri[tri.fcIndex(fp)]);

                if (e == iter.key())
                {
                    fEdges[fp] = edgeI;
                    break;
                }
            }
        }


        // Add face to edgeFaces

        edgeFace0[edgeI] = eFaces[0];

        if (eFaces.size() >= 2)
        {
            edgeFace1[edgeI] = eFaces[1];

            if (eFaces.size() > 2)
            {
                edgeFacesRest.insert
                (
                    edgeI,
                    SubList<label>(eFaces, eFaces.size()-2, 2)
                );
            }
        }

        edgeI++;
    }
}


void CML::isoSurface::walkOrientation
(
    const triSurface& surf,
    const List<FixedList<label, 3> >& faceEdges,
    const labelList& edgeFace0,
    const labelList& edgeFace1,
    const label seedTriI,
    labelList& flipState
)
{
    // Do walk for consistent orientation.
    DynamicList<label> changedFaces(surf.size());

    changedFaces.append(seedTriI);

    while (changedFaces.size())
    {
        DynamicList<label> newChangedFaces(changedFaces.size());

        forAll(changedFaces, i)
        {
            label triI = changedFaces[i];
            const labelledTri& tri = surf[triI];
            const FixedList<label, 3>& fEdges = faceEdges[triI];

            forAll(fEdges, fp)
            {
                label edgeI = fEdges[fp];

                // my points:
                label p0 = tri[fp];
                label p1 = tri[tri.fcIndex(fp)];

                label nbrI =
                (
                    edgeFace0[edgeI] != triI
                  ? edgeFace0[edgeI]
                  : edgeFace1[edgeI]
                );

                if (nbrI != -1 && flipState[nbrI] == -1)
                {
                    const labelledTri& nbrTri = surf[nbrI];

                    // nbr points
                    label nbrFp = findIndex(nbrTri, p0);

                    if (nbrFp == -1)
                    {
                        FatalErrorInFunction
                            << "triI:" << triI
                            << " tri:" << tri
                            << " p0:" << p0
                            << " p1:" << p1
                            << " fEdges:" << fEdges
                            << " edgeI:" << edgeI
                            << " edgeFace0:" << edgeFace0[edgeI]
                            << " edgeFace1:" << edgeFace1[edgeI]
                            << " nbrI:" << nbrI
                            << " nbrTri:" << nbrTri
                            << abort(FatalError);
                    }


                    label nbrP1 = nbrTri[nbrTri.rcIndex(nbrFp)];

                    bool sameOrientation = (p1 == nbrP1);

                    if (flipState[triI] == 0)
                    {
                        flipState[nbrI] = (sameOrientation ? 0 : 1);
                    }
                    else
                    {
                        flipState[nbrI] = (sameOrientation ? 1 : 0);
                    }
                    newChangedFaces.append(nbrI);
                }
            }
        }

        changedFaces.transfer(newChangedFaces);
    }
}


void CML::isoSurface::orientSurface
(
    triSurface& surf,
    const List<FixedList<label, 3> >& faceEdges,
    const labelList& edgeFace0,
    const labelList& edgeFace1,
    const Map<labelList>& edgeFacesRest
)
{
    // -1 : unvisited
    //  0 : leave as is
    //  1 : flip
    labelList flipState(surf.size(), -1);

    label seedTriI = 0;

    while (true)
    {
        // Find first unvisited triangle
        for
        (
            ;
            seedTriI < surf.size() && flipState[seedTriI] != -1;
            seedTriI++
        )
        {}

        if (seedTriI == surf.size())
        {
            break;
        }

        // Note: Determine orientation of seedTriI?
        // for now assume it is ok
        flipState[seedTriI] = 0;

        walkOrientation
        (
            surf,
            faceEdges,
            edgeFace0,
            edgeFace1,
            seedTriI,
            flipState
        );
    }

    // Do actual flipping
    surf.clearOut();
    forAll(surf, triI)
    {
        if (flipState[triI] == 1)
        {
            labelledTri tri(surf[triI]);

            surf[triI][0] = tri[0];
            surf[triI][1] = tri[2];
            surf[triI][2] = tri[1];
        }
        else if (flipState[triI] == -1)
        {
            FatalErrorInFunction
                << "problem" << abort(FatalError);
        }
    }
}


// Checks if triangle is connected through edgeI only.
bool CML::isoSurface::danglingTriangle
(
    const FixedList<label, 3>& fEdges,
    const labelList& edgeFace1
)
{
    label nOpen = 0;
    forAll(fEdges, i)
    {
        if (edgeFace1[fEdges[i]] == -1)
        {
            nOpen++;
        }
    }

    if (nOpen == 1 || nOpen == 2 || nOpen == 3)
    {
        return true;
    }
    else
    {
        return false;
    }
}


// Mark triangles to keep. Returns number of dangling triangles.
CML::label CML::isoSurface::markDanglingTriangles
(
    const List<FixedList<label, 3> >& faceEdges,
    const labelList& edgeFace0,
    const labelList& edgeFace1,
    const Map<labelList>& edgeFacesRest,
    boolList& keepTriangles
)
{
    keepTriangles.setSize(faceEdges.size());
    keepTriangles = true;

    label nDangling = 0;

    // Remove any dangling triangles
    forAllConstIter(Map<labelList>,  edgeFacesRest, iter)
    {
        // These are all the non-manifold edges. Filter out all triangles
        // with only one connected edge (= this edge)

        label edgeI = iter.key();
        const labelList& otherEdgeFaces = iter();

        // Remove all dangling triangles
        if (danglingTriangle(faceEdges[edgeFace0[edgeI]], edgeFace1))
        {
            keepTriangles[edgeFace0[edgeI]] = false;
            nDangling++;
        }
        if (danglingTriangle(faceEdges[edgeFace1[edgeI]], edgeFace1))
        {
            keepTriangles[edgeFace1[edgeI]] = false;
            nDangling++;
        }
        forAll(otherEdgeFaces, i)
        {
            label triI = otherEdgeFaces[i];
            if (danglingTriangle(faceEdges[triI], edgeFace1))
            {
                keepTriangles[triI] = false;
                nDangling++;
            }
        }
    }
    return nDangling;
}


CML::triSurface CML::isoSurface::subsetMesh
(
    const triSurface& s,
    const labelList& newToOldFaces,
    labelList& oldToNewPoints,
    labelList& newToOldPoints
)
{
    const boolList include
    (
        createWithValues<boolList>
        (
            s.size(),
            false,
            newToOldFaces,
            true
        )
    );

    newToOldPoints.setSize(s.points().size());
    oldToNewPoints.setSize(s.points().size());
    oldToNewPoints = -1;
    {
        label pointi = 0;

        forAll(include, oldFacei)
        {
            if (include[oldFacei])
            {
                // Renumber labels for triangle
                const labelledTri& tri = s[oldFacei];

                forAll(tri, fp)
                {
                    label oldPointi = tri[fp];

                    if (oldToNewPoints[oldPointi] == -1)
                    {
                        oldToNewPoints[oldPointi] = pointi;
                        newToOldPoints[pointi++] = oldPointi;
                    }
                }
            }
        }
        newToOldPoints.setSize(pointi);
    }

    // Extract points
    pointField newPoints(newToOldPoints.size());
    forAll(newToOldPoints, i)
    {
        newPoints[i] = s.points()[newToOldPoints[i]];
    }
    // Extract faces
    List<labelledTri> newTriangles(newToOldFaces.size());

    forAll(newToOldFaces, i)
    {
        // Get old vertex labels
        const labelledTri& tri = s[newToOldFaces[i]];

        newTriangles[i][0] = oldToNewPoints[tri[0]];
        newTriangles[i][1] = oldToNewPoints[tri[1]];
        newTriangles[i][2] = oldToNewPoints[tri[2]];
        newTriangles[i].region() = tri.region();
    }

    // Reuse storage.
    return triSurface(newTriangles, s.patches(), newPoints, true);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::isoSurface::isoSurface
(
    const volScalarField& cVals,
    const scalarField& pVals,
    const scalar iso,
    const bool regularise,
    const scalar mergeTol
)
:
    mesh_(cVals.mesh()),
    pVals_(pVals),
    iso_(iso),
    regularise_(regularise),
    mergeDistance_(mergeTol*mesh_.bounds().mag())
{
    if (debug)
    {
        Pout<< "isoSurface:" << nl
            << "    isoField      : " << cVals.name() << nl
            << "    cell min/max  : "
            << min(cVals.internalField()) << " / "
            << max(cVals.internalField()) << nl
            << "    point min/max : "
            << min(pVals_) << " / "
            << max(pVals_) << nl
            << "    isoValue      : " << iso << nl
            << "    regularise    : " << regularise_ << nl
            << "    mergeTol      : " << mergeTol << nl
            << endl;
    }

    const polyBoundaryMesh& patches = mesh_.boundaryMesh();


    // Rewrite input field
    // ~~~~~~~~~~~~~~~~~~~
    // Rewrite input volScalarField to have interpolated values
    // on separated patches.

    cValsPtr_.reset(adaptPatchFields(cVals).ptr());


    // Construct cell centres field consistent with cVals
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Generate field to interpolate. This is identical to the mesh.C()
    // except on separated coupled patches and on empty patches.

    slicedVolVectorField meshC
    (
        IOobject
        (
            "C",
            mesh_.pointsInstance(),
            mesh_.meshSubDir,
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        ),
        mesh_,
        dimLength,
        mesh_.cellCentres(),
        mesh_.faceCentres()
    );
    forAll(patches, patchi)
    {
        const polyPatch& pp = patches[patchi];

        // Adapt separated coupled (proc and cyclic) patches
        if (pp.coupled())
        {
            fvPatchVectorField& pfld = const_cast<fvPatchVectorField&>
            (
                meshC.boundaryField()[patchi]
            );

            PackedBoolList isCollocated
            (
                collocatedFaces(refCast<const coupledPolyPatch>(pp))
            );

            forAll(isCollocated, i)
            {
                if (!isCollocated[i])
                {
                    pfld[i] = mesh_.faceCentres()[pp.start()+i];
                }
            }
        }
        else if (isA<emptyPolyPatch>(pp))
        {
            typedef slicedVolVectorField::GeometricBoundaryField bType;

            bType& bfld = const_cast<bType&>(meshC.boundaryField());

            // Clear old value. Cannot resize it since is a slice.
            bfld.set(patchi, nullptr);

            // Set new value we can change
            bfld.set
            (
                patchi,
                new calculatedFvPatchField<vector>
                (
                    mesh_.boundary()[patchi],
                    meshC
                )
            );

            // Change to face centres
            bfld[patchi] = pp.patchSlice(mesh_.faceCentres());
        }
    }



    // Pre-calculate patch-per-face to avoid whichPatch call.
    labelList boundaryRegion(mesh_.nFaces()-mesh_.nInternalFaces());

    forAll(patches, patchi)
    {
        const polyPatch& pp = patches[patchi];

        label facei = pp.start();

        forAll(pp, i)
        {
            boundaryRegion[facei-mesh_.nInternalFaces()] = patchi;
            facei++;
        }
    }



    // Determine if any cut through face/cell
    calcCutTypes(boundaryRegion, meshC, cValsPtr_(), pVals_);


    DynamicList<point> snappedPoints(nCutCells_);

    // Per cc -1 or a point inside snappedPoints.
    labelList snappedCc;
    if (regularise_)
    {
        calcSnappedCc
        (
            boundaryRegion,
            meshC,
            cValsPtr_(),
            pVals_,

            snappedPoints,
            snappedCc
        );
    }
    else
    {
        snappedCc.setSize(mesh_.nCells());
        snappedCc = -1;
    }



    if (debug)
    {
        Pout<< "isoSurface : shifted " << snappedPoints.size()
            << " cell centres to intersection." << endl;
    }

    label nCellSnaps = snappedPoints.size();


    // Per point -1 or a point inside snappedPoints.
    labelList snappedPoint;
    if (regularise_)
    {
        // Determine if point is on boundary.
        PackedBoolList isBoundaryPoint(mesh_.nPoints());

        forAll(patches, patchi)
        {
            // Mark all boundary points that are not physically coupled
            // (so anything but collocated coupled patches)

            if (patches[patchi].coupled())
            {
                const coupledPolyPatch& cpp =
                    refCast<const coupledPolyPatch>
                    (
                        patches[patchi]
                    );

                PackedBoolList isCollocated(collocatedFaces(cpp));

                forAll(isCollocated, i)
                {
                    if (!isCollocated[i])
                    {
                        const face& f = mesh_.faces()[cpp.start()+i];

                        forAll(f, fp)
                        {
                            isBoundaryPoint.set(f[fp], 1);
                        }
                    }
                }
            }
            else
            {
                const polyPatch& pp = patches[patchi];

                forAll(pp, i)
                {
                    const face& f = mesh_.faces()[pp.start()+i];

                    forAll(f, fp)
                    {
                        isBoundaryPoint.set(f[fp], 1);
                    }
                }
            }
        }

        calcSnappedPoint
        (
            isBoundaryPoint,
            boundaryRegion,
            meshC,
            cValsPtr_(),
            pVals_,

            snappedPoints,
            snappedPoint
        );
    }
    else
    {
        snappedPoint.setSize(mesh_.nPoints());
        snappedPoint = -1;
    }

    if (debug)
    {
        Pout<< "isoSurface : shifted " << snappedPoints.size()-nCellSnaps
            << " vertices to intersection." << endl;
    }



    DynamicList<point> triPoints(nCutCells_);
    DynamicList<label> triMeshCells(nCutCells_);

    generateTriPoints
    (
        cValsPtr_(),
        pVals_,

        meshC,
        mesh_.points(),

        snappedPoints,
        snappedCc,
        snappedPoint,

        triPoints,
        triMeshCells
    );

    if (debug)
    {
        Pout<< "isoSurface : generated " << triMeshCells.size()
            << " unmerged triangles from " << triPoints.size()
            << " unmerged points." << endl;
    }


    // Merge points and compact out non-valid triangles
    labelList triMap;           // merged to unmerged triangle
    triSurface::operator=
    (
        stitchTriPoints
        (
            true,               // check for duplicate tris
            triPoints,
            triPointMergeMap_,  // unmerged to merged point
            triMap
        )
    );

    if (debug)
    {
        Pout<< "isoSurface : generated " << triMap.size()
            << " merged triangles." << endl;
    }

    meshCells_.setSize(triMap.size());
    forAll(triMap, i)
    {
        meshCells_[i] = triMeshCells[triMap[i]];
    }

    if (debug)
    {
        Pout<< "isoSurface : checking " << size()
            << " triangles for validity." << endl;

        forAll(*this, triI)
        {
            // Copied from surfaceCheck
            validTri(*this, triI);
        }
    }


    if (false)
    {
        List<FixedList<label, 3> > faceEdges;
        labelList edgeFace0, edgeFace1;
        Map<labelList> edgeFacesRest;


        while (true)
        {
            // Calculate addressing
            calcAddressing
            (
                *this,
                faceEdges,
                edgeFace0,
                edgeFace1,
                edgeFacesRest
            );

            // See if any dangling triangles
            boolList keepTriangles;
            label nDangling = markDanglingTriangles
            (
                faceEdges,
                edgeFace0,
                edgeFace1,
                edgeFacesRest,
                keepTriangles
            );

            if (debug)
            {
                Pout<< "isoSurface : detected " << nDangling
                    << " dangling triangles." << endl;
            }

            if (nDangling == 0)
            {
                break;
            }

            // Create face map (new to old)
            labelList subsetTriMap(findIndices(keepTriangles, true));

            labelList subsetPointMap;
            labelList reversePointMap;
            triSurface::operator=
            (
                subsetMesh
                (
                    *this,
                    subsetTriMap,
                    reversePointMap,
                    subsetPointMap
                )
            );
            meshCells_ = labelField(meshCells_, subsetTriMap);
            inplaceRenumber(reversePointMap, triPointMergeMap_);
        }

        orientSurface(*this, faceEdges, edgeFace0, edgeFace1, edgeFacesRest);
    }


    if (debug)
    {
        fileName stlFile = mesh_.time().path() + ".stl";
        Pout<< "Dumping surface to " << stlFile << endl;
        triSurface::write(stlFile);
    }
}


// ************************************************************************* //
