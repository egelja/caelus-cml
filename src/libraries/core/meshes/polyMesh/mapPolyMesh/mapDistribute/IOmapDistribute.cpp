/*---------------------------------------------------------------------------*\
Copyright (C) 2014 OpenFOAM Foundation
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

#include "IOmapDistribute.hpp"

/* * * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * */

namespace CML
{
    defineTypeNameAndDebug(IOmapDistribute, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::IOmapDistribute::IOmapDistribute(const IOobject& io)
:
    regIOobject(io)
{
    // Temporary warning
    if (io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
    {
        WarningInFunction
            << "IOmapDistribute " << name()
            << " constructed with IOobject::MUST_READ_IF_MODIFIED"
            " but IOmapDistribute does not support automatic rereading."
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


CML::IOmapDistribute::IOmapDistribute
(
    const IOobject& io,
    const mapDistribute& map
)
:
    regIOobject(io)
{
     // Temporary warning
    if (io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
    {
        WarningInFunction
            << "IOmapDistribute " << name()
            << " constructed with IOobject::MUST_READ_IF_MODIFIED"
            " but IOmapDistribute does not support automatic rereading."
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
        mapDistribute::operator=(map);
    }
}


CML::IOmapDistribute::IOmapDistribute
(
    const IOobject& io,
    const Xfer<mapDistribute>& map
)
:
    regIOobject(io)
{
    // Temporary warning
    if (io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
    {
        WarningInFunction
            << "IOmapDistribute " << name()
            << " constructed with IOobject::MUST_READ_IF_MODIFIED"
            " but IOmapDistribute does not support automatic rereading."
            << endl;
    }

    mapDistribute::transfer(map());

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


// * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * * //

CML::IOmapDistribute::~IOmapDistribute()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::IOmapDistribute::readData(Istream& is)
{
    return (is >> *this).good();
}


bool CML::IOmapDistribute::writeData(Ostream& os) const
{
    return (os << *this).good();
}


// ************************************************************************* //
