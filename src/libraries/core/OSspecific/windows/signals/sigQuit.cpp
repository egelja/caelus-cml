/*---------------------------------------------------------------------------*\
Copyright (C) 2011 Symscape
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

Class
    sigQuit

\*---------------------------------------------------------------------------*/

#include "error.hpp"
#include "sigQuit.hpp"
#include "JobInfo.hpp"
#include "IOstreams.hpp"

// SIGBREAK is best alternative to SIGQUIT on windows

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

__p_sig_fn_t CML::sigQuit::oldAction_ = SIG_DFL;

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::sigQuit::sigQuitHandler(int)
{
    // Reset old handling
    const __p_sig_fn_t success = ::signal(SIGBREAK, oldAction_);

    if (SIG_ERR == success)
    {
        FatalErrorInFunction   
            << "Cannot reset SIGBREAK trapping"
            << abort(FatalError);    
    }

    // Update jobInfo file
    jobInfo.signalEnd();

    error::printStack(Perr);

    // Throw signal (to old handler)
    ::raise(SIGBREAK);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::sigQuit::sigQuit()
{
    oldAction_ = SIG_DFL;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::sigQuit::~sigQuit()
{
    // Reset old handling
    if (SIG_DFL != oldAction_)
    {
        const __p_sig_fn_t success = ::signal(SIGBREAK, oldAction_);
        oldAction_ = SIG_DFL;

        if (SIG_ERR == success)
        {
            FatalErrorInFunction
                << "Cannot reset SIGBREAK trapping"
                << abort(FatalError);    
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::sigQuit::set(const bool verbose)
{
    if (SIG_DFL != oldAction_)
    {
        FatalErrorInFunction   
            << "Cannot call sigQuit::set() more than once"
            << abort(FatalError);
    }

    oldAction_ = ::signal(SIGBREAK, &CML::sigQuit::sigQuitHandler);        

    if (SIG_ERR == oldAction_)
    {
        oldAction_ = SIG_DFL;

        // Not a FatalErrorInFunction or abort because fails under wine
        WarningInFunction
            << "Cannot set SIGBREAK trapping" << endl;
    }
}


// ************************************************************************* //
