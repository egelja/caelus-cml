/*---------------------------------------------------------------------------*\
Copyright Niels G. Jacobsen and Sopheak Seng Technical University of Denmark
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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

Typedef
    CML::IOsampledSurfaceElevation

Description
    Instance of the generic IOOutputFilter for sampledSurfaceElevation.

\*---------------------------------------------------------------------------*/

#ifndef IOsampledSurfaceElevation_HPP
#define IOsampledSurfaceElevation_HPP

#include "sampledSurfaceElevation.hpp"
#include "IOOutputFilter.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    typedef IOOutputFilter<sampledSurfaceElevation> IOsampledSurfaceElevation;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
