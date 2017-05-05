/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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
    CML::JobInfo

Description
    Helper class for recording information about run/finished jobs.

    Writes the following files:
      - $CAELUS_JOB_DIR/runningJobs
      - $CAELUS_JOB_DIR/finishedJobs

SourceFiles
    JobInfo.cpp

\*---------------------------------------------------------------------------*/

#ifndef JobInfo_H
#define JobInfo_H

#include "dictionary.hpp"
#include "fileName.hpp"
#include "cpuTime.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class JobInfo Declaration
\*---------------------------------------------------------------------------*/

class JobInfo
:
    public dictionary
{
    // Private data
        fileName runningJobPath_;
        fileName finishedJobPath_;
        cpuTime  cpuTime_;

    // Private Member Functions

        bool write(Ostream&) const;
        void end(const word& terminationType);


public:

    static bool constructed;
    static bool writeJobInfo;

    // Constructors

        //- Construct null
        JobInfo();


    //- Destructor
    ~JobInfo();


    // Member Functions

        // Write

            void write() const;

            void end();

            void exit();

            void abort();

            void signalEnd() const;
};


extern JobInfo jobInfo;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
