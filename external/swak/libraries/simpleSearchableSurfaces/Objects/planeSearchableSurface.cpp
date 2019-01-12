/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2008 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

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

Contributors/Copyright:
    2009, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "planeSearchableSurface.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(planeSearchableSurface, 0);
addToRunTimeSelectionTable(searchableSurface, planeSearchableSurface, dict);

}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

CML::pointIndexHit CML::planeSearchableSurface::findNearest
(
    const point& sample,
    const scalar nearestDistSqr
) const
{
    pointIndexHit info(false, sample, -1);

    point pt=plane_.nearestPoint(sample);

    if (nearestDistSqr > magSqr(pt-sample))
    {
        info.rawPoint() = pt;
        info.setHit();
        info.setIndex(0);
    }

    return info;
}


void CML::planeSearchableSurface::findLineAll
(
    const point& start,
    const point& end,
    pointIndexHit& hit
) const
{
    hit.setMiss();

    vector dir=end-start;

    scalar f=plane_.normalIntersect(start,dir);
    if(0<=f && f<=1) {
        hit.rawPoint()=start+f*dir;
        hit.setHit();
        hit.setIndex(0);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::planeSearchableSurface::planeSearchableSurface
(
    const IOobject& io,
    const dictionary& dict
)
:
    searchableSurface(io),
    plane_(dict.subDict("plane"))
{
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::planeSearchableSurface::~planeSearchableSurface()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::wordList& CML::planeSearchableSurface::regions() const
{
    if (regions_.size() == 0)
    {
        regions_.setSize(1);
        regions_[0] = "region0";
    }
    return regions_;
}



void CML::planeSearchableSurface::findNearest
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


void CML::planeSearchableSurface::findLine
(
    const pointField& start,
    const pointField& end,
    List<pointIndexHit>& info
) const
{
    info.setSize(start.size());

    forAll(start, i)
    {
        findLineAll(start[i], end[i], info[i]);
    }
}


void CML::planeSearchableSurface::findLineAny
(
    const pointField& start,
    const pointField& end,
    List<pointIndexHit>& info
) const
{
    info.setSize(start.size());

    forAll(start, i)
    {
        findLineAll(start[i], end[i], info[i]);
    }
}


void CML::planeSearchableSurface::findLineAll
(
    const pointField& start,
    const pointField& end,
    List<List<pointIndexHit> >& info
) const
{
    info.setSize(start.size());

    forAll(start, i)
    {
        info[i].setSize(1);
        findLineAll(start[i], end[i], info[i][0]);
    }
}


void CML::planeSearchableSurface::getRegion
(
    const List<pointIndexHit>& info,
    labelList& region
) const
{
    region.setSize(info.size());
    region = 0;
}


void CML::planeSearchableSurface::getNormal
(
    const List<pointIndexHit>& info,
    vectorField& normal
) const
{
    normal.setSize(info.size());
    normal = plane_.normal();
}


void CML::planeSearchableSurface::getVolumeType
(
    const pointField& points,
    List<volumeType>& volType
) const
{
    volType.setSize(points.size());
    volType = INSIDE;

    forAll(points, pointI)
    {
        const point& pt = points[pointI];

        if ( ((pt - plane_.refPoint()) & plane_.normal()) < 0 )
        {
            volType[pointI] = INSIDE;
        }
        else
        {
            volType[pointI] = OUTSIDE;
        }
    }
}

CML::tmp<CML::pointField>
CML::planeSearchableSurface::coordinates() const
{
    return tmp<pointField>(new pointField(1,plane_.refPoint()));
}

CML::tmp<CML::pointField> CML::planeSearchableSurface::points() const
{
    return coordinates();
}

bool CML::planeSearchableSurface::overlaps(const boundBox& bb) const
{
    NotImplemented;

    return false;
}

void CML::planeSearchableSurface::boundingSpheres
(
    pointField& centres,
    scalarField& radiusSqr
) const
{
    centres.setSize(1);
    centres[0] = plane_.refPoint();

    radiusSqr.setSize(1);
    radiusSqr[0] = CML::sqr(GREAT);
}

// ************************************************************************* //
