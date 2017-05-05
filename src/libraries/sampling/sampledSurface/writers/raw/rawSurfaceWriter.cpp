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

#include "rawSurfaceWriter.hpp"

#include "OFstream.hpp"
#include "OSspecific.hpp"
#include "IOmanip.hpp"

#include "makeSurfaceWriterMethods.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    makeSurfaceWriterType(rawSurfaceWriter);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

inline void CML::rawSurfaceWriter::writeLocation
(
    Ostream& os,
    const pointField& points,
    const label pointI
)
{
    const point& pt = points[pointI];
    os  << pt.x() << ' ' << pt.y() << ' ' << pt.z() << ' ';
}


inline void CML::rawSurfaceWriter::writeLocation
(
    Ostream& os,
    const pointField& points,
    const faceList& faces,
    const label faceI
)
{
    const point& ct = faces[faceI].centre(points);
    os  << ct.x() << ' ' << ct.y() << ' ' << ct.z() << ' ';
}


namespace CML
{
    template<>
    void CML::rawSurfaceWriter::writeHeader
    (
        Ostream& os,
        const word& fieldName,
        const Field<scalar>& values
    )
    {
        os  << values.size() << nl
            << "#  x  y  z  " << fieldName << nl;
    }


    template<>
    void CML::rawSurfaceWriter::writeHeader
    (
        Ostream& os,
        const word& fieldName,
        const Field<vector>& values
    )
    {
        os  << values.size() << nl
            << "#  x  y  z  "
            << fieldName << "_x  "
            << fieldName << "_y  "
            << fieldName << "_z  "
            << endl;
    }


    template<>
    void CML::rawSurfaceWriter::writeHeader
    (
        Ostream& os,
        const word& fieldName,
        const Field<sphericalTensor>& values
    )
    {
        os  << values.size() << nl
            << "#  ii  "
            << fieldName << "_ii" << nl;
    }


    template<>
    void CML::rawSurfaceWriter::writeHeader
    (
        Ostream& os,
        const word& fieldName,
        const Field<symmTensor>& values
    )
    {
        os  << values.size() << nl
            << "#  xx  xy  xz  yy  yz ";
        for (int i=0; i<6; ++i)
        {
            os  << fieldName << "_" << i << "  ";
        }
        os  << endl;
    }


    template<>
    void CML::rawSurfaceWriter::writeHeader
    (
        Ostream& os,
        const word& fieldName,
        const Field<tensor>& values
    )
    {
        os  << values.size() << nl
            << "#  xx  xy  xz  yx  yy  yz  zx  zy  zz";
        for (int i=0; i<9; ++i)
        {
            os  << fieldName << "_" << i << "  ";
        }
        os  << nl;
    }


    template<>
    inline void CML::rawSurfaceWriter::writeData
    (
        Ostream& os,
        const scalar& v
    )
    {
        os  << v << nl;
    }


    template<>
    inline void CML::rawSurfaceWriter::writeData
    (
        Ostream& os,
        const vector& v
    )
    {
        os  << v[0] << ' ' << v[1] << ' ' << v[2] << nl;
    }


    template<>
    inline void CML::rawSurfaceWriter::writeData
    (
        Ostream& os,
        const sphericalTensor& v
    )
    {
        os  << v[0] << nl;
    }


    template<>
    inline void CML::rawSurfaceWriter::writeData
    (
        Ostream& os,
        const symmTensor& v
    )
    {
        os  << v[0] << ' ' << v[1] << ' ' << v[2] << ' '
            << v[3] << ' ' << v[4] << ' ' << v[5] << nl;
    }


    template<>
    inline void CML::rawSurfaceWriter::writeData
    (
        Ostream& os,
        const tensor& v
    )
    {
        os  << v[0] << ' ' << v[1] << ' ' << v[2] << ' '
            << v[3] << ' ' << v[4] << ' ' << v[5] << ' '
            << v[6] << ' ' << v[7] << ' ' << v[8] << nl;
    }

}


template<class Type>
void CML::rawSurfaceWriter::writeTemplate
(
    const fileName& outputDir,
    const fileName& surfaceName,
    const pointField& points,
    const faceList& faces,
    const word& fieldName,
    const Field<Type>& values,
    const bool isNodeValues,
    const bool verbose
) const
{
    if (!isDir(outputDir))
    {
        mkDir(outputDir);
    }

    OFstream os(outputDir/fieldName + '_' + surfaceName + ".raw");

    if (verbose)
    {
        Info<< "Writing field " << fieldName << " to " << os.name() << endl;
    }

    // header
    os  << "# " << fieldName;
    if (isNodeValues)
    {
        os  << "  POINT_DATA ";
    }
    else
    {
        os  << "  FACE_DATA ";
    }

    // header
    writeHeader(os, fieldName, values);

    // values
    if (isNodeValues)
    {
        forAll(values, elemI)
        {
            writeLocation(os, points, elemI);
            writeData(os, values[elemI]);
        }
    }
    else
    {
        forAll(values, elemI)
        {
            writeLocation(os, points, faces, elemI);
            writeData(os, values[elemI]);
        }
    }
}



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::rawSurfaceWriter::rawSurfaceWriter()
:
    surfaceWriter()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::rawSurfaceWriter::~rawSurfaceWriter()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::rawSurfaceWriter::write
(
    const fileName& outputDir,
    const fileName& surfaceName,
    const pointField& points,
    const faceList& faces,
    const bool verbose
) const
{
    if (!isDir(outputDir))
    {
        mkDir(outputDir);
    }

    OFstream os(outputDir/surfaceName + ".raw");

    if (verbose)
    {
        Info<< "Writing geometry to " << os.name() << endl;
    }


    // header
    os  << "# geometry NO_DATA " << faces.size() << nl
        << "#  x  y  z" << nl;

    // Write faces centres
    forAll(faces, elemI)
    {
        writeLocation(os, points, faces, elemI);
        os  << nl;
    }

    os  << nl;
}


// create write methods
defineSurfaceWriterWriteFields(CML::rawSurfaceWriter);


// ************************************************************************* //
