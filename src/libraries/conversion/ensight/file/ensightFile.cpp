/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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

#include "ensightFile.hpp"
#include <sstream>

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

bool CML::ensightFile::allowUndef_ = false;

CML::scalar CML::ensightFile::undefValue_ = CML::floatScalarVGREAT;

// default is width 8
CML::string CML::ensightFile::mask_ = "********";

CML::string CML::ensightFile::dirFmt_ = "%08d";


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

CML::string CML::ensightFile::mask()
{
    return mask_;
}


CML::string CML::ensightFile::subDir(const label n)
{
    char buf[32];

    sprintf(buf, dirFmt_.c_str(), n);
    return buf;
}


void CML::ensightFile::subDirWidth(const label n)
{
    // enforce max limit to avoid buffer overflow in subDir()
    if (n < 1 || n > 31)
    {
        return;
    }

    // appropriate printf format
    std::ostringstream oss;
    oss << "%0" << n << "d";
    dirFmt_ = oss.str();

    // set mask accordingly
    mask_.resize(n, '*');
}


CML::label CML::ensightFile::subDirWidth()
{
    return mask_.size();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::ensightFile::ensightFile
(
    const fileName& pathname,
    IOstream::streamFormat format
)
:
    OFstream(pathname, format)
{
    // ascii formatting specs
    setf
    (
        ios_base::scientific,
        ios_base::floatfield
    );
    precision(5);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::ensightFile::~ensightFile()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::ensightFile::allowUndef()
{
    return allowUndef_;
}


bool CML::ensightFile::allowUndef(bool value)
{
    bool old = allowUndef_;
    allowUndef_ = value;
    return old;
}


CML::scalar CML::ensightFile::undefValue(const scalar value)
{
    // enable its use too
    allowUndef_ = true;

    scalar old = undefValue_;
    undefValue_ = value;
    return old;
}


CML::Ostream& CML::ensightFile::write
(
    const char* buf,
    std::streamsize count
)
{
    stdStream().write(buf, count);
    return *this;
}


CML::Ostream& CML::ensightFile::write(const char* value)
{
    return write(string(value));
}


CML::Ostream& CML::ensightFile::write(const string& value)
{
    char buf[80];

    for (string::size_type i = 0; i < 80; ++i)
    {
        buf[i] = 0;
    }

    string::size_type n = value.size();
    if (n >= 80)
    {
        n = 79;
    }

    for (string::size_type i = 0; i < n; ++i)
    {
        buf[i] = value[i];
    }

    if (format() == IOstream::BINARY)
    {
        write
        (
            reinterpret_cast<char const *>(buf),
            sizeof(buf)
        );
    }
    else
    {
        stdStream() << buf;
    }

    return *this;
}


CML::Ostream& CML::ensightFile::write(const label value)
{
    if (format() == IOstream::BINARY)
    {
        unsigned int ivalue(value);

        write
        (
            reinterpret_cast<char const *>(&ivalue),
            sizeof(ivalue)
        );
    }
    else
    {
        stdStream().width(10);
        stdStream() << value;
    }

    return *this;
}


CML::Ostream& CML::ensightFile::write
(
    const label value,
    const label fieldWidth
)
{
    if (format() == IOstream::BINARY)
    {
        unsigned int ivalue(value);

        write
        (
            reinterpret_cast<char const *>(&ivalue),
            sizeof(ivalue)
        );
    }
    else
    {
        stdStream().width(fieldWidth);
        stdStream() << value;
    }

    return *this;
}


CML::Ostream& CML::ensightFile::write(const scalar value)
{
    float fvalue(value);

    if (format() == IOstream::BINARY)
    {
        write
        (
            reinterpret_cast<char const *>(&fvalue),
            sizeof(fvalue)
        );
    }
    else
    {
        stdStream().width(12);
        stdStream() << fvalue;
    }

    return *this;
}


void CML::ensightFile::newline()
{
    if (format() == IOstream::ASCII)
    {
        stdStream() << nl;
    }
}


CML::Ostream& CML::ensightFile::writeUndef()
{
    write(undefValue_);
    return *this;
}


CML::Ostream& CML::ensightFile::writeKeyword(const string& key)
{
    if (allowUndef_)
    {
        write(string(key + " undef"));
        newline();
        write(undefValue_);
        newline();
    }
    else
    {
        write(key);
        newline();
    }
    return *this;
}


CML::Ostream& CML::ensightFile::writeBinaryHeader()
{
    if (format() == IOstream::BINARY)
    {
        write("C Binary");
    }

    return *this;
}


// ************************************************************************* //
