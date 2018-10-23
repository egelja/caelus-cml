/*---------------------------------------------------------------------------*\
Copyright (C) 2016 Applied CCM
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

#include "kOmegaSSTDES.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{
namespace incompressible
{
namespace LESModels
{

defineTypeNameAndDebug(kOmegaSSTDES, 0);
addToRunTimeSelectionTable(LESModel, kOmegaSSTDES, dictionary);

tmp<volScalarField> kOmegaSSTDES::CDkOmega() const
{
    return (2*alphaOmega2_)*(fvc::grad(k_) & fvc::grad(omega_))/omega_;
}


tmp<volScalarField> kOmegaSSTDES::F1() const
{
    tmp<volScalarField> CDkOmegaPlus = max
    (
        CDkOmega(),
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

tmp<volScalarField> kOmegaSSTDES::F2() const
{
    tmp<volScalarField> arg2 = max
    (
        (scalar(2)/betaStar_)*sqrt(k_)/(omega_*y_),
        scalar(500)*nu()/(sqr(y_)*omega_)
    );

    return tanh(sqr(arg2));
}

tmp<volScalarField> kOmegaSSTDES::Lt() const
{
    return sqrt(k_)/(betaStar_*omega_);
}

tmp<volScalarField> kOmegaSSTDES::FDES() const
{
    return max(Lt()/(CDES_*delta()),scalar(1.0));
}


kOmegaSSTDES::kOmegaSSTDES
(
    volVectorField const & U,
    surfaceScalarField const& phi,
    transportModel& transport,
    word const& turbulenceModelName,
    word const& modelName
) :
    LESModel(modelName, U, phi, transport, turbulenceModelName),
    curvatureCorrection_
    (
        coeffDict_.lookupOrDefault<Switch>("curvatureCorrection", false)
    ),
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
    kappa_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "kappa",
            coeffDict_,
            0.41
        )
    ),
    CDES_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "CDES",
            coeffDict_,
            0.61
        )
    ),
    Cr1_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Cr1",
            coeffDict_,
            1.0
        )
    ),
    Cr2_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Cr2",
            coeffDict_,
            2.0
        )
    ),
    Cr3_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Cr3",
            coeffDict_,
            1.0
        )
    ),
    Cscale_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Cscale",
            coeffDict_,
            1.0
        )
    ),
    frMax_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "frMax",
            coeffDict_,
            1.25
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
    nuSgs_
    (
        IOobject
        (
            "nuSgs",
            runTime_.timeName(),
            mesh_,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh_
    ),
    fr1_
    (
        IOobject
        (
            "fr1",
            runTime_.timeName(),
            mesh_,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        mesh_,
        dimensionedScalar("one", dimless, 1)
    )
{
    bound(k_, kMin_);
    bound(omega_, dimensionedScalar("zero", omega_.dimensions(), 0.0));

    nuSgs_ =
    (
        a1_*k_
      / max
        (
            a1_*omega_,
            F2()*sqrt(2.0)*mag(symm(fvc::grad(U_)))
        )
    );
    nuSgs_.correctBoundaryConditions();

    if (curvatureCorrection_)
    {
        Info<<" Curvature correction modification enabled" << endl;
    }

    printCoeffs();
}


tmp<volSymmTensorField> kOmegaSSTDES::B() const
{
    return ((2.0/3.0)*I)*k_ - nuSgs_*twoSymm(fvc::grad(U_));
}


tmp<volSymmTensorField> kOmegaSSTDES::devReff() const
{
    return -nuEff()*dev(twoSymm(fvc::grad(U_)));
}


tmp<fvVectorMatrix> kOmegaSSTDES::divDevReff(volVectorField& U) const
{
    return
    (
      - fvm::laplacian(nuEff(), U)
      - fvc::div(nuEff()*dev(T(fvc::grad(U))))
    );
}

tmp<fvVectorMatrix> kOmegaSSTDES::divDevRhoReff
(
    volScalarField const& rho,
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

bool kOmegaSSTDES::read()
{
    if (LESModel::read())
    {
        curvatureCorrection_.readIfPresent("curvatureCorrection",coeffDict());
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
        kappa_.readIfPresent(coeffDict());
        CDES_.readIfPresent(coeffDict());
        Cr1_.readIfPresent(coeffDict());
        Cr2_.readIfPresent(coeffDict());
        Cr3_.readIfPresent(coeffDict());
        Cscale_.readIfPresent(coeffDict());
        frMax_.readIfPresent(coeffDict());

        return true;
    }
    else
    {
        return false;
    }
}


void kOmegaSSTDES::correct(tmp<volTensorField> const& gradU)
{
    LESModel::correct(gradU);

    if (mesh_.changing())
    {
        y_.correct();
        y_.boundaryField() = max(y_.boundaryField(), VSMALL);
    }

    volScalarField const S2(2*magSqr(symm(fvc::grad(U_))));
    volScalarField const Omega(2*magSqr(skew(fvc::grad(U_))));
    volScalarField G(GName(), nuSgs_*S2);

    // Update omega and G at the wall
    omega_.boundaryField().updateCoeffs();

    volScalarField const CDkOmega
    (
        (2*alphaOmega2_)*(fvc::grad(k_) & fvc::grad(omega_))/omega_
    );

    volScalarField const F1(this->F1());

    dimensionedScalar nuSgsMin
    (
        "nuSgsMin",
        k_.dimensions()/omega_.dimensions(),
        SMALL
     );

    // Curvature correction terms
    if (curvatureCorrection_)
    {    
        volTensorField const Omegaij(skew(fvc::grad(this->U_)));
        volScalarField const sqrOmega(2*magSqr(Omegaij));
        volSymmTensorField const Sij(symm(fvc::grad(this->U_)));    
        volScalarField const sqrS(2*magSqr(Sij));
        dimensionedScalar const smallOmega
        (
            "smallOmega",
            sqrOmega.dimensions(),
            SMALL
        );
        volScalarField const sqrD(0.5*(sqrS + sqrOmega + smallOmega));
        volScalarField const rStar(sqrt(sqrS)/sqrt(sqrOmega+smallOmega));
        volSymmTensorField const DSijDt(fvc::DDt(this->phi_,Sij));
        volScalarField const rTilda
        (  
            (scalar(2.0)/sqr(sqrD))*(Omegaij && (Sij & DSijDt))
        );
        volScalarField const frotation
        (
            (scalar(1.0) + Cr1_)
            *scalar(2.0)*rStar/(scalar(1.0) + rStar)*
            (scalar(1.0)-Cr3_*atan(Cr2_*rTilda)) - Cr1_
        );
        volScalarField const frTilda(max(min(frotation, frMax_), scalar(0))); 
        fr1_ = max(scalar(0.0), scalar(1.0) + Cscale_*(frTilda - scalar(1.0)));
    }

    // Turbulent frequency equation
    tmp<fvScalarMatrix> omegaEqn
    (
        fvm::ddt(omega_)
      + fvm::div(phi_, omega_)
      - fvm::laplacian(DomegaEff(F1), omega_)
     ==
        gamma(F1)* min(fr1_*G, c1_*betaStar_*k_*omega_)/(nuSgs_ + nuSgsMin)
      - fvm::Sp(beta(F1)*omega_, omega_)
      + 2*(1-F1)*alphaOmega2_*(fvc::grad(k_)&fvc::grad(omega_))/omega_
    );

    omegaEqn().relax();
    omegaEqn().boundaryManipulate(omega_.boundaryField());
    mesh_.updateFvMatrix(omegaEqn());
    omegaEqn().solve();

    bound(omega_, dimensionedScalar("zero", omega_.dimensions(), 0.0));

    // Turbulent kinetic energy equation
    tmp<fvScalarMatrix> kEqn
    (
        fvm::ddt(k_)
      + fvm::div(phi_, k_)
      - fvm::laplacian(DkEff(F1), k_)
     ==
        min(fr1_*G, c1_*betaStar_*k_*omega_)
      - fvm::Sp(betaStar_*omega_*FDES(), k_)
    );

    kEqn().relax();
    mesh_.updateFvMatrix(kEqn());
    kEqn().solve();
    
    bound(k_, kMin_);


    // Re-calculate viscosity
    nuSgs_ = a1_*k_/max(a1_*omega_, F2()*sqrt(S2));
    nuSgs_.correctBoundaryConditions();
}


}
}
}


