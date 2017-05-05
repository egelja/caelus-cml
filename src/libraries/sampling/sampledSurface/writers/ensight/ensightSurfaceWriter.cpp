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

#include "ensightSurfaceWriter.hpp"

#include "OFstream.hpp"
#include "OSspecific.hpp"
#include "IOmanip.hpp"
#include "ensightPartFaces.hpp"

#include "makeSurfaceWriterMethods.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    makeSurfaceWriterType(ensightSurfaceWriter);
    addToRunTimeSelectionTable(surfaceWriter, ensightSurfaceWriter, wordDict);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
void CML::ensightSurfaceWriter::writeTemplate
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
    if (!isDir(outputDir/fieldName))
    {
        mkDir(outputDir/fieldName);
    }

    // const scalar timeValue = CML::name(this->mesh().time().timeValue());
    const scalar timeValue = 0.0;

    OFstream osCase(outputDir/fieldName/surfaceName + ".case");
    ensightGeoFile osGeom
    (
        outputDir/fieldName/surfaceName + ".000.mesh",
        writeFormat_
    );
    ensightFile osField
    (
        outputDir/fieldName/surfaceName + ".000." + fieldName,
        writeFormat_
    );

    if (verbose)
    {
        Info<< "Writing case file to " << osCase.name() << endl;
    }

    osCase
        << "FORMAT" << nl
        << "type: ensight gold" << nl
        << nl
        << "GEOMETRY" << nl
        << "model:        1     " << osGeom.name().name() << nl
        << nl
        << "VARIABLE" << nl
        << pTraits<Type>::typeName << " per "
        << word(isNodeValues ? "node:" : "element:") << setw(10) << 1
        << "       " << fieldName
        << "       " << surfaceName.c_str() << ".***." << fieldName << nl
        << nl
        << "TIME" << nl
        << "time set:                      1" << nl
        << "number of steps:               1" << nl
        << "filename start number:         0" << nl
        << "filename increment:            1" << nl
        << "time values:" << nl
        << timeValue << nl
        << nl;

    ensightPartFaces ensPart(0, osGeom.name().name(), points, faces, true);
    osGeom << ensPart;

    // Write field
    osField.writeKeyword(pTraits<Type>::typeName);
    ensPart.writeField(osField, values, isNodeValues);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::ensightSurfaceWriter::ensightSurfaceWriter()
:
    surfaceWriter(),
    writeFormat_(IOstream::ASCII)
{}


CML::ensightSurfaceWriter::ensightSurfaceWriter(const dictionary& options)
:
    surfaceWriter(),
    writeFormat_(IOstream::ASCII)
{
    // choose ascii or binary format
    if (options.found("format"))
    {
        writeFormat_ = IOstream::formatEnum(options.lookup("format"));
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::ensightSurfaceWriter::~ensightSurfaceWriter()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::ensightSurfaceWriter::write
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

    // const scalar timeValue = CML::name(this->mesh().time().timeValue());
    const scalar timeValue = 0.0;

    OFstream osCase(outputDir/surfaceName + ".case");
    ensightGeoFile osGeom
    (
        outputDir/surfaceName + ".000.mesh",
        writeFormat_
    );

    if (verbose)
    {
        Info<< "Writing case file to " << osCase.name() << endl;
    }

    osCase
        << "FORMAT" << nl
        << "type: ensight gold" << nl
        << nl
        << "GEOMETRY" << nl
        << "model:        1     " << osGeom.name().name() << nl
        << nl
        << "TIME" << nl
        << "time set:                      1" << nl
        << "number of steps:               1" << nl
        << "filename start number:         0" << nl
        << "filename increment:            1" << nl
        << "time values:" << nl
        << timeValue << nl
        << nl;

    ensightPartFaces ensPart(0, osGeom.name().name(), points, faces, true);
    osGeom << ensPart;
}


// create write methods
defineSurfaceWriterWriteFields(CML::ensightSurfaceWriter);


// ************************************************************************* //
