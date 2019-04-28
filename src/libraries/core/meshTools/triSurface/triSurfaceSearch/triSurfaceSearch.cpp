/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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

#include "triSurfaceSearch.hpp"
#include "triSurface.hpp"
#include "PatchTools.hpp"
#include "volumeType.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

bool CML::triSurfaceSearch::checkUniqueHit
(
    const pointIndexHit& currHit,
    const DynamicList<pointIndexHit, 1, 1>& hits,
    const vector& lineVec
) const
{
    // Classify the hit
    label nearType = -1;
    label nearLabel = -1;

    const labelledTri& f = surface()[currHit.index()];

    f.nearestPointClassify
    (
        currHit.hitPoint(),
        surface().points(),
        nearType,
        nearLabel
    );

    if (nearType == triPointRef::POINT)
    {
        // near point

        const label nearPointi = f[nearLabel];

        const labelList& pointFaces =
            surface().pointFaces()[surface().meshPointMap()[nearPointi]];

        forAll(pointFaces, pI)
        {
            const label pointFacei = pointFaces[pI];

            if (pointFacei != currHit.index())
            {
                forAll(hits, hi)
                {
                    const pointIndexHit& hit = hits[hi];

                    if (hit.index() == pointFacei)
                    {
                        return false;
                    }
                }
            }
        }
    }
    else if (nearType == triPointRef::EDGE)
    {
        // near edge
        // check if the other face of the edge is already hit

        const labelList& fEdges = surface().faceEdges()[currHit.index()];

        const label edgeI = fEdges[nearLabel];

        const labelList& edgeFaces = surface().edgeFaces()[edgeI];

        forAll(edgeFaces, fi)
        {
            const label edgeFacei = edgeFaces[fi];

            if (edgeFacei != currHit.index())
            {
                forAll(hits, hi)
                {
                    const pointIndexHit& hit = hits[hi];

                    if (hit.index() == edgeFacei)
                    {
                        // Check normals
                        const vector currHitNormal =
                            surface().faceNormals()[currHit.index()];

                        const vector existingHitNormal =
                            surface().faceNormals()[edgeFacei];

                        const label signCurrHit =
                            pos(currHitNormal & lineVec);

                        const label signExistingHit =
                            pos(existingHitNormal & lineVec);

                        if (signCurrHit == signExistingHit)
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::triSurfaceSearch::triSurfaceSearch(const triSurface& surface)
:
    surface_(surface),
    tolerance_(indexedOctree<treeDataTriSurface>::perturbTol()),
    maxTreeDepth_(10),
    treePtr_(nullptr)
{}


CML::triSurfaceSearch::triSurfaceSearch
(
    const triSurface& surface,
    const dictionary& dict
)
:
    surface_(surface),
    tolerance_(indexedOctree<treeDataTriSurface>::perturbTol()),
    maxTreeDepth_(10),
    treePtr_(nullptr)
{
    // Have optional non-standard search tolerance for gappy surfaces.
    if (dict.readIfPresent("tolerance", tolerance_) && tolerance_ > 0)
    {
        Info<< "    using intersection tolerance " << tolerance_ << endl;
    }

    // Have optional non-standard tree-depth to limit storage.
    if (dict.readIfPresent("maxTreeDepth", maxTreeDepth_) && maxTreeDepth_ > 0)
    {
        Info<< "    using maximum tree depth " << maxTreeDepth_ << endl;
    }
}


CML::triSurfaceSearch::triSurfaceSearch
(
    const triSurface& surface,
    const scalar tolerance,
    const label maxTreeDepth
)
:
    surface_(surface),
    tolerance_(tolerance),
    maxTreeDepth_(maxTreeDepth),
    treePtr_(nullptr)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::triSurfaceSearch::~triSurfaceSearch()
{
    clearOut();
}


void CML::triSurfaceSearch::clearOut()
{
    treePtr_.clear();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::indexedOctree<CML::treeDataTriSurface>&
CML::triSurfaceSearch::tree() const
{
    if (treePtr_.empty())
    {
        // Calculate bb without constructing local point numbering.
        treeBoundBox bb(vector::zero, vector::zero);

        if (surface().size())
        {
            label nPoints;
            PatchTools::calcBounds(surface(), bb, nPoints);

            if (nPoints != surface().points().size())
            {
                WarningInFunction
                    << "Surface does not have compact point numbering."
                    << " Of " << surface().points().size()
                    << " only " << nPoints
                    << " are used. This might give problems in some routines."
                    << endl;
            }

            // Slightly extended bb. Slightly off-centred just so on symmetric
            // geometry there are less face/edge aligned items.
            bb = bb.extend(1e-4);
        }

        scalar oldTol = indexedOctree<treeDataTriSurface>::perturbTol();
        indexedOctree<treeDataTriSurface>::perturbTol() = tolerance_;

        treePtr_.reset
        (
            new indexedOctree<treeDataTriSurface>
            (
                treeDataTriSurface(true, surface_, tolerance_),
                bb,
                maxTreeDepth_,  // maxLevel
                10,             // leafsize
                3.0             // duplicity
            )
        );

        indexedOctree<treeDataTriSurface>::perturbTol() = oldTol;
    }

    return treePtr_();
}


// Determine inside/outside for samples
CML::boolList CML::triSurfaceSearch::calcInside
(
    const pointField& samples
) const
{
    boolList inside(samples.size());

    forAll(samples, sampleI)
    {
        const point& sample = samples[sampleI];

        if (!tree().bb().contains(sample))
        {
            inside[sampleI] = false;
        }
        else if (tree().getVolumeType(sample) == volumeType::INSIDE)
        {
            inside[sampleI] = true;
        }
        else
        {
            inside[sampleI] = false;
        }
    }
    return inside;
}


void CML::triSurfaceSearch::findNearest
(
    const pointField& samples,
    const scalarField& nearestDistSqr,
    List<pointIndexHit>& info
) const
{
    scalar oldTol = indexedOctree<treeDataTriSurface>::perturbTol();
    indexedOctree<treeDataTriSurface>::perturbTol() = tolerance();

    const indexedOctree<treeDataTriSurface>& octree = tree();

    info.setSize(samples.size());

    forAll(samples, i)
    {
        static_cast<pointIndexHit&>(info[i]) = octree.findNearest
        (
            samples[i],
            nearestDistSqr[i],
            treeDataTriSurface::findNearestOp(octree)
        );
    }

    indexedOctree<treeDataTriSurface>::perturbTol() = oldTol;
}


CML::pointIndexHit CML::triSurfaceSearch::nearest
(
    const point& pt,
    const vector& span
)
const
{
    const scalar nearestDistSqr = 0.25*magSqr(span);

    return tree().findNearest(pt, nearestDistSqr);
}


void CML::triSurfaceSearch::findLine
(
    const pointField& start,
    const pointField& end,
    List<pointIndexHit>& info
) const
{
    const indexedOctree<treeDataTriSurface>& octree = tree();

    info.setSize(start.size());

    scalar oldTol = indexedOctree<treeDataTriSurface>::perturbTol();
    indexedOctree<treeDataTriSurface>::perturbTol() = tolerance();

    forAll(start, i)
    {
        static_cast<pointIndexHit&>(info[i]) = octree.findLine
        (
            start[i],
            end[i]
        );
    }

    indexedOctree<treeDataTriSurface>::perturbTol() = oldTol;
}


void CML::triSurfaceSearch::findLineAny
(
    const pointField& start,
    const pointField& end,
    List<pointIndexHit>& info
) const
{
    const indexedOctree<treeDataTriSurface>& octree = tree();

    info.setSize(start.size());

    scalar oldTol = indexedOctree<treeDataTriSurface>::perturbTol();
    indexedOctree<treeDataTriSurface>::perturbTol() = tolerance();

    forAll(start, i)
    {
        static_cast<pointIndexHit&>(info[i]) = octree.findLineAny
        (
            start[i],
            end[i]
        );
    }

    indexedOctree<treeDataTriSurface>::perturbTol() = oldTol;
}


void CML::triSurfaceSearch::findLineAll
(
    const pointField& start,
    const pointField& end,
    List<List<pointIndexHit> >& info
) const
{
    const indexedOctree<treeDataTriSurface>& octree = tree();

    info.setSize(start.size());

    scalar oldTol = indexedOctree<treeDataTriSurface>::perturbTol();
    indexedOctree<treeDataTriSurface>::perturbTol() = tolerance();

    // Work array
    DynamicList<pointIndexHit, 1, 1> hits;

    DynamicList<label> shapeMask;

    treeDataTriSurface::findAllIntersectOp allIntersectOp(octree, shapeMask);

    forAll(start, i)
    {
        hits.clear();
        shapeMask.clear();

        while (true)
        {
            // See if any intersection between pt and end
            pointIndexHit inter = octree.findLine
            (
                start[i],
                end[i],
                allIntersectOp
            );

            if (inter.hit())
            {
                vector lineVec = end[i] - start[i];
                lineVec /= mag(lineVec) + VSMALL;

                if(checkUniqueHit(inter, hits, lineVec))
                {
                    hits.append(inter);
                }

                shapeMask.append(inter.index());
            }
            else
            {
                break;
            }
        }

        info[i].transfer(hits);
    }

    indexedOctree<treeDataTriSurface>::perturbTol() = oldTol;
}


// ************************************************************************* //
