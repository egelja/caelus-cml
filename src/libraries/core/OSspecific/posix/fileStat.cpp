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

#include "fileStat.hpp"
#include "IOstreams.hpp"
#include "timer.hpp"

#include <signal.h>
#include <unistd.h>
#ifndef darwin
#include <sys/sysmacros.h>
#endif

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::fileStat::fileStat()
:
    isValid_(false)
{}


CML::fileStat::fileStat(const fileName& fName, const unsigned int maxTime)
{
    // Work on volatile
    volatile bool locIsValid = false;

    timer myTimer(maxTime);

    if (!timedOut(myTimer))
    {
        if (::stat(fName.c_str(), &status_) != 0)
        {
            locIsValid = false;
        }
        else
        {
            locIsValid = true;
        }
    }

    // Copy into (non-volatile, possible register based) member var
    isValid_ = locIsValid;
}


CML::fileStat::fileStat(Istream& is)
{
    is >> *this;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::fileStat::sameDevice(const fileStat& stat2) const
{
    return
        isValid_
     && (
            major(status_.st_dev) == major(stat2.status().st_dev)
         && minor(status_.st_dev) == minor(stat2.status().st_dev)
        );
}


bool CML::fileStat::sameINode(const fileStat& stat2) const
{
    return isValid_ && (status_.st_ino == stat2.status().st_ino);
}


bool CML::fileStat::sameINode(const label iNode) const
{
    return isValid_ && (status_.st_ino == ino_t(iNode));
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, fileStat& fStat)
{
    FixedList<label, 13> stat(is);

    fStat.isValid_ = stat[0];

    dev_t st_dev = makedev(stat[1], stat[2]);
    fStat.status_.st_dev = st_dev;

    fStat.status_.st_ino = stat[3];
    fStat.status_.st_mode = stat[4];
    fStat.status_.st_uid = stat[5];
    fStat.status_.st_gid = stat[6];

    dev_t st_rdev = makedev(stat[7], stat[8]);
    fStat.status_.st_rdev = st_rdev;

    fStat.status_.st_size = stat[9];
    fStat.status_.st_atime = stat[10];
    fStat.status_.st_mtime = stat[11];
    fStat.status_.st_ctime = stat[12];

    // Check state of Istream
    is.check("Istream& operator>>(Istream&, fileStat&)");

    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const fileStat& fStat)
{
    FixedList<label, 13> stat;

    stat[0] = label(fStat.isValid_);
    stat[1] = label(major(fStat.status_.st_dev));
    stat[2] = label(minor(fStat.status_.st_dev));
    stat[3] = label(fStat.status_.st_ino);
    stat[4] = label(fStat.status_.st_mode);
    stat[5] = label(fStat.status_.st_uid);
    stat[6] = label(fStat.status_.st_gid);
    stat[7] = label(major(fStat.status_.st_rdev));
    stat[8] = label(minor(fStat.status_.st_rdev));
    stat[9] = label(fStat.status_.st_size);
    stat[10] = label(fStat.status_.st_atime);
    stat[11] = label(fStat.status_.st_mtime);
    stat[12] = label(fStat.status_.st_ctime);

    return os << stat;
}


// ************************************************************************* //
