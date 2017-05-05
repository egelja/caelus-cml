/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
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


#include "IOMRFZoneList.hpp"
#include "fvMesh.hpp"
#include "Time.hpp"

CML::IOobject CML::IOMRFZoneList::createIOobject
(
    const fvMesh& mesh
) const
{
    IOobject io
    (
        "MRFProperties",
        mesh.time().constant(),
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    );

    if (io.headerOk())
    {
        Info<< "Creating MRF zone list from " << io.name() << endl;

        io.readOpt() = IOobject::MUST_READ_IF_MODIFIED;
        return io;
    }
    else
    {
        Info<< "No MRF models present" << nl << endl;

        io.readOpt() = IOobject::NO_READ;
        return io;
    }
}


CML::IOMRFZoneList::IOMRFZoneList
(
    const fvMesh& mesh
)
:
    IOdictionary(createIOobject(mesh)),
    MRFZoneList(mesh, *this)
{}


bool CML::IOMRFZoneList::read()
{
    if (regIOobject::read())
    {
        MRFZoneList::read(*this);
        return true;
    }
    else
    {
        return false;
    }
}




