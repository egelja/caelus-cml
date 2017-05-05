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

\*----------------------------------------------------------------------------*/

#include "meshWriter.hpp"
#include "cellModeller.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

CML::string CML::meshWriter::defaultMeshName = "meshExport";


const CML::cellModel* CML::meshWriter::unknownModel = CML::cellModeller::
lookup
(
    "unknown"
);


const CML::cellModel* CML::meshWriter::tetModel = CML::cellModeller::
lookup
(
    "tet"
);


const CML::cellModel* CML::meshWriter::pyrModel = CML::cellModeller::
lookup
(
    "pyr"
);


const CML::cellModel* CML::meshWriter::prismModel = CML::cellModeller::
lookup
(
    "prism"
);


const CML::cellModel* CML::meshWriter::hexModel = CML::cellModeller::
lookup
(
    "hex"
);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::meshWriter::meshWriter(const polyMesh& mesh, const scalar scaleFactor)
:
    mesh_(mesh),
    scaleFactor_(scaleFactor),
    writeBoundary_(true),
    boundaryRegion_(),
    cellTable_(),
    cellTableId_()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::meshWriter::~meshWriter()
{}


// ************************************************************************* //
