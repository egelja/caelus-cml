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

#include "CASsurfaceFormatCore.hpp"
#include "clock.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::fileFormats::CASsurfaceFormatCore::writeHeader
(
    Ostream& os,
    const pointField& pointLst,
    const UList<surfZone>& zoneLst
)
{
    // just emit some information until we get a nice IOobject
    IOobject::writeBanner(os)
        << "// CAELUS Surface Format - written "
        << clock::dateTime().c_str() << nl
        << "// ~~~~~~~~~~~~~~~~~~~~~~~" << nl << nl
        << "// surfZones:" << nl;


    // treat a single zone as being unzoned
    if (zoneLst.size() <= 1)
    {
        os  << "0" << token::BEGIN_LIST << token::END_LIST << nl << nl;
    }
    else
    {
        os  << zoneLst.size() << nl << token::BEGIN_LIST << incrIndent << nl;

        forAll(zoneLst, zoneI)
        {
            zoneLst[zoneI].writeDict(os);
        }
        os  << decrIndent << token::END_LIST << nl << nl;
    }

    // Note: write with global point numbering

    IOobject::writeDivider(os)
        << "\n// points:" << nl << pointLst << nl;

    IOobject::writeDivider(os);
}


// ************************************************************************* //
