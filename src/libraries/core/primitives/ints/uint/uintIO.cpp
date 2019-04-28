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
    Reads an uint from an input stream, for a given version
    number and File format. If an ascii File is being read,
    then the line numbers are counted and an erroneous read
    ised.

\*---------------------------------------------------------------------------*/

#include "error.hpp"

#include "uint.hpp"
#include "IOstreams.hpp"

#include <sstream>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::word CML::name(const unsigned int val)
{
    std::ostringstream buf;
    buf << val;
    return buf.str();
}

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, unsigned int& i)
{
    token t(is);

    if (!t.good())
    {
        is.setBad();
        return is;
    }

    if (t.isLabel())
    {
        i = static_cast<unsigned int>(t.labelToken());
    }
    else
    {
        is.setBad();
        FatalIOErrorInFunction(is)
            << "wrong token type - expected unsigned int, found " << t.info()
            << exit(FatalIOError);

        return is;
    }

    // Check state of Istream
    is.check("Istream& operator>>(Istream&, unsigned int&)");

    return is;
}


unsigned int CML::readUint(Istream& is)
{
    unsigned int val;
    is >> val;

    return val;
}


CML::Ostream& CML::operator<<(Ostream& os, const unsigned int i)
{
    os.write(label(i));
    os.check("Ostream& operator<<(Ostream&, const unsigned int)");
    return os;
}


// ************************************************************************* //
