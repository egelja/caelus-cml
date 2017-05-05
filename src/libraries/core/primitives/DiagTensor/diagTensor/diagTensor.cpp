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
    DiagTensor of scalars.

\*---------------------------------------------------------------------------*/

#include "diagTensor.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<>
const char* const CML::diagTensor::vsType::typeName = "diagTensor";

template<>
const char* const CML::diagTensor::vsType::componentNames[] =
{
    "xx", "yy", "zz"
};

template<>
const CML::diagTensor CML::diagTensor::vsType::vsType::zero
(
    diagTensor::uniform(0)
);

template<>
const CML::diagTensor CML::diagTensor::vsType::one
(
    diagTensor::uniform(1)
);

template<>
const CML::diagTensor CML::diagTensor::vsType::max
(
    diagTensor::uniform(VGREAT)
);

template<>
const CML::diagTensor CML::diagTensor::vsType::min
(
    diagTensor::uniform(-VGREAT)
);

template<>
const CML::diagTensor CML::diagTensor::vsType::rootMax
(
    diagTensor::uniform(ROOTVGREAT)
);

template<>
const CML::diagTensor CML::diagTensor::vsType::rootMin
(
    diagTensor::uniform(-ROOTVGREAT)
);


// ************************************************************************* //
