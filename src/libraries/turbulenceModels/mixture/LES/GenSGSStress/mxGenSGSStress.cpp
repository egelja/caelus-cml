/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2019 Applied CCM Pty Ltd
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

#include "mxGenSGSStress.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace mixture
{
namespace LESModels
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameWithName(GenSGSStress, "GenSGSStress");


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

GenSGSStress::GenSGSStress
(
    const volScalarField& rho,
    const volVectorField& U,
    const surfaceScalarField& phi,
    const twoPhaseMixture& transport,
    const word& turbulenceModelName,
    const word& modelName
)
:
    LESModel(modelName, rho, U, phi, transport, turbulenceModelName),
    ce_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "ce",
            coeffDict_,
            1.048
        )
    ),
    B_
    (
        IOobject
        (
            "B",
            runTime_.timeName(),
            mesh_,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh_
    ),
    muSgs_
    (
        IOobject
        (
            "muSgs",
            runTime_.timeName(),
            mesh_,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh_
    )
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

tmp<volSymmTensorField> GenSGSStress::devRhoBeff() const
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
            rho()*B_ - mu()*dev(twoSymm(fvc::grad(U())))
        )
    );
}


tmp<fvVectorMatrix> GenSGSStress::divDevRhoBeff(volVectorField& U) const
{
    return
    (
        fvc::div(rho()*B_ + 0.05*muSgs_*fvc::grad(U))
      + fvc::laplacian(0.95*muSgs_, U, "laplacian(muEff,U)")
      - fvm::laplacian(muEff(), U)
      - fvc::div(mu()*dev2(T(fvc::grad(U))))
    );
}


void GenSGSStress::correct(const tmp<volTensorField>& gradU)
{
    LESModel::correct(gradU);
}


bool GenSGSStress::read()
{
    if (LESModel::read())
    {
        ce_.readIfPresent(coeffDict());

        return true;
    }
    else
    {
        return false;
    }
}


} // End namespace LESModels
} // End namespace mixture
} // End namespace CML
