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

#include "error.hpp"
#include "block.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::block::block
(
    const pointField& blockPointField,
    const curvedEdgeList& edges,
    Istream& is
)
:
    blockDescriptor(blockPointField, edges, is),
    vertices_(0),
    cells_(0),
    boundaryPatches_(0)
{}


CML::block::block(const blockDescriptor& blockDesc)
:
    blockDescriptor(blockDesc),
    vertices_(0),
    cells_(0),
    boundaryPatches_(0)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::block::~block()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::pointField& CML::block::points() const
{
    if (vertices_.empty())
    {
        createPoints();
    }

    return vertices_;
}


const CML::labelListList& CML::block::cells() const
{
    if (cells_.empty())
    {
        createCells();
    }

    return cells_;
}


const CML::labelListListList& CML::block::boundaryPatches() const
{
    if (boundaryPatches_.empty())
    {
        createBoundary();
    }

    return boundaryPatches_;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const block& b)
{
    os << b.points() << nl
       << b.cells() << nl
       << b.boundaryPatches() << endl;

    return os;
}


// ************************************************************************* //
