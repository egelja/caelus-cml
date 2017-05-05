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

Description
    SpatialVector of scalars.

\*---------------------------------------------------------------------------*/

#include "spatialVector.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<>
const char* const CML::spatialVector::vsType::typeName = "spatialVector";

template<>
const char* const CML::spatialVector::vsType::componentNames[] =
{
    "wx", "wy", "wz", "lx", "ly", "lz"
};

template<>
const CML::spatialVector CML::spatialVector::vsType::zero
(
    CML::spatialVector::uniform(0)
);

template<>
const CML::spatialVector CML::spatialVector::vsType::one
(
    spatialVector::uniform(1)
);

template<>
const CML::spatialVector CML::spatialVector::vsType::max
(
    spatialVector::uniform(VGREAT)
);

template<>
const CML::spatialVector CML::spatialVector::vsType::min
(
    spatialVector::uniform(-VGREAT)
);

template<>
const CML::spatialVector CML::spatialVector::vsType::rootMax
(
    spatialVector::uniform(ROOTVGREAT)
);

template<>
const CML::spatialVector CML::spatialVector::vsType::rootMin
(
    spatialVector::uniform(-ROOTVGREAT)
);


// ************************************************************************* //
