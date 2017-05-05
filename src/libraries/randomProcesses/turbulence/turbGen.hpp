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
    CML::turbGen

Description
    Generate a turbulent velocity field conforming to a given
    energy spectrum and being divergence free.

SourceFiles
    turbGen.cpp

\*---------------------------------------------------------------------------*/

#ifndef turbGen_H
#define turbGen_H

#include "Random.hpp"
#include "primitiveFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class Kmesh;

/*---------------------------------------------------------------------------*\
                           Class turbGen Declaration
\*---------------------------------------------------------------------------*/

class turbGen
{
    // Private data

        const Kmesh& K;
        const scalar Ea;
        const scalar k0;
        Random RanGen;


public:

    // Constructors

        //- Construct from components
        turbGen(const Kmesh& k, const scalar EA, const scalar K0);


    // Member Functions

        //- Generate and return a velocity field
        vectorField U();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
