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

#include "dimensionSet.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

const CML::dimensionSet CML::dimless(0, 0, 0, 0, 0, 0, 0);

const CML::dimensionSet CML::dimMass(1, 0, 0, 0, 0, 0, 0);
const CML::dimensionSet CML::dimLength(0, 1, 0, 0, 0, 0, 0);
const CML::dimensionSet CML::dimTime(0, 0, 1, 0, 0, 0, 0);
const CML::dimensionSet CML::dimTemperature(0, 0, 0, 1, 0, 0, 0);
const CML::dimensionSet CML::dimMoles(0, 0, 0, 0, 1, 0, 0);
const CML::dimensionSet CML::dimCurrent(0, 0, 0, 0, 0, 1, 0);
const CML::dimensionSet CML::dimLuminousIntensity(0, 0, 0, 0, 0, 0, 1);

const CML::dimensionSet CML::dimArea(sqr(dimLength));
const CML::dimensionSet CML::dimVolume(pow3(dimLength));
const CML::dimensionSet CML::dimVol(dimVolume);

const CML::dimensionSet CML::dimVelocity(dimLength/dimTime);
const CML::dimensionSet CML::dimAcceleration(dimVelocity/dimTime);

const CML::dimensionSet CML::dimDensity(dimMass/dimVolume);
const CML::dimensionSet CML::dimForce(dimMass*dimAcceleration);
const CML::dimensionSet CML::dimEnergy(dimForce*dimLength);
const CML::dimensionSet CML::dimPower(dimEnergy/dimTime);

const CML::dimensionSet CML::dimPressure(dimForce/dimArea);
const CML::dimensionSet CML::dimGasConstant(dimEnergy/dimMass/dimTemperature);
const CML::dimensionSet CML::dimSpecificHeatCapacity(dimGasConstant);


// ************************************************************************* //
