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
    CML::atomicWeightTable

Description
    A table of atomic weights for all the elements

SourceFiles
    atomicWeights.cpp

\*---------------------------------------------------------------------------*/

#ifndef atomicWeights_HPP
#define atomicWeights_HPP

#include "scalar.hpp"
#include "HashTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class atomicWeights Declaration
\*---------------------------------------------------------------------------*/

class atomicWeightTable
:
    public HashTable<scalar>
{

public:

    //- Structure to hold the element name and atomic weight pair
    struct atomicWeight
    {
        char name[3];
        scalar weight;
    };

    static const int nElements = 104;

    //- Static table of the weights of all known elements
    static const atomicWeight atomicWeights[nElements];


    //- Construct from atomicWeights_
    atomicWeightTable();

};


// * * * * * * * * * * * * * * * * Global data  * * * * * * * * * * * * * * //

// Atomic weights table for every element in the periodic table
extern atomicWeightTable atomicWeights;


} // End namespace CML


#endif
