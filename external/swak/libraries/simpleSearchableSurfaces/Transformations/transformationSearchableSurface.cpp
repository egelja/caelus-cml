/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright held by original author
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
    2009, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "transformationSearchableSurface.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "SortableList.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(transformationSearchableSurface, 0);
    // addToRunTimeSelectionTable(searchableSurface, transformationSearchableSurface, dict);

}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::transformationSearchableSurface::transformationSearchableSurface
(
    const IOobject& io,
    const dictionary& dict
)
:
    searchableSurface(io),
    rename_(
        dict.subDict("surface").found("rename")
        ?
        new IOobject(
            word(dict.subDict("surface")["rename"]),
            static_cast<const searchableSurface&>(*this).instance(),
            io.local(),
            io.db(),
            io.readOpt(),
            io.writeOpt(),
            false
        )
        :
        nullptr
    ),
    delegate_(
        searchableSurface::New
        (
            word(dict.subDict("surface").lookup("type")),
            (rename_.valid() ? rename_() : *this),
            dict.subDict("surface")
        )
    )
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::transformationSearchableSurface::~transformationSearchableSurface()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::pointIndexHit CML::transformationSearchableSurface::transform
(
    const pointIndexHit &orig
) const
{
    return pointIndexHit
        (
            orig.hit(),
            transform(orig.rawPoint()),
            orig.index()
        );
}

CML::pointIndexHit CML::transformationSearchableSurface::inverseTransform
(
    const pointIndexHit &orig
) const
{
    return pointIndexHit
        (
            orig.hit(),
            inverseTransform(orig.rawPoint()),
            orig.index()
        );
}

const CML::wordList& CML::transformationSearchableSurface::regions() const
{
    return delegate().regions();
}

CML::tmp<CML::pointField>
CML::transformationSearchableSurface::coordinates() const
{
    tmp<pointField> tResult(new pointField(delegate().coordinates()));
    pointField &result=tResult();

    forAll(result,i) {
        result[i]=transform(result[i]);
    }

    return tResult;
}

// CML::pointIndexHit CML::transformationSearchableSurface::findNearest
// (
//     const point& sample,
//     const scalar nearestDistSqr
// ) const
// {
//     pointIndexHit result=delegate().findNearest(
//         inverseTransform(sample),
//         nearestDistSqr
//     );

//     return transform(result);
// }


// CML::pointIndexHit CML::transformationSearchableSurface::findNearestOnEdge
// (
//     const point& sample,
//     const scalar nearestDistSqr
// ) const
// {
//     pointIndexHit result=delegate().findNearestOnEdge(
//         inverseTransform(sample),
//         nearestDistSqr
//     );

//     return transform(result);
// }


// CML::pointIndexHit CML::transformationSearchableSurface::findNearest
// (
//     const linePointRef& ln,
//     treeBoundBox& tightest,
//     point& linePoint
// ) const
// {
//     NotImplemented
//     (
//         "transformationSearchableSurface::findNearest"
//         "(const linePointRef&, treeBoundBox&, point&)"
//     );
//     return pointIndexHit();
// }


// CML::pointIndexHit CML::transformationSearchableSurface::findLine
// (
//     const point& start,
//     const point& end
// ) const
// {
//     pointIndexHit result=delegate().findLine(
//         inverseTransform(start),
//         inverseTransform(end)
//     );

//     return transform(result);
// }


// CML::pointIndexHit CML::transformationSearchableSurface::findLineAny
// (
//     const point& start,
//     const point& end
// ) const
// {
//     pointIndexHit result=delegate().findLineAny(
//         inverseTransform(start),
//         inverseTransform(end)
//     );

//     return transform(result);
// }


void CML::transformationSearchableSurface::findNearest
(
    const pointField& samples,
    const scalarField& nearestDistSqr,
    List<pointIndexHit>& info
) const
{
    pointField iSamples(samples.size());
    List<pointIndexHit> iInfo;

    forAll(samples,i) {
        iSamples[i]=inverseTransform(samples[i]);
    }

    delegate().findNearest
        (
            iSamples,
            nearestDistSqr,
            iInfo
        );

    info.setSize(iInfo.size());
    forAll(info,i) {
        info[i]=transform(iInfo[i]);
    }
}


void CML::transformationSearchableSurface::findLine
(
    const pointField& start,
    const pointField& end,
    List<pointIndexHit>& info
) const
{
    pointField iStart(start.size());
    pointField iEnd(end.size());
    List<pointIndexHit> iInfo;

    forAll(start,i) {
        iStart[i]=inverseTransform(start[i]);
        iEnd[i]=inverseTransform(end[i]);
    }

    delegate().findLine
        (
            iStart,
            iEnd,
            iInfo
        );

    info.setSize(iInfo.size());
    forAll(info,i) {
        info[i]=transform(iInfo[i]);
    }
}


void CML::transformationSearchableSurface::findLineAny
(
    const pointField& start,
    const pointField& end,
    List<pointIndexHit>& info
) const
{
    pointField iStart(start.size());
    pointField iEnd(end.size());
    List<pointIndexHit> iInfo;

    forAll(start,i) {
        iStart[i]=inverseTransform(start[i]);
        iEnd[i]=inverseTransform(end[i]);
    }

    delegate().findLineAny
        (
            iStart,
            iEnd,
            iInfo
        );

    info.setSize(iInfo.size());
    forAll(info,i) {
        info[i]=transform(iInfo[i]);
    }
}


void CML::transformationSearchableSurface::findLineAll
(
    const pointField& start,
    const pointField& end,
    List<List<pointIndexHit> >& info
) const
{
    pointField iStart(start.size());
    pointField iEnd(end.size());
    List<List<pointIndexHit> > iInfo;

    forAll(start,i) {
        iStart[i]=inverseTransform(start[i]);
        iEnd[i]=inverseTransform(end[i]);
    }

    delegate().findLineAll
        (
            iStart,
            iEnd,
            iInfo
        );

    info.setSize(iInfo.size());
    forAll(info,i) {
        info[i].setSize(iInfo[i].size());
        forAll(info[i],j) {
            info[i][j]=transform(iInfo[i][j]);
        }
    }
}


void CML::transformationSearchableSurface::getRegion
(
    const List<pointIndexHit>& info,
    labelList& region
) const
{
    List<pointIndexHit> iInfo(info.size());
    forAll(info,i) {
        iInfo[i]=inverseTransform(info[i]);
    }

    delegate().getRegion
        (
            iInfo,
            region
        );
}


void CML::transformationSearchableSurface::getNormal
(
    const List<pointIndexHit>& info,
    vectorField& normal
) const
{
    List<pointIndexHit> iInfo(info.size());
    forAll(info,i) {
        iInfo[i]=inverseTransform(info[i]);
    }

    delegate().getNormal
        (
            iInfo,
            normal
        );
    forAll(normal,i) {
        //        Info << normal[i] << " at " << iInfo[i].rawPoint() << endl;
        //        Info << info[i].rawPoint() << transform(iInfo[i].rawPoint()) << endl;
        vector toPt=iInfo[i].rawPoint()+normal[i];
        normal[i]=transform(toPt)-info[i].rawPoint();
        //        Info << " -> " << normal[i] << " " << toPt << " " << transform(toPt) << endl;
    }
}


void CML::transformationSearchableSurface::getVolumeType
(
    const pointField& points,
    List<volumeType>& volType
) const
{
    pointField iPoints(points.size());
    forAll(points,i) {
        iPoints[i]=inverseTransform(points[i]);
    }

    delegate().getVolumeType
        (
            iPoints,
            volType
        );
}


CML::tmp<CML::pointField> CML::transformationSearchableSurface::points() const
{
    pointField result(delegate().points());

    forAll(result,i) {
        result[i]=transform(result[i]);
    }

    return result;
}


bool CML::transformationSearchableSurface::overlaps(const boundBox& bb) const
{
    NotImplemented;

    return false;
}


// ************************************************************************* //
