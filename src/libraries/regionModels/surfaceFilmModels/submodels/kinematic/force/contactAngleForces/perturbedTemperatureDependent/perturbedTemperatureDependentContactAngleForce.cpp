/*---------------------------------------------------------------------------*\
Copyright (C) 2017-2018 OpenFOAM Foundation
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

#include "perturbedTemperatureDependentContactAngleForce.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(perturbedTemperatureDependentContactAngleForce, 0);
addToRunTimeSelectionTable
(
    force,
    perturbedTemperatureDependentContactAngleForce,
    dictionary
);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

perturbedTemperatureDependentContactAngleForce::
perturbedTemperatureDependentContactAngleForce
(
    surfaceFilmRegionModel& film,
    const dictionary& dict
)
:
    contactAngleForce(typeName, film, dict),
    thetaPtr_(DataEntry<scalar>::New("theta", coeffDict_)),
    rndGen_(label(0)),
    distribution_
    (
        distributionModel::New
        (
            coeffDict_.subDict("distribution"),
            rndGen_
        )
    )
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

perturbedTemperatureDependentContactAngleForce::
~perturbedTemperatureDependentContactAngleForce()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

tmp<volScalarField>
perturbedTemperatureDependentContactAngleForce::theta() const
{
    tmp<volScalarField> ttheta
    (
        new volScalarField
        (
            IOobject
            (
                typeName + ":theta",
                filmModel_.time().timeName(),
                filmModel_.regionMesh()
            ),
            filmModel_.regionMesh(),
            dimensionedScalar("0", dimless, 0)
        )
    );

    volScalarField& theta = ttheta();
    DimensionedField<scalar, volMesh>& thetai = theta.dimensionedInternalField();

    const volScalarField& T = filmModel_.T();

    // Initialize with the function of temperature
    thetai.field() = thetaPtr_->value(T);

    // Add the stochastic perturbation
    forAll(thetai, celli)
    {
        thetai[celli] += distribution_->sample();
    }

    forAll(theta.boundaryField(), patchi)
    {
        if (!filmModel_.isCoupledPatch(patchi))
        {
            fvPatchField<scalar>& thetaf = theta.boundaryField()[patchi];

            // Initialize with the function of temperature
            thetaf = thetaPtr_->value(T.boundaryField()[patchi]);

            // Add the stochastic perturbation
            forAll(thetaf, facei)
            {
                thetaf[facei] += distribution_->sample();
            }
        }
    }

    return ttheta;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // End namespace regionModels
} // End namespace CML

// ************************************************************************* //
