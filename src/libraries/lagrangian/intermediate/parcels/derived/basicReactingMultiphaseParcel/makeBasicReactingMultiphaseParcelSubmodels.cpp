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

#include "basicReactingMultiphaseCloud.hpp"

#include "makeParcelCloudFunctionObjects.hpp"

// Kinematic
#include "makeParcelForces.hpp"
#include "makeParcelDispersionModels.hpp"
#include "makeReactingMultiphaseParcelInjectionModels.hpp" // MP variant
#include "makeParcelPatchInteractionModels.hpp"
#include "makeReactingMultiphaseParcelStochasticCollisionModels.hpp" // MP variant
#include "makeReactingParcelSurfaceFilmModels.hpp" // Reacting variant

// Thermodynamic
#include "makeParcelHeatTransferModels.hpp"

// Reacting
#include "makeReactingMultiphaseParcelCompositionModels.hpp" // MP Variant
#include "makeReactingParcelPhaseChangeModels.hpp"

// Reacting multiphase
#include "makeReactingMultiphaseParcelDevolatilisationModels.hpp"
#include "makeReactingMultiphaseParcelSurfaceReactionModels.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    makeParcelCloudFunctionObjects(basicReactingMultiphaseCloud);

    // Kinematic sub-models
    makeParcelForces(basicReactingMultiphaseCloud);
    makeParcelDispersionModels(basicReactingMultiphaseCloud);
    makeReactingMultiphaseParcelInjectionModels(basicReactingMultiphaseCloud);
    makeParcelPatchInteractionModels(basicReactingMultiphaseCloud);
    makeReactingMultiphaseParcelStochasticCollisionModels
    (
        basicReactingMultiphaseCloud
    );
    makeReactingParcelSurfaceFilmModels(basicReactingMultiphaseCloud);

    // Thermo sub-models
    makeParcelHeatTransferModels(basicReactingMultiphaseCloud);

    // Reacting sub-models
    makeReactingMultiphaseParcelCompositionModels
    (
        basicReactingMultiphaseCloud
    );
    makeReactingParcelPhaseChangeModels(basicReactingMultiphaseCloud);

    // Reacting multiphase sub-models
    makeReactingMultiphaseParcelDevolatilisationModels
    (
        basicReactingMultiphaseCloud
    );
    makeReactingMultiphaseParcelSurfaceReactionModels
    (
        basicReactingMultiphaseCloud
    );
}

// ************************************************************************* //
