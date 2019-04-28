/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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

#include "treeDataPoint.hpp"
#include "treeBoundBox.hpp"
#include "indexedOctree.hpp"
#include "triangleFuncs.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


namespace CML
{
    defineTypeNameAndDebug(treeDataPoint, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::treeDataPoint::treeDataPoint(const pointField& points)
:
    points_(points),
    useSubset_(false)
{}


CML::treeDataPoint::treeDataPoint
(
    const pointField& points,
    const labelList& pointLabels
)
:
    points_(points),
    pointLabels_(pointLabels),
    useSubset_(true)
{}


CML::treeDataPoint::findNearestOp::findNearestOp
(
    const indexedOctree<treeDataPoint>& tree
)
:
    tree_(tree)
{}


CML::treeDataPoint::findIntersectOp::findIntersectOp
(
    const indexedOctree<treeDataPoint>& tree
)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::pointField CML::treeDataPoint::shapePoints() const
{
    if (useSubset_)
    {
        return pointField(points_, pointLabels_);
    }
    else
    {
        return points_;
    }
}


//- Get type (inside,outside,mixed,unknown) of point w.r.t. surface.
//  Only makes sense for closed surfaces.
CML::volumeType CML::treeDataPoint::getVolumeType
(
    const indexedOctree<treeDataPoint>& oc,
    const point& sample
) const
{
    return volumeType::UNKNOWN;
}


// Check if any point on shape is inside cubeBb.
bool CML::treeDataPoint::overlaps
(
    const label index,
    const treeBoundBox& cubeBb
) const
{
    label pointI = (useSubset_ ? pointLabels_[index] : index);
    return cubeBb.contains(points_[pointI]);
}


// Check if any point on shape is inside sphere.
bool CML::treeDataPoint::overlaps
(
    const label index,
    const point& centre,
    const scalar radiusSqr
) const
{
    label pointI = (useSubset_ ? pointLabels_[index] : index);

    if (magSqr(points_[pointI] - centre) <= radiusSqr)
    {
        return true;
    }

    return false;
}


void CML::treeDataPoint::findNearestOp::operator()
(
    const labelUList& indices,
    const point& sample,

    scalar& nearestDistSqr,
    label& minIndex,
    point& nearestPoint
) const
{
    const treeDataPoint& shape = tree_.shapes();

    forAll(indices, i)
    {
        const label index = indices[i];
        label pointI =
        (
            shape.useSubset()
          ? shape.pointLabels()[index]
          : index
        );

        const point& pt = shape.points()[pointI];

        scalar distSqr = magSqr(pt - sample);

        if (distSqr < nearestDistSqr)
        {
            nearestDistSqr = distSqr;
            minIndex = index;
            nearestPoint = pt;
        }
    }
}


void CML::treeDataPoint::findNearestOp::operator()
(
    const labelUList& indices,
    const linePointRef& ln,

    treeBoundBox& tightest,
    label& minIndex,
    point& linePoint,
    point& nearestPoint
) const
{
    const treeDataPoint& shape = tree_.shapes();

    // Best so far
    scalar nearestDistSqr = GREAT;
    if (minIndex >= 0)
    {
        nearestDistSqr = magSqr(linePoint - nearestPoint);
    }

    forAll(indices, i)
    {
        const label index = indices[i];
        label pointI =
        (
            shape.useSubset()
          ? shape.pointLabels()[index]
          : index
        );

        const point& shapePt = shape.points()[pointI];

        if (tightest.contains(shapePt))
        {
            // Nearest point on line
            pointHit pHit = ln.nearestDist(shapePt);
            scalar distSqr = sqr(pHit.distance());

            if (distSqr < nearestDistSqr)
            {
                nearestDistSqr = distSqr;
                minIndex = index;
                linePoint = pHit.rawPoint();
                nearestPoint = shapePt;

                {
                    point& minPt = tightest.min();
                    minPt = min(ln.start(), ln.end());
                    minPt.x() -= pHit.distance();
                    minPt.y() -= pHit.distance();
                    minPt.z() -= pHit.distance();
                }
                {
                    point& maxPt = tightest.max();
                    maxPt = max(ln.start(), ln.end());
                    maxPt.x() += pHit.distance();
                    maxPt.y() += pHit.distance();
                    maxPt.z() += pHit.distance();
                }
            }
        }
    }
}


bool CML::treeDataPoint::findIntersectOp::operator()
(
    const label index,
    const point& start,
    const point& end,
    point& result
) const
{
    NotImplemented;
    return false;
}


// ************************************************************************* //
