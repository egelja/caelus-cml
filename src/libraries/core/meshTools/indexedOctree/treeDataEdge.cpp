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

#include "treeDataEdge.hpp"
#include "indexedOctree.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(CML::treeDataEdge, 0);


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

CML::treeBoundBox CML::treeDataEdge::calcBb(const label edgeI) const
{
    const edge& e = edges_[edgeI];
    const point& p0 = points_[e[0]];
    const point& p1 = points_[e[1]];

    return treeBoundBox(min(p0, p1), max(p0, p1));
}


void CML::treeDataEdge::update()
{
    if (cacheBb_)
    {
        bbs_.setSize(edgeLabels_.size());

        forAll(edgeLabels_, i)
        {
            bbs_[i] = calcBb(edgeLabels_[i]);
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::treeDataEdge::treeDataEdge
(
    const bool cacheBb,
    const edgeList& edges,
    const pointField& points,
    const labelUList& edgeLabels
)
:
    edges_(edges),
    points_(points),
    edgeLabels_(edgeLabels),
    cacheBb_(cacheBb)
{
    update();
}


CML::treeDataEdge::treeDataEdge
(
    const bool cacheBb,
    const edgeList& edges,
    const pointField& points,
    const Xfer<labelList>& edgeLabels
)
:
    edges_(edges),
    points_(points),
    edgeLabels_(edgeLabels),
    cacheBb_(cacheBb)
{
    update();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::pointField CML::treeDataEdge::shapePoints() const
{
    pointField eMids(edgeLabels_.size());

    forAll(edgeLabels_, i)
    {
        const edge& e = edges_[edgeLabels_[i]];

        eMids[i] = e.centre(points_);
    }
    return eMids;
}


//- Get type (inside,outside,mixed,unknown) of point w.r.t. surface.
//  Only makes sense for closed surfaces.
CML::label CML::treeDataEdge::getVolumeType
(
    const indexedOctree<treeDataEdge>& oc,
    const point& sample
) const
{
    return indexedOctree<treeDataEdge>::UNKNOWN;
}


// Check if any point on shape is inside cubeBb.
bool CML::treeDataEdge::overlaps
(
    const label index,
    const treeBoundBox& cubeBb
) const
{
    if (cacheBb_)
    {
        return cubeBb.overlaps(bbs_[index]);
    }
    else
    {
        return cubeBb.overlaps(calcBb(edgeLabels_[index]));
    }
}


// Calculate nearest point to sample. Updates (if any) nearestDistSqr, minIndex,
// nearestPoint.
void CML::treeDataEdge::findNearest
(
    const labelUList& indices,
    const point& sample,

    scalar& nearestDistSqr,
    label& minIndex,
    point& nearestPoint
) const
{
    forAll(indices, i)
    {
        const label index = indices[i];

        const edge& e = edges_[edgeLabels_[index]];

        pointHit nearHit = e.line(points_).nearestDist(sample);

        scalar distSqr = sqr(nearHit.distance());

        if (distSqr < nearestDistSqr)
        {
            nearestDistSqr = distSqr;
            minIndex = index;
            nearestPoint = nearHit.rawPoint();
        }
    }
}


//- Calculates nearest (to line) point in shape.
//  Returns point and distance (squared)
void CML::treeDataEdge::findNearest
(
    const labelUList& indices,
    const linePointRef& ln,

    treeBoundBox& tightest,
    label& minIndex,
    point& linePoint,
    point& nearestPoint
) const
{
    // Best so far
    scalar nearestDistSqr = magSqr(linePoint - nearestPoint);

    forAll(indices, i)
    {
        const label index = indices[i];

        const edge& e = edges_[edgeLabels_[index]];

        // Note: could do bb test ? Worthwhile?

        // Nearest point on line
        point ePoint, lnPt;
        scalar dist = e.line(points_).nearestDist(ln, ePoint, lnPt);
        scalar distSqr = sqr(dist);

        if (distSqr < nearestDistSqr)
        {
            nearestDistSqr = distSqr;
            minIndex = index;
            linePoint = lnPt;
            nearestPoint = ePoint;

            {
                point& minPt = tightest.min();
                minPt = min(ln.start(), ln.end());
                minPt.x() -= dist;
                minPt.y() -= dist;
                minPt.z() -= dist;
            }
            {
                point& maxPt = tightest.max();
                maxPt = max(ln.start(), ln.end());
                maxPt.x() += dist;
                maxPt.y() += dist;
                maxPt.z() += dist;
            }
        }
    }
}


// ************************************************************************* //
