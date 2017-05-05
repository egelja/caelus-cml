/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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

#include "makeReactionThermo.hpp"

#include "hReactionThermo.hpp"
#include "hRhoMixtureThermo.hpp"

#include "perfectGas.hpp"

#include "hConstThermo.hpp"
#include "janafThermo.hpp"
#include "specieThermo.hpp"

#include "constTransport.hpp"
#include "sutherlandTransport.hpp"

#include "homogeneousMixture.hpp"
#include "inhomogeneousMixture.hpp"
#include "veryInhomogeneousMixture.hpp"
#include "dieselMixture.hpp"
#include "multiComponentMixture.hpp"
#include "reactingMixture.hpp"
#include "singleStepReactingMixture.hpp"

#include "thermoPhysicsTypes.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makeReactionThermo
(
    hReactionThermo,
    hRhoMixtureThermo,
    homogeneousMixture,
    constTransport,
    hConstThermo,
    perfectGas
);

makeReactionThermo
(
    hReactionThermo,
    hRhoMixtureThermo,
    inhomogeneousMixture,
    constTransport,
    hConstThermo,
    perfectGas
);

makeReactionThermo
(
    hReactionThermo,
    hRhoMixtureThermo,
    veryInhomogeneousMixture,
    constTransport,
    hConstThermo,
    perfectGas
);

makeReactionThermo
(
    hReactionThermo,
    hRhoMixtureThermo,
    homogeneousMixture,
    sutherlandTransport,
    janafThermo,
    perfectGas
);

makeReactionThermo
(
    hReactionThermo,
    hRhoMixtureThermo,
    inhomogeneousMixture,
    sutherlandTransport,
    janafThermo,
    perfectGas
);

makeReactionThermo
(
    hReactionThermo,
    hRhoMixtureThermo,
    veryInhomogeneousMixture,
    sutherlandTransport,
    janafThermo,
    perfectGas
);


makeReactionThermo
(
    hReactionThermo,
    hRhoMixtureThermo,
    dieselMixture,
    sutherlandTransport,
    janafThermo,
    perfectGas
);


// Multi-component thermo

makeReactionMixtureThermo
(
    hReactionThermo,
    hRhoMixtureThermo,
    multiComponentMixture,
    constGasThermoPhysics
);

makeReactionMixtureThermo
(
    hReactionThermo,
    hRhoMixtureThermo,
    multiComponentMixture,
    icoPoly8ThermoPhysics
);

makeReactionMixtureThermo
(
    hReactionThermo,
    hRhoMixtureThermo,
    multiComponentMixture,
    gasThermoPhysics
);


// Multi-component reaction thermo

makeReactionMixtureThermo
(
    hReactionThermo,
    hRhoMixtureThermo,
    reactingMixture,
    constGasThermoPhysics
);

makeReactionMixtureThermo
(
    hReactionThermo,
    hRhoMixtureThermo,
    reactingMixture,
    constIsobaricGasThermoPhysics
);

makeReactionMixtureThermo
(
    hReactionThermo,
    hRhoMixtureThermo,
    reactingMixture,
    icoPoly8ThermoPhysics
);

makeReactionMixtureThermo
(
    hReactionThermo,
    hRhoMixtureThermo,
    reactingMixture,
    gasThermoPhysics
);

makeReactionMixtureThermo
(
    hReactionThermo,
    hRhoMixtureThermo,
    reactingMixture,
    isobaricGasThermoPhysics
);

makeReactionMixtureThermo
(
    hReactionThermo,
    hRhoMixtureThermo,
    singleStepReactingMixture,
    gasThermoPhysics
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
