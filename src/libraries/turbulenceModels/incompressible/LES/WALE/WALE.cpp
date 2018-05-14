/*---------------------------------------------------------------------------*\
Copyright (C) 2018 OpenFOAM Foundation
Copyright (C) 2018 Applied CCM
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

#include "WALE.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{
namespace incompressible
{
namespace LESModels
{

defineTypeNameAndDebug(WALE, 0);
addToRunTimeSelectionTable(LESModel, WALE, dictionary);

void WALE::updateSubGridScaleFields(volTensorField const& gradU)
{
    nuSgs_ = ck_*delta()*sqrt(k(gradU));
    nuSgs_.correctBoundaryConditions();
}

WALE::WALE
(
    volVectorField const& U,
    surfaceScalarField const& phi,
    transportModel& transport,
    word const& turbulenceModelName,
    word const& modelName
) :
    LESModel(modelName, U, phi, transport, turbulenceModelName),
    GenEddyVisc(U, phi, transport),

    ck_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "ck",
            coeffDict_,
            0.094
        )
     ),
    cw_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "cw",
            coeffDict_,
            0.325
        )
    )
{
    updateSubGridScaleFields(fvc::grad(U));

    printCoeffs();
}

void WALE::correct(tmp<volTensorField> const& gradU)
{
    GenEddyVisc::correct(gradU);
    updateSubGridScaleFields(gradU());
}


bool WALE::read()
{
    if (GenEddyVisc::read())
    {
        ck_.readIfPresent(coeffDict());
	cw_.readIfPresent(coeffDict());

        return true;
    }
    else
    {
        return false;
    }
}

} // End namespace LESModels
} // End namespace incompressible
} // End namespace CML

