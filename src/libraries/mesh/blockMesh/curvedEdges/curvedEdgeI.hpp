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

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::label CML::curvedEdge::start() const
{
    return start_;
}


inline CML::label CML::curvedEdge::end() const
{
    return end_;
}


inline int CML::curvedEdge::compare(const label start, const label end) const
{
    if (start_ == start && end_ == end)
    {
        return 1;
    }
    else if (start_ == end && end_ == start)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}


inline int CML::curvedEdge::compare(const curvedEdge& e) const
{
    return CML::curvedEdge::compare(e.start(), e.end());
}


inline int CML::curvedEdge::compare(const edge& e) const
{
    return CML::curvedEdge::compare(e.start(), e.end());
}


// ************************************************************************* //
