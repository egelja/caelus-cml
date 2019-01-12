/*---------------------------------------------------------------------------*\
Copyright (C) 2012 Symscape
Copyright (C) 2018 Applied CCM Pty Ltd
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

#include "sigStopAtWriteNow.hpp"
#include "error.hpp"
#include "JobInfo.hpp"
#include "IOstreams.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

// Signal number to catch
int CML::sigStopAtWriteNow::signal_
(
    debug::optimisationSwitch("stopAtWriteNowSignal", -1)
);

CML::Time const* CML::sigStopAtWriteNow::runTimePtr_ = nullptr;

__p_sig_fn_t CML::sigStopAtWriteNow::oldAction_ = SIG_DFL;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::sigStopAtWriteNow::sigHandler(int)
{
    // Reset old handling
    const __p_sig_fn_t success = ::signal(signal_, oldAction_);
    oldAction_ = SIG_DFL;
    
    if (SIG_ERR == success)
    {
        FatalErrorInFunction
            << "Cannot reset " << signal_ << " trapping"
            << abort(FatalError);
    }

    // Update jobInfo file
    jobInfo.signalEnd();

    Info<< "sigStopAtWriteNow :"
        << " setting up write and stop at end of the next iteration"
        << nl << endl;
    runTimePtr_->stopAt(Time::saWriteNow);

    //// Throw signal (to old handler)
    //raise(signal_);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::sigStopAtWriteNow::sigStopAtWriteNow(){}


CML::sigStopAtWriteNow::sigStopAtWriteNow
(
    const bool verbose,
    const Time& runTime
)
{
    // Store runTime
    runTimePtr_ = &runTime;

    set(verbose);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::sigStopAtWriteNow::~sigStopAtWriteNow()
{
    // Reset old handling
    if (signal_ > 0)
    {
        const __p_sig_fn_t success = ::signal(signal_, oldAction_);
        oldAction_ = SIG_DFL;

        if (SIG_ERR == success)
        {
            FatalErrorInFunction
                << "Cannot reset " << signal_ << " trapping"
                << abort(FatalError);
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::sigStopAtWriteNow::set(const bool verbose)
{
    if (signal_ > 0)
    {
        // Check that the signal is different from the writeNowSignal
        if (sigWriteNow::signal_ == signal_)
        {
            FatalErrorInFunction
                << "stopAtWriteNowSignal : " << signal_
                << " cannot be the same as the writeNowSignal."
                << " Please change this in the etc/controlDict."
                << exit(FatalError);
        }


        oldAction_ = ::signal(signal_, &CML::sigWriteNow::sigHandler);        

        if (SIG_ERR == oldAction_)
        {
            FatalErrorInFunction
                << "Cannot set " << signal_ << " trapping"
                << abort(FatalError);
        }

        if (verbose)
        {
            Info<< "sigStopAtWriteNow :"
                << " Enabling writing and stopping upon signal " << signal_
                << endl;
        }
    }
}


bool CML::sigStopAtWriteNow::active() const
{
    return signal_ > 0;
}


// ************************************************************************* //
