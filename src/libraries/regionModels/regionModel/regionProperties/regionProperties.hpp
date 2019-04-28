/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::regionProperties

Description
    Simple class to hold region information for coupled region simulations.

    Gives per physics ('fluid', 'solid') the names of the regions. There
    is no assumption on this level that one region should only have one
    set of physics.

SourceFiles
    regionProperties.cpp

\*---------------------------------------------------------------------------*/

#ifndef regionProperties_HPP
#define regionProperties_HPP

#include "Time.hpp"
#include "HashTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class regionProperties Declaration
\*---------------------------------------------------------------------------*/

class regionProperties
:
    public HashTable<wordList>
{

        //- Disallow default bitwise copy construct
        regionProperties(const regionProperties&);

        //- Disallow default bitwise assignment
        void operator=(const regionProperties&);

public:

    // Constructors

        //- Construct from components
        regionProperties(const Time& runTime);


    //- Destructor
    ~regionProperties();

};


const word& regionDir(const word& regionName);

wordList selectRegionNames(const argList& args, const Time& runTime);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
