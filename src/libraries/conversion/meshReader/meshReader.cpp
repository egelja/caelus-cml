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

#include "meshReader.hpp"
#include "Time.hpp"
#include "polyMesh.hpp"
#include "faceSet.hpp"
#include "emptyPolyPatch.hpp"
#include "cellModeller.hpp"
#include "demandDrivenData.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const CML::cellModel* CML::meshReader::unknownModel = CML::cellModeller::
lookup
(
    "unknown"
);

const CML::cellModel* CML::meshReader::tetModel = CML::cellModeller::
lookup
(
    "tet"
);

const CML::cellModel* CML::meshReader::pyrModel = CML::cellModeller::
lookup
(
    "pyr"
);

const CML::cellModel* CML::meshReader::prismModel = CML::cellModeller::
lookup
(
    "prism"
);

const CML::cellModel* CML::meshReader::hexModel = CML::cellModeller::
lookup
(
    "hex"
);


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::meshReader::addCellZones(polyMesh& mesh) const
{
    cellTable_.addCellZones(mesh, cellTableId_);
    warnDuplicates("cellZones", mesh.cellZones().names());
}


void CML::meshReader::addFaceZones(polyMesh& mesh) const
{
    label nZone = monitoringSets_.size();
    mesh.faceZones().setSize(nZone);

    if (!nZone)
    {
        return;
    }

    nZone = 0;
    for
    (
        HashTable<List<label>, word, string::hash>::const_iterator
        iter = monitoringSets_.begin();
        iter != monitoringSets_.end();
        ++iter
    )
    {
        Info<< "faceZone " << nZone
            << " (size: " << iter().size() << ") name: "
            << iter.key() << endl;

        mesh.faceZones().set
        (
            nZone,
            new faceZone
            (
                iter.key(),
                iter(),
                List<bool>(iter().size(), false),
                nZone,
                mesh.faceZones()
            )
        );

        nZone++;
    }
    mesh.faceZones().writeOpt() = IOobject::AUTO_WRITE;
    warnDuplicates("faceZones", mesh.faceZones().names());
}


CML::autoPtr<CML::polyMesh> CML::meshReader::mesh
(
    const objectRegistry& registry
)
{
    readGeometry();

    Info<< "Creating a polyMesh" << endl;
    createPolyCells();

    Info<< "Number of internal faces: " << nInternalFaces_ << endl;

    createPolyBoundary();
    clearExtraStorage();

    autoPtr<polyMesh> mesh
    (
        new polyMesh
        (
            IOobject
            (
                polyMesh::defaultRegion,
                "constant",
                registry
            ),
            xferMove(points_),
            xferMove(meshFaces_),
            xferMove(cellPolys_)
        )
    );

    // adding patches also checks the mesh
    mesh().addPatches(polyBoundaryPatches(mesh));

    warnDuplicates("boundaries", mesh().boundaryMesh().names());

    addCellZones(mesh());
    addFaceZones(mesh());

    return mesh;
}


void CML::meshReader::writeMesh
(
    const polyMesh& mesh,
    IOstream::streamFormat fmt
) const
{
    mesh.removeFiles();

    Info<< "Writing polyMesh" << endl;
    mesh.writeObject
    (
        fmt,
        IOstream::currentVersion,
        IOstream::UNCOMPRESSED
    );
    writeAux(mesh);
}


void CML::meshReader::clearExtraStorage()
{
    cellFaces_.clear();
    baffleFaces_.clear();
    boundaryIds_.clear();
    baffleIds_.clear();

    deleteDemandDrivenData(pointCellsPtr_);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::meshReader::meshReader
(
    const fileName& fileOrPrefix,
    const scalar scaleFactor
)
    :
    pointCellsPtr_(nullptr),
    nInternalFaces_(0),
    patchStarts_(0),
    patchSizes_(0),
    interfaces_(0),
    baffleIds_(0),
    meshFaces_(0),
    cellPolys_(0),
    geometryFile_(fileOrPrefix),
    scaleFactor_(scaleFactor),
    points_(0),
    origCellId_(0),
    boundaryIds_(0),
    patchTypes_(0),
    patchNames_(0),
    patchPhysicalTypes_(0),
    cellFaces_(0),
    baffleFaces_(0),
    cellTableId_(0),
    cellTable_()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::meshReader::~meshReader()
{
    deleteDemandDrivenData(pointCellsPtr_);
}


// ************************************************************************* //
