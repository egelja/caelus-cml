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

#include "closedTriSurfaceMesh.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(closedTriSurfaceMesh, 0);
addToRunTimeSelectionTable(searchableSurface, closedTriSurfaceMesh, dict);

}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::closedTriSurfaceMesh::closedTriSurfaceMesh
(
    const IOobject& io,
    const triSurface& s
)
:
    triSurfaceMesh(io, s)
{}


CML::closedTriSurfaceMesh::closedTriSurfaceMesh(const IOobject& io)
:
    triSurfaceMesh(io)
{}


CML::closedTriSurfaceMesh::closedTriSurfaceMesh
(
    const IOobject& io,
    const dictionary& dict
)
:
    triSurfaceMesh(io, dict)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::closedTriSurfaceMesh::~closedTriSurfaceMesh()
{}


// ************************************************************************* //
