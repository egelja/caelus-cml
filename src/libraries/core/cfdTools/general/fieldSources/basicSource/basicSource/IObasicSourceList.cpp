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

#include "IObasicSourceList.hpp"
#include "fvMesh.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

CML::IOobject CML::IObasicSourceList::createIOobject
(
    const fvMesh& mesh
) const
{
    IOobject io
    (
        "sourcesProperties",
        mesh.time().constant(),
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    );

    if (io.headerOk())
    {
        Info<< "Creating field source list from " << io.name() << nl << endl;

        io.readOpt() = IOobject::MUST_READ_IF_MODIFIED;
        return io;
    }
    else
    {
        Info<< "No field sources present" << nl << endl;

        io.readOpt() = IOobject::NO_READ;
        return io;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::IObasicSourceList::IObasicSourceList
(
    const fvMesh& mesh
)
:
    IOdictionary(createIOobject(mesh)),
    basicSourceList(mesh, *this)
{}


bool CML::IObasicSourceList::read()
{
    if (regIOobject::read())
    {
        basicSourceList::read(*this);
        return true;
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //

