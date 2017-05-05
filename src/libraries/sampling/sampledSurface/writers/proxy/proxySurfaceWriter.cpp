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

#include "proxySurfaceWriter.hpp"

#include "MeshedSurfaceProxy.hpp"
#include "OFstream.hpp"
#include "OSspecific.hpp"

#include "makeSurfaceWriterMethods.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(proxySurfaceWriter, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::proxySurfaceWriter::proxySurfaceWriter(const word& ext)
:
    surfaceWriter(),
    ext_(ext)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::proxySurfaceWriter::~proxySurfaceWriter()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::proxySurfaceWriter::write
(
    const fileName& outputDir,
    const fileName& surfaceName,
    const pointField& points,
    const faceList& faces,
    const bool verbose
) const
{
    // avoid bad values
    if (ext_.empty())
    {
        return;
    }

    if (!isDir(outputDir))
    {
        mkDir(outputDir);
    }

    fileName outName(outputDir/surfaceName + "." + ext_);

    if (verbose)
    {
        Info<< "Writing geometry to " << outName << endl;
    }

    MeshedSurfaceProxy<face>(points, faces).write(outName);
}


// ************************************************************************* //
