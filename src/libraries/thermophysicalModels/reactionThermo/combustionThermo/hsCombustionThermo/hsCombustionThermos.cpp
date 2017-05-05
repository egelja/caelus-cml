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

#include "makeHsCombustionThermo.hpp"

#include "hsCombustionThermo.hpp"
#include "hsPsiMixtureThermo.hpp"

#include "perfectGas.hpp"

#include "hConstThermo.hpp"
#include "janafThermo.hpp"
#include "specieThermo.hpp"

#include "constTransport.hpp"
#include "sutherlandTransport.hpp"

#include "dieselMixture.hpp"
#include "homogeneousMixture.hpp"
#include "inhomogeneousMixture.hpp"
#include "veryInhomogeneousMixture.hpp"
#include "singleStepReactingMixture.hpp"

#include "reactingMixture.hpp"
#include "multiComponentMixture.hpp"

#include "thermoPhysicsTypes.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makeHsCombustionThermo
(
    hsCombustionThermo,
    hsPsiMixtureThermo,
    homogeneousMixture,
    constTransport,
    hConstThermo,
    perfectGas
);

makeHsCombustionThermo
(
    hsCombustionThermo,
    hsPsiMixtureThermo,
    inhomogeneousMixture,
    constTransport,
    hConstThermo,
    perfectGas
);

makeHsCombustionThermo
(
    hsCombustionThermo,
    hsPsiMixtureThermo,
    veryInhomogeneousMixture,
    constTransport,
    hConstThermo,
    perfectGas
);

makeHsCombustionThermo
(
    hsCombustionThermo,
    hsPsiMixtureThermo,
    homogeneousMixture,
    sutherlandTransport,
    janafThermo,
    perfectGas
);

makeHsCombustionThermo
(
    hsCombustionThermo,
    hsPsiMixtureThermo,
    inhomogeneousMixture,
    sutherlandTransport,
    janafThermo,
    perfectGas
);

makeHsCombustionThermo
(
    hsCombustionThermo,
    hsPsiMixtureThermo,
    veryInhomogeneousMixture,
    sutherlandTransport,
    janafThermo,
    perfectGas
);

makeHsCombustionThermo
(
    hsCombustionThermo,
    hsPsiMixtureThermo,
    dieselMixture,
    sutherlandTransport,
    janafThermo,
    perfectGas
);

// Multi-component thermo

makeHsCombustionMixtureThermo
(
    hsCombustionThermo,
    hsPsiMixtureThermo,
    multiComponentMixture,
    constGasThermoPhysics
);

makeHsCombustionMixtureThermo
(
    hsCombustionThermo,
    hsPsiMixtureThermo,
    multiComponentMixture,
    gasThermoPhysics
);


// Multi-component reaction thermo

makeHsCombustionMixtureThermo
(
    hsCombustionThermo,
    hsPsiMixtureThermo,
    reactingMixture,
    constGasThermoPhysics
);

makeHsCombustionMixtureThermo
(
    hsCombustionThermo,
    hsPsiMixtureThermo,
    reactingMixture,
    gasThermoPhysics
);

makeHsCombustionMixtureThermo
(
    hsCombustionThermo,
    hsPsiMixtureThermo,
    reactingMixture,
    constIsobaricGasThermoPhysics
);

makeHsCombustionMixtureThermo
(
    hsCombustionThermo,
    hsPsiMixtureThermo,
    reactingMixture,
    isobaricGasThermoPhysics
);

makeHsCombustionMixtureThermo
(
    hsCombustionThermo,
    hsPsiMixtureThermo,
    singleStepReactingMixture,
    gasThermoPhysics
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
