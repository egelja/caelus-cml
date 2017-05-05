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
    sigInt

\*---------------------------------------------------------------------------*/

#include "error.hpp"
#include "sigInt.hpp"
#include "JobInfo.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

__p_sig_fn_t CML::sigInt::oldAction_ = SIG_DFL;

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::sigInt::sigIntHandler(int)
{
    // Reset old handling
    const __p_sig_fn_t success = ::signal(SIGINT, oldAction_);

    if (SIG_ERR == success)
    {
        FatalErrorIn
        (
            "CML::sigInt::sigIntHandler()"
        )   << "Cannot reset SIGINT trapping"
            << abort(FatalError);
    }

    // Update jobInfo file
    jobInfo.signalEnd();

    // Throw signal (to old handler)
    ::raise(SIGINT);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::sigInt::sigInt()
{
    oldAction_ = SIG_DFL;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::sigInt::~sigInt()
{
    // Reset old handling
    const __p_sig_fn_t success = ::signal(SIGINT, oldAction_);
    oldAction_ = SIG_DFL;

    if (SIG_ERR == success)
    {
         FatalErrorIn
        (
            "CML::sigInt::~sigInt()"
        )   << "Cannot reset SIGINT trapping"
            << abort(FatalError);    
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::sigInt::set(const bool verbose)
{
    if (SIG_DFL != oldAction_)
    {
        FatalErrorIn
        (
            "CML::sigInt::set()"
        )   << "Cannot call sigInt::set() more than once"
            << abort(FatalError);
    }

    oldAction_ = ::signal(SIGINT, &CML::sigInt::sigIntHandler);        

    if (SIG_ERR == oldAction_)
    {
        oldAction_ = SIG_DFL;
 
        FatalErrorIn
        (
            "CML::sigInt::set()"
        )   << "Cannot set SIGINT trapping"
            << abort(FatalError);    
    }
}


// ************************************************************************* //
