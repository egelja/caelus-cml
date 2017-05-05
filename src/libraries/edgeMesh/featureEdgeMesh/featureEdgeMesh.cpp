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

#include "featureEdgeMesh.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(featureEdgeMesh, 0);

}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::featureEdgeMesh::featureEdgeMesh(const IOobject& io)
:
    regIOobject(io),
    edgeMesh(pointField(0), edgeList(0))
{
    if
    (
        io.readOpt() == IOobject::MUST_READ
     || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        readStream(typeName) >> *this;
        close();
    }

    if (debug)
    {
        Pout<< "featureEdgeMesh::featureEdgeMesh :"
            << " constructed from IOobject :"
            << " points:" << points().size()
            << " edges:" << edges().size()
            << endl;
    }
}


//- Construct from components
CML::featureEdgeMesh::featureEdgeMesh
(
    const IOobject& io,
    const pointField& points,
    const edgeList& edges
)
:
    regIOobject(io),
    edgeMesh(points, edges)
{}


// Construct as copy
CML::featureEdgeMesh::featureEdgeMesh
(
    const IOobject& io,
    const featureEdgeMesh& em
)
:
    regIOobject(io),
    edgeMesh(em)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::featureEdgeMesh::readData(Istream& is)
{
    is >> *this;
    return !is.bad();
}


bool CML::featureEdgeMesh::writeData(Ostream& os) const
{
    os << *this;

    return os.good();
}


// ************************************************************************* //
