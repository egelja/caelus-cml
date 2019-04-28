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
    sigSegv

\*---------------------------------------------------------------------------*/

#include "error.hpp"
#include "sigSegv.hpp"
#include "JobInfo.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

__p_sig_fn_t CML::sigSegv::oldAction_ = SIG_DFL;

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::sigSegv::sigSegvHandler(int)
{
    // Reset old handling
    const __p_sig_fn_t success = ::signal(SIGSEGV, oldAction_);

    if (SIG_ERR == success)
    {
         FatalErrorInFunction
            << "Cannot reset SIGSEGV trapping"
            << abort(FatalError);    
    }

    // Update jobInfo file
    jobInfo.signalEnd();

    error::printStack(Perr);

    // Throw signal (to old handler)
    ::raise(SIGSEGV);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::sigSegv::sigSegv()
{
    oldAction_ = SIG_DFL;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::sigSegv::~sigSegv()
{
    // Reset old handling
    const __p_sig_fn_t success = ::signal(SIGSEGV, oldAction_);
    oldAction_ = SIG_DFL;

    if (SIG_ERR == success)
    {
        FatalErrorInFunction
            << "Cannot reset SIGSEGV trapping"
            << abort(FatalError);    
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::sigSegv::set(const bool verbose)
{
    if (SIG_DFL != oldAction_)
    {
        FatalErrorInFunction
            << "Cannot call sigSegv::set() more than once"
            << abort(FatalError);
    }

    oldAction_ = ::signal(SIGSEGV, &CML::sigSegv::sigSegvHandler);        

    if (SIG_ERR == oldAction_)
    {
        oldAction_ = SIG_DFL;

        FatalErrorInFunction
            << "Cannot set SIGSEGV trapping"
            << abort(FatalError);    
    }
}


// ************************************************************************* //
