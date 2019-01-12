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

#include "thermocapillaryForce.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvcGrad.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(thermocapillaryForce, 0);
addToRunTimeSelectionTable(force, thermocapillaryForce, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

thermocapillaryForce::thermocapillaryForce
(
    surfaceFilmRegionModel& film,
    const dictionary& dict
)
:
    force(film)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

thermocapillaryForce::~thermocapillaryForce()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

tmp<fvVectorMatrix> thermocapillaryForce::correct(volVectorField& U)
{
    const volScalarField& sigma = filmModel_.sigma();

    tmp<fvVectorMatrix>
        tfvm(new fvVectorMatrix(U, dimForce/dimArea*dimVolume));

    tfvm() += fvc::grad(sigma);

    return tfvm;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // End namespace regionModels
} // End namespace CML

// ************************************************************************* //
