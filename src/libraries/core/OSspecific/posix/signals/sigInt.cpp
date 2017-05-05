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

#include "sigInt.hpp"
#include "error.hpp"
#include "JobInfo.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

struct sigaction CML::sigInt::oldAction_;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::sigInt::sigHandler(int)
{
    // Reset old handling
    if (sigaction(SIGINT, &oldAction_, NULL) < 0)
    {
        FatalErrorIn
        (
            "CML::sigInt::sigHandler()"
        )   << "Cannot reset SIGINT trapping"
            << abort(FatalError);
    }

    // Update jobInfo file
    jobInfo.signalEnd();

    // Throw signal (to old handler)
    raise(SIGINT);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::sigInt::sigInt()
{
    oldAction_.sa_handler = NULL;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::sigInt::~sigInt()
{
    // Reset old handling
    if (sigaction(SIGINT, &oldAction_, NULL) < 0)
    {
        FatalErrorIn
        (
            "CML::sigInt::~sigInt()"
        )   << "Cannot reset SIGINT trapping"
            << abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::sigInt::set(const bool)
{
    if (oldAction_.sa_handler)
    {
        FatalErrorIn
        (
            "CML::sigInt::set()"
        )   << "Cannot call sigInt::set() more than once"
            << abort(FatalError);
    }

    struct sigaction newAction;
    newAction.sa_handler = sigHandler;
    newAction.sa_flags = SA_NODEFER;
    sigemptyset(&newAction.sa_mask);
    if (sigaction(SIGINT, &newAction, &oldAction_) < 0)
    {
        FatalErrorIn
        (
            "CML::sigInt::set()"
        )   << "Cannot set SIGINT trapping"
            << abort(FatalError);
    }
}


// ************************************************************************* //
