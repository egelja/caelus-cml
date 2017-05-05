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

#include "ISstream.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::ISstream::ISstream
(
    istream& is,
    const string& name,
    streamFormat format,
    versionNumber version,
    compressionType compression
)
:
    Istream(format, version, compression),
    name_(name),
    is_(is)
{
    if (is_.good())
    {
        setOpened();
        setGood();
    }
    else
    {
        setState(is_.rdstate());
    }
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

inline CML::ISstream& CML::ISstream::get(char& c)
{
    is_.get(c);
    setState(is_.rdstate());

    if (c == '\n')
    {
        lineNumber_++;
    }

    return *this;
}


inline int CML::ISstream::peek()
{
    return is_.peek();
}


inline CML::ISstream& CML::ISstream::getLine(string& s)
{
    getline(is_, s);
    setState(is_.rdstate());
    lineNumber_++;

    return *this;
}


inline CML::ISstream& CML::ISstream::putback(const char& c)
{
    if (c == '\n')
    {
        lineNumber_--;
    }

    if (!is_.putback(c))
    {
        setBad();
    }

    setState(is_.rdstate());

    return *this;
}


// ************************************************************************* //
