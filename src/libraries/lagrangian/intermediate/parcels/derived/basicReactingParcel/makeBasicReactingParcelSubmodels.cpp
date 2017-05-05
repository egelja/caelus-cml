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

#include "basicReactingCloud.hpp"

#include "makeParcelCloudFunctionObjects.hpp"

// Kinematic
#include "makeThermoParcelForces.hpp" // thermo variant
#include "makeParcelDispersionModels.hpp"
#include "makeReactingParcelInjectionModels.hpp" // Reacting variant
#include "makeParcelPatchInteractionModels.hpp"

// Thermodynamic
#include "makeParcelHeatTransferModels.hpp"

// Reacting
#include "makeReactingParcelCompositionModels.hpp"
#include "makeReactingParcelPhaseChangeModels.hpp"
#include "makeReactingParcelSurfaceFilmModels.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    makeParcelCloudFunctionObjects(basicReactingCloud);

    // Kinematic sub-models
    makeThermoParcelForces(basicReactingCloud);
    makeParcelDispersionModels(basicReactingCloud);
    makeReactingParcelInjectionModels(basicReactingCloud);
    makeParcelPatchInteractionModels(basicReactingCloud);

    // Thermo sub-models
    makeParcelHeatTransferModels(basicReactingCloud);

    // Reacting sub-models
    makeReactingParcelCompositionModels(basicReactingCloud);
    makeReactingParcelPhaseChangeModels(basicReactingCloud);
    makeReactingParcelSurfaceFilmModels(basicReactingCloud);
}


// ************************************************************************* //
