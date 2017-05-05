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

#include "rhoChemistryCombustionModel.hpp"

/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */

namespace CML
{
namespace combustionModels
{
    defineTypeNameAndDebug(rhoChemistryCombustionModel, 0);
    defineRunTimeSelectionTable(rhoChemistryCombustionModel, dictionary);



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

rhoChemistryCombustionModel::rhoChemistryCombustionModel
(
    const word& modelType,
    const fvMesh& mesh
)
:
    combustionModel(modelType, mesh),
    pChemistry_(rhoChemistryModel::New(mesh))
{}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

rhoChemistryCombustionModel::~rhoChemistryCombustionModel()
{}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //


bool rhoChemistryCombustionModel::read()
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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML
} // End namespace combustionModels

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
