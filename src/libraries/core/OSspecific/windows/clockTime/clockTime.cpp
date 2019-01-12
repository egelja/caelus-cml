/*---------------------------------------------------------------------------*\
    Copyright            : (C) 2011 Symscape
    Website              : www.symscape.com
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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Static Members  * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void clockTime::getTime(timeType& t)
{
    gettimeofday(&t, nullptr);
}


double clockTime::timeDifference(const timeType& start, const timeType& end)
{
    return end.tv_sec - start.tv_sec + 1e-6*(end.tv_usec - start.tv_usec);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

clockTime::clockTime()
{
    getTime(startTime_);
    lastTime_ = startTime_;
    newTime_ = startTime_;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

double clockTime::elapsedTime() const
{
    getTime(newTime_);
    return timeDifference(startTime_, newTime_);
}


double clockTime::timeIncrement() const
{
    lastTime_ = newTime_;
    getTime(newTime_);
    return timeDifference(lastTime_, newTime_);
}


} // End namespace CML

// ************************************************************************* //
