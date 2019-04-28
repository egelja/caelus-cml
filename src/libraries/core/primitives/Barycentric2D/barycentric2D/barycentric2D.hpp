/*---------------------------------------------------------------------------*\
Copyright (C) 2017-2018 OpenFOAM Foundation
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

Typedef
    CML::barycentric2D

Description
    A scalar version of the templated Barycentric2D

\*---------------------------------------------------------------------------*/

#ifndef barycentric2D_HPP
#define barycentric2D_HPP

#include "scalar.hpp"
#include "Barycentric2D_.hpp"
#include "contiguous.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class Random;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

typedef Barycentric2D<scalar> barycentric2D;


//- Generate a random barycentric coordinate within the unit triangle
barycentric2D barycentric2D01(Random& rndGen);


template<>
inline bool contiguous<barycentric2D>()
{
    return true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
