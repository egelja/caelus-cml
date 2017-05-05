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
#include "ITstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void CML::ITstream::print(Ostream& os) const
{
    os  << "ITstream : " << name_.c_str();

    if (size())
    {
        if (begin()->lineNumber() == rbegin()->lineNumber())
        {
            os  << ", line " << begin()->lineNumber() << ", ";
        }
        else
        {
            os  << ", lines " << begin()->lineNumber()
                << '-' << rbegin()->lineNumber() << ", ";
        }
    }
    else
    {
        os  << ", line " << lineNumber() << ", ";
    }

    IOstream::print(os);
}


CML::Istream& CML::ITstream::read(token& t)
{
    // Return the put back token if it exists
    if (Istream::getBack(t))
    {
        lineNumber_ = t.lineNumber();
        return *this;
    }

    if (tokenIndex_ < size())
    {
        t = operator[](tokenIndex_++);
        lineNumber_ = t.lineNumber();

        if (tokenIndex_ == size())
        {
            setEof();
        }
    }
    else
    {
        if (eof())
        {
            FatalIOErrorIn
            (
                "ITstream::read(token&)",
                *this
            )   << "attempt to read beyond EOF"
                << exit(FatalIOError);

            setBad();
        }
        else
        {
            setEof();
        }

        t = token::undefinedToken;

        if (size())
        {
            t.lineNumber() = tokenList::last().lineNumber();
        }
        else
        {
            t.lineNumber() = lineNumber();
        }
    }

    return *this;
}


CML::Istream& CML::ITstream::read(char&)
{
    notImplemented("Istream& ITstream::read(char&)");
    return *this;
}


CML::Istream& CML::ITstream::read(word&)
{
    notImplemented("Istream& ITstream::read(word&)");
    return *this;
}


CML::Istream& CML::ITstream::read(string&)
{
    notImplemented("Istream& ITstream::read(string&)");
    return *this;
}


CML::Istream& CML::ITstream::read(label&)
{
    notImplemented("Istream& ITstream::read(label&)");
    return *this;
}


CML::Istream& CML::ITstream::read(floatScalar&)
{
    notImplemented("Istream& ITstream::read(floatScalar&)");
    return *this;
}


CML::Istream& CML::ITstream::read(doubleScalar&)
{
    notImplemented("Istream& ITstream::read(doubleScalar&)");
    return *this;
}


CML::Istream& CML::ITstream::read(char*, std::streamsize)
{
    notImplemented("Istream& ITstream::read(char*, std::streamsize)");
    return *this;
}


CML::Istream& CML::ITstream::rewind()
{
    tokenIndex_ = 0;

    if (size())
    {
        lineNumber_ = tokenList::first().lineNumber();
    }

    setGood();

    return *this;
}


// ************************************************************************* //
