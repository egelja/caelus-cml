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

#include "sigQuit.hpp"
#include "error.hpp"
#include "JobInfo.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

struct sigaction CML::sigQuit::oldAction_;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::sigQuit::sigHandler(int)
{
    // Reset old handling
    if (sigaction(SIGQUIT, &oldAction_, NULL) < 0)
    {
        FatalErrorIn
        (
            "CML::sigQuit::sigHandler()"
        )   << "Cannot reset SIGQUIT trapping"
            << abort(FatalError);
    }

    // Update jobInfo file
    jobInfo.signalEnd();

    error::printStack(Perr);

    // Throw signal (to old handler)
    raise(SIGQUIT);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::sigQuit::sigQuit()
{
    oldAction_.sa_handler = NULL;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::sigQuit::~sigQuit()
{
    // Reset old handling
    if (oldAction_.sa_handler && sigaction(SIGQUIT, &oldAction_, NULL) < 0)
    {
        FatalErrorIn
        (
            "CML::sigQuit::~sigQuit()"
        )   << "Cannot reset SIGQUIT trapping"
            << abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::sigQuit::set(const bool verbose)
{
    if (oldAction_.sa_handler)
    {
        FatalErrorIn
        (
            "CML::sigQuit::set()"
        )   << "Cannot call sigQuit::set() more than once"
            << abort(FatalError);
    }

    struct sigaction newAction;
    newAction.sa_handler = sigHandler;
    newAction.sa_flags = SA_NODEFER;
    sigemptyset(&newAction.sa_mask);
    if (sigaction(SIGQUIT, &newAction, &oldAction_) < 0)
    {
        FatalErrorIn
        (
            "CML::sigQuit::set()"
        )   << "Cannot set SIGQUIT trapping"
            << abort(FatalError);
    }
}


// ************************************************************************* //
