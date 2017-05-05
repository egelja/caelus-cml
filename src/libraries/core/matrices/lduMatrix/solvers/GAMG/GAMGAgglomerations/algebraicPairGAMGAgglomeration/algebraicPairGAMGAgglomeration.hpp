/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2016 Applied CCM
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
    CML::algebraicPairGAMGAgglomeration

Description
    Agglomerate using the pair algorithm.

SourceFiles
    algebraicPairGAMGAgglomeration.cpp

\*---------------------------------------------------------------------------*/

#ifndef algebraicPairGAMGAgglomeration_H
#define algebraicPairGAMGAgglomeration_H

#include "pairGAMGAgglomeration.hpp"

namespace CML
{

class algebraicPairGAMGAgglomeration : public pairGAMGAgglomeration
{

public:

    //- Runtime type information
    TypeName("algebraicPair");


    // Constructors

    //- Construct given mesh and controls
    algebraicPairGAMGAgglomeration
    (
        lduMatrix const& matrix,
        dictionary const& controlDict
    );
};

} // End namespace CML

#endif

