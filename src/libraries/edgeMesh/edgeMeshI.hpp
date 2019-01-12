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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::edgeMesh::edgeMesh(const edgeMesh& em)
:
    fileFormats::edgeFormatsCore(),
    points_(em.points_),
    edges_(em.edges_),
    pointEdgesPtr_(nullptr)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::pointField& CML::edgeMesh::points() const
{
    return points_;
}


inline const CML::edgeList& CML::edgeMesh::edges() const
{
    return edges_;
}


inline const CML::labelListList& CML::edgeMesh::pointEdges() const
{
    if (pointEdgesPtr_.empty())
    {
        calcPointEdges();
    }
    return pointEdgesPtr_();
}


inline CML::pointField& CML::edgeMesh::storedPoints()
{
    return points_;
}


inline CML::edgeList& CML::edgeMesh::storedEdges()
{
    return edges_;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void CML::edgeMesh::operator=(const edgeMesh& rhs)
{
    points_ = rhs.points_;
    edges_ = rhs.edges_;
    pointEdgesPtr_.clear();
}


// ************************************************************************* //
