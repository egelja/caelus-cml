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

\*---------------------------------------------------------------------------*/

#include "compressibleVLESLaminar.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{
namespace compressible
{
namespace VLESModels
{

defineTypeNameAndDebug(laminar, 0);
addToRunTimeSelectionTable(VLESModel, laminar, dictionary);

laminar::laminar
(
    const volScalarField& rho,
    const volVectorField& U,
    const surfaceScalarField& phi,
    const fluidThermo& thermoPhysicalModel,
    const word& turbulenceModelName,
    const word& modelName
) : VLESModel(modelName, rho, U, phi, thermoPhysicalModel, turbulenceModelName)
{}

tmp<volScalarField> laminar::Fr() const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "Fr",
                runTime_.timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedScalar("fr", dimless, 1)
        )
    );
}

tmp<volScalarField> laminar::mut() const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "mut",
                runTime_.timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedScalar("mut", mu().dimensions(), 0.0)
        )
    );
}

tmp<volScalarField> laminar::alphat() const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "alphat",
                runTime_.timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedScalar("alphat", alpha().dimensions(), 0.0)
        )
    );
}

tmp<volScalarField> laminar::k() const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "k",
                runTime_.timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedScalar("k", sqr(U_.dimensions()), 0.0)
        )
    );
}

tmp<volScalarField> laminar::epsilon() const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "epsilon",
                runTime_.timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedScalar
            (
                "epsilon", sqr(U_.dimensions())/dimTime, 0.0
            )
        )
    );
}

tmp<volSymmTensorField> laminar::R() const
{
    return tmp<volSymmTensorField>
    (
        new volSymmTensorField
        (
            IOobject
            (
                "R",
                runTime_.timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedSymmTensor
            (
                "R", sqr(U_.dimensions()), symmTensor::zero
            )
        )
    );
}

tmp<volSymmTensorField> laminar::devRhoReff() const
{
    return tmp<volSymmTensorField>
    (
        new volSymmTensorField
        (
            IOobject
            (
                "devRhoReff",
                runTime_.timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
           -mu()*dev(twoSymm(fvc::grad(U_)))
        )
    );
}

tmp<fvVectorMatrix> laminar::divDevRhoReff(volVectorField& U) const
{
    return
    (
      - fvm::laplacian(mu(), U)
      - fvc::div(mu()*dev2(T(fvc::grad(U))))
    );
}

bool laminar::read()
{
    return VLESModel::read();
}

void laminar::correct()
{
    turbulenceModel::correct();
}

}
}
}

