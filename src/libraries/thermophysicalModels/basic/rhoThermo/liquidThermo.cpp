/*---------------------------------------------------------------------------*\
Copyright (C) 2017-2018 OpenFOAM Foundation
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

#include "liquidThermo.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */

defineTemplateTypeNameAndDebugWithName
(
    heRhoThermopureMixtureliquidProperties,
    "heRhoThermo<pureMixture<liquid,sensibleInternalEnergy>>",
    0
);

addToRunTimeSelectionTable
(
    basicThermo,
    heRhoThermopureMixtureliquidProperties,
    fvMesh
);

addToRunTimeSelectionTable
(
    fluidThermo,
    heRhoThermopureMixtureliquidProperties,
    fvMesh
);

addToRunTimeSelectionTable
(
    rhoThermo,
    heRhoThermopureMixtureliquidProperties,
    fvMesh
);


defineTemplateTypeNameAndDebugWithName
(
    heRhoThermopureMixtureEnthalpyliquidProperties,
    "heRhoThermo<pureMixture<liquid,sensibleEnthalpy>>",
    0
);

addToRunTimeSelectionTable
(
    basicThermo,
    heRhoThermopureMixtureEnthalpyliquidProperties,
    fvMesh
);

addToRunTimeSelectionTable
(
    fluidThermo,
    heRhoThermopureMixtureEnthalpyliquidProperties,
    fvMesh
);

addToRunTimeSelectionTable
(
    rhoThermo,
    heRhoThermopureMixtureEnthalpyliquidProperties,
    fvMesh
);


} // End namespace CML
