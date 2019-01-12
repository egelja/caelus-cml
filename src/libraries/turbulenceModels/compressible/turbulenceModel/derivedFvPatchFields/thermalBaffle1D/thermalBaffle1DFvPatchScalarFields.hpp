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

#ifndef thermalBaffle1DFvPatchScalarFields_H
#define thermalBaffle1DFvPatchScalarFields_H

#include "thermalBaffle1DFvPatchScalarField.hpp"
#include "solidThermoPhysicsTypes.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace compressible
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    typedef thermalBaffle1DFvPatchScalarField
    <
        hConstSolidThermoPhysics
    >constSolid_thermalBaffle1DFvPatchScalarField;

    typedef thermalBaffle1DFvPatchScalarField
    <
        hPowerSolidThermoPhysics
    >expoSolid_thermalBaffle1DFvPatchScalarField;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

}
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
