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

#include "unitCylinderSearchableSurface.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(unitCylinderSearchableSurface, 0);
addToRunTimeSelectionTable(searchableSurface, unitCylinderSearchableSurface, dict);

}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

CML::pointIndexHit CML::unitCylinderSearchableSurface::findNearest
(
    const point& sample,
    const scalar nearestDistSqr
) const
{
    pointIndexHit info(false, sample, -1);

    List<point> pts(3);
    List<bool>  hit(3,false);

    point &topPt=pts[0];
    topPt=top_.nearestPoint(sample);
    scalar topRadius=sqrt(topPt.x()*topPt.x()+topPt.y()*topPt.y());
    if(topRadius>1) {
        topPt.x()/=topRadius;
        topPt.y()/=topRadius;
    } else {
        hit[0]=false;
    }
    point &bottomPt=pts[1];
    bottomPt=bottom_.nearestPoint(sample);
    scalar bottomRadius=sqrt(bottomPt.x()*bottomPt.x()+bottomPt.y()*bottomPt.y());
    if(bottomRadius>1) {
        bottomPt.x()/=bottomRadius;
        bottomPt.y()/=bottomRadius;
    } else {
        hit[1]=false;
    }

    scalar r=max(
        sqrt(sample.x()*sample.x()+sample.y()*sample.y()),
        SMALL
    );

    point &hullPoint=pts[2];
    hullPoint=point(sample.x()/r,sample.y()/r,sample.z());
    if(hullPoint.z()>1) {
        hullPoint.z()=1;
    } else if(hullPoint.z()<-1) {
        hullPoint.z()=-1;
    } else {
        hit[2]=true;
    }

    scalar minDist=HUGE_VAL;
    label minIndex=-1;
    forAll(pts,i) {
        if(mag(sample-pts[i])<minDist) {
            minDist=mag(sample-pts[i]);
            minIndex=i;
        }
    }

    if (nearestDistSqr > minDist)
    {
        info.rawPoint() = pts[minIndex];
        info.setHit();
        info.setIndex(minIndex);
    }

    return info;
}


void CML::unitCylinderSearchableSurface::findLineAll
(
    const point& start,
    const point& end,
    List<pointIndexHit>& hit
) const
{
    List<scalar> f(4,HUGE_VAL);

    vector dir=end-start;

    f[0]=max(min(top_.normalIntersect(start,dir),HUGE_VAL),-HUGE_VAL);
    point pt0=start+f[0]*dir;
    pt0.z()=0;
    if(magSqr(pt0)>1) {
        f[0]=HUGE_VAL;
    }

    f[1]=max(min(bottom_.normalIntersect(start,dir),HUGE_VAL),-HUGE_VAL);
    point pt1=start+f[1]*dir;
    pt1.z()=0;
    if(magSqr(pt1)>1) {
        f[1]=HUGE_VAL;
    }

    {
        scalar a=min(dir.x()*dir.x()+dir.y()*dir.y(),SMALL);
        scalar b=2*(start.x()*dir.x()+start.y()*dir.y());
        scalar c=start.x()*start.x()+start.y()*start.y()-1;
        scalar D=b*b-4*a*c;
        if(D>SMALL) {
            f[2]=(-b-sqrt(D))/(2*a);
            f[3]=(-b+sqrt(D))/(2*a);
        } else if(D>0) {
            f[2]=(-b)/(2*a);
        }
        scalar z2=start.z()+f[2]*dir.z();
        if(z2<-1 || z2>1) {
            f[2]=HUGE_VAL;
        }
        scalar z3=start.z()+f[3]*dir.z();
        if(z3<-1 || z3>1) {
            f[3]=HUGE_VAL;
        }
    }

    label nr=0;
    forAll(f,i) {
        if(0<=f[i] && f[i]<=1) {
            nr++;
        }
    }

    hit.setSize(nr);
    label cnt=0;
    forAll(f,i) {
        if(0<=f[i] && f[i]<=1) {
            hit[cnt].rawPoint()=start+f[i]*dir;
            hit[cnt].setHit();
            hit[cnt].setIndex( i<3 ? i : 2 );
            cnt++;
        }
    }

    if(cnt!=nr) {
        FatalErrorInFunction
            << "Expected number of hits " << nr << " does not match the actual "
            << cnt << endl
            << abort(FatalError);
    }

    for(label i=0;i<nr-1;i++) {
        for(label j=i+1;j<nr;j++) {
            if(mag(start-hit[i].rawPoint())>mag(start-hit[j].rawPoint())) {
                pointIndexHit tmp=hit[i];
                hit[i]=hit[j];
                hit[j]=tmp;
            }
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::unitCylinderSearchableSurface::unitCylinderSearchableSurface
(
    const IOobject& io,
    const dictionary& dict
)
:
    searchableSurface(io),
    top_(point(0,0,1),vector(0,0,1)),
    bottom_(point(0,0,-1),vector(0,0,-1))
{
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::unitCylinderSearchableSurface::~unitCylinderSearchableSurface()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::wordList& CML::unitCylinderSearchableSurface::regions() const
{
    if (regions_.size() == 0)
    {
        regions_.setSize(3);
        regions_[0] = "top";
        regions_[1] = "bottom";
        regions_[2] = "hull";
    }
    return regions_;
}

CML::tmp<CML::pointField>
CML::unitCylinderSearchableSurface::coordinates() const
{
    pointField result(3);

    result[0]=top_.refPoint();
    result[1]=bottom_.refPoint();
    result[2]=0.5*(top_.refPoint()+bottom_.refPoint());

    return tmp<pointField>(new pointField(result));
}

bool CML::unitCylinderSearchableSurface::overlaps(const boundBox& bb) const
{
    NotImplemented;

    return false;
}


CML::tmp<CML::pointField> CML::unitCylinderSearchableSurface::points() const
{
    pointField result(2);

    result[0]=top_.refPoint();
    result[1]=bottom_.refPoint();

    return result;
}

void CML::unitCylinderSearchableSurface::findNearest
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


void CML::unitCylinderSearchableSurface::findLine
(
    const pointField& start,
    const pointField& end,
    List<pointIndexHit>& info
) const
{
    info.setSize(start.size());

    forAll(start, i)
    {
        List<pointIndexHit> hits;

        findLineAll(start[i], end[i], hits);
        if(hits.size()>0) {
            info[i]=hits[0];
        }
    }
}


void CML::unitCylinderSearchableSurface::findLineAny
(
    const pointField& start,
    const pointField& end,
    List<pointIndexHit>& info
) const
{
    info.setSize(start.size());

    forAll(start, i)
    {
        List<pointIndexHit> hits;

        findLineAll(start[i], end[i], hits);
        if(hits.size()>0) {
            info[i]=hits[0];
        }
    }
}


void CML::unitCylinderSearchableSurface::findLineAll
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
        findLineAll(start[i], end[i], info[i]);
    }
}


void CML::unitCylinderSearchableSurface::getRegion
(
    const List<pointIndexHit>& info,
    labelList& region
) const
{
    region.setSize(info.size());
    forAll(info,i) {
        region[i] = info[i].index();
    }
}


void CML::unitCylinderSearchableSurface::getNormal
(
    const List<pointIndexHit>& info,
    vectorField& normal
) const
{
    normal.setSize(info.size());

    forAll(info,i) {
        switch(info[i].index()) {
            case 0:
                normal[i]=vector(0,0,1);
                break;
            case 1:
                normal[i]=vector(0,0,-1);
                break;
            default:
                vector tmp=info[i].rawPoint();
                tmp.z()=0;
                normal[i]=tmp/max(mag(tmp),SMALL);
        }
    }
}


void CML::unitCylinderSearchableSurface::getVolumeType
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

        if(
            pt.z()>1
            ||
            pt.z()<-1
            ||
            (pt.x()*pt.x() + pt.y()*pt.y()) > 1
        )
        {
            volType[pointI] = OUTSIDE;
        }
        else
        {
            volType[pointI] = INSIDE;
        }
    }
}

void CML::unitCylinderSearchableSurface::boundingSpheres
(
    pointField& centres,
    scalarField& radiusSqr
) const
{
    centres.setSize(1);
    centres[0] = 0.5*(top_.refPoint()+bottom_.refPoint());

    radiusSqr.setSize(1);
    radiusSqr[0] = 1*1 + 1*1 + SMALL;
}


// ************************************************************************* //
