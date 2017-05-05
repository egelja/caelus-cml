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

\*---------------------------------------------------------------------------*/

#include "error.hpp"
#include "sigFpe.hpp"

#include "JobInfo.hpp"
#include "OSspecific.hpp"
#include "IOstreams.hpp"

#include <float.h> // *fp functions


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

__p_sig_fn_t CML::sigFpe::oldAction_ = SIG_DFL;

static unsigned int fpOld_ = 0;


static void clearFpe()
{
    _clearfp ();
    _controlfp (fpOld_, 0xFFFFFFFF);
}


void CML::sigFpe::sigFpeHandler(int)
{
    const __p_sig_fn_t success = ::signal(SIGFPE, oldAction_);

    // Reset old handling
    if (SIG_ERR == success)
    {
        FatalErrorIn
        (
            "CML::sigSegv::sigFpeHandler()"
        )   << "Cannot reset SIGFPE trapping"
            << abort(FatalError);    
    }

    // Update jobInfo file
    jobInfo.signalEnd();

    error::printStack(Perr);

    clearFpe();

    // Throw signal (to old handler)
    ::raise(SIGFPE);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::sigFpe::sigFpe()
{
    oldAction_ = SIG_DFL;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::sigFpe::~sigFpe()
{
    if (env("CAELUS_SIGFPE"))
    {
        clearFpe();

        // Reset signal
        const __p_sig_fn_t success = ::signal(SIGFPE, oldAction_);
        oldAction_ = SIG_DFL;

        if (SIG_ERR == success)
        {
            FatalErrorIn
            (
                "CML::sigFpe::~sigFpe()"
            )   << "Cannot reset SIGFPE trapping"
                << abort(FatalError);    
        }
    }

    if (env("CAELUS_SETNAN"))
    {
        WarningIn("CML::sigFpe::~sigFpe()")
            << "CAELUS_SETNAN not supported under MSwindows "
            << endl;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::sigFpe::set(const bool verbose)
{
    if (SIG_DFL != oldAction_)
    {
        FatalErrorIn
        (
            "CML::sigFpe::set()"
        )   << "Cannot call sigFpe::set() more than once"
            << abort(FatalError);
    }

    if (env("CAELUS_SIGFPE"))
    {
        if (verbose)
        {
            Info<< "SigFpe : Enabling floating point exception trapping"
                << " (CAELUS_SIGFPE)." << endl;
        }

        fpOld_ = _controlfp(0, 0);
        const unsigned int fpNew = 
          fpOld_ & ~(_EM_ZERODIVIDE | _EM_INVALID | _EM_OVERFLOW);
        _controlfp(fpNew, _MCW_EM);

        oldAction_ = ::signal(SIGFPE, &CML::sigFpe::sigFpeHandler);        

        if (SIG_ERR == oldAction_)
        {
            oldAction_ = SIG_DFL;

            FatalErrorIn
            (
                "CML::sigFpe::set()"
            )   << "Cannot set SIGFPE trapping"
                << abort(FatalError);    
        }
    }


    if (env("CAELUS_SETNAN"))
    {
        if (verbose)
        {
            WarningIn("CML::sigFpe::set()")
              << "CAELUS_SETNAN not supported under MSwindows "
              << endl;
        }
    }
}


// ************************************************************************* //
