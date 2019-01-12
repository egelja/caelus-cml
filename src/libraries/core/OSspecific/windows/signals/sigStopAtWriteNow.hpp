/*---------------------------------------------------------------------------*\
Copyright (C) 2012 Symscape
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
    CML::sigStopAtWriteNow

Description
    Signal handler for interrupt defined by
        OptimisationSwitches::stopAtWriteNowSignal

    Write and stop the job.

SourceFiles
    sigStopAtWriteNow.cpp

\*---------------------------------------------------------------------------*/

#ifndef sigStopAtWriteNow_H
#define sigStopAtWriteNow_H

#include <signal.h>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class Time;

/*---------------------------------------------------------------------------*\
                           Class sigStopAtWriteNow Declaration
\*---------------------------------------------------------------------------*/

class sigStopAtWriteNow
{
    // Private data

        //- Number of signal to use
        static int signal_;

        //- Pointer to Time
        static Time const* runTimePtr_;

        //- Saved old signal trapping setting
        static __p_sig_fn_t oldAction_;

    // Private Member Functions

        static void sigHandler(int);


public:

    // Constructors

        //- Construct null
        sigStopAtWriteNow();

        //- Construct from components
        sigStopAtWriteNow(const bool verbose, const Time& runTime);


    //- Destructor
    ~sigStopAtWriteNow();


    // Member functions

        //- (re)set signal catcher
        static void set(const bool verbose);

        //- Is active?
        bool active() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
