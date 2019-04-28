/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    Reads a ulong from an input stream.

\*---------------------------------------------------------------------------*/

#include "error.hpp"

#include "ulong.hpp"
#include "IOstreams.hpp"

#include <sstream>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::word CML::name(const unsigned long val)
{
    std::ostringstream buf;
    buf << val;
    return buf.str();
}

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, unsigned long& val)
{
    token t(is);

    if (!t.good())
    {
        is.setBad();
        return is;
    }

    if (t.isLabel())
    {
        val = static_cast<unsigned long>(t.labelToken());
    }
    else
    {
        is.setBad();
        FatalIOErrorInFunction(is)
            << "wrong token type - expected unsigned long, found " << t.info()
            << exit(FatalIOError);

        return is;
    }

    // Check state of Istream
    is.check("Istream& operator>>(Istream&, unsigned long&)");

    return is;
}


unsigned long CML::readUlong(Istream& is)
{
    unsigned long val;
    is >> val;

    return val;
}


CML::Ostream& CML::operator<<(Ostream& os, const unsigned long val)
{
    os.write(label(val));
    os.check("Ostream& operator<<(Ostream&, const unsigned long)");
    return os;
}


// ************************************************************************* //
