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

#include "makeSolidChemistrySolverType.hpp"
#include "solidThermoPhysicsTypes.hpp"
#include "thermoPhysicsTypes.hpp"

#include "chemistrySolver.hpp"

#include "ODESolidChemistryModel.hpp"
#include "solidChemistryModel.hpp"

#include "ode_.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    typedef ODESolidChemistryModel
        <solidChemistryModel, constSolidThermoPhysics, gasThermoPhysics>
            solidODEChemistryConstThermo;

    makeChemistrySolver(solidODEChemistryConstThermo)

    makeSolidChemistrySolverType
    (
        ode,
        ODESolidChemistryModel,
        solidChemistryModel,
        constSolidThermoPhysics,
        gasThermoPhysics
    )

    typedef ODESolidChemistryModel
        <solidChemistryModel, expoSolidThermoPhysics, gasThermoPhysics>
            solidODEChemistryExpThermo;

    makeChemistrySolver(solidODEChemistryExpThermo)

    makeSolidChemistrySolverType
    (
        ode,
        ODESolidChemistryModel,
        solidChemistryModel,
        expoSolidThermoPhysics,
        gasThermoPhysics
    )
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
