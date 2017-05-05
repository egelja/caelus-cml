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
    Reads an bool from an input stream, for a given version number and file
    format. If an ASCII file is being read, then the line numbers are counted
    and an erroneous read is reported.

\*---------------------------------------------------------------------------*/

#include "error.hpp"

#include "bool.hpp"
#include "Switch.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, bool& b)
{
    if (is.good())
    {
        b = Switch(is);
    }

    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const bool b)
{
    // we could also write as text string without any difficulty
    // os  << (b ? "true" : "false");
    os.write(label(b));
    os.check("Ostream& operator<<(Ostream&, const bool)");
    return os;
}


bool CML::readBool(Istream& is)
{
    bool b;
    is  >> b;

    return b;
}


// ************************************************************************* //
