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

#include "renameRegionsSearchableSurface.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "Pair.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

    defineTypeNameAndDebug(renameRegionsSearchableSurface, 0);
    addToRunTimeSelectionTable(searchableSurface, renameRegionsSearchableSurface, dict);

}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::renameRegionsSearchableSurface::renameRegionsSearchableSurface
(
    const IOobject& io,
    const dictionary& dict
)
:
    wrapperSearchableSurface(io,dict)
{
    regions_=delegate().regions();
    List<Pair<word> > replace(dict.lookup("replacementPairs"));

    if(debug) {
        Info << "renameRegionsSearchableSurface::renameRegionsSearchableSurface"
            << endl;
        Info << "Old regions: " << regions_ << endl;
        Info << "Replacements: " << replace << endl;
    }
    forAll(replace,i) {
        if(replace[i].second()=="_") {
            replace[i].second()="";
        }
        forAll(regions_,j) {
            regions_[j].replaceAll
                (
                    replace[i].first(),
                    replace[i].second()
                );
        }
    }
    if(debug) {
        Info << "New regions: " << regions_ << endl;
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::renameRegionsSearchableSurface::~renameRegionsSearchableSurface()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


const CML::wordList& CML::renameRegionsSearchableSurface::regions() const
{
    return regions_;
}



// ************************************************************************* //
