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

#include "exclusiveSearchableSurface.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "transform.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(exclusiveSearchableSurface, 0);
addToRunTimeSelectionTable(searchableSurface, exclusiveSearchableSurface, dict);

}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::exclusiveSearchableSurface::exclusiveSearchableSurface
(
    const IOobject& io,
    const dictionary& dict
)
:
    binaryOperationSearchableSurface(io,dict)
{
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::exclusiveSearchableSurface::~exclusiveSearchableSurface()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::exclusiveSearchableSurface::decidePoint(
    const hitWhom who,
    const bool inA,
    const bool inB
) const
{
    if(
        who!=BOTH
    ) {
        return true;
    }
    return false;
}

void CML::exclusiveSearchableSurface::getVolumeType
(
    const pointField& points,
    List<volumeType>& volType
) const
{
    List<volumeType> inA;
    List<volumeType> inB;

    a().getVolumeType(points,inA);
    b().getVolumeType(points,inB);

    volType.setSize(points.size());

    forAll(volType,i) {
        if
            (
                ( inA[i]==INSIDE && inB[i]==OUTSIDE )
                ||
                ( inA[i]==OUTSIDE && inB[i]==INSIDE )
            ) {
            volType[i]=INSIDE;
        } else if( inA[i]==UNKNOWN || inB[i]==UNKNOWN) {
            volType[i]=UNKNOWN;
        } else {
            volType[i]=OUTSIDE;
        }
    }
}

bool CML::exclusiveSearchableSurface::revertNormalA(const pointIndexHit& h) const
{
    pointField pt(1,h.rawPoint());
    List<volumeType> inside;

    b().getVolumeType(pt,inside);

    return inside[0]==INSIDE;
}

bool CML::exclusiveSearchableSurface::revertNormalB(const pointIndexHit& h) const
{
    pointField pt(1,h.rawPoint());
    List<volumeType> inside;

    a().getVolumeType(pt,inside);

    return inside[0]==INSIDE;
}

// ************************************************************************* //
