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

#include "labelSymmTensor.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<>
const char* const CML::labelSymmTensor::vsType::typeName = "labelSymmTensor";

template<>
const char* const CML::labelSymmTensor::vsType::componentNames[] =
{
    "xx", "xy", "xz",
          "yy", "yz",
                "zz"
};

template<>
const CML::labelSymmTensor CML::labelSymmTensor::vsType::vsType::zero
(
    labelSymmTensor::uniform(0)
);

template<>
const CML::labelSymmTensor CML::labelSymmTensor::vsType::one
(
    labelSymmTensor::uniform(1)
);

template<>
const CML::labelSymmTensor CML::labelSymmTensor::vsType::max
(
    labelSymmTensor::uniform(labelMax)
);

template<>
const CML::labelSymmTensor CML::labelSymmTensor::vsType::min
(
    labelSymmTensor::uniform(-labelMax)
);

template<>
const CML::labelSymmTensor CML::labelSymmTensor::vsType::rootMax
(
    labelSymmTensor::uniform(sqrt(scalar(labelMax)))
);

template<>
const CML::labelSymmTensor CML::labelSymmTensor::vsType::rootMin
(
    labelSymmTensor::uniform(-sqrt(scalar(labelMax)))
);

template<>
const CML::labelSymmTensor CML::labelSymmTensor::I
(
    1, 0, 0,
       1, 0,
          1
);


// ************************************************************************* //
