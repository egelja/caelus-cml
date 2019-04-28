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

#include "searchablePlane.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "SortableList.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(searchablePlane, 0);
addToRunTimeSelectionTable(searchableSurface, searchablePlane, dict);

}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

CML::pointIndexHit CML::searchablePlane::findLine
(
    const point& start,
    const point& end
) const
{
    pointIndexHit info(true, vector::zero, 0);

    linePointRef l(start, end);

    scalar t = lineIntersect(l);

    if (t < 0 || t > 1)
    {
        info.setMiss();
        info.setIndex(-1);
    }
    else
    {
        info.setPoint(start+t*l.vec());
    }

    return info;
}


CML::boundBox CML::searchablePlane::calcBounds() const
{
    point max(VGREAT, VGREAT, VGREAT);

    for (direction dir = 0; dir < vector::nComponents; dir++)
    {
        if (mag(normal()[dir]) - 1 < SMALL)
        {
            max[dir] = 0;

            break;
        }
    }

    point min = -max;

    return boundBox(min, max);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::searchablePlane::searchablePlane
(
    const IOobject& io,
    const point& basePoint,
    const vector& normal
)
:
    searchableSurface(io),
    plane(basePoint, normal)
{
    bounds() = calcBounds();
}


CML::searchablePlane::searchablePlane
(
    const IOobject& io,
    const dictionary& dict
)
:
    searchableSurface(io),
    plane(dict)
{
    bounds() = calcBounds();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::searchablePlane::~searchablePlane()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::wordList& CML::searchablePlane::regions() const
{
    if (regions_.empty())
    {
        regions_.setSize(1);
        regions_[0] = "region0";
    }
    return regions_;
}


void CML::searchablePlane::boundingSpheres
(
    pointField& centres,
    scalarField& radiusSqr
) const
{
    centres.setSize(1);
    centres[0] = refPoint();

    radiusSqr.setSize(1);
    radiusSqr[0] = CML::sqr(GREAT);
}


void CML::searchablePlane::findNearest
(
    const pointField& samples,
    const scalarField& nearestDistSqr,
    List<pointIndexHit>& info
) const
{
    info.setSize(samples.size());

    forAll(samples, i)
    {
        info[i].setPoint(nearestPoint(samples[i]));

        if (magSqr(samples[i]-info[i].rawPoint()) > nearestDistSqr[i])
        {
            info[i].setIndex(-1);
            info[i].setMiss();
        }
        else
        {
            info[i].setIndex(0);
            info[i].setHit();
        }
    }
}


void CML::searchablePlane::findLine
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


void CML::searchablePlane::findLineAny
(
    const pointField& start,
    const pointField& end,
    List<pointIndexHit>& info
) const
{
    findLine(start, end, info);
}


void CML::searchablePlane::findLineAll
(
    const pointField& start,
    const pointField& end,
    List<List<pointIndexHit> >& info
) const
{
    List<pointIndexHit> nearestInfo;
    findLine(start, end, nearestInfo);

    info.setSize(start.size());
    forAll(info, pointI)
    {
        if (nearestInfo[pointI].hit())
        {
            info[pointI].setSize(1);
            info[pointI][0] = nearestInfo[pointI];
        }
        else
        {
            info[pointI].clear();
        }
    }
}


void CML::searchablePlane::getRegion
(
    const List<pointIndexHit>& info,
    labelList& region
) const
{
    region.setSize(info.size());
    region = 0;
}


void CML::searchablePlane::getNormal
(
    const List<pointIndexHit>& info,
    vectorField& n
) const
{
    n.setSize(info.size());
    n = normal();
}


void CML::searchablePlane::getVolumeType
(
    const pointField& points,
    List<volumeType>& volType
) const
{
    FatalErrorInFunction
        << "Volume type not supported for plane."
        << exit(FatalError);
}


// ************************************************************************* //
