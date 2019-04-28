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

#include "mergeRegionsSearchableSurface.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

    defineTypeNameAndDebug(mergeRegionsSearchableSurface, 0);
    addToRunTimeSelectionTable(searchableSurface, mergeRegionsSearchableSurface, dict);

}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::mergeRegionsSearchableSurface::mergeRegionsSearchableSurface
(
    const IOobject& io,
    const dictionary& dict
)
:
    wrapperSearchableSurface(io,dict)
{
    word name(dict.lookup("newName"));
    wordList children(dict.lookup("regions"));
    const wordList &oldNames=delegate().regions();
    const pointField oldCoords(delegate().coordinates());

    label size=oldNames.size()-children.size()+1;

    if(size<1) {
        FatalErrorInFunction
            << "To few regions " << oldNames << " to merge to "
            << children << endl
            << abort(FatalError);
    }

    regions_.setSize(size);
    coordinates_.setSize(size);
    toRegion_.setSize(oldNames.size());

    regions_[0]=name;
    label cnt=1;
    bool hasZeroCoord=false;

    forAll(oldNames,i) {
        const word &n=oldNames[i];
        bool found=false;
        forAll(children,j) {
            if(children[j]==n) {
                found=true;
                break;
            }
        }
        if(found) {
            toRegion_[i]=0;
            if(!hasZeroCoord) {
                coordinates_[0]=oldCoords[i];
                hasZeroCoord=true;
            }
        } else {
            if(cnt>=size) {
                FatalErrorInFunction
                    << "Too few matched regions in " << children
                    << " (old: " << oldNames << ")\n" << endl
                    << abort(FatalError);
            }
            regions_[cnt]=n;
            coordinates_[cnt]=oldCoords[i];
            toRegion_[i]=cnt;
            cnt++;
        }
    }

    if(cnt!=size) {
        FatalErrorInFunction
            << "Mismatch with " << children
            << " (old: " << oldNames << ")\n" << endl
            << abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::mergeRegionsSearchableSurface::~mergeRegionsSearchableSurface()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


const CML::wordList& CML::mergeRegionsSearchableSurface::regions() const
{
    return regions_;
}

CML::tmp<CML::pointField>
CML::mergeRegionsSearchableSurface::coordinates() const
{
    return tmp<pointField>(new pointField(coordinates_));
}


void CML::mergeRegionsSearchableSurface::getRegion
(
    const List<pointIndexHit>& info,
    labelList& region
) const
{
    delegate().getRegion(info,region);

    forAll(region,i) {
        region[i]=toRegion_[region[i]];
    }
}




// ************************************************************************* //
