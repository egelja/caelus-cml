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
    sigFpe

Description
    Sets up trapping for floating point exceptions (sigfpe).

    Controlled by two env vars:
    CAELUS_SIGFPE : exception trapping
    CAELUS_SETNAN : initialization of all malloced memory to NaN. If also
                  CAELUS_SIGFPE set this will cause usage of uninitialized scalars
                  to trigger an abort.

SourceFiles
    sigFpe.cpp

\*---------------------------------------------------------------------------*/

#ifndef sigFpe_H
#define sigFpe_H

#include <signal.h>
#include "UList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class sigFpe Declaration
\*---------------------------------------------------------------------------*/

class sigFpe
{
    // Private data

        //- Saved old signal trapping setting
        static __p_sig_fn_t oldAction_;


    // Static data members

        //- Handler for caught signals
        static void sigFpeHandler(int);

public:


    // Constructors

        sigFpe();


    // Destructor

        ~sigFpe();


    // Member functions

        //- Activate SIGFPE signal handler when CAELUS_SIGFPE is %set
        //  Fill memory with NaN when CAELUS_SETNAN is %set
        void set(const bool verbose);

        //- Helper: fill block of data with NaN
        static void fillNan(UList<scalar>&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
