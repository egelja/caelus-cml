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
    Vector2D of scalars.

\*---------------------------------------------------------------------------*/

#include "vector2D.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<>
const char* const CML::vector2D::vsType::typeName = "vector2D";

template<>
const char* const CML::vector2D::vsType::componentNames[] = {"x", "y"};

template<>
const CML::vector2D CML::vector2D::vsType::vsType::zero
(
    vector2D::uniform(0)
);

template<>
const CML::vector2D CML::vector2D::vsType::one
(
    vector2D::uniform(1)
);

template<>
const CML::vector2D CML::vector2D::vsType::max
(
    vector2D::uniform(VGREAT)
);

template<>
const CML::vector2D CML::vector2D::vsType::min
(
    vector2D::uniform(-VGREAT)
);

template<>
const CML::vector2D CML::vector2D::vsType::rootMax
(
    vector2D::uniform(ROOTVGREAT)
);

template<>
const CML::vector2D CML::vector2D::vsType::rootMin
(
    vector2D::uniform(-ROOTVGREAT)
);


// ************************************************************************* //
