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

\*---------------------------------------------------------------------------*/

#include "labelSymmTensor.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<>
const char* const labelSymmTensor::typeName = "labelSymmTensor";

template<>
const char* labelSymmTensor::componentNames[] =
{
    "xx", "xy", "xz",
          "yy", "yz",
                "zz"
};

template<>
const labelSymmTensor labelSymmTensor::zero
(
    0, 0, 0,
       0, 0,
          0
);

template<>
const labelSymmTensor labelSymmTensor::one
(
    1, 1, 1,
       1, 1,
          1
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
