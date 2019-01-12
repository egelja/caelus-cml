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

Class
    clockTime

Description
    Starts timing (using rtc) and returns elapsed time from start.
    Better resolution (2uSec instead of ~20mSec) than cpuTime.

SourceFiles
    clockTime.C

\*---------------------------------------------------------------------------*/

#ifndef clockTime_H
#define clockTime_H

#include <sys/types.h>
#include <sys/time.h> // timeval definition

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                          Class clockTime Declaration
\*---------------------------------------------------------------------------*/

class clockTime
{
    // Private data

        //- Time structure used
        typedef struct timeval timeType;

        timeType startTime_;

        mutable timeType lastTime_;
        mutable timeType newTime_;

    // Private Member Functions

        //- Retrieve the current time values from the system
        static void getTime(timeType&);

        //- Difference between two times
        static double timeDifference(const timeType& start, const timeType& end);


public:

    // Constructors

        //- Construct with the current clock time
        clockTime();


    // Member Functions

        //- Return time (in seconds) from the start
        double elapsedTime() const;

        //- Return time (in seconds) since last call to timeIncrement()
        double timeIncrement() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
