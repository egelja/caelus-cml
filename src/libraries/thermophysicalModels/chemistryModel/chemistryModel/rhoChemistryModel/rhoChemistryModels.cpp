/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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

InClass
    CML::rhoChemistryModel

Description
    Creates chemistry model instances templated on the type of thermodynamics

\*---------------------------------------------------------------------------*/

#include "makeChemistryModel.hpp"

#include "rhoChemistryModel.hpp"
#include "ODEChemistryModel.hpp"
#include "thermoPhysicsTypes.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    makeChemistryModel
    (
        ODEChemistryModel,
        rhoChemistryModel,
        constGasThermoPhysics
    );
    makeChemistryModel
    (
        ODEChemistryModel,
        rhoChemistryModel,
        gasThermoPhysics
    );
    makeChemistryModel
    (
        ODEChemistryModel,
        rhoChemistryModel,
        constIsobaricGasThermoPhysics
    );
    makeChemistryModel
    (
        ODEChemistryModel,
        rhoChemistryModel,
        isobaricGasThermoPhysics
    );
    makeChemistryModel
    (
        ODEChemistryModel,
        rhoChemistryModel,
        icoPoly8ThermoPhysics
    );
}

// ************************************************************************* //
