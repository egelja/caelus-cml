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

#include "prefixOSstream.hpp"
#include "Pstream.hpp"
#include "token.hpp"

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

inline void CML::prefixOSstream::checkWritePrefix()
{
    if (printPrefix_ && prefix_.size())
    {
        OSstream::write(prefix_.c_str());
        printPrefix_ = false;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::prefixOSstream::prefixOSstream
(
    ostream& os,
    const string& name,
    streamFormat format,
    versionNumber version,
    compressionType compression
)
:
    OSstream(os, name, format, version, compression),
    printPrefix_(true),
    prefix_("")
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void CML::prefixOSstream::print(Ostream& os) const
{
    os  << "prefixOSstream ";
    OSstream::print(os);
}


CML::Ostream& CML::prefixOSstream::write(const token&)
{
    return *this;
}


CML::Ostream& CML::prefixOSstream::write(const char c)
{
    checkWritePrefix();
    OSstream::write(c);

    if (c == token::NL)
    {
        printPrefix_ = true;
    }

    return *this;
}


CML::Ostream& CML::prefixOSstream::write(const char* str)
{
    checkWritePrefix();
    OSstream::write(str);

    size_t len = strlen(str);
    if (len && str[len-1] == token::NL)
    {
        printPrefix_ = true;
    }

    return *this;
}


CML::Ostream& CML::prefixOSstream::write(const word& val)
{
    checkWritePrefix();
    return OSstream::write(val);
}


CML::Ostream& CML::prefixOSstream::write(const string& val)
{
    checkWritePrefix();
    return OSstream::write(val);
}


CML::Ostream& CML::prefixOSstream::writeQuoted
(
    const std::string& val,
    const bool quoted
)
{
    checkWritePrefix();
    return OSstream::writeQuoted(val, quoted);
}


CML::Ostream& CML::prefixOSstream::write(const label val)
{
    checkWritePrefix();
    return OSstream::write(val);
}


CML::Ostream& CML::prefixOSstream::write(const floatScalar val)
{
    checkWritePrefix();
    return OSstream::write(val);
}


CML::Ostream& CML::prefixOSstream::write(const doubleScalar val)
{
    checkWritePrefix();
    return OSstream::write(val);
}


CML::Ostream& CML::prefixOSstream::write
(
    const char* buf,
    std::streamsize count
)
{
    checkWritePrefix();
    return OSstream::write(buf, count);
}


void CML::prefixOSstream::indent()
{
    checkWritePrefix();
    OSstream::indent();
}

// ************************************************************************* //
