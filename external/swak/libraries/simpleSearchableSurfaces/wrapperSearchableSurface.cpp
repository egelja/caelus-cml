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
    2009, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "wrapperSearchableSurface.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "SortableList.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(wrapperSearchableSurface, 0);
    // addToRunTimeSelectionTable(searchableSurface, wrapperSearchableSurface, dict);

}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::wrapperSearchableSurface::wrapperSearchableSurface
(
    const IOobject& io,
    const dictionary& dict
)
:
    searchableSurface(io),
    delegate_(
        searchableSurface::New
        (
            word(dict.subDict("surface").lookup("type")),
            IOobject(
                io.name()+"_wrappedBy_"+word(dict.lookup("type")),
                io.instance(),
                io.db(),
                io.readOpt(),
                io.writeOpt()
            ),
            dict.subDict("surface")
        )
    )
{
    if(debug) {
        Info << "wrapperSearchableSurface::wrapperSearchableSurface" << endl
            << name() << " wraps " << delegate().name() << endl;
    }
    if(regions().size()!=size()) {
        WarningInFunction
            << "Number of regions " << regions().size() << " not equal to size "
            << size() << nl << "Regions: " << regions()
            << endl
            << "in " << name() << " wraps " << delegate().name() << endl;
        //                << exit(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::wrapperSearchableSurface::~wrapperSearchableSurface()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::wordList& CML::wrapperSearchableSurface::regions() const
{
    return delegate().regions();
}


void CML::wrapperSearchableSurface::findNearest
(
    const pointField& samples,
    const scalarField& nearestDistSqr,
    List<pointIndexHit>& info
) const
{
    delegate().findNearest
        (
            samples,
            nearestDistSqr,
            info
        );
}


void CML::wrapperSearchableSurface::findLine
(
    const pointField& start,
    const pointField& end,
    List<pointIndexHit>& info
) const
{
    delegate().findLine
        (
            start,
            end,
            info
        );
}


void CML::wrapperSearchableSurface::findLineAny
(
    const pointField& start,
    const pointField& end,
    List<pointIndexHit>& info
) const
{
    delegate().findLineAny
        (
            start,
            end,
            info
        );
}


void CML::wrapperSearchableSurface::findLineAll
(
    const pointField& start,
    const pointField& end,
    List<List<pointIndexHit> >& info
) const
{
    delegate().findLineAll
        (
            start,
            end,
            info
        );
}


void CML::wrapperSearchableSurface::getRegion
(
    const List<pointIndexHit>& info,
    labelList& region
) const
{
    delegate().getRegion
        (
            info,
            region
        );
}


void CML::wrapperSearchableSurface::getNormal
(
    const List<pointIndexHit>& info,
    vectorField& normal
) const
{
    delegate().getNormal
        (
            info,
            normal
        );
}


void CML::wrapperSearchableSurface::getVolumeType
(
    const pointField& points,
    List<volumeType>& volType
) const
{
    delegate().getVolumeType
        (
            points,
            volType
        );
}

bool CML::wrapperSearchableSurface::overlaps(const boundBox& bb) const
{
    NotImplemented;

    return false;
}

void CML::wrapperSearchableSurface::boundingSpheres
(
    pointField& centres,
    scalarField& radiusSqr
) const
{
    delegate().boundingSpheres(
        centres,
        radiusSqr
    );
}

// ************************************************************************* //
