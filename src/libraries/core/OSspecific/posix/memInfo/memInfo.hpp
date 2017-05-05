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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class memInfo Declaration
\*---------------------------------------------------------------------------*/

class memInfo
{
    // Private data

        //- Peak memory used by the process (VmPeak in /proc/\<pid\>/status)
        int peak_;

        //- Memory used by the process (VmSize in /proc/\<pid\>/status)
        int size_;

        //- Resident set size of the process (VmRSS in /proc/\<pid\>/status)
        int rss_;


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

            //- True if the memory information appears valid
            bool valid() const;


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
