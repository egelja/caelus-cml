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

#include "symmTensor.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<>
const char* const symmTensor::typeName = "symmTensor";

template<>
const char* symmTensor::componentNames[] =
{
    "xx", "xy", "xz",
          "yy", "yz",
                "zz"
};

template<>
const symmTensor symmTensor::zero
(
    0, 0, 0,
       0, 0,
          0
);

template<>
const symmTensor symmTensor::one
(
    1, 1, 1,
       1, 1,
          1
);

template<>
const symmTensor symmTensor::max
(
    VGREAT, VGREAT, VGREAT,
            VGREAT, VGREAT,
                    VGREAT
);

template<>
const symmTensor symmTensor::min
(
    -VGREAT, -VGREAT, -VGREAT,
             -VGREAT, -VGREAT,
                      -VGREAT
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
