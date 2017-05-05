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

#include "psiChemistryModel.hpp"
#include "fvMesh.hpp"
#include "Time.hpp"

/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */

namespace CML
{
    defineTypeNameAndDebug(psiChemistryModel, 0);
    defineRunTimeSelectionTable(psiChemistryModel, fvMesh);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::psiChemistryModel::psiChemistryModel
(
    const fvMesh& mesh,
    const word& thermoTypeName
)
:
    basicChemistryModel(mesh),
    thermo_(hsCombustionThermo::NewType(mesh, thermoTypeName))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::psiChemistryModel::~psiChemistryModel()
{}


// ************************************************************************* //
