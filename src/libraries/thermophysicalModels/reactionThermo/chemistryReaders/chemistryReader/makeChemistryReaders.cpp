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

#include "makeReactionThermo.hpp"
#include "thermoPhysicsTypes.hpp"
#include "solidThermoPhysicsTypes.hpp"

#include "chemistryReader.hpp"
#include "caelusChemistryReader.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Solid chemistry readers based on sensibleEnthalpy

makeChemistryReader(constGasHThermoPhysics);
makeChemistryReader(gasHThermoPhysics);
makeChemistryReader(constIncompressibleGasHThermoPhysics);
makeChemistryReader(incompressibleGasHThermoPhysics);
makeChemistryReader(icoPoly8HThermoPhysics);
makeChemistryReader(constFluidHThermoPhysics);
makeChemistryReader(constAdiabaticFluidHThermoPhysics);
makeChemistryReader(constHThermoPhysics);

makeChemistryReaderType(caelusChemistryReader, constGasHThermoPhysics);
makeChemistryReaderType(caelusChemistryReader, gasHThermoPhysics);
makeChemistryReaderType
(
    caelusChemistryReader,
    constIncompressibleGasHThermoPhysics
);
makeChemistryReaderType(caelusChemistryReader, incompressibleGasHThermoPhysics);
makeChemistryReaderType(caelusChemistryReader, icoPoly8HThermoPhysics);
makeChemistryReaderType(caelusChemistryReader, constFluidHThermoPhysics);
makeChemistryReaderType(caelusChemistryReader, constAdiabaticFluidHThermoPhysics);
makeChemistryReaderType(caelusChemistryReader, constHThermoPhysics);


// Solid chemistry readers based on sensibleInternalEnergy

makeChemistryReader(constGasEThermoPhysics);
makeChemistryReader(gasEThermoPhysics);
makeChemistryReader(constIncompressibleGasEThermoPhysics);
makeChemistryReader(incompressibleGasEThermoPhysics);
makeChemistryReader(icoPoly8EThermoPhysics);
makeChemistryReader(constFluidEThermoPhysics);
makeChemistryReader(constAdiabaticFluidEThermoPhysics);
makeChemistryReader(constEThermoPhysics);

makeChemistryReaderType(caelusChemistryReader, constGasEThermoPhysics);
makeChemistryReaderType(caelusChemistryReader, gasEThermoPhysics);
makeChemistryReaderType
(
    caelusChemistryReader,
    constIncompressibleGasEThermoPhysics
);
makeChemistryReaderType(caelusChemistryReader, incompressibleGasEThermoPhysics);
makeChemistryReaderType(caelusChemistryReader, icoPoly8EThermoPhysics);
makeChemistryReaderType(caelusChemistryReader, constFluidEThermoPhysics);
makeChemistryReaderType(caelusChemistryReader, constAdiabaticFluidEThermoPhysics);
makeChemistryReaderType(caelusChemistryReader, constEThermoPhysics);


// Solid chemistry readers for solids based on sensibleInternalEnergy

makeChemistryReader(hConstSolidThermoPhysics);
makeChemistryReader(hPowerSolidThermoPhysics);
makeChemistryReader(hExpKappaConstSolidThermoPhysics);

makeChemistryReaderType(caelusChemistryReader, hConstSolidThermoPhysics);
//makeChemistryReaderType(caelusChemistryReader, hPowerSolidThermoPhysics);
makeChemistryReaderType(caelusChemistryReader, hExpKappaConstSolidThermoPhysics);


} // End namespace CML
