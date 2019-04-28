/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
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
    CML::memInfo

Description
    Memory usage information for the process running this object.

Note
    Uses the information from /proc/\<pid\>/status

SourceFiles
    memInfo.cpp

\*---------------------------------------------------------------------------*/

#ifndef memInfo_H
#define memInfo_H

#include "OSspecific.hpp"
#include "posix.hpp"
#include "IFstream.hpp"

#ifdef darwin
#include <sys/types.h>
#include <sys/sysctl.h>
#include<mach/mach.h>
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class memInfo;

Istream& operator>>(Istream&, memInfo&);
Ostream& operator<<(Ostream&, const memInfo&);


/*---------------------------------------------------------------------------*\
                           Class memInfo Declaration
\*---------------------------------------------------------------------------*/

class memInfo
{
    // Private data

        //- Peak memory (KB) by the process (VmPeak in /proc/\<pid\>/status)
        int peak_;

        //- Memory used (KB) the process (VmSize in /proc/\<pid\>/status)
        int size_;

        //- Resident set size (KB) the process (VmRSS in /proc/\<pid\>/status)
        int rss_;

        //- Amount of swap memory used (KB) VmSwap in /proc/\<pid\>/status)
        int swap_;


public:

    // Constructors

        //- Construct null
        memInfo();


    //- Destructor
    ~memInfo();


    // Member Functions

        //- Parse /proc/\<pid\>/status
        const memInfo& update();

        // Access

            //- Access the stored peak memory (VmPeak in /proc/\<pid\>/status)
            //  The value is stored from the previous update()
            int peak() const
            {
                return peak_;
            }

            //- Access the stored memory size (VmSize in /proc/\<pid\>/status)
            //  The value is stored from the previous update()
            int size() const
            {
                return size_;
            }

            //- Access the stored rss value (VmRSS in /proc/\<pid\>/status)
            //  The value is stored from the previous update()
            int rss() const
            {
                return rss_;
            }

            //- Access the stored swap value (VmRSS in /proc/\<pid\>/status)
            //  The value is stored from the previous update()
            int swap() const
            {
                return swap_;
            }

            //- True if the memory information appears valid
            bool valid() const;

            //- True if the processing is using swap memory
            bool swapping() const;



    // IOstream Operators

        //- Read peak/size/rss from stream
        friend Istream& operator>>(Istream&, memInfo&);

        //- Write peak/size/rss to stream
        friend Ostream& operator<<(Ostream&, const memInfo&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
