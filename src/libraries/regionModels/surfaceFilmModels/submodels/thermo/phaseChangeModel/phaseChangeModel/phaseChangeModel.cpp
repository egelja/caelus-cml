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

#include "phaseChangeModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(phaseChangeModel, 0);
defineRunTimeSelectionTable(phaseChangeModel, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

phaseChangeModel::phaseChangeModel
(
    const surfaceFilmModel& owner
)
:
    filmSubModelBase(owner),
    latestMassPC_(0.0),
    totalMassPC_(0.0)
{}


phaseChangeModel::phaseChangeModel
(
    const word& type,
    const surfaceFilmModel& owner,
    const dictionary& dict
)
:
    filmSubModelBase(type, owner, dict),
    latestMassPC_(0.0),
    totalMassPC_(0.0)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

phaseChangeModel::~phaseChangeModel()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void phaseChangeModel::correct
(
    const scalar dt,
    scalarField& availableMass,
    volScalarField& dMass,
    volScalarField& dEnergy
)
{
    correctModel
    (
        dt,
        availableMass,
        dMass,
        dEnergy
    );

    latestMassPC_ = sum(dMass.internalField());
    totalMassPC_ += latestMassPC_;

    availableMass -= dMass;
    dMass.correctBoundaryConditions();
}


void phaseChangeModel::info(Ostream& os) const
{
    const scalar massPCRate =
        returnReduce(latestMassPC_, sumOp<scalar>())
       /owner_.time().deltaTValue();

    os  << indent << "mass phase change  = "
        << returnReduce(totalMassPC_, sumOp<scalar>()) << nl
        << indent << "vapourisation rate = " << massPCRate << nl;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // end namespace surfaceFilmModels
} // end namespace regionModels
} // end namespace CML

// ************************************************************************* //
