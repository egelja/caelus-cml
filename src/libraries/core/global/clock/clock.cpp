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

#include "clock.hpp"
#include "string.hpp"

#include <sstream>
#include <iomanip>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const char *CML::clock::monthNames[] =
{
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
};


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

time_t CML::clock::getTime()
{
    return ::time(reinterpret_cast<time_t*>(0));
}


const struct tm CML::clock::rawDate()
{
    time_t t = getTime();
    struct tm *timeStruct = localtime(&t);
    return *timeStruct;
}


CML::string CML::clock::dateTime()
{
    std::ostringstream osBuffer;

    time_t t = getTime();
    struct tm *timeStruct = localtime(&t);

    osBuffer
        << std::setfill('0')
        << std::setw(4) << timeStruct->tm_year + 1900
        << '-' << std::setw(2) << timeStruct->tm_mon + 1
        << '-' << std::setw(2) << timeStruct->tm_mday
        << 'T'
        << std::setw(2) << timeStruct->tm_hour
        << ':' << std::setw(2) << timeStruct->tm_min
        << ':' << std::setw(2) << timeStruct->tm_sec;

    return osBuffer.str();
}

CML::string CML::clock::date()
{
    std::ostringstream osBuffer;

    time_t t = getTime();
    struct tm *timeStruct = localtime(&t);

    osBuffer
        << monthNames[timeStruct->tm_mon]
        << ' ' << std::setw(2) << std::setfill('0') << timeStruct->tm_mday
        << ' ' << std::setw(4) << timeStruct->tm_year + 1900;

    return osBuffer.str();
}


CML::string CML::clock::clockTime()
{
    std::ostringstream osBuffer;

    time_t t = getTime();
    struct tm *timeStruct = localtime(&t);

    osBuffer
        << std::setfill('0')
        << std::setw(2) << timeStruct->tm_hour
        << ':' << std::setw(2) << timeStruct->tm_min
        << ':' << std::setw(2) << timeStruct->tm_sec;

    return osBuffer.str();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::clock::clock()
:
    startTime_(getTime()),
    lastTime_(startTime_),
    newTime_(startTime_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::clock::TIME_T CML::clock::elapsedClockTime() const
{
    newTime_ = getTime();
    return newTime_ - startTime_;
}


CML::clock::TIME_T CML::clock::clockTimeIncrement() const
{
    lastTime_ = newTime_;
    newTime_ = getTime();
    return newTime_ - lastTime_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// ************************************************************************* //
