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

Description

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct null
inline CML::cell::cell()
{}


// Construct given size
inline CML::cell::cell(label s)
:
    labelList(s, -1)
{}


// Construct from components
inline CML::cell::cell(const labelUList& lst)
:
    labelList(lst)
{}


inline CML::cell::cell(const Xfer<labelList>& lst)
:
    labelList(lst)
{}


// Construct from Istream
inline CML::cell::cell(Istream& is)
:
    labelList(is)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Number of faces
inline CML::label CML::cell::nFaces() const
{
    return size();
}


inline bool CML::operator!=(const cell& a, const cell& b)
{
    return (!(a == b));
}

// ************************************************************************* //
