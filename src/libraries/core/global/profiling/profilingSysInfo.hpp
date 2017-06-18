/*---------------------------------------------------------------------------*\
Copyright (C) 2016 OpenCFD Ltd
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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

Class
    CML::profiling::SysInfo

Description
    General system information

SourceFiles
    profilingSysInfo.cpp

\*---------------------------------------------------------------------------*/

#ifndef profilingSysInfo_HPP
#define profilingSysInfo_HPP

#include "profiling.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class Ostream;

/*---------------------------------------------------------------------------*\
                     Class profiling::sysInfo Declaration
\*---------------------------------------------------------------------------*/

class profiling::sysInfo
{
    // Private Static Data Members


    // Private Data Members


    // Private Member Functions

        //- Disallow default bitwise copy construct
        sysInfo(const sysInfo&);

        //- Disallow default bitwise assignment
        void operator=(const sysInfo&);


protected:

    // Friendship

        friend class profiling;


    // Member Functions

public:


    // Constructors

        //- Construct from components
        sysInfo();


    //- Destructor
    ~sysInfo();


    // Member Functions

    // Access


    // Edit

        //- Update it with a new timing information
        void update();

        //- Write the profiling times, optionally with additional values
        //  Use dictionary format.
        Ostream& write(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
