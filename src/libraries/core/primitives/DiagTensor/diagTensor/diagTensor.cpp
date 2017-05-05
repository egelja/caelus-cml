/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<>
const char* const diagTensor::typeName = "diagTensor";

template<>
const char* diagTensor::componentNames[] = {"xx", "yy", "zz"};

template<>
const diagTensor diagTensor::zero(0, 0, 0);

template<>
const diagTensor diagTensor::one(1, 1, 1);

template<>
const diagTensor diagTensor::max(VGREAT, VGREAT, VGREAT);

template<>
const diagTensor diagTensor::min(-VGREAT, -VGREAT, -VGREAT);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
