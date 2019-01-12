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

#include "sigWriteNow.hpp"
#include "error.hpp"
#include "JobInfo.hpp"
#include "IOstreams.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

// Signal number to catch
int CML::sigWriteNow::signal_
(
    debug::optimisationSwitch("writeNowSignal", -1)
);

static CML::Time* runTimePtr_ = nullptr;


struct sigaction CML::sigWriteNow::oldAction_;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::sigWriteNow::sigHandler(int)
{
    Info<< "sigWriteNow :"
        << " setting up write at end of the next iteration" << nl << endl;
    runTimePtr_->writeOnce();

    //// Throw signal (to old handler)
    //raise(signal_);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::sigWriteNow::sigWriteNow()
{}


CML::sigWriteNow::sigWriteNow(const bool verbose, Time& runTime)
{
    if (signal_ >= 0)
    {
        // Store runTime
        runTimePtr_ = &runTime;

        struct sigaction newAction;
        newAction.sa_handler = sigHandler;
        newAction.sa_flags = SA_NODEFER;
        sigemptyset(&newAction.sa_mask);
        if (sigaction(signal_, &newAction, &oldAction_) < 0)
        {
            FatalErrorInFunction
                << "Cannot set " << signal_ << " trapping"
                << abort(FatalError);
        }

        if (verbose)
        {
            Info<< "sigWriteNow :"
                << " Enabling writing upon signal " << signal_
                << endl;
        }
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::sigWriteNow::~sigWriteNow()
{
    // Reset old handling
    if (signal_ > 0)
    {
        if (sigaction(signal_, &oldAction_, nullptr) < 0)
        {
            FatalErrorInFunction
                << "Cannot reset " << signal_ << " trapping"
                << abort(FatalError);
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::sigWriteNow::active() const
{
    return signal_ > 0;
}


// ************************************************************************* //
