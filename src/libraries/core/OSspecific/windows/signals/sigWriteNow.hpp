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
    CML::sigWriteNow

Description
    Signal handler for interrupt defined by OptimisationSwitches::writeNowSignal

    Write once and continue.

SourceFiles
    sigWriteNow.cpp

\*---------------------------------------------------------------------------*/

#ifndef sigWriteNow_H
#define sigWriteNow_H

#include <signal.h>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class Time;

/*---------------------------------------------------------------------------*\
                           Class sigWriteNow Declaration
\*---------------------------------------------------------------------------*/

class sigWriteNow
{
    // Private data

        //- Number of signal to use
        static int signal_;

        //- Pointer to Time
        static Time* runTimePtr_;

        //- Saved old signal trapping setting
        static __p_sig_fn_t  oldAction_;

    // Private Member Functions

        static void sigHandler(int);


public:

    friend class sigStopAtWriteNow;

    // Constructors

        //- Construct null
        sigWriteNow();

        //- Construct from components
        sigWriteNow(const bool verbose, Time& runTime);


    //- Destructor
    ~sigWriteNow();


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
