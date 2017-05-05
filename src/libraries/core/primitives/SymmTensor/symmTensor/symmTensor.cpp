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

#include "symmTensor.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<>
const char* const CML::symmTensor::vsType::typeName = "symmTensor";

template<>
const char* const CML::symmTensor::vsType::componentNames[] =
{
    "xx", "xy", "xz",
          "yy", "yz",
                "zz"
};

template<>
const CML::symmTensor CML::symmTensor::vsType::vsType::zero
(
    symmTensor::uniform(0)
);

template<>
const CML::symmTensor CML::symmTensor::vsType::one
(
    symmTensor::uniform(1)
);

template<>
const CML::symmTensor CML::symmTensor::vsType::max
(
    symmTensor::uniform(VGREAT)
);

template<>
const CML::symmTensor CML::symmTensor::vsType::min
(
    symmTensor::uniform(-VGREAT)
);

template<>
const CML::symmTensor CML::symmTensor::vsType::rootMax
(
    symmTensor::uniform(ROOTVGREAT)
);

template<>
const CML::symmTensor CML::symmTensor::vsType::rootMin
(
    symmTensor::uniform(-ROOTVGREAT)
);

template<>
const CML::symmTensor CML::symmTensor::I
(
    1, 0, 0,
       1, 0,
          1
);


// ************************************************************************* //
