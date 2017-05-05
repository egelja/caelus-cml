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

#include "basicSprayCloud.hpp"

#include "makeParcelCloudFunctionObjects.hpp"

// Kinematic
#include "makeThermoParcelForces.hpp" // thermo variant
#include "makeParcelDispersionModels.hpp"
#include "makeSprayParcelInjectionModels.hpp" // Spray variant
#include "makeParcelPatchInteractionModels.hpp"

// Thermodynamic
#include "makeParcelHeatTransferModels.hpp"

// Reacting
#include "makeReactingParcelCompositionModels.hpp"
#include "makeReactingParcelPhaseChangeModels.hpp"
#include "makeReactingParcelSurfaceFilmModels.hpp"

// Spray
#include "makeSprayParcelAtomizationModels.hpp"
#include "makeSprayParcelBreakupModels.hpp"
#include "makeSprayParcelCollisionModels.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    makeParcelCloudFunctionObjects(basicSprayCloud);

    // Kinematic sub-models
    makeThermoParcelForces(basicSprayCloud);
    makeParcelDispersionModels(basicSprayCloud);
    makeSprayParcelInjectionModels(basicSprayCloud);
    makeParcelPatchInteractionModels(basicSprayCloud);

    // Thermo sub-models
    makeParcelHeatTransferModels(basicSprayCloud);

    // Reacting sub-models
    makeReactingParcelCompositionModels(basicSprayCloud);
    makeReactingParcelPhaseChangeModels(basicSprayCloud);
    makeReactingParcelSurfaceFilmModels(basicSprayCloud);

    // Spray sub-models
    makeSprayParcelAtomizationModels(basicSprayCloud);
    makeSprayParcelBreakupModels(basicSprayCloud);
    makeSprayParcelCollisionModels(basicSprayCloud);
};


// ************************************************************************* //
