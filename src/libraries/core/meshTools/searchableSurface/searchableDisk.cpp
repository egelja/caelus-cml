/*---------------------------------------------------------------------------*\
Copyright (C) 2014-2015 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "searchableDisk.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(searchableDisk, 0);
addToRunTimeSelectionTable(searchableSurface, searchableDisk, dict);

}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

CML::pointIndexHit CML::searchableDisk::findNearest
(
    const point& sample,
    const scalar nearestDistSqr
) const
{
    pointIndexHit info(false, sample, -1);

    vector v(sample - origin_);

    // Decompose sample-origin into normal and parallel component
    scalar parallel = (v & normal_);

    // Remove the parallel component and normalise
    v -= parallel*normal_;
    scalar magV = mag(v);

    if (magV < ROOTVSMALL)
    {
        v = vector::zero;
    }
    else
    {
        v /= magV;
    }

    // Clip to radius.
    info.setPoint(origin_ + min(magV, radius_)*v);

    if (magSqr(sample - info.rawPoint()) < nearestDistSqr)
    {
        info.setHit();
        info.setIndex(0);
    }

    return info;
}


void CML::searchableDisk::findLine
(
    const point& start,
    const point& end,
    pointIndexHit& info
) const
{
    info = pointIndexHit(false, vector::zero, -1);

    vector v(start - origin_);

    // Decompose sample-origin into normal and parallel component
    scalar parallel = (v & normal_);

    if (sign(parallel) == sign((end - origin_) & normal_))
    {
        return;
    }

    // Remove the parallel component and normalise
    v -= parallel*normal_;
    scalar magV = mag(v);

    if (magV < ROOTVSMALL)
    {
        v = vector::zero;
    }
    else
    {
        v /= magV;
    }

    // Set (hit or miss) to intersection of ray and plane of disk
    info.setPoint(origin_ + magV*v);

    if (magV <= radius_)
    {
        info.setHit();
        info.setIndex(0);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::searchableDisk::searchableDisk
(
    const IOobject& io,
    const point& origin,
    const point& normal,
    const scalar radius
)
:
    searchableSurface(io),
    origin_(origin),
    normal_(normal/mag(normal)),
    radius_(radius)
{
    // Rough approximation of bounding box
    //vector span(radius_, radius_, radius_);

    // See searchableCylinder
    vector span
    (
        sqrt(sqr(normal_.y()) + sqr(normal_.z())),
        sqrt(sqr(normal_.x()) + sqr(normal_.z())),
        sqrt(sqr(normal_.x()) + sqr(normal_.y()))
    );
    span *= radius_;

    bounds().min() = origin_ - span;
    bounds().max() = origin_ + span;
}


CML::searchableDisk::searchableDisk
(
    const IOobject& io,
    const dictionary& dict
)
:
    searchableSurface(io),
    origin_(dict.lookup("origin")),
    normal_(dict.lookup("normal")),
    radius_(readScalar(dict.lookup("radius")))
{
    normal_ /= mag(normal_);

    // Rough approximation of bounding box
    //vector span(radius_, radius_, radius_);

    // See searchableCylinder
    vector span
    (
        sqrt(sqr(normal_.y()) + sqr(normal_.z())),
        sqrt(sqr(normal_.x()) + sqr(normal_.z())),
        sqrt(sqr(normal_.x()) + sqr(normal_.y()))
    );
    span *= radius_;

    bounds().min() = origin_ - span;
    bounds().max() = origin_ + span;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::searchableDisk::~searchableDisk()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::wordList& CML::searchableDisk::regions() const
{
    if (regions_.empty())
    {
        regions_.setSize(1);
        regions_[0] = "region0";
    }
    return regions_;
}


void CML::searchableDisk::boundingSpheres
(
    pointField& centres,
    scalarField& radiusSqr
) const
{
    centres.setSize(1);
    centres[0] = origin_;

    radiusSqr.setSize(1);
    radiusSqr[0] = sqr(radius_);

    // Add a bit to make sure all points are tested inside
    radiusSqr += CML::sqr(SMALL);
}


void CML::searchableDisk::findNearest
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


void CML::searchableDisk::findLine
(
    const pointField& start,
    const pointField& end,
    List<pointIndexHit>& info
) const
{
    info.setSize(start.size());

    forAll(start, i)
    {
        findLine(start[i], end[i], info[i]);
    }
}


void CML::searchableDisk::findLineAny
(
    const pointField& start,
    const pointField& end,
    List<pointIndexHit>& info
) const
{
    findLine(start, end, info);
}


void CML::searchableDisk::findLineAll
(
    const pointField& start,
    const pointField& end,
    List<List<pointIndexHit> >& info
) const
{
    info.setSize(start.size());

    forAll(start, i)
    {
        pointIndexHit inter;
        findLine(start[i], end[i], inter);

        if (inter.hit())
        {
            info[i].setSize(1);
            info[i][0] = inter;
        }
        else
        {
            info[i].clear();
        }
    }
}


void CML::searchableDisk::getRegion
(
    const List<pointIndexHit>& info,
    labelList& region
) const
{
    region.setSize(info.size());
    region = 0;
}


void CML::searchableDisk::getNormal
(
    const List<pointIndexHit>& info,
    vectorField& normal
) const
{
    normal.setSize(info.size());
    normal = normal_;
}


void CML::searchableDisk::getVolumeType
(
    const pointField& points,
    List<volumeType>& volType
) const
{
    FatalErrorInFunction
        << "Volume type not supported for disk."
        << exit(FatalError);
}


// ************************************************************************* //
