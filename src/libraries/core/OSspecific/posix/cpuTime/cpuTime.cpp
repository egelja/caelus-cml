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

#include "cpuTime.hpp"
#include <unistd.h>

// * * * * * * * * * * * * * * * Static Members  * * * * * * * * * * * * * * //

const long CML::cpuTime::Hz_(sysconf(_SC_CLK_TCK));


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::cpuTime::getTime(timeType& t)
{
    times(&t);
}


double CML::cpuTime::timeDifference(const timeType& beg, const timeType& end)
{
    return
    (
        double
        (
            (end.tms_utime + end.tms_stime)
          - (beg.tms_utime + beg.tms_stime)
        )/Hz_
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::cpuTime::cpuTime()
{
    getTime(startTime_);
    lastTime_ = startTime_;
    newTime_ = startTime_;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

double CML::cpuTime::elapsedCpuTime() const
{
    getTime(newTime_);
    return timeDifference(startTime_, newTime_);
}


double CML::cpuTime::cpuTimeIncrement() const
{
    lastTime_ = newTime_;
    getTime(newTime_);
    return timeDifference(lastTime_, newTime_);
}


// ************************************************************************* //
