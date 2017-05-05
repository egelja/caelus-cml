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

#include "spatialTensor.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<>
const char* const CML::spatialTensor::vsType::typeName = "spatialTensor";

template<>
const char* const CML::spatialTensor::vsType::componentNames[] =
{
    "Exx",  "Exy",  "Exz",    "Erxx", "Erxy", "Erxz",
    "Eyx",  "Eyy",  "Eyz",    "Eryx", "Eryy", "Eryz",
    "Ezx",  "Ezy",  "Ezz",    "Erzx", "Erzy", "Erzz"

    "Erxx", "Erxy", "Erxz",   "Exx",  "Exy",  "Exz",
    "Eryx", "Eryy", "Eryz",   "Eyx",  "Eyy",  "Eyz",
    "Erzx", "Erzy", "Erzz",   "Ezx",  "Ezy",  "Ezz"
};

template<>
const CML::spatialTensor CML::spatialTensor::vsType::zero
(
    CML::spatialTensor::uniform(0)
);

template<>
const CML::spatialTensor CML::spatialTensor::vsType::one
(
    spatialTensor::uniform(1)
);

template<>
const CML::spatialTensor CML::spatialTensor::vsType::max
(
    spatialTensor::uniform(VGREAT)
);

template<>
const CML::spatialTensor CML::spatialTensor::vsType::min
(
    spatialTensor::uniform(-VGREAT)
);

template<>
const CML::spatialTensor CML::spatialTensor::vsType::rootMax
(
    spatialTensor::uniform(ROOTVGREAT)
);

template<>
const CML::spatialTensor CML::spatialTensor::vsType::rootMin
(
    spatialTensor::uniform(-ROOTVGREAT)
);

template<>
const CML::spatialTensor CML::spatialTensor::I
(
    CML::spatialTensor::identity()
);


// ************************************************************************* //
