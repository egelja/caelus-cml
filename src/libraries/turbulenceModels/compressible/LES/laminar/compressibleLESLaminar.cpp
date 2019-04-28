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

#include "compressibleLESLaminar.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace compressible
{
namespace LESModels
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(laminar, 0);
addToRunTimeSelectionTable(LESModel, laminar, dictionary);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

laminar::laminar
(
    const volScalarField& rho,
    const volVectorField& U,
    const surfaceScalarField& phi,
    const fluidThermo& thermophysicalModel,
    const word& turbulenceModelName,
    const word& modelName
)
:
    LESModel(modelName, rho, U, phi, thermophysicalModel, turbulenceModelName)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

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
            dimensionedScalar("k", sqr(U().dimensions()), 0.0)
        )
    );
}

tmp<volScalarField> laminar::epsilon() const
{
    // NOT CORRECT - INCOMPRESSIBLE DEFINITION
    return 2*(mu()/rho())*magSqr(symm(fvc::grad(U())));
}

tmp<volScalarField> laminar::muSgs() const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "muSgs",
                runTime_.timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedScalar("muSgs", mu().dimensions(), 0.0)
        )
    );
}

tmp<volScalarField> laminar::muEff() const
{
    return tmp<volScalarField>
    (
        new volScalarField("muEff", mu())
    );
}


tmp<volScalarField> laminar::alphaSgs() const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "alphaSgs",
                runTime_.timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedScalar("alphaSgs", alpha().dimensions(), 0.0)
        )
    );
}

tmp<volScalarField> laminar::alphaEff() const
{
    return tmp<volScalarField>
    (
        new volScalarField("alphaEff", alpha())
    );
}


tmp<volSymmTensorField> laminar::B() const
{
    return tmp<volSymmTensorField>
    (
        new volSymmTensorField
        (
            IOobject
            (
                "B",
                runTime_.timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedSymmTensor
            (
                "B", sqr(U().dimensions()), symmTensor::zero
            )
        )
    );
}


tmp<volSymmTensorField> laminar::devRhoBeff() const
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
            -mu()*dev(twoSymm(fvc::grad(U())))
        )
    );
}


tmp<fvVectorMatrix> laminar::divDevRhoBeff(volVectorField& U) const
{
    return
    (
      - fvm::laplacian(muEff(), U)
      - fvc::div(muEff()*dev2(T(fvc::grad(U))))
    );
}


bool laminar::read()
{
    return LESModel::read();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace LESModels
} // End namespace incompressible
} // End namespace CML

// ************************************************************************* //
