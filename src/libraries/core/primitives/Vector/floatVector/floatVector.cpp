/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Description
    Vector of floats.

\*---------------------------------------------------------------------------*/

#include "floatVector.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<>
const char* const CML::floatVector::vsType::typeName = "floatVector";

template<>
const char* const CML::floatVector::vsType::componentNames[] =
{
    "x", "y", "z"
};

template<>
const CML::floatVector CML::floatVector::vsType::zero
(
    floatVector::uniform(0)
);

template<>
const CML::floatVector CML::floatVector::vsType::one
(
    floatVector::uniform(1)
);

template<>
const CML::floatVector CML::floatVector::vsType::max
(
    floatVector::uniform(floatScalarVGREAT)
);

template<>
const CML::floatVector CML::floatVector::vsType::min
(
    floatVector::uniform(-floatScalarVGREAT)
);

template<>
const CML::floatVector CML::floatVector::vsType::rootMax
(
    floatVector::uniform(floatScalarROOTVGREAT)
);

template<>
const CML::floatVector CML::floatVector::vsType::rootMin
(
    floatVector::uniform(-floatScalarROOTVGREAT)
);


// ************************************************************************* //
