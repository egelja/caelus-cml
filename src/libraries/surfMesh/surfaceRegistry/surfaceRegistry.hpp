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
    CML::surfaceRegistry

Description
    Wraps the normal objectRegistry with a local instance for %surfaces.

SourceFiles
    surfaceRegistry.cpp

\*---------------------------------------------------------------------------*/

#ifndef surfaceRegistry_H
#define surfaceRegistry_H

#include "objectRegistry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class surfaceRegistry Declaration
\*---------------------------------------------------------------------------*/

class surfaceRegistry
:
    public objectRegistry
{

    // Private Member Functions

        //- Disallow default bitwise copy construct
        surfaceRegistry(const surfaceRegistry&);

        //- Disallow default bitwise assignment
        void operator=(const surfaceRegistry&);


public:

    //- Runtime type information
    TypeName("surfaceRegistry");

        //- The prefix to local: %surfaces
        static const word prefix;

        //- The default surface name: %default
        static word defaultName;


    // Constructors

        //- Construct for the given objectRegistry and named surface
        surfaceRegistry(const objectRegistry&, const word& surfName = "");


    //- Destructor
    virtual ~surfaceRegistry();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
