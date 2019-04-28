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

#include "makeChemistrySolverTypes.hpp"

#include "thermoPhysicsTypes.hpp"
#include "psiReactionThermo.hpp"
#include "rhoReactionThermo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    // Chemistry solvers based on sensibleEnthalpy
    makeChemistrySolverTypes(psiReactionThermo, constGasHThermoPhysics);
    makeChemistrySolverTypes(psiReactionThermo, gasHThermoPhysics);
    makeChemistrySolverTypes
    (
        psiReactionThermo,
        constIncompressibleGasHThermoPhysics
    );
    makeChemistrySolverTypes
    (
        psiReactionThermo,
        incompressibleGasHThermoPhysics
    );
    makeChemistrySolverTypes(psiReactionThermo, icoPoly8HThermoPhysics);
    makeChemistrySolverTypes(psiReactionThermo, constFluidHThermoPhysics);
    makeChemistrySolverTypes
    (
        psiReactionThermo,
        constAdiabaticFluidHThermoPhysics
    );
    makeChemistrySolverTypes(psiReactionThermo, constHThermoPhysics);

    makeChemistrySolverTypes(rhoReactionThermo, constGasHThermoPhysics);
    makeChemistrySolverTypes(rhoReactionThermo, gasHThermoPhysics);
    makeChemistrySolverTypes
    (
        rhoReactionThermo,
        constIncompressibleGasHThermoPhysics
    );
    makeChemistrySolverTypes
    (
        rhoReactionThermo,
        incompressibleGasHThermoPhysics
    );
    makeChemistrySolverTypes(rhoReactionThermo, icoPoly8HThermoPhysics);
    makeChemistrySolverTypes(rhoReactionThermo, constFluidHThermoPhysics);
    makeChemistrySolverTypes
    (
        rhoReactionThermo,
        constAdiabaticFluidHThermoPhysics
    );
    makeChemistrySolverTypes(rhoReactionThermo, constHThermoPhysics);

    // Chemistry solvers based on sensibleInternalEnergy
    makeChemistrySolverTypes(psiReactionThermo, constGasEThermoPhysics);
    makeChemistrySolverTypes(psiReactionThermo, gasEThermoPhysics);
    makeChemistrySolverTypes
    (
        psiReactionThermo,
        constIncompressibleGasEThermoPhysics
    );
    makeChemistrySolverTypes
    (
        psiReactionThermo,
        incompressibleGasEThermoPhysics
    );
    makeChemistrySolverTypes(psiReactionThermo, icoPoly8EThermoPhysics);
    makeChemistrySolverTypes(psiReactionThermo, constFluidEThermoPhysics);
    makeChemistrySolverTypes
    (
        psiReactionThermo,
        constAdiabaticFluidEThermoPhysics
    );
    makeChemistrySolverTypes(psiReactionThermo, constEThermoPhysics);

    makeChemistrySolverTypes(rhoReactionThermo, constGasEThermoPhysics);
    makeChemistrySolverTypes(rhoReactionThermo, gasEThermoPhysics);
    makeChemistrySolverTypes
    (
        rhoReactionThermo,
        constIncompressibleGasEThermoPhysics
    );
    makeChemistrySolverTypes
    (
        rhoReactionThermo,
        incompressibleGasEThermoPhysics
    );
    makeChemistrySolverTypes(rhoReactionThermo, icoPoly8EThermoPhysics);
    makeChemistrySolverTypes(rhoReactionThermo, constFluidEThermoPhysics);
    makeChemistrySolverTypes
    (
        rhoReactionThermo,
        constAdiabaticFluidEThermoPhysics
    );
    makeChemistrySolverTypes(rhoReactionThermo, constEThermoPhysics);
}
