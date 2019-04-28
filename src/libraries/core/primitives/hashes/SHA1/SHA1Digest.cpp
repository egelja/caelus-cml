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

\*---------------------------------------------------------------------------*/

#include "SHA1Digest.hpp"
#include "IOstreams.hpp"

#include <cstring>

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const CML::SHA1Digest CML::SHA1Digest::null;

//! \cond fileScope
static const char hexChars[] = "0123456789abcdef";
//! \endcond


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

unsigned char CML::SHA1Digest::readHexDigit(Istream& is)
{
    // Takes into account that 'a' (or 'A') is 10
    static const int alphaOffset = toupper('A') - 10;
    // Takes into account that '0' is 0
    static const int zeroOffset = int('0');


    // silently ignore leading or intermediate '_'
    char c = 0;
    do
    {
        is.read(c);
    }
    while (c == '_');

    if (!isxdigit(c))
    {
        FatalIOErrorInFunction(is)
            << "Illegal hex digit: '" << c << "'"
            << exit(FatalIOError);
    }

    if (isdigit(c))
    {
        return int(c) - zeroOffset;
    }
    else
    {
        return toupper(c) - alphaOffset;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::SHA1Digest::SHA1Digest()
{
    clear();
}


CML::SHA1Digest::SHA1Digest(Istream& is)
{
    is  >> *this;
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void CML::SHA1Digest::clear()
{
    memset(v_, 0, length);
}


bool CML::SHA1Digest::empty() const
{
    for (unsigned i = 0; i < length; ++i)
    {
        if (v_[i])
        {
            return false;
        }
    }

    return true;
}


std::string CML::SHA1Digest::str(const bool prefixed) const
{
    std::string buf;
    unsigned nChar = 0;

    if (prefixed)
    {
        buf.resize(1 + length*2);
        buf[nChar++] = '_';
    }
    else
    {
        buf.resize(length*2);
    }

    for (unsigned i = 0; i < length; ++i)
    {
        buf[nChar++] = hexChars[((v_[i] >> 4) & 0xF)];
        buf[nChar++] = hexChars[(v_[i] & 0xF)];
    }

    return buf;
}


CML::Ostream& CML::SHA1Digest::write(Ostream& os, const bool prefixed) const
{
    if (prefixed)
    {
        os.write('_');
    }

    for (unsigned i = 0; i < length; ++i)
    {
        os.write(hexChars[((v_[i] >> 4) & 0xF)]);
        os.write(hexChars[(v_[i] & 0xF)]);
    }

    os.check("SHA1Digest::write(Ostream&, const bool)");
    return os;
}


// * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * * //

bool CML::SHA1Digest::operator==(const SHA1Digest& rhs) const
{
    for (unsigned i = 0; i < length; ++i)
    {
        if (v_[i] != rhs.v_[i])
        {
            return false;
        }
    }

    return true;
}


bool CML::SHA1Digest::operator==(const std::string& hexdigits) const
{
    // null or empty string is not an error - interpret as '0000..'
    if (hexdigits.empty())
    {
        return empty();
    }

    // skip possible '_' prefix
    unsigned charI = 0;
    if (hexdigits[0] == '_')
    {
        ++charI;
    }

    // incorrect length - can never match
    if (hexdigits.size() != charI + length*2)
    {
        return false;
    }

    for (unsigned i = 0; i < length; ++i)
    {
        const char c1 = hexChars[((v_[i] >> 4) & 0xF)];
        const char c2 = hexChars[(v_[i] & 0xF)];

        if (c1 != hexdigits[charI++]) return false;
        if (c2 != hexdigits[charI++]) return false;
    }

    return true;
}


bool CML::SHA1Digest::operator==(const char* hexdigits) const
{
    // null or empty string is not an error - interpret as '0000..'
    if (!hexdigits || !*hexdigits)
    {
        return empty();
    }

    // skip possible '_' prefix
    unsigned charI = 0;
    if (hexdigits[0] == '_')
    {
        ++charI;
    }

    // incorrect length - can never match
    if (strlen(hexdigits) != charI + length*2)
    {
        return false;
    }

    for (unsigned i = 0; i < length; ++i)
    {
        const char c1 = hexChars[((v_[i] >> 4) & 0xF)];
        const char c2 = hexChars[(v_[i] & 0xF)];

        if (c1 != hexdigits[charI++]) return false;
        if (c2 != hexdigits[charI++]) return false;
    }

    return true;
}


bool CML::SHA1Digest::operator!=(const SHA1Digest& rhs) const
{
    return !operator==(rhs);
}


bool CML::SHA1Digest::operator!=(const std::string& rhs) const
{
    return !operator==(rhs);
}


bool CML::SHA1Digest::operator!=(const char* rhs) const
{
    return !operator==(rhs);
}


// * * * * * * * * * * * * * * Friend Operators * * * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, SHA1Digest& dig)
{
    unsigned char *v = dig.v_;

    for (unsigned i = 0; i < dig.length; ++i)
    {
        unsigned char c1 = SHA1Digest::readHexDigit(is);
        unsigned char c2 = SHA1Digest::readHexDigit(is);

        v[i] = (c1 << 4) + c2;
    }

    is.check("Istream& operator>>(Istream&, SHA1Digest&)");
    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const SHA1Digest& dig)
{
    return dig.write(os);
}


// ************************************************************************* //
