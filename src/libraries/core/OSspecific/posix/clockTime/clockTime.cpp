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

#include "clockTime.hpp"
#include <sys/time.h>

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::clockTime::getTime(timeType& t)
{
    gettimeofday(&t, 0);
}


double CML::clockTime::timeDifference(const timeType& beg, const timeType& end)
{
    return end.tv_sec - beg.tv_sec + 1E-6*(end.tv_usec - beg.tv_usec);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::clockTime::clockTime()
{
    getTime(startTime_);
    lastTime_ = startTime_;
    newTime_ = startTime_;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

double CML::clockTime::elapsedTime() const
{
    getTime(newTime_);
    return timeDifference(startTime_, newTime_);
}


double CML::clockTime::timeIncrement() const
{
    lastTime_ = newTime_;
    getTime(newTime_);
    return timeDifference(lastTime_, newTime_);
}


// ************************************************************************* //
