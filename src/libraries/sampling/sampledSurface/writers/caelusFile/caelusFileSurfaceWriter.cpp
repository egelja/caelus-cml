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

#include "caelusFileSurfaceWriter.hpp"

#include "OFstream.hpp"
#include "OSspecific.hpp"

#include "makeSurfaceWriterMethods.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    makeSurfaceWriterType(caelusFileSurfaceWriter);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
void CML::caelusFileSurfaceWriter::writeTemplate
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
    fileName surfaceDir(outputDir/surfaceName);

    if (!isDir(surfaceDir))
    {
        mkDir(surfaceDir);
    }

    if (verbose)
    {
        Info<< "Writing field " << fieldName << " to " << surfaceDir << endl;
    }

    // geometry should already have been written
    // Values to separate directory (e.g. "scalarField/p")

    fileName caelusName(pTraits<Type>::typeName);
    fileName valuesDir(surfaceDir  / (caelusName + Field<Type>::typeName));

    if (!isDir(valuesDir))
    {
        mkDir(valuesDir);
    }

    // values
    OFstream(valuesDir/fieldName)()  << values;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::caelusFileSurfaceWriter::caelusFileSurfaceWriter()
:
    surfaceWriter()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::caelusFileSurfaceWriter::~caelusFileSurfaceWriter()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::caelusFileSurfaceWriter::write
(
    const fileName& outputDir,
    const fileName& surfaceName,
    const pointField& points,
    const faceList& faces,
    const bool verbose
) const
{
    fileName surfaceDir(outputDir/surfaceName);

    if (!isDir(surfaceDir))
    {
        mkDir(surfaceDir);
    }

    if (verbose)
    {
        Info<< "Writing geometry to " << surfaceDir << endl;
    }


    // Points
    OFstream(surfaceDir/"points")() << points;

    // Faces
    OFstream(surfaceDir/"faces")() << faces;

    // Face centers. Not really necessary but very handy when reusing as inputs
    // for e.g. timeVaryingMapped bc.
    pointField faceCentres(faces.size(),point::zero);

    forAll(faces, faceI)
    {
        faceCentres[faceI] = faces[faceI].centre(points);
    }

    OFstream(surfaceDir/"faceCentres")() << faceCentres;
}


// create write methods
defineSurfaceWriterWriteFields(CML::caelusFileSurfaceWriter);


// ************************************************************************* //
