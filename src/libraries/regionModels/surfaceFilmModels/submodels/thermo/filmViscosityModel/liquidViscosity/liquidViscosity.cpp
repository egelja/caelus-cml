/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2018 OpenFOAM Foundation
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

#include "liquidViscosity.hpp"
#include "thermoSingleLayer.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(liquidViscosity, 0);

addToRunTimeSelectionTable
(
    filmViscosityModel,
    liquidViscosity,
    dictionary
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

liquidViscosity::liquidViscosity
(
    surfaceFilmRegionModel& film,
    const dictionary& dict,
    volScalarField& mu
)
:
    filmViscosityModel(typeName, film, dict, mu)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

liquidViscosity::~liquidViscosity()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void liquidViscosity::correct
(
    const volScalarField& p,
    const volScalarField& T
)
{
    const thermoSingleLayer& film = filmType<thermoSingleLayer>();

    mu_ = film.filmThermo().mu();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // End namespace regionModels
} // End namespace CML

// ************************************************************************* //
