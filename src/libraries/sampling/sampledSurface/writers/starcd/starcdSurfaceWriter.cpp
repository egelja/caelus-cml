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

#include "starcdSurfaceWriter.hpp"

#include "MeshedSurfaceProxy.hpp"
#include "OFstream.hpp"
#include "OSspecific.hpp"

#include "makeSurfaceWriterMethods.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    makeSurfaceWriterType(starcdSurfaceWriter);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

namespace CML
{
    template<>
    inline void CML::starcdSurfaceWriter::writeData
    (
        Ostream& os,
        const scalar& v
    )
    {
        os  << v << nl;
    }


    template<>
    inline void CML::starcdSurfaceWriter::writeData
    (
        Ostream& os,
        const vector& v
    )
    {
        os  << v[0] << ' ' << v[1] << ' ' << v[2] << nl;
    }


    template<>
    inline void CML::starcdSurfaceWriter::writeData
    (
        Ostream& os,
        const sphericalTensor& v
    )
    {
        os  << v[0] << nl;
    }

}


template<class Type>
inline void CML::starcdSurfaceWriter::writeData
(
    Ostream& os,
    const Type& v
)
{}


template<class Type>
void CML::starcdSurfaceWriter::writeTemplate
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

    OFstream os(outputDir/fieldName + '_' + surfaceName + ".usr");

    if (verbose)
    {
        Info<< "Writing field " << fieldName << " to " << os.name() << endl;
    }

    // no header, just write values
    forAll(values, elemI)
    {
        os  << elemI+1 << ' ';
        writeData(os, values[elemI]);
    }
}



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::starcdSurfaceWriter::starcdSurfaceWriter()
:
    surfaceWriter()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::starcdSurfaceWriter::~starcdSurfaceWriter()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::starcdSurfaceWriter::write
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

    fileName outName(outputDir/surfaceName + ".inp");

    if (verbose)
    {
        Info<< "Writing geometry to " << outName << endl;
    }

    MeshedSurfaceProxy<face>(points, faces).write(outName);
}


// create write methods
defineSurfaceWriterWriteField(CML::starcdSurfaceWriter, scalar);
defineSurfaceWriterWriteField(CML::starcdSurfaceWriter, vector);
defineSurfaceWriterWriteField(CML::starcdSurfaceWriter, sphericalTensor);


// ************************************************************************* //
