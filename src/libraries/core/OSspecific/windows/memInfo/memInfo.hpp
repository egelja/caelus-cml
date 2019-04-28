/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Modifications

    This file has been created by blueCAPE's unofficial mingw patches for
    OpenFOAM.
    For more information about these patches, visit:
        http://bluecfd.com/Core

    Modifications made:
      - Source for modifications: http://stackoverflow.com/a/633112

Class
    CML::memInfo

Description
    Memory usage information for the process running this object.

Note
    Uses the information from ::GetProcessMemoryInfo.

SourceFiles
    memInfo.cpp

\*---------------------------------------------------------------------------*/

#ifndef memInfo_HPP
#define memInfo_HPP

#include "OSspecific.hpp"
#include "Windows.hpp"
#include "IFstream.hpp"

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
    //- Peak memory used (KB) by the process
    int peak_;

    //- Memory used (KB) by the process
    int size_;

    //- Resident set size (KB) of the process
    int rss_;
	
	//- Amount of swap memory used (KB) (VmSwap in /proc/\<pid\>/status)
    int swap_;
        
    //- Function pointer for the kernel based function
    void *GetProcessMemoryInfo_;


public:

    //- Construct null
    memInfo();


    //- Destructor
    ~memInfo();


    // Member Functions

    //- Update 
    const memInfo& update();

    //- Access the stored peak memory
    //  The value is stored from the previous update()
    int peak() const
    {
        return peak_;
    }

    //- Access the stored memory size
    //  The value is stored from the previous update()
    int size() const
    {
        return size_;
    }

    //- Access the stored rss value
    //  The value is stored from the previous update()
    int rss() const
    {
        return rss_;
    }

    //- Access the stored swap value
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


} // End namespace CML
#endif
