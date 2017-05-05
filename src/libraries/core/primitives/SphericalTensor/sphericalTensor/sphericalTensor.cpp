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

#include "sphericalTensor.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<>
const char* const CML::sphericalTensor::vsType::typeName = "sphericalTensor";

template<>
const char* const CML::sphericalTensor::vsType::componentNames[] = {"ii"};

template<>
const CML::sphericalTensor CML::sphericalTensor::vsType::zero
(
    sphericalTensor::uniform(0)
);

template<>
const CML::sphericalTensor CML::sphericalTensor::vsType::one
(
    sphericalTensor::uniform(1)
);

template<>
const CML::sphericalTensor CML::sphericalTensor::vsType::max
(
    sphericalTensor::uniform(VGREAT)
);

template<>
const CML::sphericalTensor CML::sphericalTensor::vsType::min
(
    sphericalTensor::uniform(-VGREAT)
);

template<>
const CML::sphericalTensor CML::sphericalTensor::vsType::rootMax
(
    sphericalTensor::uniform(ROOTVGREAT)
);

template<>
const CML::sphericalTensor CML::sphericalTensor::vsType::rootMin
(
    sphericalTensor::uniform(-ROOTVGREAT)
);

template<>
const CML::sphericalTensor CML::sphericalTensor::I(1);

template<>
const CML::sphericalTensor CML::sphericalTensor::oneThirdI(1.0/3.0);

template<>
const CML::sphericalTensor CML::sphericalTensor::twoThirdsI(2.0/3.0);

// ************************************************************************* //
