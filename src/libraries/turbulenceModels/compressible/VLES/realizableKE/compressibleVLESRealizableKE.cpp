/*---------------------------------------------------------------------------*\
Copyright (C) 2014 - 2016 Applied CCM
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

#include "compressibleVLESRealizableKE.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{
namespace compressible
{
namespace VLESModels
{

defineTypeNameAndDebug(realizableVLESKE, 0);
addToRunTimeSelectionTable(VLESModel, realizableVLESKE, dictionary);

tmp<volScalarField> realizableVLESKE::F1() const
{
    if (!delayed_)
    {
        return tmp<volScalarField>
        (
            new volScalarField   
            (
                IOobject
                (
                    "zero",
                    mesh_.time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh_,
                dimensionedScalar("zero", dimless, 0),
                calculatedFvPatchScalarField::typeName
            )
        );
    }    
    else
    {

        const volScalarField CDkOmega
        (
            (2*0.856*rho_)*(fvc::grad(k_) 
            & 
            fvc::grad(epsilon_/k_/0.09))/(epsilon_/k_/0.09)
        );

        tmp<volScalarField> CDkOmegaPlus = max
        (
            CDkOmega,
            dimensionedScalar("1.0e-10", dimDensity/sqr(dimTime), 1.0e-10)
        );

        tmp<volScalarField> arg1 = min
        (
            max
            (
                (scalar(1)/0.09)*sqrt(k_)/(epsilon_/k_*y_/0.09),
                scalar(500)*(mu()/rho_)/(sqr(y_)*epsilon_/k_/0.09)
            ),
            (4*0.856*rho_)*k_/(CDkOmegaPlus*sqr(y_))
        );

        return tanh(pow4(arg1));
    }
}


tmp<volScalarField> realizableVLESKE::rCmu
(
    volTensorField const& gradU,
    volScalarField const& S2,
    volScalarField const& magS
)
{
    tmp<volSymmTensorField> tS = dev(symm(gradU));
    volSymmTensorField const& S = tS();

    tmp<volScalarField> W
    (
        (2*sqrt(2.0))*((S&S)&&S)
       /(
            magS*S2
          + dimensionedScalar("small", dimensionSet(0, 0, -3, 0, 0), SMALL)
        )
    );

    tS.clear();

    tmp<volScalarField> phis
    (
        (1.0/3.0)*acos(min(max(sqrt(6.0)*W, -scalar(1)), scalar(1)))
    );
    tmp<volScalarField> As = sqrt(6.0)*cos(phis);
    tmp<volScalarField> Us = sqrt(S2/2.0 + magSqr(skew(gradU)));

    return 1.0/(A0_ + As*Us*k_/epsilon_);
}


tmp<volScalarField> realizableVLESKE::rCmu
(
    volTensorField const& gradU
)
{
    volScalarField const S2(2*magSqr(dev(symm(gradU))));
    tmp<volScalarField> magS = sqrt(S2);
    return rCmu(gradU, S2, magS);
}


realizableVLESKE::realizableVLESKE
(
    volScalarField const& rho,
    volVectorField const& U,
    surfaceScalarField const& phi,
    const fluidThermo& thermoPhysicalModel,
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
    outputFr_
    (
        coeffDict_.lookupOrDefault<Switch>
        (
            "outputFr",
            false
        )
    ),
    Cmu_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Cmu",
            coeffDict_,
            0.09
        )
    ),
    A0_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "A0",
            coeffDict_,
            4.04
        )
    ),
    C2_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "C2",
            coeffDict_,
            1.9
        )
    ),
    sigmak_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "sigmak",
            coeffDict_,
            1.0
        )
    ),
    sigmaEps_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "sigmaEps",
            coeffDict_,
            1.2
        )
    ),
    Prt_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Prt",
            coeffDict_,
            1.0
        )
    ),
    Cx_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Cx",
            coeffDict_,
            0.61
        )
    ),
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
    epsilon_
    (
        IOobject
        (
            "epsilon",
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
    muSgs_
    (
        IOobject
        (
            "muSgs",
            runTime_.timeName(),
            mesh_,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        mesh_,
        dimensionedScalar("muSgs",mut_.dimensions(),0)
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
    alphaSgs_
    (
        IOobject
        (
            "alphaSgs",
            runTime_.timeName(),
            mesh_,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        mesh_,
        dimensionedScalar("alphaSgs",alphat_.dimensions(),0)
    ),
    Fr_
    (
        IOobject
        (
            "Fr",
            runTime_.timeName(),
            mesh_,
            IOobject::NO_READ,
            outputFr_ ? IOobject::AUTO_WRITE : IOobject::NO_WRITE
        ),
        mesh_,
        dimensionedScalar("fr", dimless, 1),
        calculatedFvPatchScalarField::typeName
    ),

    y_(mesh_)

{
    bound(k_, kMin_);
    bound(epsilon_, epsilonMin_);

    mut_ = rCmu(fvc::grad(U_))*rho_*sqr(k_)/epsilon_;
    mut_.correctBoundaryConditions();

    alphat_ = mut_/Prt_;
    alphat_.correctBoundaryConditions();

    printCoeffs();
}


bool realizableVLESKE::read()
{
    if (VLESModel::read())
    {
        delayed_.readIfPresent("delayed", coeffDict());
        Cmu_.readIfPresent(coeffDict());
        A0_.readIfPresent(coeffDict());
        C2_.readIfPresent(coeffDict());
        sigmak_.readIfPresent(coeffDict());
        sigmaEps_.readIfPresent(coeffDict());
        Prt_.readIfPresent(coeffDict());
        Cx_.readIfPresent(coeffDict());

        return true;
    }
    else
    {
        return false;
    }
}


void realizableVLESKE::correct()
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

    scalar const Cx = 0.61;

    if (nD == 3)
    {
        Lc.internalField() = Cx*pow(mesh_.V(), 1.0/3.0);
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

        Lc.internalField() = Cx*sqrt(mesh_.V()/thickness);
    }
    else
    {
        FatalErrorInFunction
            << "Case is not 3D or 2D, LES is not applicable"
            << exit(FatalError);
    }

    tmp<volScalarField> Li = pow(k_,3.0/2.0)/epsilon_;
    tmp<volScalarField> Lk = pow((mu()/rho_),3.0/4.0)/pow(epsilon_,1.0/4.0);

    volScalarField divU(fvc::div(phi_/fvc::interpolate(rho_)));

    if (mesh_.moving())
    {
        divU += fvc::div(mesh_.phi());
    }

    volTensorField const gradU(fvc::grad(U_));
    volScalarField const S2(2*magSqr(dev(symm(gradU))));
    volScalarField const magS(sqrt(S2));

    volScalarField const eta(magS*k_/epsilon_);
    tmp<volScalarField> C1 = max(eta/(scalar(5) + eta), scalar(0.43));

    volScalarField G(GName(), mut_*S2);

    // Update epsilon and G at the wall
    epsilon_.boundaryField().updateCoeffs();

    if (delayed_)
    {
        Fr_ = max
        (
            min
            (
                scalar(1.0),
                pow
                (
                    (scalar(1.0)-(1-this->F1())*exp(-0.002*Lc/Lk()))
                    /
                    (scalar(1.0)-(1-this->F1())*exp(-0.002*Li()/Lk())),
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

    // Dissipation equation
    tmp<fvScalarMatrix> epsEqn
    (
        fvm::ddt(rho_, epsilon_)
      + fvm::div(phi_, epsilon_)
      - fvm::Sp(fvc::div(phi_), epsilon_)
      - fvm::laplacian(DepsilonEff(), epsilon_)
     ==
        C1*magS*rho_*epsilon_
      - fvm::Sp
        (
            C2_*rho_*epsilon_/(k_ + sqrt((mu()/rho_)*epsilon_)),
            epsilon_
        )
    );

    epsEqn().relax();

    epsEqn().boundaryManipulate(epsilon_.boundaryField());

    solve(epsEqn);
    bound(epsilon_, epsilonMin_);


    // Turbulent kinetic energy equation
    tmp<fvScalarMatrix> kEqn
    (
        fvm::ddt(rho_, k_)
      + fvm::div(phi_, k_)
      - fvm::Sp(fvc::div(phi_), k_)
      - fvm::laplacian(DkEff(), k_)
     ==
        G
      - fvm::SuSp(2.0/3.0*rho_*divU, k_)
      - fvm::Sp(rho_*epsilon_/k_, k_)
    );

    kEqn().relax();
    solve(kEqn);
    bound(k_, kMin_);

    // Re-calculate viscosity
    mut_ = rCmu(gradU, S2, magS)*rho_*sqr(k_)/epsilon_;
    mut_.correctBoundaryConditions();

    muSgs_ = Fr_*mut_;
    muSgs_.correctBoundaryConditions();

    // Re-calculate thermal diffusivity
    alphat_ = mut_/Prt_;
    alphat_.correctBoundaryConditions();

    alphaSgs_ = Fr_*alphat_;
    alphaSgs_.correctBoundaryConditions();
}


}
}
}

