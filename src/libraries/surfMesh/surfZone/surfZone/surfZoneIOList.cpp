/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "surfZoneIOList.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


namespace CML
{
    defineTypeNameAndDebug(surfZoneIOList, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::surfZoneIOList::surfZoneIOList
(
    const IOobject& io
)
:
    surfZoneList(),
    regIOobject(io)
{
    CML::string functionName =
        "surfZoneIOList::surfZoneIOList"
        "(const IOobject& io)";


    if
    (
        readOpt() == IOobject::MUST_READ
     || readOpt() == IOobject::MUST_READ_IF_MODIFIED
    )
    {
        surfZoneList& zones = *this;

        Istream& is = readStream(typeName);

        PtrList<entry> dictEntries(is);
        zones.setSize(dictEntries.size());

        label faceI = 0;
        forAll(zones, zoneI)
        {
            const dictionary& dict = dictEntries[zoneI].dict();

            label zoneSize = readLabel(dict.lookup("nFaces"));
            label startFaceI = readLabel(dict.lookup("startFace"));

            zones[zoneI] = surfZone
            (
                dictEntries[zoneI].keyword(),
                zoneSize,
                startFaceI,
                zoneI
            );

            word geoType;
            if (dict.readIfPresent("geometricType", geoType))
            {
                zones[zoneI].geometricType() = geoType;
            }

            if (startFaceI != faceI)
            {
                FatalErrorInFunction
                    << "surfZones are not ordered. Start of zone " << zoneI
                    << " does not correspond to sum of preceding zones." << nl
                    << "while reading " << io.objectPath() << endl
                    << exit(FatalError);
            }

            faceI += zoneSize;
        }

        // Check state of IOstream
        is.check(functionName.c_str());

        close();
    }
}


CML::surfZoneIOList::surfZoneIOList
(
    const IOobject& io,
    const surfZoneList& zones
)
:
    surfZoneList(zones),
    regIOobject(io)
{}


CML::surfZoneIOList::surfZoneIOList
(
    const IOobject& io,
    const Xfer<surfZoneList>& zones
)
:
    surfZoneList(zones),
    regIOobject(io)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::surfZoneIOList::~surfZoneIOList()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


// writeData member function required by regIOobject
bool CML::surfZoneIOList::writeData(Ostream& os) const
{
    os  << *this;
    return os.good();
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const surfZoneIOList& L)
{
    os  << L.size() << nl << token::BEGIN_LIST << incrIndent << nl;

    forAll(L, i)
    {
        L[i].writeDict(os);
    }

    os  << decrIndent << token::END_LIST;

    return os;
}


// ************************************************************************* //
