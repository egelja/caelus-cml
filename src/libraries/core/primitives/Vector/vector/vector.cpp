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

Description
    Vector of scalars.

\*---------------------------------------------------------------------------*/

#include "vector.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<>
const char* const vector::typeName = "vector";

template<>
const char* vector::componentNames[] = {"x", "y", "z"};

template<>
const vector vector::zero(0, 0, 0);

template<>
const vector vector::one(1, 1, 1);

template<>
const vector vector::max(VGREAT, VGREAT, VGREAT);

template<>
const vector vector::min(-VGREAT, -VGREAT, -VGREAT);

template<>
const vector vector::rootMax(ROOTVGREAT, ROOTVGREAT, ROOTVGREAT);

template<>
const vector vector::rootMin(-ROOTVGREAT, -ROOTVGREAT, -ROOTVGREAT);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
