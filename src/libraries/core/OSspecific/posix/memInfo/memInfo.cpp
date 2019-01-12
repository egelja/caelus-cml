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
    rss_(-1),
    swap_(0)
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
    swap_ = 0;

    #ifndef darwin
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
                peak_ = int(value);
            }
            else if (!strcmp(tag, "VmSize:"))
            {
                size_ = int(value);
            }
            else if (!strcmp(tag, "VmRSS:"))
            {
                rss_ = int(value);
            }
            else if (!strcmp(tag, "VmSwap:"))
            {
                swap_ = int(value);
            }
            else if (!strcmp(tag, "VmSwap:"))
            {
                swap_ = value;
            }
        }
    }
    #else
    int mib[2];
    int64_t physical_memory;
    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;
    unsigned long length = sizeof(int64_t);
    sysctl(mib, 2, &physical_memory, &length, NULL, 0);

    struct task_basic_info t_info;
    mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;
    task_info
    (
        mach_task_self(),
        TASK_BASIC_INFO,
        (task_info_t)&t_info,
        &t_info_count
    );

    size_ = int(t_info.virtual_size*0.0009765625);
    peak_ = size_;
    rss_ = int(t_info.resident_size*0.0009765625);
    
    if (rss_ > int(physical_memory*0.0009765625))
    {
        swap_ = rss_ - int(physical_memory*0.0009765625);
    }

    #endif

    return *this;
}


bool CML::memInfo::valid() const
{
    return peak_ != -1;
}


bool CML::memInfo::swapping() const
{
    if (swap_ > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
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
        << m.peak() << token::SPACE << m.size() << token::SPACE << m.rss()
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
