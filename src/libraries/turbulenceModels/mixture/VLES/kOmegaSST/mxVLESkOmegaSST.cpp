/*---------------------------------------------------------------------------*\
Copyright (C) 2015 - 2019 Applied CCM
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

#include "mxVLESkOmegaSST.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{
namespace mixture
{
namespace VLESModels
{

defineTypeNameAndDebug(VLESKOmegaSST, 0);
addToRunTimeSelectionTable(VLESModel, VLESKOmegaSST, dictionary);


tmp<volScalarField> VLESKOmegaSST::F1(volScalarField const& CDkOmega) const
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
}


tmp<volScalarField> VLESKOmegaSST::F2() const
{
    tmp<volScalarField> arg2 = max
    (
        (scalar(2)/betaStar_)*sqrt(k_)/(omega_*y_),
        scalar(500)*(mu()/rho_)/(sqr(y_)*omega_)
    );

    return tanh(sqr(arg2));
}


VLESKOmegaSST::VLESKOmegaSST
(
    const volScalarField& rho,
    const volVectorField& U,
    const surfaceScalarField& phi,
    const twoPhaseMixture& transport,
    const word& turbulenceModelName,
    const word& modelName
) :
    VLESModel(modelName, rho, U, phi, transport, turbulenceModelName),
    delayed_
    (
        coeffDict_.lookupOrDefault<Switch>
        (
            "delayed", 
            true
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
    curvatureCorrection_
    (
        coeffDict_.lookupOrDefault<Switch>
        (
            "curvatureCorrection",
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
    outputFr_
    (
        coeffDict_.lookupOrDefault<Switch>
        (
            "outputFr",
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
    Cx_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Cx",
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
    yStar_(pow(0.09,0.25)*pow(k_,0.5)*y_*rho/mu()),
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
        yStar_ = pow(0.09,0.25)*pow(k_,0.5)*y_*rho_/mu();
        Fd_ = scalar(0.1) + (scalar(1) - scalar(0.1)*tanh(pow(0.03*yStar_,4)))
            * (scalar(0.9)+scalar(0.1)*tanh(pow(0.005*yStar_,8)));
        Fd_.correctBoundaryConditions();
        mut_ =
        (
            rho_*a1_*k_
          / max
            (
                a1_*omega_,
                F2()*sqrt(2.0)*mag(symm(fvc::grad(U_)))
            )
        );        
        mut_ *= Fd_;
    }
    else
    {
        mut_ =
        (
            rho_*a1_*k_
          / max
            (
                a1_*omega_,
                F2()*sqrt(2.0)*mag(symm(fvc::grad(U_)))
            )
        );
    }
    
    mut_.correctBoundaryConditions();

    if (curvatureCorrection_)
    {
        Info<<" Curvature correction modification enabled" << endl;
    }

    printCoeffs();
}


bool VLESKOmegaSST::read()
{
    if (VLESModel::read())
    {
        delayed_.readIfPresent("delayed", coeffDict());
        damped_.readIfPresent("damped", coeffDict());
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
        Cx_.readIfPresent(coeffDict());
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


void VLESKOmegaSST::correct()
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

    if (nD == 3)
    {
        Lc.internalField() = Cx_*pow(mesh_.V(), 1.0/3.0);
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

        Lc.internalField() = Cx_*sqrt(mesh_.V()/thickness);
    }
    else
    {
        FatalErrorInFunction
            << "Case is not 3D or 2D, VLES is not applicable"
            << exit(FatalError);
    }  

    tmp<volScalarField> const Li = pow(k_,3.0/2.0)/(betaStar_*k_*omega_);
    tmp<volScalarField> const Lk = 
        pow((mu()/rho_),3.0/4.0)/pow(betaStar_*k_*omega_,1.0/4.0);

    volScalarField divU(fvc::div(phi_/fvc::interpolate(rho_)));

    if (mesh_.moving())
    {
        divU += fvc::div(mesh_.phi());
    }

    volTensorField const gradU(fvc::grad(U_));
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

    if (curvatureCorrection_)
    {    
        volTensorField const Omegaij(skew(fvc::grad(this->U_)));
        volScalarField const sqrOmega(2*magSqr(Omegaij));
        volSymmTensorField const Sij(symm(fvc::grad(this->U_)));    
        volScalarField const sqrS(2*magSqr(Sij));
        dimensionedScalar const smallOmega("smallOmega",sqrOmega.dimensions(), SMALL);
        volScalarField const sqrD(0.5*(sqrS + sqrOmega + smallOmega));
        volScalarField const rStar(sqrt(sqrS)/sqrt(sqrOmega+smallOmega));
        surfaceScalarField const u(this->phi_/fvc::interpolate(rho_));
        volSymmTensorField const DSijDt(fvc::DDt(u,Sij));
        volScalarField const rTilda(  
            (scalar(2.0)/sqr(sqrD))*(Omegaij && (Sij & DSijDt)));
        volScalarField const frotation ((scalar(1.0) + Cr1_)
            *scalar(2.0)*rStar/(scalar(1.0) + rStar)*
            (scalar(1.0)-Cr3_*atan(Cr2_*rTilda)) - Cr1_);
        volScalarField const frTilda(max(min(frotation, frMax_), scalar(0))); 
        fr1_ = max(scalar(0.0), scalar(1.0) + Cscale_*(frTilda - scalar(1.0)));
    }
    
    volScalarField const rhoGammaF1(rho_*gamma(F1));

    // Turbulent frequency equation
    tmp<fvScalarMatrix> omegaEqn
    (
        fvm::ddt(rho_, omega_)
      + fvm::div(phi_, omega_)
      - fvm::laplacian(DomegaEff(F1), omega_)
     ==
        rhoGammaF1*min(fr1_*G, c1_*rho_*betaStar_*k_*omega_)/(mut_ + mutMin)
      - fvm::SuSp((2.0/3.0)*rhoGammaF1*divU, omega_)
      - fvm::Sp(beta(F1)*rho_*omega_, omega_)
      + 2*(1-F1)*rho_*alphaOmega2_*(fvc::grad(k_)&fvc::grad(omega_))/omega_
    );

    omegaEqn().relax();

    omegaEqn().boundaryManipulate(omega_.boundaryField());
    
    mesh_.updateFvMatrix(omegaEqn());
    solve(omegaEqn);
    bound(omega_, omegaMin_);

    // Turbulent kinetic energy equation
    tmp<fvScalarMatrix> kEqn
    (
        fvm::ddt(rho_, k_)
      + fvm::div(phi_, k_)
      - fvm::laplacian(DkEff(F1), k_)
     ==
        min(fr1_*G, c1_*rho_*betaStar_*k_*omega_)
      - fvm::SuSp(2.0/3.0*rho_*divU, k_)
      - fvm::Sp(betaStar_*rho_*omega_, k_)
    );

    kEqn().relax();
    mesh_.updateFvMatrix(kEqn());
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

    if (damped_)
    {
        yStar_ = pow(0.09,0.25)*pow(k_,0.5)*y_*rho_/mu();
        Fd_ = scalar(0.1) + (scalar(1) - scalar(0.1)*tanh(pow(0.03*yStar_,4)))
            * (scalar(0.9)+scalar(0.1)*tanh(pow(0.005*yStar_,8)));
        Fd_.correctBoundaryConditions();
        mut_ = rho_*a1_*k_/max(a1_*omega_, F2()*sqrt(S2));
        mut_ *= Fd_;
    }
    else
    {
        mut_ = rho_*a1_*k_/max(a1_*omega_, F2()*sqrt(S2));

    }
    mut_.correctBoundaryConditions();
    muSgs_ = Fr_*mut_;
    muSgs_.correctBoundaryConditions();

}


} // End namespace VLESModels
} // End namespace mixture
} // End namespace CML
