/*---------------------------------------------------------------------------*\
Copyright (C) Creative Fields, Ltd.
-------------------------------------------------------------------------------
License
    This file is part of cfMesh.

    cfMesh is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    cfMesh is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with cfMesh.  If not, see <http://www.gnu.org/licenses/>.

Description
    An graph of faces which supports automated output.

Author: Franjo Juretic (franjo.juretic@c-fields.com)

\*---------------------------------------------------------------------------*/

#include "faceIOGraph.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

faceIOGraph::faceIOGraph(const IOobject& io)
:
    regIOobject(io),
    VRWGraph()
{}

faceIOGraph::faceIOGraph
(
    const IOobject& io,
    const label size
)
:
    regIOobject(io),
    VRWGraph(size)
{}

faceIOGraph::faceIOGraph
(
    const IOobject& io,
    const VRWGraph& g
)
:
    regIOobject(io),
    VRWGraph(g)
{}

void faceIOGraph::operator=(const faceIOGraph& rhs)
{
    VRWGraph::operator=(rhs);
}

void faceIOGraph::operator=(const VRWGraph& rhs)
{
    VRWGraph::operator=(rhs);
}

bool faceIOGraph::writeData(Ostream& os) const
{
    return (os << *this).good();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameWithName(faceIOGraph, "faceList");

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
