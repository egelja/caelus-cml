/*---------------------------------------------------------------------------*\
Copyright (C) Creative Fields, Ltd.
-------------------------------------------------------------------------------
License
    This file is part of cfMesh.

    cfMesh is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    cfMesh is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with cfMesh.  If not, see <http://www.gnu.org/licenses/>.

Description
    Finds feature edges and corners of a triangulated surface

Author: Franjo Juretic (franjo.juretic@c-fields.com)

\*---------------------------------------------------------------------------*/

#include "argList.hpp"
#include "IFstream.hpp"
#include "fileName.hpp"
#include "triSurf.hpp"
#include "demandDrivenData.hpp"

#include "triSurfaceCopyParts.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:
using namespace CML;

int main(int argc, char *argv[])
{
    argList::noParallel();
    argList::validArgs.clear();
    argList::validArgs.append("input surface file");
    argList::validArgs.append("output surface file");
    argList::validArgs.append("patch/subset name");
    argList args(argc, argv);

    const fileName inFileName(args.args()[1]);
    const fileName outFileName(args.args()[2]);

    if( outFileName == inFileName )
    {
        FatalErrorInFunction
            << "Output file " << outFileName
            << " would overwrite the input file."
            << exit(FatalError);
    }

    wordList patches(1);
    patches[0] = args.args()[3];

    triSurf originalSurface(inFileName);

    triSurfaceCopyParts copyPart(originalSurface);

    triSurf copy;

    copyPart.copySurface(patches, copy);

    copy.writeSurface(outFileName);

    Info << "End\n" << endl;

    return 0;
}


// ************************************************************************* //
