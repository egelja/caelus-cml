/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
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

#include "symmTensor2D.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<>
const char* const CML::symmTensor2D::vsType::typeName = "symmTensor2D";

template<>
const char* const CML::symmTensor2D::vsType::componentNames[] =
{
    "xx", "xy",
          "yy"
};

template<>
const CML::symmTensor2D CML::symmTensor2D::vsType::vsType::zero
(
    symmTensor2D::uniform(0)
);

template<>
const CML::symmTensor2D CML::symmTensor2D::vsType::one
(
    symmTensor2D::uniform(1)
);

template<>
const CML::symmTensor2D CML::symmTensor2D::vsType::max
(
    symmTensor2D::uniform(VGREAT)
);

template<>
const CML::symmTensor2D CML::symmTensor2D::vsType::min
(
    symmTensor2D::uniform(-VGREAT)
);

template<>
const CML::symmTensor2D CML::symmTensor2D::vsType::rootMax
(
    symmTensor2D::uniform(ROOTVGREAT)
);

template<>
const CML::symmTensor2D CML::symmTensor2D::vsType::rootMin
(
    symmTensor2D::uniform(-ROOTVGREAT)
);

template<>
const CML::symmTensor2D CML::symmTensor2D::I
(
    1, 0,
       1
);


// ************************************************************************* //
