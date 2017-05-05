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

#include "labelTensor.hpp"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    template<>
    const char* const CML::labelTensor::typeName = "labelTensor";

    template<>
    const char* CML::labelTensor::componentNames[] =
    {
        "xx", "xy", "xz",
        "yx", "yy", "yz",
        "zx", "zy", "zz"
    };

    template<>
    const CML::labelTensor CML::labelTensor::zero
    (
        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    );

    template<>
    const CML::labelTensor CML::labelTensor::one
    (
        1, 1, 1,
        1, 1, 1,
        1, 1, 1
    );
}


// ************************************************************************* //
