/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
Copyright (C) 2014 Applied CCM
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


#include "kOmegaSSTSAS.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{
namespace incompressible
{
namespace RASModels
{

defineTypeNameAndDebug(kOmegaSSTSAS, 0);
addToRunTimeSelectionTable(RASModel, kOmegaSSTSAS, dictionary);

tmp<volScalarField> kOmegaSSTSAS::F1(const volScalarField& CDkOmega) const
{
    tmp<volScalarField> CDkOmegaPlus = max
    (
        CDkOmega,
        dimensionedScalar("1.0e-10", dimless/sqr(dimTime), 1.0e-10)
    );

    tmp<volScalarField> arg1 = min
    (
        max
        (
            (scalar(1)/betaStar_)*sqrt(k_)/(omega_*y_),
            scalar(500)*nu()/(sqr(y_)*omega_)
        ),
        (4*alphaOmega2_)*k_/(CDkOmegaPlus*sqr(y_))
    );

    return tanh(pow4(arg1));
}

tmp<volScalarField> kOmegaSSTSAS::F2() const
{
    tmp<volScalarField> arg2 = max
    (
        (scalar(2)/betaStar_)*sqrt(k_)/(omega_*y_),
        scalar(500)*nu()/(sqr(y_)*omega_)
    );

    return tanh(sqr(arg2));
}


kOmegaSSTSAS::kOmegaSSTSAS
(
    const volVectorField& U,
    const surfaceScalarField& phi,
    transportModel& transport,
    const word& turbulenceModelName,
    const word& modelName
)
:
    RASModel(modelName, U, phi, transport, turbulenceModelName),

    alphaK1_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "alphaK1",
            coeffDict_,
            0.85
        )
    ),
    alphaK2_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "alphaK2",
            coeffDict_,
            1.0
        )
    ),
    alphaOmega1_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "alphaOmega1",
            coeffDict_,
            0.5
        )
    ),
    alphaOmega2_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "alphaOmega2",
            coeffDict_,
            0.856
        )
    ),
    gamma1_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "gamma1",
            coeffDict_,
            0.5555
        )
    ),
    gamma2_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "gamma2",
            coeffDict_,
            0.44
        )
    ),
    beta1_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "beta1",
            coeffDict_,
            0.075
        )
    ),
    beta2_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "beta2",
            coeffDict_,
            0.0828
        )
    ),
    betaStar_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "betaStar",
            coeffDict_,
            0.09
        )
    ),
    a1_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "a1",
            coeffDict_,
            0.31
        )
    ),
    c1_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "c1",
            coeffDict_,
            10.0
        )
    ),
    C_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "C",
            coeffDict_,
            2.0
        )
    ),
    Cs_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Cs",
            coeffDict_,
            0.11
        )
    ),
    zeta2_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "zeta2",
            coeffDict_,
            3.51
        )
    ),
    sigmaPhi_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "sigmaPhi",
            coeffDict_,
            2.0/3.0
        )
    ),
    kappa_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "kappa",
            coeffDict_,
            0.41
        )
    ),
    Cmu_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Cmu_",
            coeffDict_,
            0.09
        )
    ),

    y_(mesh_),

    k_
    (
        IOobject
        (
            "k",
            runTime_.timeName(),
            mesh_,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh_
    ),
    omega_
    (
        IOobject
        (
            "omega",
            runTime_.timeName(),
            mesh_,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh_
    ),
    nut_
    (
        IOobject
        (
            "nut",
            runTime_.timeName(),
            mesh_,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh_
    )
{
    bound(k_, kMin_);
    bound(omega_, omegaMin_);

    nut_ =
    (
        a1_*k_
      / max
        (
            a1_*omega_,
            F2()*sqrt(2.0)*mag(symm(fvc::grad(U_)))
        )
    );
    nut_.correctBoundaryConditions();

    printCoeffs();
}


tmp<volSymmTensorField> kOmegaSSTSAS::R() const
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
            ((2.0/3.0)*I)*k_ - nut_*twoSymm(fvc::grad(U_)),
            k_.boundaryField().types()
        )
    );
}


tmp<volSymmTensorField> kOmegaSSTSAS::devReff() const
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
           -nuEff()*dev(twoSymm(fvc::grad(U_)))
        )
    );
}


tmp<fvVectorMatrix> kOmegaSSTSAS::divDevReff(volVectorField& U) const
{
    return
    (
      - fvm::laplacian(nuEff(), U)
      - fvc::div(nuEff()*dev(T(fvc::grad(U))))
    );
}

tmp<fvVectorMatrix> kOmegaSSTSAS::divDevRhoReff
(
    const volScalarField& rho,
    volVectorField& U
) const
{
    volScalarField muEff("muEff", rho*nuEff());

    return
    (
      - fvm::laplacian(muEff, U)
      - fvc::div(muEff*dev(T(fvc::grad(U))))
    );
}

bool kOmegaSSTSAS::read()
{
    if (RASModel::read())
    {
        alphaK1_.readIfPresent(coeffDict());
        alphaK2_.readIfPresent(coeffDict());
        alphaOmega1_.readIfPresent(coeffDict());
        alphaOmega2_.readIfPresent(coeffDict());
        gamma1_.readIfPresent(coeffDict());
        gamma2_.readIfPresent(coeffDict());
        beta1_.readIfPresent(coeffDict());
        beta2_.readIfPresent(coeffDict());
        betaStar_.readIfPresent(coeffDict());
        a1_.readIfPresent(coeffDict());
        c1_.readIfPresent(coeffDict());
        C_.readIfPresent(coeffDict());
        Cs_.readIfPresent(coeffDict());
        zeta2_.readIfPresent(coeffDict());
        sigmaPhi_.readIfPresent(coeffDict());
        kappa_.readIfPresent(coeffDict());
        Cmu_.readIfPresent(coeffDict());

        return true;
    }
    else
    {
        return false;
    }
}


void kOmegaSSTSAS::correct()
{
    RASModel::correct();

    if (!turbulence_)
    {
        return;
    }

    if (mesh_.changing())
    {
        y_.correct();
    }

    const volScalarField S2(2*magSqr(symm(fvc::grad(U_))));
    const volScalarField Omega(2*magSqr(skew(fvc::grad(U_))));
    volScalarField G(GName(), nut_*S2);

    // Update omega and G at the wall
    omega_.boundaryField().updateCoeffs();

    const volScalarField CDkOmega
    (
        (2*alphaOmega2_)*(fvc::grad(k_) & fvc::grad(omega_))/omega_
    );

    const volScalarField F1(this->F1(CDkOmega));

    dimensionedScalar nutMin
    (
        "nutMin", 
        kMin_.dimensions()/omegaMin_.dimensions(), 
        SMALL
     );

    // Computation of the von Karman scale LvK
    tmp<volScalarField> tmpLvK 
    ( 
        new volScalarField
        (
            IOobject 
            (
                "LvK",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_, 
            dimensionedScalar
            (
                "zero", 
                dimensionSet(0, 1, 0, 0, 0, 0, 0),
                scalar(0)
            )
        )
    );

    volScalarField& LvK = tmpLvK();
    
    tmp<volScalarField> tmpDelta 
    ( 
        new volScalarField
        (
            IOobject 
            (
                "Delta",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_, 
            dimensionedScalar
            (
                "zero", 
                dimensionSet(0, 1, 0, 0, 0, 0, 0),
                scalar(0)
            )
        )
    );

    // Cell size
    volScalarField& delta = tmpDelta(); 
    scalarField& iDelta = delta.internalField();
    iDelta = pow(this->mesh_.V(), 1/3);

    dimensionedScalar LapSmall
    (
        "LapSmall",
        dimensionSet(0,-1,-1,0,0,0,0),
        VSMALL
    );

    LvK = max
    (
        kappa_*sqrt(S2)/(mag(fvc::laplacian(this->U_)) + LapSmall),
        Cs_*sqrt(kappa_*zeta2_/(beta(F1)/Cmu_ - gamma(F1)))*delta
    );

    tmp<volScalarField> msOmega = magSqr(fvc::grad(omega_))/sqr(omega_);
    tmp<volScalarField> msK = magSqr(fvc::grad(k_))/sqr(k_);

    volScalarField L(sqrt(k_)/(pow(Cmu_,0.25)*omega_));

    dimensionedScalar Q0
    (
        "Q0",
        dimensionSet(0, 0, -2, 0, 0), 
        scalar(0)
    );

    tmp<volScalarField> Qsas = max
    (
        zeta2_*kappa_*S2*sqr(L/LvK)-C_*scalar(2)*k_*max(msOmega,msK)/sigmaPhi_,
        Q0
    );

    // Turbulent frequency equation
    tmp<fvScalarMatrix> omegaEqn
    (
        fvm::ddt(omega_)
      + fvm::div(phi_, omega_)
      - fvm::laplacian(DomegaEff(F1), omega_)
     ==
        gamma(F1)* min(G, c1_*betaStar_*k_*omega_)/(nut_ + nutMin)
      - fvm::Sp(beta(F1)*omega_, omega_)
      + 2*(1-F1)*alphaOmega2_*(fvc::grad(k_)&fvc::grad(omega_))/omega_
      + Qsas
    );

    omegaEqn().relax();

    omegaEqn().boundaryManipulate(omega_.boundaryField());

    mesh_.updateFvMatrix(omegaEqn());
    solve(omegaEqn);
    bound(omega_, omegaMin_);

    // Turbulent kinetic energy equation
    tmp<fvScalarMatrix> kEqn
    (
        fvm::ddt(k_)
      + fvm::div(phi_, k_)
      - fvm::laplacian(DkEff(F1), k_)
     ==
        min(G, c1_*betaStar_*k_*omega_)
      - fvm::Sp(betaStar_*omega_, k_)
    );

    kEqn().relax();
    mesh_.updateFvMatrix(kEqn());
    solve(kEqn);
    bound(k_, kMin_);


    // Re-calculate viscosity
    nut_ = a1_*k_/max(a1_*omega_, F2()*sqrt(S2));
    nut_.correctBoundaryConditions();
}


}
}
}


