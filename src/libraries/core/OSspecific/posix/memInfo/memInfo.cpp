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

#include "memInfo.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::memInfo::memInfo()
:
    peak_(-1),
    size_(-1),
    rss_(-1)
{
    update();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::memInfo::~memInfo()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const CML::memInfo& CML::memInfo::update()
{
    // reset to invalid values first
    peak_ = size_ = rss_ = -1;
    IFstream is("/proc/" + name(pid()) + "/status");

    while (is.good())
    {
        string line;
        is.getLine(line);
        char tag[32];
        int value;

        if (sscanf(line.c_str(), "%30s %d", tag, &value) == 2)
        {
            if (!strcmp(tag, "VmPeak:"))
            {
                peak_ = value;
            }
            else if (!strcmp(tag, "VmSize:"))
            {
                size_ = value;
            }
            else if (!strcmp(tag, "VmRSS:"))
            {
                rss_ = value;
            }
        }
    }

    return *this;
}


bool CML::memInfo::valid() const
{
    return peak_ != -1;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, memInfo& m)
{
    is.readBegin("memInfo");

    is  >> m.peak_ >> m.size_ >> m.rss_;

    is.readEnd("memInfo");

    // Check state of Istream
    is.check
    (
        "CML::Istream& CML::operator>>(CML::Istream&, CML::memInfo&)"
    );

    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const memInfo& m)
{
    os  << token::BEGIN_LIST
        << m.peak_ << token::SPACE << m.size_ << token::SPACE << m.rss_
        << token::END_LIST;

    // Check state of Ostream
    os.check
    (
        "CML::Ostream& CML::operator<<(CML::Ostream&, "
        "const CML::memInfo&)"
    );

    return os;
}


// ************************************************************************* //
