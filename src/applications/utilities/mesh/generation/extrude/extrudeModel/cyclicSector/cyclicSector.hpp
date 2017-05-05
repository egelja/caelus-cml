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
    CML::extrudeModels::cyclicSector

Description
    Extrudes a sector.

SeeAlso
    CML::extrudeModels::sector

\*---------------------------------------------------------------------------*/

#ifndef cyclicSector_H
#define cyclicSector_H

#include "sector.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace extrudeModels
{

/*---------------------------------------------------------------------------*\
                         Class cyclicSector Declaration
\*---------------------------------------------------------------------------*/

class cyclicSector
:
    public sector
{

public:

    //- Runtime type information
    TypeName("cyclicSector");

    // Constructors

        //- Construct from dictionary
        cyclicSector(const dictionary& dict);


    //- Destructor
    virtual ~cyclicSector();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace extrudeModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
