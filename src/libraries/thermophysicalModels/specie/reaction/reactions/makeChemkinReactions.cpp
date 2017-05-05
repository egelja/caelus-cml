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

#include "reactionTypes.hpp"
#include "makeReactionThermo.hpp"

#include "ArrheniusReactionRate.hpp"
#include "infiniteReactionRate.hpp"
#include "LandauTellerReactionRate.hpp"
#include "thirdBodyArrheniusReactionRate.hpp"

#include "ChemicallyActivatedReactionRate.hpp"
#include "JanevReactionRate.hpp"
#include "powerSeriesReactionRate.hpp"

#include "FallOffReactionRate.hpp"
#include "LindemannFallOffFunction.hpp"
#include "SRIFallOffFunction.hpp"
#include "TroeFallOffFunction.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTemplateTypeNameAndDebug(gasReaction, 0);
defineTemplateRunTimeSelectionTable(gasReaction, Istream);
defineTemplateRunTimeSelectionTable(gasReaction, dictionary);


// * * * * * * * * * * * * * Make CHEMKIN reactions  * * * * * * * * * * * * //

makeIRNReactions(gasThermoPhysics, ArrheniusReactionRate)
makeIRNReactions(gasThermoPhysics, infiniteReactionRate)
makeIRNReactions(gasThermoPhysics, LandauTellerReactionRate)
makeIRNReactions(gasThermoPhysics, thirdBodyArrheniusReactionRate)

makeIRReactions(gasThermoPhysics, JanevReactionRate)
makeIRReactions(gasThermoPhysics, powerSeriesReactionRate)

makePressureDependentReactions
(
    gasThermoPhysics,
    ArrheniusReactionRate,
    LindemannFallOffFunction
)

makePressureDependentReactions
(
    gasThermoPhysics,
    ArrheniusReactionRate,
    TroeFallOffFunction
)

makePressureDependentReactions
(
    gasThermoPhysics,
    ArrheniusReactionRate,
    SRIFallOffFunction
)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
