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

#include "makeCombustionThermo.hpp"

#include "hCombustionThermo.hpp"
#include "hPsiMixtureThermo.hpp"

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

// constTransport, hConstThermo

makeCombustionThermo
(
    hCombustionThermo,
    hPsiMixtureThermo,
    homogeneousMixture,
    constTransport,
    hConstThermo,
    perfectGas
);

makeCombustionThermo
(
    hCombustionThermo,
    hPsiMixtureThermo,
    inhomogeneousMixture,
    constTransport,
    hConstThermo,
    perfectGas
);

makeCombustionThermo
(
    hCombustionThermo,
    hPsiMixtureThermo,
    veryInhomogeneousMixture,
    constTransport,
    hConstThermo,
    perfectGas
);


// sutherlandTransport, hConstThermo

makeCombustionThermo
(
    hCombustionThermo,
    hPsiMixtureThermo,
    homogeneousMixture,
    sutherlandTransport,
    hConstThermo,
    perfectGas
);

makeCombustionThermo
(
    hCombustionThermo,
    hPsiMixtureThermo,
    inhomogeneousMixture,
    sutherlandTransport,
    hConstThermo,
    perfectGas
);

makeCombustionThermo
(
    hCombustionThermo,
    hPsiMixtureThermo,
    veryInhomogeneousMixture,
    sutherlandTransport,
    hConstThermo,
    perfectGas
);


// sutherlandTransport, janafThermo

makeCombustionThermo
(
    hCombustionThermo,
    hPsiMixtureThermo,
    homogeneousMixture,
    sutherlandTransport,
    janafThermo,
    perfectGas
);

makeCombustionThermo
(
    hCombustionThermo,
    hPsiMixtureThermo,
    inhomogeneousMixture,
    sutherlandTransport,
    janafThermo,
    perfectGas
);

makeCombustionThermo
(
    hCombustionThermo,
    hPsiMixtureThermo,
    veryInhomogeneousMixture,
    sutherlandTransport,
    janafThermo,
    perfectGas
);


makeCombustionThermo
(
    hCombustionThermo,
    hPsiMixtureThermo,
    dieselMixture,
    sutherlandTransport,
    janafThermo,
    perfectGas
);

// Multi-component thermo

makeCombustionMixtureThermo
(
    hCombustionThermo,
    hPsiMixtureThermo,
    multiComponentMixture,
    constGasThermoPhysics
);

makeCombustionMixtureThermo
(
    hCombustionThermo,
    hPsiMixtureThermo,
    multiComponentMixture,
    gasThermoPhysics
);

makeCombustionMixtureThermo
(
    hCombustionThermo,
    hPsiMixtureThermo,
    multiComponentMixture,
    constIsobaricGasThermoPhysics
);

makeCombustionMixtureThermo
(
    hCombustionThermo,
    hPsiMixtureThermo,
    multiComponentMixture,
    isobaricGasThermoPhysics
);


// Multi-component reaction thermo

makeCombustionMixtureThermo
(
    hCombustionThermo,
    hPsiMixtureThermo,
    reactingMixture,
    constGasThermoPhysics
);

makeCombustionMixtureThermo
(
    hCombustionThermo,
    hPsiMixtureThermo,
    reactingMixture,
    gasThermoPhysics
);

makeCombustionMixtureThermo
(
    hCombustionThermo,
    hPsiMixtureThermo,
    reactingMixture,
    constIsobaricGasThermoPhysics
);

makeCombustionMixtureThermo
(
    hCombustionThermo,
    hPsiMixtureThermo,
    reactingMixture,
    isobaricGasThermoPhysics
);

makeCombustionMixtureThermo
(
    hCombustionThermo,
    hPsiMixtureThermo,
    singleStepReactingMixture,
    gasThermoPhysics
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
