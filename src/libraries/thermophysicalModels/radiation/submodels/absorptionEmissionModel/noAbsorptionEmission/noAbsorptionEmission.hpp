/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::radiation::noAbsorptionEmission

Description
    Dummy absorption-emission model for 'none'

SourceFiles
    noAbsorptionEmission.cpp

\*---------------------------------------------------------------------------*/

#ifndef radiationNoAbsorptionEmission_HPP
#define radiationNoAbsorptionEmission_HPP

#include "absorptionEmissionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace radiation
{

/*---------------------------------------------------------------------------*\
                  Class noAbsorptionEmission Declaration
\*---------------------------------------------------------------------------*/

class noAbsorptionEmission
:
    public absorptionEmissionModel
{

public:

    //- Runtime type information
    TypeName("none");


    //- Construct from components
    noAbsorptionEmission(const dictionary& dict, const fvMesh& mesh);


    //- Destructor
    virtual ~noAbsorptionEmission()
    {}

};


} // End namespace radiation
} // End namespace CML


#endif
