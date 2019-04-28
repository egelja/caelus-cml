/*---------------------------------------------------------------------------*\
Copyright (C) 2011 Symscape
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
    sigSegv

Description
    Signal handler for SEGV interrupt.  The standard interrupt handler is
    overridden to ensure that the runningJob file is removed.

SourceFiles
    sigSegv.cpp

\*---------------------------------------------------------------------------*/

#ifndef sigSegv_H
#define sigSegv_H

#include <signal.h>
typedef	void (*__p_sig_fn_t)(int);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class sigSegv Declaration
\*---------------------------------------------------------------------------*/

class sigSegv
{
    // Private data

        //- Saved old signal trapping setting
        static __p_sig_fn_t oldAction_;


    // Private Member Functions

        static void sigSegvHandler(int);


public:

    // Constructors

        sigSegv();


    // Destructor

        ~sigSegv();


    // Member functions

        //- Activate SIGSEGV signal handler
        void set(const bool verbose);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
