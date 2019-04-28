/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "sigSegv.hpp"
#include "error.hpp"
#include "JobInfo.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

struct sigaction CML::sigSegv::oldAction_;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::sigSegv::sigHandler(int)
{
    // Reset old handling
    if (sigaction(SIGSEGV, &oldAction_, nullptr) < 0)
    {
        FatalErrorInFunction
            << "Cannot reset SIGSEGV trapping"
            << abort(FatalError);
    }

    // Update jobInfo file
    jobInfo.signalEnd();

    error::printStack(Perr);

    // Throw signal (to old handler)
    raise(SIGSEGV);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::sigSegv::sigSegv()
{
    oldAction_.sa_handler = nullptr;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::sigSegv::~sigSegv()
{
    // Reset old handling
    if (sigaction(SIGSEGV, &oldAction_, nullptr) < 0)
    {
        FatalErrorInFunction
            << "Cannot reset SIGSEGV trapping"
            << abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::sigSegv::set(const bool)
{
    if (oldAction_.sa_handler)
    {
        FatalErrorInFunction
           << "Cannot call sigSegv::set() more than once"
            << abort(FatalError);
    }

    struct sigaction newAction;
    newAction.sa_handler = sigHandler;
    newAction.sa_flags = SA_NODEFER;
    sigemptyset(&newAction.sa_mask);
    if (sigaction(SIGSEGV, &newAction, &oldAction_) < 0)
    {
        FatalErrorInFunction
            << "Cannot set SIGSEGV trapping"
            << abort(FatalError);
    }
}


// ************************************************************************* //
