/*---------------------------------------------------------------------------*\
Copyright (C) 2015 Applied CCM
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
    blendedVanAlbada scheme class.

\*---------------------------------------------------------------------------*/

#include "fvMesh.hpp"
#include "blendedVanAlbada.hpp"
#include "vanAlbadaSlopeLimiter.hpp"
#include "blendedCorrection.hpp"

makeVanAlbadaSlopeLimiter(blended)
makeBlendedCorrection(VanAlbada)

namespace CML
{
    makelimitedSurfaceInterpolationTypeScheme(blendedVanAlbada, scalar)
    makelimitedSurfaceInterpolationTypeScheme(blendedVanAlbada, vector)
}

