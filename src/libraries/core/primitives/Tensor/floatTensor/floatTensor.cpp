/*---------------------------------------------------------------------------*\
Copyright (C) 2016 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "floatTensor.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<>
const char* const CML::floatTensor::vsType::typeName = "floatTensor";

template<>
const char* const CML::floatTensor::vsType::componentNames[] =
{
    "xx", "xy", "xz",
    "yx", "yy", "yz",
    "zx", "zy", "zz"
};

template<>
const CML::floatTensor CML::floatTensor::vsType::zero
(
    floatTensor::uniform(0)
);

template<>
const CML::floatTensor CML::floatTensor::vsType::one
(
    floatTensor::uniform(1)
);

template<>
const CML::floatTensor CML::floatTensor::vsType::max
(
    floatTensor::uniform(floatScalarVGREAT)
);

template<>
const CML::floatTensor CML::floatTensor::vsType::min
(
    floatTensor::uniform(-floatScalarVGREAT)
);

template<>
const CML::floatTensor CML::floatTensor::vsType::rootMax
(
    floatTensor::uniform(floatScalarROOTVGREAT)
);

template<>
const CML::floatTensor CML::floatTensor::vsType::rootMin
(
    floatTensor::uniform(-floatScalarROOTVGREAT)
);

template<>
const CML::floatTensor CML::floatTensor::I
(
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
);


// ************************************************************************* //
