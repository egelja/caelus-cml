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

#include "error.hpp"
#include "OSstream.hpp"
#include "token.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::Ostream& CML::OSstream::write(const token&)
{
    return *this;
}


CML::Ostream& CML::OSstream::write(const char c)
{
    os_ << c;
    if (c == token::NL)
    {
        lineNumber_++;
    }
    setState(os_.rdstate());
    return *this;
}


CML::Ostream& CML::OSstream::write(const char* str)
{
    lineNumber_ += string(str).count(token::NL);
    os_ << str;
    setState(os_.rdstate());
    return *this;
}


CML::Ostream& CML::OSstream::write(const word& str)
{
    os_ << str;
    setState(os_.rdstate());
    return *this;
}


CML::Ostream& CML::OSstream::write(const string& str)
{
    os_ << token::BEGIN_STRING;

    register int backslash = 0;
    for (string::const_iterator iter = str.begin(); iter != str.end(); ++iter)
    {
        register char c = *iter;

        if (c == '\\')
        {
            backslash++;
            // suppress output until we know if other characters follow
            continue;
        }
        else if (c == token::NL)
        {
            lineNumber_++;
            backslash++;    // backslash escape for newline
        }
        else if (c == token::END_STRING)
        {
            backslash++;    // backslash escape for quote
        }

        // output pending backslashes
        while (backslash)
        {
            os_ << '\\';
            backslash--;
        }

        os_ << c;
    }

    // silently drop any trailing backslashes
    // they would otherwise appear like an escaped end-quote

    os_ << token::END_STRING;

    setState(os_.rdstate());
    return *this;
}


CML::Ostream& CML::OSstream::writeQuoted
(
    const std::string& str,
    const bool quoted
)
{
    if (quoted)
    {
        os_ << token::BEGIN_STRING;

        register int backslash = 0;
        for
        (
            string::const_iterator iter = str.begin();
            iter != str.end();
            ++iter
        )
        {
            register char c = *iter;

            if (c == '\\')
            {
                backslash++;
                // suppress output until we know if other characters follow
                continue;
            }
            else if (c == token::NL)
            {
                lineNumber_++;
                backslash++;    // backslash escape for newline
            }
            else if (c == token::END_STRING)
            {
                backslash++;    // backslash escape for quote
            }

            // output pending backslashes
            while (backslash)
            {
                os_ << '\\';
                backslash--;
            }

            os_ << c;
        }

        // silently drop any trailing backslashes
        // they would otherwise appear like an escaped end-quote
        os_ << token::END_STRING;
    }
    else
    {
        // output unquoted string, only advance line number on newline
        lineNumber_ += string(str).count(token::NL);
        os_ << str;
    }

    setState(os_.rdstate());
    return *this;
}


CML::Ostream& CML::OSstream::write(const label val)
{
    os_ << val;
    setState(os_.rdstate());
    return *this;
}


CML::Ostream& CML::OSstream::write(const floatScalar val)
{
    os_ << val;
    setState(os_.rdstate());
    return *this;
}


CML::Ostream& CML::OSstream::write(const doubleScalar val)
{
    os_ << val;
    setState(os_.rdstate());
    return *this;
}


CML::Ostream& CML::OSstream::write(const char* buf, std::streamsize count)
{
    if (format() != BINARY)
    {
        FatalIOErrorInFunction(*this)
            << "stream format not binary"
            << abort(FatalIOError);
    }

    os_ << token::BEGIN_LIST;
    os_.write(buf, count);
    os_ << token::END_LIST;

    setState(os_.rdstate());

    return *this;
}


void CML::OSstream::indent()
{
    for (register unsigned short i = 0; i < indentLevel_*indentSize_; i++)
    {
        os_ << ' ';
    }
}


void CML::OSstream::flush()
{
    os_.flush();
}


// Add carriage return and flush stream
void CML::OSstream::endl()
{
    write('\n');
    os_.flush();
}


// Get flags of output stream
std::ios_base::fmtflags CML::OSstream::flags() const
{
    return os_.flags();
}


// Set flags of output stream
std::ios_base::fmtflags CML::OSstream::flags(const ios_base::fmtflags f)
{
    return os_.flags(f);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// Get width of output field
int CML::OSstream::width() const
{
    return os_.width();
}

// Set width of output field (and return old width)
int CML::OSstream::width(const int w)
{
    return os_.width(w);
}

// Get precision of output field
int CML::OSstream::precision() const
{
    return os_.precision();
}

// Set precision of output field (and return old precision)
int CML::OSstream::precision(const int p)
{
    return os_.precision(p);
}


// ************************************************************************* //
