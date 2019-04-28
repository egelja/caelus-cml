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

#include "constantHeatTransfer.hpp"
#include "volFields.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(constantHeatTransfer, 0);

addToRunTimeSelectionTable
(
    heatTransferModel,
    constantHeatTransfer,
    dictionary
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

constantHeatTransfer::constantHeatTransfer
(
    surfaceFilmRegionModel& film,
    const dictionary& dict
)
:
    heatTransferModel(typeName, film, dict),
    c0_(readScalar(coeffDict_.lookup("c0")))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

constantHeatTransfer::~constantHeatTransfer()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void constantHeatTransfer::correct()
{}


tmp<volScalarField> constantHeatTransfer::h() const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "htc",
                filmModel_.time().timeName(),
                filmModel_.regionMesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            filmModel_.regionMesh(),
            dimensionedScalar
            (
                "c0",
                dimEnergy/dimTime/sqr(dimLength)/dimTemperature,
                c0_
            )
        )
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // End namespace regionModels
} // End namespace CML

// ************************************************************************* //
