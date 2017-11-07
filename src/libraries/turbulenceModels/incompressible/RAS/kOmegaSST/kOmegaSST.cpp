/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
Copyright (C) 2014-2016 Applied CCM
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

#include "kOmegaSST.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{
namespace incompressible
{
namespace RASModels
{

defineTypeNameAndDebug(kOmegaSST, 0);
addToRunTimeSelectionTable(RASModel, kOmegaSST, dictionary);

tmp<volScalarField> kOmegaSST::F1(volScalarField const& CDkOmega) const
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

tmp<volScalarField> kOmegaSST::F2() const
{
    tmp<volScalarField> arg2 = max
    (
        (scalar(2)/betaStar_)*sqrt(k_)/(omega_*y_),
        scalar(500)*nu()/(sqr(y_)*omega_)
    );

    return tanh(sqr(arg2));
}


kOmegaSST::kOmegaSST
(
    volVectorField const& U,
    surfaceScalarField const& phi,
    transportModel& transport,
    word const& turbulenceModelName,
    word const& modelName
) :
    RASModel(modelName, U, phi, transport, turbulenceModelName),
    curvatureCorrection_
    (
        coeffDict_.lookupOrDefault<Switch>
        (
            "curvatureCorrection",
            false
        )
    ),
    damped_
    (
        coeffDict_.lookupOrDefault<Switch>
        (
            "damped",
            false
        )
    ),
    outputfr1_
    (
        coeffDict_.lookupOrDefault<Switch>
        (
            "outputfr1",
            false
        )
    ),
    outputFd_
    (
        coeffDict_.lookupOrDefault<Switch>
        (
            "outputFd",
            false
        )
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
    ),
    yStar_(pow(0.09,0.25)*pow(k_,0.5)*y_/nu()),
    fr1_
    (
        IOobject
        (
            "fr1",
            runTime_.timeName(),
            mesh_,
            IOobject::NO_READ,
            outputfr1_ ? IOobject::AUTO_WRITE : IOobject::NO_WRITE
        ),
        mesh_,
        dimensionedScalar("one", dimless, 1)
    ),
    Fd_
    (
        IOobject
        (
            "Fd",
            runTime_.timeName(),
            mesh_,
            IOobject::NO_READ,
            outputFd_ ? IOobject::AUTO_WRITE : IOobject::NO_WRITE
        ),
        mesh_,
        dimensionedScalar("fd", dimless, 1),
        calculatedFvPatchScalarField::typeName
    )
{
    bound(k_, kMin_);
    bound(omega_, omegaMin_);

    if (damped_)
    {
        Info << "Buffer layer damping enabled" << endl;
        yStar_ = pow(0.09,0.25)*pow(k_,0.5)*y_/nu();
        Fd_ = scalar(0.1) + (scalar(1) - scalar(0.1)*tanh(pow(0.03*yStar_,4)))
            * (scalar(0.9)+scalar(0.1)*tanh(pow(0.005*yStar_,8)));
        Fd_.correctBoundaryConditions();
        nut_ =
        (
            a1_*k_
          / max
            (
                a1_*omega_,
                F2()*sqrt(2.0)*mag(symm(fvc::grad(U_)))
            )
        );
        nut_ *= Fd_;
    }
    else
    {
        nut_ =
        (
            a1_*k_
          / max
            (
                a1_*omega_,
                F2()*sqrt(2.0)*mag(symm(fvc::grad(U_)))
            )
        );
    }

    nut_.correctBoundaryConditions();

    if (curvatureCorrection_)
    {
        Info<<" Curvature correction modification enabled" << endl;
    }

    printCoeffs();
}


tmp<volSymmTensorField> kOmegaSST::R() const
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


tmp<volSymmTensorField> kOmegaSST::devReff() const
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


tmp<fvVectorMatrix> kOmegaSST::divDevReff(volVectorField& U) const
{
    return
    (
      - fvm::laplacian(nuEff(), U)
      - fvc::div(nuEff()*dev(T(fvc::grad(U))))
    );
}

tmp<fvVectorMatrix> kOmegaSST::divDevRhoReff
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

bool kOmegaSST::read()
{
    if (RASModel::read())
    {
        curvatureCorrection_.readIfPresent("curvatureCorrection",coeffDict());
        damped_.readIfPresent("damped", coeffDict());
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


void kOmegaSST::correct()
{
    if (!turbulence_)
    {
        return;
    }

    if (mesh_.changing())
    {
        y_.correct();
    }

    RASModel::correct();

    volScalarField const S2(2*magSqr(symm(fvc::grad(U_))));
    volScalarField const Omega(2*magSqr(skew(fvc::grad(U_))));
    volScalarField G(GName(), nut_*S2);

    // Update omega and G at the wall
    omega_.boundaryField().updateCoeffs();

    volScalarField const CDkOmega
    (
        (2*alphaOmega2_)*(fvc::grad(k_) & fvc::grad(omega_))/omega_
    );

    volScalarField const F1(this->F1(CDkOmega));

    dimensionedScalar nutMin
    (
        "nutMin", 
        kMin_.dimensions()/omegaMin_.dimensions(), 
        SMALL
     );

    // Curvature correction terms
    if (curvatureCorrection_)
    {    
        volTensorField const Omegaij(skew(fvc::grad(this->U_)));
        volScalarField const sqrOmega(2*magSqr(Omegaij));
        volSymmTensorField const Sij(symm(fvc::grad(this->U_)));    
        volScalarField const sqrS(2*magSqr(Sij));
        dimensionedScalar const smallOmega("smallOmega",sqrOmega.dimensions(), SMALL);
        volScalarField const sqrD(0.5*(sqrS + sqrOmega + smallOmega));
        volScalarField const rStar(sqrt(sqrS)/sqrt(sqrOmega+smallOmega));
        volSymmTensorField const DSijDt(fvc::DDt(this->phi_,Sij));
        volScalarField const rTilda(  
            (scalar(2.0)/sqr(sqrD))*(Omegaij && (Sij & DSijDt)));
        volScalarField const frotation ((scalar(1.0) + Cr1_)
            *scalar(2.0)*rStar/(scalar(1.0) + rStar)*
            (scalar(1.0)-Cr3_*atan(Cr2_*rTilda)) - Cr1_);
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
        gamma(F1)* min(fr1_*G, c1_*betaStar_*k_*omega_)/(nut_ + nutMin)
      - fvm::Sp(beta(F1)*omega_, omega_)
      + 2*(1-F1)*alphaOmega2_*(fvc::grad(k_)&fvc::grad(omega_))/omega_
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
        min(fr1_*G, c1_*betaStar_*k_*omega_)
      - fvm::Sp(betaStar_*omega_, k_)
    );

    kEqn().relax();

    mesh_.updateFvMatrix(kEqn());

    solve(kEqn);
    bound(k_, kMin_);


    // Re-calculate viscosity
    if (damped_)
    {
        yStar_ = pow(0.09,0.25)*pow(k_,0.5)*y_/nu();
        Fd_ = scalar(0.1) + (scalar(1) - scalar(0.1)*tanh(pow(0.03*yStar_,4)))
            * (scalar(0.9)+scalar(0.1)*tanh(pow(0.005*yStar_,8)));
        Fd_.correctBoundaryConditions();
        nut_ = a1_*k_/max(a1_*omega_, F2()*sqrt(S2));
        nut_ *= nut_;
    }
    else
    {
        nut_ = a1_*k_/max(a1_*omega_, F2()*sqrt(S2));
    }
    
    nut_.correctBoundaryConditions();
}


}
}
}


