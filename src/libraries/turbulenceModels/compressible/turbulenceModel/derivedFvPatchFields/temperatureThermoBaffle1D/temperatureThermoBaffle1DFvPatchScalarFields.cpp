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

#include "temperatureThermoBaffle1DFvPatchScalarFields.hpp"
#include "addToRunTimeSelectionTable.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //



// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTemplateTypeNameAndDebugWithName
(
    CML::compressible::constSolid_temperatureThermoBaffle1DFvPatchScalarField,
    "compressible::temperatureThermoBaffle1D<constSolidThermoPhysics>",
    0
);


defineTemplateTypeNameAndDebugWithName
(
    CML::compressible::expoSolid_temperatureThermoBaffle1DFvPatchScalarField,
    "compressible::temperatureThermoBaffle1D<expoSolidThermoPhysics>",
    0
);

namespace CML
{
namespace compressible
{
    addToPatchFieldRunTimeSelection
    (
        fvPatchScalarField,
        constSolid_temperatureThermoBaffle1DFvPatchScalarField
    );

    addToPatchFieldRunTimeSelection
    (
        fvPatchScalarField,
        expoSolid_temperatureThermoBaffle1DFvPatchScalarField
    );
} // End namespace compressible
} // End namespace CML
// ************************************************************************* //
