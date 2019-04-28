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

#include "makeCombustionTypes.hpp"

#include "thermoPhysicsTypes.hpp"
#include "psiReactionThermo.hpp"
#include "rhoReactionThermo.hpp"
#include "FSD.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Combustion models based on sensibleEnthalpy
makeCombustionTypesThermo
(
    FSD,
    psiReactionThermo,
    gasHThermoPhysics
);

makeCombustionTypesThermo
(
    FSD,
    psiReactionThermo,
    constGasHThermoPhysics
);

makeCombustionTypesThermo
(
    FSD,
    rhoReactionThermo,
    gasHThermoPhysics
);

makeCombustionTypesThermo
(
    FSD,
    rhoReactionThermo,
    constGasHThermoPhysics
);

// Combustion models based on sensibleInternalEnergy
makeCombustionTypesThermo
(
    FSD,
    psiReactionThermo,
    gasEThermoPhysics
);

makeCombustionTypesThermo
(
    FSD,
    psiReactionThermo,
    constGasEThermoPhysics
);

makeCombustionTypesThermo
(
    FSD,
    rhoReactionThermo,
    gasEThermoPhysics
);

makeCombustionTypesThermo
(
    FSD,
    rhoReactionThermo,
    constGasEThermoPhysics
);

}
