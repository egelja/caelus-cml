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

\*---------------------------------------------------------------------------*/

#include "labelVector.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<>
const char* const CML::labelVector::vsType::typeName = "labelVector";

template<>
const char* const CML::labelVector::vsType::componentNames[] =
{
    "x", "y", "z"
};

template<>
const CML::labelVector CML::labelVector::vsType::zero
(
    labelVector::uniform(0)
);

template<>
const CML::labelVector CML::labelVector::vsType::one
(
    labelVector::uniform(1)
);

template<>
const CML::labelVector CML::labelVector::vsType::max
(
    labelVector::uniform(labelMax)
);

template<>
const CML::labelVector CML::labelVector::vsType::min
(
    labelVector::uniform(-labelMax)
);

template<>
const CML::labelVector CML::labelVector::vsType::rootMax
(
    labelVector::uniform(sqrt(scalar(labelMax)))
);

template<>
const CML::labelVector CML::labelVector::vsType::rootMin
(
    labelVector::uniform(-sqrt(scalar(labelMax)))
);


// ************************************************************************* //
