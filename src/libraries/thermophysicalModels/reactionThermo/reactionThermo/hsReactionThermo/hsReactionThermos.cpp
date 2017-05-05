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

#include "makeHsReactionThermo.hpp"

#include "hsReactionThermo.hpp"
#include "hsRhoMixtureThermo.hpp"

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

makeHsReactionThermo
(
    hsReactionThermo,
    hsRhoMixtureThermo,
    homogeneousMixture,
    constTransport,
    hConstThermo,
    perfectGas
);

makeHsReactionThermo
(
    hsReactionThermo,
    hsRhoMixtureThermo,
    inhomogeneousMixture,
    constTransport,
    hConstThermo,
    perfectGas
);

makeHsReactionThermo
(
    hsReactionThermo,
    hsRhoMixtureThermo,
    veryInhomogeneousMixture,
    constTransport,
    hConstThermo,
    perfectGas
);

makeHsReactionThermo
(
    hsReactionThermo,
    hsRhoMixtureThermo,
    homogeneousMixture,
    sutherlandTransport,
    janafThermo,
    perfectGas
);

makeHsReactionThermo
(
    hsReactionThermo,
    hsRhoMixtureThermo,
    inhomogeneousMixture,
    sutherlandTransport,
    janafThermo,
    perfectGas
);

makeHsReactionThermo
(
    hsReactionThermo,
    hsRhoMixtureThermo,
    veryInhomogeneousMixture,
    sutherlandTransport,
    janafThermo,
    perfectGas
);


makeHsReactionThermo
(
    hsReactionThermo,
    hsRhoMixtureThermo,
    dieselMixture,
    sutherlandTransport,
    janafThermo,
    perfectGas
);


// Multi-component thermo

makeHsReactionMixtureThermo
(
    hsReactionThermo,
    hsRhoMixtureThermo,
    multiComponentMixture,
    constGasThermoPhysics
);

makeHsReactionMixtureThermo
(
    hsReactionThermo,
    hsRhoMixtureThermo,
    multiComponentMixture,
    icoPoly8ThermoPhysics
);

makeHsReactionMixtureThermo
(
    hsReactionThermo,
    hsRhoMixtureThermo,
    multiComponentMixture,
    gasThermoPhysics
);


// Multi-component reaction thermo

makeHsReactionMixtureThermo
(
    hsReactionThermo,
    hsRhoMixtureThermo,
    reactingMixture,
    constGasThermoPhysics
);

makeHsReactionMixtureThermo
(
    hsReactionThermo,
    hsRhoMixtureThermo,
    reactingMixture,
    constIsobaricGasThermoPhysics
);

makeHsReactionMixtureThermo
(
    hsReactionThermo,
    hsRhoMixtureThermo,
    reactingMixture,
    icoPoly8ThermoPhysics
);

makeHsReactionMixtureThermo
(
    hsReactionThermo,
    hsRhoMixtureThermo,
    reactingMixture,
    gasThermoPhysics
);

makeHsReactionMixtureThermo
(
    hsReactionThermo,
    hsRhoMixtureThermo,
    reactingMixture,
    isobaricGasThermoPhysics
);

makeHsReactionMixtureThermo
(
    hsReactionThermo,
    hsRhoMixtureThermo,
    singleStepReactingMixture,
    gasThermoPhysics
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
