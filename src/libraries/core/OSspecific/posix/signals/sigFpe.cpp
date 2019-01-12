/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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

#include "sigFpe.hpp"
#include "error.hpp"
#include "JobInfo.hpp"
#include "OSspecific.hpp"
#include "IOstreams.hpp"

#ifdef LINUX_GNUC

#   ifndef __USE_GNU
#       define __USE_GNU
#   endif

#   include <fenv.h>
#   include <malloc.h>

#elif defined(sgiN32) || defined(sgiN32Gcc)

#   include <sigfpe.h>

#elif defined(__APPLE__)

// #   include <fenv.h>
#include <xmmintrin.h>

#endif

#include <stdint.h>

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

struct sigaction CML::sigFpe::oldAction_;


#ifdef LINUX

void *(*CML::sigFpe::oldMallocHook_)(size_t, const void *) = nullptr;

void* CML::sigFpe::nanMallocHook_(size_t size, const void *caller)
{
    void *result;

    // Restore all old hooks
    __malloc_hook = oldMallocHook_;

    // Call recursively
    result = malloc(size);

    // initialize to signalling NaN
#   ifdef WM_SP

    const uint32_t sNAN = 0x7ff7fffflu;
    uint32_t* dPtr = reinterpret_cast<uint32_t*>(result);

#   else

    const uint64_t sNAN = 0x7ff7ffffffffffffllu;
    uint64_t* dPtr = reinterpret_cast<uint64_t*>(result);

#   endif

    const size_t nScalars = size/sizeof(scalar);
    for (size_t i = 0; i < nScalars; ++i)
    {
        *dPtr++ = sNAN;
    }


    // Restore our own hooks
    __malloc_hook = nanMallocHook_;

    return result;
}

#endif


#if defined(LINUX_GNUC) || defined(__APPLE__)

void CML::sigFpe::sigHandler(int)
{
    // Reset old handling
    if (sigaction(SIGFPE, &oldAction_, nullptr) < 0)
    {
        FatalErrorInFunction
            << "Cannot reset SIGFPE trapping"
            << abort(FatalError);
    }

    // Update jobInfo file
    jobInfo.signalEnd();

    error::printStack(Perr);

    // Throw signal (to old handler)
    raise(SIGFPE);
}

#endif


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::sigFpe::sigFpe()
{
    oldAction_.sa_handler = nullptr;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::sigFpe::~sigFpe()
{
    if (env("CAELUS_SIGFPE"))
    {
#       ifdef LINUX_GNUC

        // Reset signal
        if (oldAction_.sa_handler && sigaction(SIGFPE, &oldAction_, nullptr) < 0)
        {
            FatalErrorInFunction
                << "Cannot reset SIGFPE trapping"
                << abort(FatalError);
        }

#       endif
    }

    if (env("CAELUS_SETNAN"))
    {
#       ifdef LINUX_GNUC

        // Reset to standard malloc
        if (oldAction_.sa_handler)
        {
            __malloc_hook = oldMallocHook_;
        }

#       endif
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::sigFpe::set(const bool verbose)
{
    if (oldAction_.sa_handler)
    {
        FatalErrorInFunction
            << "Cannot call sigFpe::set() more than once"
            << abort(FatalError);
    }

    if (env("CAELUS_SIGFPE"))
    {
        bool supported = false;

#       ifdef LINUX_GNUC
        supported = true;

        feenableexcept
        (
            FE_DIVBYZERO
          | FE_INVALID
          | FE_OVERFLOW
        );

        struct sigaction newAction;
        newAction.sa_handler = sigHandler;
        newAction.sa_flags = SA_NODEFER;
        sigemptyset(&newAction.sa_mask);
        if (sigaction(SIGFPE, &newAction, &oldAction_) < 0)
        {
            FatalErrorInFunction
                << "Cannot set SIGFPE trapping"
                << abort(FatalError);
        }


#       elif defined(sgiN32) || defined(sgiN32Gcc)
        supported = true;

        sigfpe_[_DIVZERO].abort=1;
        sigfpe_[_OVERFL].abort=1;
        sigfpe_[_INVALID].abort=1;

        sigfpe_[_DIVZERO].trace=1;
        sigfpe_[_OVERFL].trace=1;
        sigfpe_[_INVALID].trace=1;

        handle_sigfpes
        (
            _ON,
            _EN_DIVZERO
          | _EN_INVALID
          | _EN_OVERFL,
            0,
            _ABORT_ON_ERROR,
            nullptr
        );

#       elif defined(__APPLE__)

        struct sigaction newAction;
        newAction.sa_handler = sigHandler;
        newAction.sa_flags = SA_NODEFER;
        sigemptyset(&newAction.sa_mask);
        if (sigaction(SIGFPE, &newAction, &oldAction_) < 0)
        {
            FatalErrorInFunction
                << "Cannot set SIGFPE trapping"
                << abort(FatalError);
        }
        _MM_SET_EXCEPTION_MASK(_MM_GET_EXCEPTION_MASK() & ~_MM_MASK_INVALID);
        _MM_SET_EXCEPTION_MASK(_MM_GET_EXCEPTION_MASK() & ~_MM_MASK_DIV_ZERO);
        
        _mm_setcsr( _MM_MASK_MASK &~
        (_MM_MASK_OVERFLOW|_MM_MASK_INVALID|_MM_MASK_DIV_ZERO) );

        supported=true;

#       endif


        if (verbose)
        {
            if (supported)
            {
                Info<< "sigFpe : Enabling floating point exception trapping"
                    << " (CAELUS_SIGFPE)." << endl;
            }
            else
            {
                Info<< "sigFpe : Floating point exception trapping"
                    << " - not supported on this platform" << endl;
            }
        }
    }


    if (env("CAELUS_SETNAN"))
    {
        bool supported = false;

#       ifdef LINUX_GNUC
        supported = true;

        // Set our malloc
        __malloc_hook = CML::sigFpe::nanMallocHook_;

#       endif


        if (verbose)
        {
            if (supported)
            {
            Info<< "SetNaN : Initialising allocated memory to NaN"
                << " (CAELUS_SETNAN)." << endl;
            }
            else
            {
                Info<< "SetNaN : Initialise allocated memory to NaN"
                    << " - not supported on this platform" << endl;
            }
        }
    }
}


// ************************************************************************* //
