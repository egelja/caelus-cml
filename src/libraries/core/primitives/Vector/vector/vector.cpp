/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
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

Description
    Vector of scalars.

\*---------------------------------------------------------------------------*/

#include "vector.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<>
const char* const CML::vector::vsType::typeName = "vector";

template<>
const char* const CML::vector::vsType::componentNames[] = {"x", "y", "z"};

template<>
const CML::vector CML::vector::vsType::vsType::zero(vector::uniform(0));

template<>
const CML::vector CML::vector::vsType::one(vector::uniform(1));

template<>
const CML::vector CML::vector::vsType::max(vector::uniform(VGREAT));

template<>
const CML::vector CML::vector::vsType::min(vector::uniform(-VGREAT));

template<>
const CML::vector CML::vector::vsType::rootMax(vector::uniform(ROOTVGREAT));

template<>
const CML::vector CML::vector::vsType::rootMin(vector::uniform(-ROOTVGREAT));


// ************************************************************************* //
