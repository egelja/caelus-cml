/*---------------------------------------------------------------------------*\
Copyright (C) 2015 Applied CCM
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

#include "coherentStructureModel.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{
namespace compressible
{
namespace VLESModels
{

defineTypeNameAndDebug(CoherentStructureModelVLES, 0);
addToRunTimeSelectionTable(VLESModel, CoherentStructureModelVLES, dictionary);

tmp<volScalarField> CoherentStructureModelVLES::F1(volScalarField const& CDkOmega) const
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

tmp<volScalarField> CoherentStructureModelVLES::F2() const
{
    tmp<volScalarField> arg2 = max
    (
        (scalar(2)/betaStar_)*sqrt(k_)/(omega_*y_),
        scalar(500)*(mu()/rho_)/(sqr(y_)*omega_)
    );

    return tanh(sqr(arg2));
}


CoherentStructureModelVLES::CoherentStructureModelVLES
(
    volScalarField const& rho,
    volVectorField const& U,
    surfaceScalarField const& phi,
    basicThermo const& thermoPhysicalModel,
    word const& turbulenceModelName,
    word const& modelName
) :
    VLESModel(modelName, rho, U, phi, thermoPhysicalModel, turbulenceModelName),
    delayed_
    (
        coeffDict_.lookupOrDefault<Switch>
        (
            "delayed", 
            true
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
    ),
    Fr_
    (
        IOobject
        (
            "Fr",
            runTime_.timeName(),
            mesh_,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        mesh_,
        dimensionedScalar("fr", dimless, 1),
        calculatedFvPatchScalarField::typeName
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

    alphat_ = mut_/Prt_;
    alphat_.correctBoundaryConditions();

    printCoeffs();
}


tmp<volSymmTensorField> CoherentStructureModelVLES::R() const
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


tmp<volSymmTensorField> CoherentStructureModelVLES::devRhoReff() const
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


tmp<fvVectorMatrix> CoherentStructureModelVLES::divDevRhoReff(volVectorField& U) const
{
    return
    (
      - fvm::laplacian(muEff(), U)
      - fvc::div(muEff()*dev2(T(fvc::grad(U))))
    );
}

bool CoherentStructureModelVLES::read()
{
    if (VLESModel::read())
    {
        delayed_.readIfPresent("delayed", coeffDict());
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

        return true;
    }
    else
    {
        return false;
    }
}


void CoherentStructureModelVLES::correct()
{
    VLESModel::correct();

    if (!turbulence_)
    {
        return;
    }

    if (mesh_.changing())
    {
        y_.correct();
    }

    volScalarField Lc
    (
        IOobject
        (
            "Lc",
            mesh_.time().timeName(),
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh_,
        dimensionedScalar("Lc", dimLength, SMALL),
        calculatedFvPatchScalarField::typeName
    );

    label nD = mesh_.nGeometricD();

    tmp<volTensorField> gradU = fvc::grad(U_);

    tmp<volScalarField> const Q = 0.5*
    (
        (skew(gradU())&&skew(gradU())) 
        - 
        (symm(gradU())&&symm(gradU()))
    );

    tmp<volScalarField> const E = 0.5*
    (
        (skew(gradU())&&skew(gradU())) 
        + 
        (symm(gradU())&&symm(gradU()))
    );


    volScalarField Fcs = 
        (Q/(E+dimensionedScalar("smallE",E->dimensions(),VSMALL)))();

    dimensionedScalar FcsNegOne("FcsNegOne",Fcs.dimensions(),scalar(-1));
    dimensionedScalar FcsPosOne("FcsPosOne",Fcs.dimensions(),scalar( 1));
    dimensionedScalar FcsPosTwo("FcsPosTwo",Fcs.dimensions(),scalar( 2));
    dimensionedScalar FcsZero("FcsZero",Fcs.dimensions(),scalar(0));

    Fcs = min(max(Fcs,FcsNegOne),FcsPosOne);

    tmp<volScalarField> const FOmega = min(max(1 - Fcs,FcsZero),FcsPosTwo);

    tmp<volScalarField> const Cx = 
        0.27663*pow(min(mag(Fcs),FcsPosOne),1.5)*FOmega;

    if (nD == 3)
    {
        Lc.internalField() = Cx().internalField()*pow(mesh_.V(), 1.0/3.0);
    }
    else if (nD == 2)
    {
        Vector<label> const& directions = mesh_.geometricD();

        scalar thickness = 0.0;
        for (direction dir=0; dir<directions.nComponents; dir++)
        {
            if (directions[dir] == -1)
            {
                thickness = mesh_.bounds().span()[dir];
                break;
            }
        }

        Lc.internalField() = Cx().internalField()*sqrt(mesh_.V()/thickness);
    }
    else
    {
        FatalErrorIn("CoherentStructureModelVLES.cpp")
            << "Case is not 3D or 2D, VLES is not applicable"
            << exit(FatalError);
    }  

    tmp<volScalarField> Li = pow(k_,3.0/2.0)/(betaStar_*k_*omega_);
    tmp<volScalarField> Lk =
        pow((mu()/rho_),3.0/4.0)/pow(betaStar_*k_*omega_,1.0/4.0);

    volScalarField divU(fvc::div(phi_/fvc::interpolate(rho_)));

    if (mesh_.moving())
    {
        divU += fvc::div(mesh_.phi());
    }

    volScalarField const S2(2*magSqr(dev(symm(gradU))));
    volScalarField G(GName(), mut_*(gradU && dev(twoSymm(gradU))));

    // Update omega and G at the wall
    omega_.boundaryField().updateCoeffs();

    volScalarField const CDkOmega
    (
        (2*rho_*alphaOmega2_)*(fvc::grad(k_) & fvc::grad(omega_))/omega_
    );

    volScalarField const F1(this->F1(CDkOmega));

    dimensionedScalar mutMin
    (
        "mutMin",
        mut_.dimensions(), 
        SMALL
     );

    volScalarField rhoGammaF1(rho_*gamma(F1));

    // Turbulent frequency equation
    tmp<fvScalarMatrix> omegaEqn
    (
        fvm::ddt(rho_, omega_)
      + fvm::div(phi_, omega_)
      - fvm::laplacian(DomegaEff(F1), omega_)
     ==
        rhoGammaF1*min(G, c1_*rho_*betaStar_*k_*omega_)/(mut_ + mutMin)
      - fvm::SuSp((2.0/3.0)*rhoGammaF1*divU, omega_)
      - fvm::Sp(beta(F1)*rho_*omega_, omega_)
      + 2*(1-F1)*rho_*alphaOmega2_*(fvc::grad(k_)&fvc::grad(omega_))/omega_
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

    if (delayed_)
    {
        Fr_ = max
        (
            min
            (
                scalar(1.0),
                pow
                (
                    (scalar(1.0)-(1-F1)*exp(-0.002*Lc/Lk()))
                    /
                    (scalar(1.0)-(1-F1)*exp(-0.002*Li()/Lk())),
                    2.0
                )
            ),
            scalar(0.0)
        );
    }
    else
    {
        Fr_ = max
        (
            min
            (
                scalar(1.0),
                pow
                (
                    (scalar(1.0)-exp(-0.002*Lc/Lk()))
                    /
                    (scalar(1.0)-exp(-0.002*Li()/Lk())),
                    2.0
                )
            ),
            scalar(0.0)
        );
    }

    Fr_.correctBoundaryConditions();
    
    // Re-calculate viscosity
    mut_ = Fr_*rho_*a1_*k_/max(a1_*omega_, F2()*sqrt(S2));
    mut_.correctBoundaryConditions();

    // Re-calculate thermal diffusivity
    alphat_ = mut_/Prt_;
    alphat_.correctBoundaryConditions();
}

}
}
}
