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
    regionProperties

Description
    Simple class to hold region information for coupled region simulations

SourceFiles
    regionProperties.cpp

\*---------------------------------------------------------------------------*/

#ifndef regionProperties_H
#define regionProperties_H

#include "IOdictionary.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class regionProperties Declaration
\*---------------------------------------------------------------------------*/

class regionProperties
:
    public IOdictionary
{
    // Private data

        //- List of the fluid region names
        List<word> fluidRegionNames_;

        //- List of the solid region names
        List<word> solidRegionNames_;


    // Private Member Functions

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


    // Member Functions

        // Access

            //- Return const reference to the list of fluid region names
            const List<word>& fluidRegionNames() const;

            //- Return const reference to the list of solid region names
            const List<word>& solidRegionNames() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
