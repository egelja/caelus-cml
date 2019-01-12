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

#include "STARCDCore.hpp"
#include "ListOps.hpp"
#include "clock.hpp"
#include "PackedBoolList.hpp"
#include "IStringStream.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::fileFormats::STARCDCore::STARCDCore()
{}


// * * * * * * * * * * *  Protected Member Functions * * * * * * * * * * * * //

bool CML::fileFormats::STARCDCore::readHeader
(
    IFstream& is,
    const word& signature
)
{
    if (!is.good())
    {
        FatalErrorInFunction
            << "cannot read " << signature  << "  " << is.name()
            << abort(FatalError);
    }

    word header;
    label majorVersion;

    string line;

    is.getLine(line);
    IStringStream(line)() >> header;

    is.getLine(line);
    IStringStream(line)() >> majorVersion;

    // add other checks ...
    if (header != signature)
    {
        Info<< "header mismatch " << signature << "  " << is.name()
            << endl;
    }

    return true;
}


void CML::fileFormats::STARCDCore::writeHeader
(
    Ostream& os,
    const word& filetype
)
{
    os  << "PROSTAR_" << filetype << nl
        << 4000
        << " " << 0
        << " " << 0
        << " " << 0
        << " " << 0
        << " " << 0
        << " " << 0
        << " " << 0
        << endl;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::fileFormats::STARCDCore::readPoints
(
    IFstream& is,
    pointField& points,
    labelList& ids
)
{
    if (!is.good())
    {
        FatalErrorInFunction
            << "Cannot read file " << is.name()
            << exit(FatalError);
    }

    readHeader(is, "PROSTAR_VERTEX");


    // reuse memory if possible
    DynamicList<point> dynPoints(points.xfer());
    DynamicList<label> dynPointId(ids.xfer());    // STAR-CD index of points

    dynPoints.clear();
    dynPointId.clear();

    label lineLabel;
    while ((is >> lineLabel).good())
    {
        scalar x, y, z;

        is >> x >> y >> z;

        dynPoints.append(point(x, y, z));
        dynPointId.append(lineLabel);
    }

    points.transfer(dynPoints);
    ids.transfer(dynPointId);

    return true;
}


void CML::fileFormats::STARCDCore::writePoints
(
    Ostream& os,
    const pointField& pointLst
)
{
    writeHeader(os, "VERTEX");

    // Set the precision of the points data to 10
    os.precision(10);

    // force decimal point for Fortran input
    os.setf(std::ios::showpoint);

    forAll(pointLst, ptI)
    {
        os
            << ptI + 1 << " "
            << pointLst[ptI].x() << " "
            << pointLst[ptI].y() << " "
            << pointLst[ptI].z() << nl;
    }
    os.flush();
}




// ************************************************************************* //
