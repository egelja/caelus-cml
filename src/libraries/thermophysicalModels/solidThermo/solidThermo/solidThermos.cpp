/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "makeSolidThermo.hpp"
#include "solidThermo.hpp"
#include "heSolidThermo.hpp"

#include "specie.hpp"
#include "rhoConst.hpp"
#include "hConstThermo.hpp"
#include "hPowerThermo.hpp"
#include "hPolynomialThermo.hpp"
#include "constIsoSolidTransport.hpp"
#include "constAnIsoSolidTransport.hpp"
#include "exponentialSolidTransport.hpp"
#include "polynomialSolidTransport.hpp"
#include "pureMixture.hpp"
#include "sensibleEnthalpy.hpp"
#include "sensibleInternalEnergy.hpp"
#include "specieThermo.hpp"

#include "solidThermoPhysicsTypes.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/* * * * * * * * * * * * * * * * * Enthalpy-based * * * * * * * * * * * * * */

makeSolidThermo
(
    solidThermo,
    heSolidThermo,
    pureMixture,
    constIsoSolidTransport,
    sensibleEnthalpy,
    hConstThermo,
    rhoConst,
    specie
);

makeSolidThermo
(
    solidThermo,
    heSolidThermo,
    pureMixture,
    constAnIsoSolidTransport,
    sensibleEnthalpy,
    hConstThermo,
    rhoConst,
    specie
);

makeSolidThermo
(
    solidThermo,
    heSolidThermo,
    pureMixture,
    exponentialSolidTransport,
    sensibleEnthalpy,
    hPowerThermo,
    rhoConst,
    specie
);

makeSolidThermoPhysicsType
(
    solidThermo,
    heSolidThermo,
    pureMixture,
    hTransportThermoPoly8SolidThermoPhysics
);


} // End namespace CML
