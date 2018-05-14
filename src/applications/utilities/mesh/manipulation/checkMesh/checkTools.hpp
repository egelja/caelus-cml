/*---------------------------------------------------------------------------*\
Copyright (C) 2015-2017 OpenFOAM Foundation
Copyright (C) 2015-2016 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/
#include "scalar.hpp"
#include "indirectPrimitivePatch.hpp"
#include "writer.hpp"

namespace CML
{
    class polyMesh;
    class surfaceWriter;
    class pointSet;
    class faceSet;
    class cellSet;
    class fileName;
    class polyMesh;

    void printMeshStats(const polyMesh& mesh, const bool allTopology, const bool meshBandwidth);

    void printMeshBandwidth(const polyMesh& mesh);

    //- Generate merged surface on master and write. Needs input patch
    //  to be of mesh faces.
    void mergeAndWrite
    (
        const polyMesh& mesh,
        const surfaceWriter& writer,
        const word& name,
        const indirectPrimitivePatch setPatch,
        const fileName& outputDir
    );

    //- Write vtk representation of (assembled) faceSet to surface file in
    //  postProcessing/ directory
    void mergeAndWrite(const surfaceWriter&, const faceSet&);

    //- Write vtk representation of (assembled) cellSet to surface file in
    //  postProcessing/ directory
    void mergeAndWrite(const surfaceWriter&, const cellSet&);

    //- Write vtk representation of (assembled) pointSet to 'set' file in
    //  postProcessing/ directory
    void mergeAndWrite(const writer<scalar>&, const pointSet&);
}
