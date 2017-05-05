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

#include "labelSphericalTensor.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<>
const char* const CML::labelSphericalTensor::vsType::typeName
(
    "labelSphericalTensor"
);

template<>
const char* const CML::labelSphericalTensor::vsType::componentNames[] =
{
    "ii"
};

template<>
const CML::labelSphericalTensor
CML::labelSphericalTensor::vsType::vsType::zero
(
    labelSphericalTensor::uniform(0)
);

template<>
const CML::labelSphericalTensor CML::labelSphericalTensor::vsType::one
(
    labelSphericalTensor::uniform(1)
);

template<>
const CML::labelSphericalTensor CML::labelSphericalTensor::vsType::max
(
    labelSphericalTensor::uniform(labelMax)
);

template<>
const CML::labelSphericalTensor CML::labelSphericalTensor::vsType::min
(
    labelSphericalTensor::uniform(-labelMax)
);

template<>
const CML::labelSphericalTensor CML::labelSphericalTensor::vsType::rootMax
(
    labelSphericalTensor::uniform(sqrt(scalar(labelMax)))
);

template<>
const CML::labelSphericalTensor CML::labelSphericalTensor::vsType::rootMin
(
    labelSphericalTensor::uniform(-sqrt(scalar(labelMax)))
);

template<>
const CML::labelSphericalTensor CML::labelSphericalTensor::I(1);


// ************************************************************************* //
