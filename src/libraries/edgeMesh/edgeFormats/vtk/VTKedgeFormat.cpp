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

#include "VTKedgeFormat.hpp"
#include "OFstream.hpp"
#include "clock.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::fileFormats::VTKedgeFormat::writeHeader
(
    Ostream& os,
    const pointField& pointLst
)
{
    // Write header
    os  << "# vtk DataFile Version 2.0" << nl
        << "featureEdgeMesh written " << clock::dateTime().c_str() << nl
        << "ASCII" << nl
        << nl
        << "DATASET POLYDATA" << nl;

    // Write vertex coords
    os  << "POINTS " << pointLst.size() << " float" << nl;
    forAll(pointLst, ptI)
    {
        const point& pt = pointLst[ptI];

        os  << pt.x() << ' ' << pt.y() << ' ' << pt.z() << nl;
    }
}


void CML::fileFormats::VTKedgeFormat::writeEdges
(
    Ostream& os,
    const UList<edge>& edgeLst
)
{
    os  << "LINES " << edgeLst.size() << ' ' << 3*edgeLst.size() << nl;

    forAll(edgeLst, edgeI)
    {
        const edge& e = edgeLst[edgeI];

        os  << "2 " << e[0] << ' ' << e[1] << nl;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::fileFormats::VTKedgeFormat::VTKedgeFormat()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::fileFormats::VTKedgeFormat::write
(
    const fileName& filename,
    const edgeMesh& eMesh
)
{
    OFstream os(filename);
    if (!os.good())
    {
        FatalErrorInFunction
            << "Cannot open file for writing " << filename
            << exit(FatalError);
    }

    writeHeader(os, eMesh.points());
    writeEdges(os, eMesh.edges());
}


// ************************************************************************* //
