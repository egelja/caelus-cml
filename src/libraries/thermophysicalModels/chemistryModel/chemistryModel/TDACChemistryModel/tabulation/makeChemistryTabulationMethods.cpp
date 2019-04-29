/*---------------------------------------------------------------------------*\
Copyright (C) 2016-2018 OpenFOAM Foundation
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

#include "makeChemistryTabulationMethods.hpp"

#include "thermoPhysicsTypes.hpp"

#include "psiReactionThermo.hpp"
#include "rhoReactionThermo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    // Chemistry solvers based on sensibleEnthalpy
    makeChemistryTabulationMethods(psiReactionThermo, constGasHThermoPhysics);
    makeChemistryTabulationMethods(psiReactionThermo, gasHThermoPhysics);
    makeChemistryTabulationMethods
    (
        psiReactionThermo,
        constIncompressibleGasHThermoPhysics
    );
    makeChemistryTabulationMethods
    (
        psiReactionThermo,
        incompressibleGasHThermoPhysics
    );
    makeChemistryTabulationMethods(psiReactionThermo, icoPoly8HThermoPhysics);
    makeChemistryTabulationMethods(psiReactionThermo, constFluidHThermoPhysics);
    makeChemistryTabulationMethods
    (
        psiReactionThermo,
        constAdiabaticFluidHThermoPhysics
    );
    makeChemistryTabulationMethods(psiReactionThermo, constHThermoPhysics);

    makeChemistryTabulationMethods(rhoReactionThermo, constGasHThermoPhysics);
    makeChemistryTabulationMethods(rhoReactionThermo, gasHThermoPhysics);
    makeChemistryTabulationMethods
    (
        rhoReactionThermo,
        constIncompressibleGasHThermoPhysics
    );
    makeChemistryTabulationMethods
    (
        rhoReactionThermo,
        incompressibleGasHThermoPhysics
    );
    makeChemistryTabulationMethods(rhoReactionThermo, icoPoly8HThermoPhysics);
    makeChemistryTabulationMethods(rhoReactionThermo, constFluidHThermoPhysics);
    makeChemistryTabulationMethods
    (
        rhoReactionThermo,
        constAdiabaticFluidHThermoPhysics
    );
    makeChemistryTabulationMethods(rhoReactionThermo, constHThermoPhysics);

    // Chemistry solvers based on sensibleInternalEnergy

    makeChemistryTabulationMethods(psiReactionThermo, constGasEThermoPhysics);
    makeChemistryTabulationMethods(psiReactionThermo, gasEThermoPhysics);
    makeChemistryTabulationMethods
    (
        psiReactionThermo,
        constIncompressibleGasEThermoPhysics
    );
    makeChemistryTabulationMethods
    (
        psiReactionThermo,
        incompressibleGasEThermoPhysics
    );
    makeChemistryTabulationMethods(psiReactionThermo, icoPoly8EThermoPhysics);
    makeChemistryTabulationMethods(psiReactionThermo, constFluidEThermoPhysics);
    makeChemistryTabulationMethods
    (
        psiReactionThermo,
        constAdiabaticFluidEThermoPhysics
    );
    makeChemistryTabulationMethods(psiReactionThermo, constEThermoPhysics);

    makeChemistryTabulationMethods(rhoReactionThermo, constGasEThermoPhysics);
    makeChemistryTabulationMethods(rhoReactionThermo, gasEThermoPhysics);
    makeChemistryTabulationMethods
    (
        rhoReactionThermo,
        constIncompressibleGasEThermoPhysics
    );
    makeChemistryTabulationMethods
    (
        rhoReactionThermo,
        incompressibleGasEThermoPhysics
    );
    makeChemistryTabulationMethods(rhoReactionThermo, icoPoly8EThermoPhysics);
    makeChemistryTabulationMethods(rhoReactionThermo, constFluidEThermoPhysics);
    makeChemistryTabulationMethods
    (
        rhoReactionThermo,
        constAdiabaticFluidEThermoPhysics
    );
    makeChemistryTabulationMethods(rhoReactionThermo, constEThermoPhysics);
}