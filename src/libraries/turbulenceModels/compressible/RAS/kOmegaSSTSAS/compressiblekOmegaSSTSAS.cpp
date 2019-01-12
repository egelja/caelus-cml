/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
Copyright (C) 2015 Applied CCM 
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

#include "compressiblekOmegaSSTSAS.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{
namespace compressible
{
namespace RASModels
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(kOmegaSSTSAS, 0);
addToRunTimeSelectionTable(RASModel, kOmegaSSTSAS, dictionary);

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

tmp<volScalarField> kOmegaSSTSAS::F1(const volScalarField& CDkOmega) const
{
    tmp<volScalarField> CDkOmegaPlus = max
    (
        CDkOmega,
        dimensionedScalar("1.0e-10", dimDensity/sqr(dimTime), 1.0e-10)
    );

    tmp<volScalarField> arg1 = min
    (
        max
        (
            (scalar(1)/betaStar_)*sqrt(k_)/(omega_*y_),
            scalar(500)*(mu()/rho_)/(sqr(y_)*omega_)
        ),
        (4*rho_*alphaOmega2_)*k_/(CDkOmegaPlus*sqr(y_))
    );

    return tanh(pow4(arg1));
}

tmp<volScalarField> kOmegaSSTSAS::F2() const
{
    tmp<volScalarField> arg2 = max
    (
        (scalar(2)/betaStar_)*sqrt(k_)/(omega_*y_),
        scalar(500)*(mu()/rho_)/(sqr(y_)*omega_)
    );

    return tanh(sqr(arg2));
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

kOmegaSSTSAS::kOmegaSSTSAS
(
    const volScalarField& rho,
    const volVectorField& U,
    const surfaceScalarField& phi,
    const fluidThermo& thermophysicalModel, 
    const word& turbulenceModelName,
    const word& modelName
)
:
    RASModel(modelName, rho, U, phi, thermophysicalModel, turbulenceModelName),

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
    Prt_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Prt",
            coeffDict_,
            0.9
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
    mut_
    (
        IOobject
        (
            "mut",
            runTime_.timeName(),
            mesh_,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh_
    ),
    alphat_
    (
        IOobject
        (
            "alphat",
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

    mut_ =
    (
        rho_*a1_*k_
      / max
        (
            a1_*omega_,
            F2()*sqrt(2.0)*mag(symm(fvc::grad(U_)))
        )
    );
    mut_.correctBoundaryConditions();

    printCoeffs();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

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
            ((2.0/3.0)*I)*k_ - (mut_/rho_)*twoSymm(fvc::grad(U_)),
            k_.boundaryField().types()
        )
    );
}


tmp<volSymmTensorField> kOmegaSSTSAS::devRhoReff() const
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
           -muEff()*dev(twoSymm(fvc::grad(U_)))
        )
    );
}


tmp<fvVectorMatrix> kOmegaSSTSAS::divDevRhoReff(volVectorField& U) const
{
    return
    (
      - fvm::laplacian(muEff(), U)
      - fvc::div(muEff()*dev2(T(fvc::grad(U))))
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
        Prt_.readIfPresent(coeffDict());
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

    volScalarField divU(fvc::div(phi_/fvc::interpolate(rho_)));

    if (mesh_.moving())
    {
        divU += fvc::div(mesh_.phi());
    }

    const volTensorField gradU(fvc::grad(U_));
    const volScalarField S2(2*magSqr(dev(symm(gradU))));
    volScalarField G(GName(), mut_*(gradU && dev(twoSymm(gradU))));

    // Update omega and G at the wall
    omega_.boundaryField().updateCoeffs();

    const volScalarField CDkOmega
    (
        (2*rho_*alphaOmega2_)*(fvc::grad(k_) & fvc::grad(omega_))/omega_
    );

    const volScalarField F1(this->F1(CDkOmega));

    dimensionedScalar mutMin
    (
        "mutMin", 
        mut_.dimensions(), 
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
        rho_*zeta2_*kappa_*S2*sqr(L/LvK)
        - C_*scalar(2)*rho_*k_*max(msOmega,msK)/sigmaPhi_,
        Q0
    );

    volScalarField rhoGammaF1(rho_*gamma(F1));

    // Turbulent frequency equation
    tmp<fvScalarMatrix> omegaEqn
    (
        fvm::ddt(rho_, omega_)
      + fvm::div(phi_, omega_)
      - fvm::laplacian(DomegaEff(F1), omega_)
     ==
        rhoGammaF1* min(G, c1_*rho_*betaStar_*k_*omega_)/(mut_ + mutMin)
      - fvm::SuSp((2.0/3.0)*rhoGammaF1*divU, omega_)
      - fvm::Sp(beta(F1)*rho_*omega_, omega_)
      + 2*(1-F1)*rho_*alphaOmega2_*(fvc::grad(k_)&fvc::grad(omega_))/omega_
      + Qsas
    );

    omegaEqn().relax();

    omegaEqn().boundaryManipulate(omega_.boundaryField());

    solve(omegaEqn);
    bound(omega_, omegaMin_);

    // Turbulent kinetic energy equation
    tmp<fvScalarMatrix> kEqn
    (
        fvm::ddt(rho_, k_)
      + fvm::div(phi_, k_)
      - fvm::laplacian(DkEff(F1), k_)
     ==
        min(G, c1_*rho_*betaStar_*k_*omega_)
      - fvm::SuSp(2.0/3.0*rho_*divU, k_)
      - fvm::Sp(betaStar_*rho_*omega_, k_)
    );

    kEqn().relax();
    solve(kEqn);
    bound(k_, kMin_);


    // Re-calculate viscosity
    mut_ = rho_*a1_*k_/max(a1_*omega_, F2()*sqrt(S2));
    mut_.correctBoundaryConditions();

    // Re-calculate thermal diffusivity
    alphat_ = mut_/Prt_;
    alphat_.correctBoundaryConditions();
}


}
}
}


