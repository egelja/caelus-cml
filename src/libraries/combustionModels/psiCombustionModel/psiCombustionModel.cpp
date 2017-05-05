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

#include "psiCombustionModel.hpp"

/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */

namespace CML
{
namespace combustionModels
{
    defineTypeNameAndDebug(psiCombustionModel, 0);
    defineRunTimeSelectionTable(psiCombustionModel, dictionary);
}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


CML::combustionModels::psiCombustionModel::psiCombustionModel
(
    const word& modelType,
    const fvMesh& mesh
)
:
    combustionModel(modelType, mesh),
    thermo_(hsCombustionThermo::New(mesh))
{}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::combustionModels::psiCombustionModel::~psiCombustionModel()
{}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //


bool CML::combustionModels::psiCombustionModel::read()
{
    if (combustionModel::read())
    {
        return true;
    }
    else
    {
        return false;
    }
}

CML::tmp<CML::volScalarField>
CML::combustionModels::psiCombustionModel::rho() const
{
    return thermo_->rho();
}

// ************************************************************************* //
