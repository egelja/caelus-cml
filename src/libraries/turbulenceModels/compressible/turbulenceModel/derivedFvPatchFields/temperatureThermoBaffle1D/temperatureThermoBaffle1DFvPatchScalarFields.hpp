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

#ifndef temperatureThermoBaffle1DFvPatchScalarFields_H
#define temperatureThermoBaffle1DFvPatchScalarFields_H

#include "temperatureThermoBaffle1DFvPatchScalarField.hpp"
#include "solidThermoPhysicsTypes.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace compressible
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    typedef temperatureThermoBaffle1DFvPatchScalarField
    <
        constSolidThermoPhysics
    >constSolid_temperatureThermoBaffle1DFvPatchScalarField;

    typedef temperatureThermoBaffle1DFvPatchScalarField
    <
        expoSolidThermoPhysics
    >expoSolid_temperatureThermoBaffle1DFvPatchScalarField;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

}
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
