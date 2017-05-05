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

#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::labelledTri::labelledTri()
:
    region_(-1)
{}


inline CML::labelledTri::labelledTri
(
    const triFace& tri,
    const label region
)
:
    triFace(tri),
    region_(region)
{}


inline CML::labelledTri::labelledTri
(
    const label a,
    const label b,
    const label c,
    const label region
)
:
    triFace(a, b, c),
    region_(region)
{}


inline CML::labelledTri::labelledTri(Istream& is)
{
    operator>>(is, *this);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::label CML::labelledTri::region() const
{
    return region_;
}

inline CML::label& CML::labelledTri::region()
{
    return region_;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

inline CML::Istream& CML::operator>>(Istream& is, labelledTri& t)
{
    if (is.format() == IOstream::ASCII)
    {
        // Read beginning of labelledTri point pair
        is.readBegin("labelledTri");

        is  >> static_cast<triFace&>(t) >> t.region_;

        // Read end of labelledTri point pair
        is.readEnd("labelledTri");
    }
    else
    {
        is.read(reinterpret_cast<char*>(&t), sizeof(labelledTri));
    }

    // Check state of Ostream
    is.check("Istream& operator>>(Istream&, labelledTri&)");

    return is;
}


inline CML::Ostream& CML::operator<<(Ostream& os, const labelledTri& t)
{
    if (os.format() == IOstream::ASCII)
    {
        os  << token::BEGIN_LIST
            << static_cast<const triFace&>(t) << token::SPACE << t.region_
            << token::END_LIST;
    }
    else
    {
        os.write
        (
            reinterpret_cast<const char*>(&t),
            sizeof(labelledTri)
        );
    }

    // Check state of Ostream
    os.check("Ostream& operator<<(Ostream&, const labelledTri&)");


    return os;
}


// ************************************************************************* //
