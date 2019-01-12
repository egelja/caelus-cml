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

#include "JobInfo.hpp"
#include "OSspecific.hpp"
#include "clock.hpp"
#include "OFstream.hpp"
#include "Pstream.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

bool CML::JobInfo::writeJobInfo(CML::debug::infoSwitch("writeJobInfo", 0));
CML::JobInfo CML::jobInfo;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Null constructor
CML::JobInfo::JobInfo()
:
    runningJobPath_(),
    finishedJobPath_(),
    cpuTime_()
{
    name() = "JobInfo";

    if (writeJobInfo && Pstream::master())
    {
        string baseDir = getEnv("CAELUS_JOB_DIR");
        string jobFile = hostName() + '.' + CML::name(pid());

        fileName runningDir(baseDir/"runningJobs");
        fileName finishedDir(baseDir/"finishedJobs");

        runningJobPath_  = runningDir/jobFile;
        finishedJobPath_ = finishedDir/jobFile;

        if (baseDir.empty())
        {
            FatalErrorInFunction
                << "Cannot get JobInfo directory $CAELUS_JOB_DIR"
                << CML::exit(FatalError);
        }

        if (!isDir(runningDir) && !mkDir(runningDir))
        {
            FatalErrorInFunction
                << "Cannot make JobInfo directory " << runningDir
                << CML::exit(FatalError);
        }

        if (!isDir(finishedDir) && !mkDir(finishedDir))
        {
            FatalErrorInFunction
                << "Cannot make JobInfo directory " << finishedDir
                << CML::exit(FatalError);
        }
    }

    constructed = true;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::JobInfo::~JobInfo()
{
    if (writeJobInfo && constructed && Pstream::master())
    {
        mv(runningJobPath_, finishedJobPath_);
    }

    constructed = false;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::JobInfo::write(Ostream& os) const
{
    if (writeJobInfo && Pstream::master())
    {
        if (os.good())
        {
            dictionary::write(os, false);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return true;
    }
}


void CML::JobInfo::write() const
{
    if (writeJobInfo && Pstream::master())
    {
        if (!write(OFstream(runningJobPath_)()))
        {
            FatalErrorInFunction
                << "Failed to write to JobInfo file "
                << runningJobPath_
                << CML::exit(FatalError);
        }
    }
}


void CML::JobInfo::end(const word& terminationType)
{
    if (writeJobInfo && constructed && Pstream::master())
    {
        add("cpuTime", cpuTime_.elapsedCpuTime());
        add("endDate", clock::date());
        add("endTime", clock::clockTime());

        if (!found("termination"))
        {
            add("termination", terminationType);
        }

        rm(runningJobPath_);
        write(OFstream(finishedJobPath_)());
    }

    constructed = false;
}


void CML::JobInfo::end()
{
    end("normal");
}


void CML::JobInfo::exit()
{
    end("exit");
}


void CML::JobInfo::abort()
{
    end("abort");
}


void CML::JobInfo::signalEnd() const
{
    if (writeJobInfo && constructed && Pstream::master())
    {
        mv(runningJobPath_, finishedJobPath_);
    }

    constructed = false;
}


// ************************************************************************* //
