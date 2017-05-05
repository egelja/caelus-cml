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

#include "compactSpatialTensor.hpp"
#include "CompactSpatialTensorT_.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<>
const char* const CML::compactSpatialTensor::vsType::typeName =
    "compactSpatialTensor";

template<>
const char* const CML::compactSpatialTensor::vsType::componentNames[] =
{
    "Exx",  "Exy",  "Exz",
    "Eyx",  "Eyy",  "Eyz",
    "Ezx",  "Ezy",  "Ezz",

    "Erxx", "Erxy", "Erxz",
    "Eryx", "Eryy", "Eryz",
    "Erzx", "Erzy", "Erzz",
};

template<>
const CML::compactSpatialTensor CML::compactSpatialTensor::vsType::zero
(
    CML::compactSpatialTensor::uniform(0)
);

template<>
const CML::compactSpatialTensor CML::compactSpatialTensor::vsType::one
(
    compactSpatialTensor::uniform(1)
);

template<>
const CML::compactSpatialTensor CML::compactSpatialTensor::vsType::max
(
    compactSpatialTensor::uniform(VGREAT)
);

template<>
const CML::compactSpatialTensor CML::compactSpatialTensor::vsType::min
(
    compactSpatialTensor::uniform(-VGREAT)
);

template<>
const CML::compactSpatialTensor CML::compactSpatialTensor::vsType::rootMax
(
    compactSpatialTensor::uniform(ROOTVGREAT)
);

template<>
const CML::compactSpatialTensor CML::compactSpatialTensor::vsType::rootMin
(
    compactSpatialTensor::uniform(-ROOTVGREAT)
);


// ************************************************************************* //
