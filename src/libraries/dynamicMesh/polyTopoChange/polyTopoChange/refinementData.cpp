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

#include "refinementData.hpp"

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<
(
    CML::Ostream& os,
    const CML::refinementData& wDist
)
{
    if (os.format() == IOstream::ASCII)
    {
        os << wDist.refinementCount_ << token::SPACE << wDist.count_;
    }
    else
    {
        os.write
        (
            reinterpret_cast<const char*>(&wDist.refinementCount_),
            sizeof(refinementData)
        );
    }

    // Check state of Ostream
    os.check("Ostream& operator<<(Ostream&, const refinementData&)");
    return os;
}


CML::Istream& CML::operator>>(CML::Istream& is, CML::refinementData& wDist)
{
    if (is.format() == IOstream::ASCII)
    {
        is >> wDist.refinementCount_ >> wDist.count_;
    }
    else
    {
        is.read
        (
            reinterpret_cast<char*>(&wDist.refinementCount_),
            sizeof(refinementData)
        );
    }

    // Check state of Istream
    is.check("Istream& operator>>(Istream&, refinementData&)");
    return is;
}


// ************************************************************************* //
