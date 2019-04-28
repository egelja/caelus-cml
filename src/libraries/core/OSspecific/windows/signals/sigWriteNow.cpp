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

CML::Time* CML::sigWriteNow::runTimePtr_ = nullptr;


__p_sig_fn_t CML::sigWriteNow::oldAction_ = SIG_DFL;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::sigWriteNow::sigHandler(int)
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
    // Store runTime
    runTimePtr_ = &runTime;

    set(verbose);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::sigWriteNow::~sigWriteNow()
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

void CML::sigWriteNow::set(const bool verbose)
{
    if (signal_ >= 0)
    {
        oldAction_ = ::signal(signal_, &CML::sigWriteNow::sigHandler);        

        if (SIG_ERR == oldAction_)
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


bool CML::sigWriteNow::active() const
{
    return signal_ > 0;
}


// ************************************************************************* //
