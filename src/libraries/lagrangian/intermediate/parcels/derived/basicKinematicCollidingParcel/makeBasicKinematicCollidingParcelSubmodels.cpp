/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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

#include "basicKinematicCollidingCloud.hpp"

#include "makeParcelCloudFunctionObjects.hpp"

// Kinematic
#include "makeParcelForces.hpp"
#include "makeParcelDispersionModels.hpp"
#include "makeParcelInjectionModels.hpp"
#include "makeParcelCollisionModels.hpp"
#include "makeParcelPatchInteractionModels.hpp"
#include "makeParcelStochasticCollisionModels.hpp"
#include "makeParcelSurfaceFilmModels.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    makeParcelCloudFunctionObjects(basicKinematicCollidingCloud);

    // Kinematic sub-models
    makeParcelForces(basicKinematicCollidingCloud);
    makeParcelDispersionModels(basicKinematicCollidingCloud);
    makeParcelInjectionModels(basicKinematicCollidingCloud);
    makeParcelCollisionModels(basicKinematicCollidingCloud);
    makeParcelPatchInteractionModels(basicKinematicCollidingCloud);
    makeParcelStochasticCollisionModels(basicKinematicCollidingCloud);
    makeParcelSurfaceFilmModels(basicKinematicCollidingCloud);
}

// ************************************************************************* //
