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

Description

\*---------------------------------------------------------------------------*/

#include "error.hpp"
#include "windows.hpp"
#include "timer.hpp"

#define WINVER 0x0500 // To access CreateTimerQueueTimer
#include <windows.h>

#define SIGALRM 14


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(CML::timer, 0);

jmp_buf CML::timer::envAlarm;

__p_sig_fn_t CML::timer::oldAction_ = SIG_DFL;

static HANDLE hTimer_ = NULL;

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

void CML::timer::signalHandler(int)
{     
    if (debug)
    {
        Info<< "CML::timer::signalHandler(int sig) : "
            << " timed out. Jumping."
            << endl;
    }
    ::longjmp(envAlarm, 1);
}


static VOID CALLBACK timerExpired(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
    ::raise(SIGALRM);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


// Construct from components
CML::timer::timer(const unsigned int newTimeOut)
:
    newTimeOut_(newTimeOut)
{

    if (newTimeOut > 0)
    {
        // Is singleton since handler is static function
        if (NULL != hTimer_)
        {
            FatalErrorIn
            (
                "CML::timer::timer(const unsigned int)"
            )   << "timer already used."
                << abort(FatalError);    
        }

        // Install alarm signal handler:
        oldAction_ = ::signal(SIGALRM, &CML::timer::signalHandler);

        if (SIG_ERR == oldAction_)
        {
            oldAction_ = SIG_DFL;

            FatalErrorIn
            (
                "CML::timer::timer(const unsigned int)"
            )   << "sigaction(SIGALRM) error"
                << abort(FatalError);    
        }

        if (debug)
        {
            Info<< "CML::timer::timer(const unsigned int) : "
                << " installing timeout " << int(newTimeOut_)
                << " seconds." << endl;
        }

        const bool success = 
          ::CreateTimerQueueTimer(&hTimer_, 
                                  NULL, 
                                  (WAITORTIMERCALLBACK)timerExpired,
                                  NULL , 
                                  newTimeOut * 1000, 
                                  0, 0);

        if (!success) 
        {
            hTimer_ = NULL;
            FatalErrorIn
            (
                "CML::timer::timer(const unsigned int)"
            )   << "CreateTimerQueueTimer, "
                << Windows::getLastError()
                << abort(FatalError);    
        }
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::timer::~timer()
{
    if (newTimeOut_ > 0)
    {
        // Reset timer
        const bool timerSuccess = 
          ::DeleteTimerQueueTimer(NULL, hTimer_, NULL);
        hTimer_ = NULL;

        if (!timerSuccess) 
        {
            FatalErrorIn
            (
                "CML::timer::~timer() "
            )   << "DeleteTimerQueueTimer, "
                << Windows::getLastError()
                << abort(FatalError);    
        }

        if (debug)
        {
            Info<< "CML::timer::~timer() timeOut="
                << int(newTimeOut_) << endl;
        }

        const __p_sig_fn_t signalSuccess = signal(SIGALRM, oldAction_);
        oldAction_ = SIG_DFL;

        // Restore signal handler
        if (SIG_ERR == signalSuccess)
        {
            FatalErrorIn
            (
                "CML::timer::~timer()"
            )   << "sigaction(SIGALRM) error"
                << abort(FatalError);    
        }
    }
}

// ************************************************************************* //
