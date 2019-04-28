/*---------------------------------------------------------------------------*\
Copyright (C) 2014-2015 OpenFOAM Foundation
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

#include "IOmapDistributePolyMesh.hpp"

/* * * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * */

namespace CML
{
defineTypeNameAndDebug(IOmapDistributePolyMesh, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::IOmapDistributePolyMesh::IOmapDistributePolyMesh(const IOobject& io)
:
    regIOobject(io)
{
    // Temporary warning
    if (io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
    {
        WarningInFunction
            << "IOmapDistributePolyMesh does not support automatic rereading."
            << endl;
    }

    if
    (
        (
            io.readOpt() == IOobject::MUST_READ
         || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED
        )
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        readStream(typeName) >> *this;
        close();
    }
}


CML::IOmapDistributePolyMesh::IOmapDistributePolyMesh
(
    const IOobject& io,
    const mapDistributePolyMesh& map
)
:
    regIOobject(io)
{
    if (io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
    {
        WarningInFunction
            << "IOmapDistributePolyMesh does not support automatic rereading."
            << endl;
    }

    if
    (
        (
            io.readOpt() == IOobject::MUST_READ
         || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED
        )
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        readStream(typeName) >> *this;
        close();
    }
    else
    {
        mapDistributePolyMesh::operator=(map);
    }
}


// * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * * //

CML::IOmapDistributePolyMesh::~IOmapDistributePolyMesh()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::IOmapDistributePolyMesh::readData(Istream& is)
{
    return (is >> *this).good();
}


bool CML::IOmapDistributePolyMesh::writeData(Ostream& os) const
{
    return (os << *this).good();
}


// ************************************************************************* //
