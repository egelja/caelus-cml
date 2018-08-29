/*---------------------------------------------------------------------------*\
Copyright (C) 2014 OpenFOAM Foundation
Copyright (C) 2015 OpenCFD Ltd.
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

#include "searchableRotatedBox.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "axesRotation.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(searchableRotatedBox, 0);
    addToRunTimeSelectionTable(searchableSurface, searchableRotatedBox, dict);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::searchableRotatedBox::searchableRotatedBox
(
    const IOobject& io,
    const dictionary& dict
)
:
    searchableSurface(io),
    box_
    (
        IOobject
        (
            io.name() + "_box",
            io.instance(),
            io.local(),
            io.db(),
            io.readOpt(),
            io.writeOpt(),
            false      //io.registerObject(),
        ),
        treeBoundBox(dict.lookup("min"), dict.lookup("max"))
    ),
    transform_
    (
        "rotation",
        dict.lookup("origin"),
        dict.lookup("e3"),
        dict.lookup("e1")
    )
{
    points_ = transform_.globalPosition(box_.points());
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::searchableRotatedBox::~searchableRotatedBox()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::wordList& CML::searchableRotatedBox::regions() const
{
    return box_.regions();
}


CML::tmp<CML::pointField> CML::searchableRotatedBox::coordinates() const
{
    return transform_.globalPosition(box_.coordinates());
}


void CML::searchableRotatedBox::boundingSpheres
(
    pointField& centres,
    scalarField& radiusSqr
) const
{
    box_.boundingSpheres(centres, radiusSqr);
    centres = transform_.globalPosition(centres);
}


CML::tmp<CML::pointField> CML::searchableRotatedBox::points() const
{
    return points_;
}


bool CML::searchableRotatedBox::overlaps(const boundBox& bb) const
{
    // (from treeDataPrimitivePatch.cpp)

    // 1. bounding box
    if (!bb.overlaps(bounds()))
    {
        return false;
    }

    // 2. Check if one or more face points inside
    const faceList& fcs = treeBoundBox::faces;
    forAll(fcs, faceI)
    {
        if (bb.containsAny(points_))
        {
            return true;
        }
    }

    // 3. Difficult case: all points are outside but connecting edges might
    // go through cube.

    const treeBoundBox treeBb(bb);

    // 3a. my edges through bb faces
    const edgeList& edges = treeBoundBox::edges;
    forAll(edges, edgeI)
    {
        const edge& e = edges[edgeI];

        point inter;
        if (treeBb.intersects(points_[e[0]], points_[e[1]], inter))
        {
            return true;
        }
    }

    // 3b. bb edges through my faces

    const pointField bbPoints(bb.points());

    forAll(fcs, faceI)
    {
        const face& f = fcs[faceI];
        point fc = f.centre(points_);

        forAll(edges, edgeI)
        {
            const edge& e = edges[edgeI];

            pointHit inter = f.intersection
            (
                bbPoints[e[0]],
                bbPoints[e[1]],
                fc,
                points_,
                intersection::HALF_RAY
            );

            if (inter.hit() && inter.distance() <= 1)
            {
                return true;
            }
        }
    }

    return false;
}


CML::pointIndexHit CML::searchableRotatedBox::findNearest
(
    const point& sample,
    const scalar nearestDistSqr
) const
{
    pointIndexHit boxNearest
    (
        box_.findNearest
        (
            transform_.localPosition(sample),
            nearestDistSqr
        )
    );

    boxNearest.rawPoint() = transform_.globalPosition(boxNearest.rawPoint());

    return boxNearest;
}


CML::pointIndexHit CML::searchableRotatedBox::findNearest
(
    const linePointRef& ln,
    treeBoundBox& tightest,
    point& linePoint
) const
{
    notImplemented
    (
        "searchableBox::findNearest"
        "(const linePointRef&, treeBoundBox&, point&)"
    );
    return pointIndexHit();
}


CML::pointIndexHit CML::searchableRotatedBox::findLine
(
    const point& start,
    const point& end
) const
{
    pointIndexHit boxHit
    (
        box_.findLine
        (
            transform_.localPosition(start),
            transform_.localPosition(end)
        )
    );

    boxHit.rawPoint() = transform_.globalPosition(boxHit.rawPoint());

    return boxHit;
}


CML::pointIndexHit CML::searchableRotatedBox::findLineAny
(
    const point& start,
    const point& end
) const
{
    return findLine(start, end);
}


void CML::searchableRotatedBox::findNearest
(
    const pointField& samples,
    const scalarField& nearestDistSqr,
    List<pointIndexHit>& info
) const
{
    info.setSize(samples.size());

    forAll(samples, i)
    {
        info[i] = findNearest(samples[i], nearestDistSqr[i]);
    }
}


void CML::searchableRotatedBox::findLine
(
    const pointField& start,
    const pointField& end,
    List<pointIndexHit>& info
) const
{
    info.setSize(start.size());

    forAll(start, i)
    {
        info[i] = findLine(start[i], end[i]);
    }
}


void CML::searchableRotatedBox::findLineAny
(
    const pointField& start,
    const pointField& end,
    List<pointIndexHit>& info
) const
{
    info.setSize(start.size());

    forAll(start, i)
    {
        info[i] = findLineAny(start[i], end[i]);
    }
}


void CML::searchableRotatedBox::findLineAll
(
    const pointField& start,
    const pointField& end,
    List<List<pointIndexHit> >& info
) const
{
    info.setSize(start.size());

    // Work array
    DynamicList<pointIndexHit, 1, 1> hits;

    // Tolerances:
    // To find all intersections we add a small vector to the last intersection
    // This is chosen such that
    // - it is significant (SMALL is smallest representative relative tolerance;
    //   we need something bigger since we're doing calculations)
    // - if the start-end vector is zero we still progress
    const vectorField dirVec(end-start);
    const scalarField magSqrDirVec(magSqr(dirVec));
    const vectorField smallVec
    (
        CML::sqrt(SMALL)*dirVec
      + vector(ROOTVSMALL,ROOTVSMALL,ROOTVSMALL)
    );

    forAll(start, pointI)
    {
        // See if any intersection between pt and end
        pointIndexHit inter = findLine(start[pointI], end[pointI]);

        if (inter.hit())
        {
            hits.clear();
            hits.append(inter);

            point pt = inter.hitPoint() + smallVec[pointI];

            while (((pt-start[pointI])&dirVec[pointI]) <= magSqrDirVec[pointI])
            {
                // See if any intersection between pt and end
                pointIndexHit inter = findLine(pt, end[pointI]);

                // Check for not hit or hit same face as before (can happen
                // if vector along surface of face)
                if
                (
                    !inter.hit()
                 || (inter.index() == hits.last().index())
                )
                {
                    break;
                }
                hits.append(inter);

                pt = inter.hitPoint() + smallVec[pointI];
            }

            info[pointI].transfer(hits);
        }
        else
        {
            info[pointI].clear();
        }
    }
}


void CML::searchableRotatedBox::getRegion
(
    const List<pointIndexHit>& info,
    labelList& region
) const
{
    region.setSize(info.size());
    region = 0;
}


void CML::searchableRotatedBox::getNormal
(
    const List<pointIndexHit>& info,
    vectorField& normal
) const
{
    // searchableBox does not use hitPoints so no need to transform
    box_.getNormal(info, normal);

    normal = transform_.globalVector(normal);
}


void CML::searchableRotatedBox::getVolumeType
(
    const pointField& points,
    List<volumeType>& volType
) const
{
    box_.getVolumeType(transform_.localPosition(points), volType);
}


// ************************************************************************* //
