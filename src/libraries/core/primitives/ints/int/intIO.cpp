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
    Reads an int from an input stream, for a given version
    number and File format. If an ascii File is being read,
    then the line numbers are counted and an erroneous read
    ised.

\*---------------------------------------------------------------------------*/

#include "error.hpp"

#include "int.hpp"
#include "IOstreams.hpp"

#include <sstream>
#include <cerrno>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::word CML::name(const int val)
{
    std::ostringstream buf;
    buf << val;
    return buf.str();
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, int& i)
{
    token t(is);

    if (!t.good())
    {
        is.setBad();
        return is;
    }

    if (t.isLabel())
    {
        i = int(t.labelToken());
    }
    else
    {
        is.setBad();
        FatalIOErrorInFunction(is)
            << "wrong token type - expected int, found " << t.info()
            << exit(FatalIOError);

        return is;
    }

    // Check state of Istream
    is.check("Istream& operator>>(Istream&, int&)");

    return is;
}


int CML::readInt(Istream& is)
{
    int val;
    is >> val;

    return val;
}


bool CML::readInt(const char* buf, int& s)
{
    char *endptr = nullptr;
    errno = 0;
    long l = strtol(buf, &endptr, 10);
    s = int(l);
    return (*endptr == 0) && (errno == 0) && (l >= INT_MIN) && (l <= INT_MAX);
}


CML::Ostream& CML::operator<<(Ostream& os, const int i)
{
    os.write(label(i));
    os.check("Ostream& operator<<(Ostream&, const int)");
    return os;
}


// ************************************************************************* //
