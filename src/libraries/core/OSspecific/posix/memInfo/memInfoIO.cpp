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

#include "memInfo.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::memInfo::memInfo(Istream& is)
:
    base1(is),
    base2(is),
    member1(is),
    member2(is)
{
    // Check state of Istream
    is.check("CML::memInfo::memInfo(CML::Istream&)");
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, memInfo&)
{
    // Check state of Istream
    is.check
    (
        "CML::Istream& CML::operator>>(CML::Istream&, CML::memInfo&)"
    );

    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const memInfo&)
{
    // Check state of Ostream
    os.check
    (
        "CML::Ostream& CML::operator<<(CML::Ostream&, "
        "const CML::memInfo&)"
    );

    return os;
}


// ************************************************************************* //
