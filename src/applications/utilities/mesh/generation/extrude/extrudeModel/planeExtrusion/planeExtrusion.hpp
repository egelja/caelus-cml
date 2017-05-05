/*---------------------------------------------------------------------------*\
Copyright (C) 2015 OpenFOAM Foundation
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
    CML::extrudeModels::plane

Description
    Extrudes by transforming points normal to the surface by 1 layer over
    a given distance.

SeeAlso
    CML::extrudeModels::linearNormal

\*---------------------------------------------------------------------------*/

#ifndef plane_H
#define plane_H

#include "linearNormal.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace extrudeModels
{

/*---------------------------------------------------------------------------*\
                         Class plane Declaration
\*---------------------------------------------------------------------------*/

class plane
:
    public linearNormal
{

public:

    //- Runtime type information
    TypeName("plane");

    // Constructors

        //- Construct from dictionary
        plane(const dictionary& dict);


    //- Destructor
    virtual ~plane();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace extrudeModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
