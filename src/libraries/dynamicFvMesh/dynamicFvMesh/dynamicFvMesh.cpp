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

#include "dynamicFvMesh.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(CML::dynamicFvMesh, 0);

defineRunTimeSelectionTable(CML::dynamicFvMesh, IOobject);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::dynamicFvMesh::dynamicFvMesh(const IOobject& io)
:
    fvMesh(io)
{}


CML::dynamicFvMesh::dynamicFvMesh
(
    const IOobject& io,
    const Xfer<pointField>& points,
    const Xfer<faceList>& faces,
    const Xfer<labelList>& allOwner,
    const Xfer<labelList>& allNeighbour,
    const bool syncPar
)
:
    fvMesh
    (
        io,
        points,
        faces,
        allOwner,
        allNeighbour,
        syncPar
    )
{}


CML::dynamicFvMesh::dynamicFvMesh
(
    const IOobject& io,
    const Xfer<pointField>& points,
    const Xfer<faceList>& faces,
    const Xfer<cellList>& cells,
    const bool syncPar
)
:
    fvMesh
    (
        io,
        points,
        faces,
        cells,
        syncPar
    )
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::dynamicFvMesh::~dynamicFvMesh()
{}


// ************************************************************************* //
