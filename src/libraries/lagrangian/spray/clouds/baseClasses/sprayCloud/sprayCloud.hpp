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
    CML::sprayCloud

Description
    Virtual abstract base class for templated SprayCloud

SourceFiles
    sprayCloud.cpp

\*---------------------------------------------------------------------------*/

#ifndef sprayCloud_H
#define sprayCloud_H

#include "typeInfo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class sprayCloud Declaration
\*---------------------------------------------------------------------------*/

class sprayCloud
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        sprayCloud(const sprayCloud&);

        //- Disallow default bitwise assignment
        void operator=(const sprayCloud&);


public:

    //- Runtime type information
    TypeName("sprayCloud");

    // Constructors

        //- Null constructor
        sprayCloud();


    //- Destructor
    virtual ~sprayCloud();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
