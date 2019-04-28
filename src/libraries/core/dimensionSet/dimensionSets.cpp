/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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

#include "dimensionSet.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

const dimensionSet dimless(0, 0, 0, 0, 0, 0, 0);

const dimensionSet dimMass(1, 0, 0, 0, 0, 0, 0);
const dimensionSet dimLength(0, 1, 0, 0, 0, 0, 0);
const dimensionSet dimTime(0, 0, 1, 0, 0, 0, 0);
const dimensionSet dimTemperature(0, 0, 0, 1, 0, 0, 0);
const dimensionSet dimMoles(0, 0, 0, 0, 1, 0, 0);
const dimensionSet dimCurrent(0, 0, 0, 0, 0, 1, 0);
const dimensionSet dimLuminousIntensity(0, 0, 0, 0, 0, 0, 1);

const dimensionSet dimArea(sqr(dimLength));
const dimensionSet dimVolume(pow3(dimLength));
const dimensionSet dimVol(dimVolume);

const dimensionSet dimVelocity(dimLength/dimTime);
const dimensionSet dimAcceleration(dimVelocity/dimTime);

const dimensionSet dimDensity(dimMass/dimVolume);
const dimensionSet dimForce(dimMass*dimAcceleration);
const dimensionSet dimEnergy(dimForce*dimLength);
const dimensionSet dimPower(dimEnergy/dimTime);

const dimensionSet dimPressure(dimForce/dimArea);
const dimensionSet dimCompressibility(dimDensity/dimPressure);
const dimensionSet dimGasConstant(dimEnergy/dimMass/dimTemperature);
const dimensionSet dimSpecificHeatCapacity(dimGasConstant);
const dimensionSet dimViscosity(dimArea/dimTime);
const dimensionSet dimDynamicViscosity(dimDensity*dimViscosity);

} // End namespace CML
// ************************************************************************* //
