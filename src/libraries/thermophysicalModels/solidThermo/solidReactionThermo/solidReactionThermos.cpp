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

#include "makeReactingSolidThermo.hpp"
#include "solidReactionThermo.hpp"
#include "heSolidThermo.hpp"

#include "specie.hpp"
#include "rhoConst.hpp"
#include "hConstThermo.hpp"
#include "hPowerThermo.hpp"
#include "constIsoSolidTransport.hpp"
#include "constAnIsoSolidTransport.hpp"
#include "exponentialSolidTransport.hpp"

#include "reactingMixture.hpp"
#include "multiComponentMixture.hpp"

#include "sensibleEnthalpy.hpp"
#include "specieThermo.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/* * * * * * * * * * * * * * * * * Enthalpy-based * * * * * * * * * * * * * */

makeReactingSolidThermo
(
    solidReactionThermo,
    heSolidThermo,
    reactingMixture,
    constIsoSolidTransport,
    sensibleEnthalpy,
    hConstThermo,
    rhoConst,
    specie
);


makeReactingSolidThermo
(
    solidReactionThermo,
    heSolidThermo,
    reactingMixture,
    constIsoSolidTransport,
    sensibleEnthalpy,
    hPowerThermo,
    rhoConst,
    specie
);


makeReactingSolidThermo
(
    solidThermo,
    heSolidThermo,
    multiComponentMixture,
    constIsoSolidTransport,
    sensibleEnthalpy,
    hConstThermo,
    rhoConst,
    specie
);


} // End namespace CML
