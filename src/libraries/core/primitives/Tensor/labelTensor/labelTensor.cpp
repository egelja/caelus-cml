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

#include "labelTensor.hpp"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<>
const char* const CML::labelTensor::vsType::typeName = "labelTensor";

template<>
const char* const CML::labelTensor::vsType::componentNames[] =
{
    "xx", "xy", "xz",
    "yx", "yy", "yz",
    "zx", "zy", "zz"
};

template<>
const CML::labelTensor CML::labelTensor::vsType::zero
(
    labelTensor::uniform(0)
);

template<>
const CML::labelTensor CML::labelTensor::vsType::one
(
    labelTensor::uniform(1)
);

template<>
const CML::labelTensor CML::labelTensor::vsType::max
(
    labelTensor::uniform(labelMax)
);

template<>
const CML::labelTensor CML::labelTensor::vsType::min
(
    labelTensor::uniform(-labelMax)
);

template<>
const CML::labelTensor CML::labelTensor::vsType::rootMax
(
    labelTensor::uniform(sqrt(scalar(labelMax)))
);

template<>
const CML::labelTensor CML::labelTensor::vsType::rootMin
(
    labelTensor::uniform(-sqrt(scalar(labelMax)))
);


// ************************************************************************* //
