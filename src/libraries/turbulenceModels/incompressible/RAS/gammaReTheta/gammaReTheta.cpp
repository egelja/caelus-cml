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

#include "gammaReTheta.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "wallFvPatch.hpp"

namespace CML
{
namespace incompressible
{
namespace RASModels
{

defineTypeNameAndDebug(gammaReTheta, 0);
addToRunTimeSelectionTable(RASModel, gammaReTheta, dictionary);

const scalar gammaReTheta::tol_ = 1.0e-4;
const int gammaReTheta::maxIter_ = 100;

volScalarField gammaReTheta::Flength() const
{
    volScalarField Flength 
    (
        IOobject
        (
            "Flength",
            runTime_.timeName(),
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        ReThetaTilda_ 
    );

    Flength = min
    (
        scalar(0.1)*exp(scalar(-0.022)*ReThetaTilda_+scalar(12))+scalar(0.45),
        scalar(300)
    );

    return Flength;
}

volScalarField gammaReTheta::ReThetac() const
{
    volScalarField ReThetac 
    (
        IOobject
        (
            "ReThetac",
            runTime_.timeName(),
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        this->ReThetaTilda_ 
    );

    ReThetac = min
    (
        max
        (
            scalar(1.47)*ReThetaTilda_-sqr(scalar(0.025)*ReThetaTilda_)-scalar(120),
            scalar(125)
        ),
        ReThetaTilda_
    );

    return ReThetac;
}

tmp<volScalarField> gammaReTheta::Fonset() const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "Fonset",
                runTime_.timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            max
            (
                min(max(Fonset1(),pow4(Fonset1())),scalar(2))
               -max(scalar(1)-pow3(Rt()/scalar(2.5)),scalar(0)),
                scalar(0)
            )
        )
    );
}

tmp<volScalarField> gammaReTheta::Fonset1() const
{
    const scalar c1 = sqrt(2.0*pTraits<scalar>::one);
    const scalar c2 = 2.193*pTraits<scalar>::one;
    const volTensorField gradU(fvc::grad(this->U_));
    return sqr(this->y_)*c1*mag(symm(gradU))/(c2*nu()*ReThetac());
}

tmp<volScalarField> gammaReTheta::Fturb() const
{
    const scalar c3 = 0.25*pTraits<scalar>::one;
    return exp(-pow4(c3*Rt()));
}

tmp<volScalarField> gammaReTheta::Freattach() const
{
    const scalar c4 = 0.05*pTraits<scalar>::one; 
    return exp(-pow4(c4*Rt()));
}

tmp<volScalarField> gammaReTheta::Fwake() const
{
    const scalar c5 = 1.0e05*pTraits<scalar>::one;
    return exp(-sqr(sqr(y_)*omega_/(c5*nu())));
}

tmp<volScalarField> gammaReTheta::FThetat() const
{
    const scalar c6 = sqrt(2.0*pTraits<scalar>::one);
    const volTensorField gradU(fvc::grad(this->U_));
    volScalarField magVort(c6*mag(skew(gradU)));

    dimensionedScalar omegaSmall
    (
        "omegaSmall",
        magVort.dimensions(),
        SMALL
    );

    magVort = max(magVort,omegaSmall);

    const scalar c7 = 375*pTraits<scalar>::one;
    const scalar c8 = pTraits<scalar>::one;

    return min
    (
        max
        (
            Fwake()*exp(-pow4(magSqr(this->U_)
           /(c7*nu()*magVort*this->ReThetaTilda_))),
            c8-sqr((this->ce2_*this->intermittency_-scalar(1.0))/(this->ce2_ - c8))
        ),
        c8
    );
}

void gammaReTheta::ReTheta(volScalarField& ReThetaField) const
{
    const volScalarField U2gradU((sqr(U_)&&(fvc::grad(U_))));
    const scalar c9 = 100.0*pTraits<scalar>::one;
    const scalar c10 = 1.5*pTraits<scalar>::one;

    forAll(ReThetaField, cellI)
    {
        int iter = 0;
        const scalar c11 = 0.027*pTraits<scalar>::one;
        const scalar Tu = max
        (
            c9*sqrt(k_[cellI]/c10)/max(mag(U_[cellI]),SMALL),
            c11
        );

        scalar dUds = U2gradU[cellI]/(sqr(max(mag(U_[cellI]),SMALL)));

        // Declare ReThetaOld in this scope
        scalar ReThetaOld;

        // Starting value
        scalar ReThetaNew = max
        (
            ReThetaEq(Tu, scalar(0), scalar(0)),
            scalar(20.0)
        );
        scalar ReThetaTol = ReThetaNew*tol_;

        const scalar c12 = 0.1*pTraits<scalar>::one;
        do
        {
            ReThetaOld = ReThetaNew;
            scalar const lambda = max
            (
                min
                (
                    sqr(ReThetaOld)*nu()()[cellI]*dUds
                   /(sqr(max(mag(U_[cellI]),SMALL))),
                   c12
                ),
                -c12
            );
            scalar const K = max
            (
                min
                (
                    nu()()[cellI]*dUds/(sqr(max(mag(this->U_[cellI]),SMALL))),
                    scalar(3e-6)
                ),
                scalar(-3e-6)
            );
            ReThetaNew = max(ReThetaEq(Tu, lambda, K),scalar(20.0));

            if (iter++ > maxIter_)
            {
                FatalErrorInFunction
                    << "Maximum number of iterations exceeded"
                << abort(FatalError);
            }
        } while(mag(ReThetaNew-ReThetaOld) > ReThetaTol);

        ReThetaField[cellI] = ReThetaNew;
    }

}

  scalar gammaReTheta::ReThetaEq(scalar Tu, scalar const lambda, scalar const K) const
{
    scalar FTu;
    scalar FlamK;

    FTu = scalar(803.73)*pow((Tu+scalar(0.6067)),scalar(-1.027));
    if (lambda > scalar(0)) 
    {
        scalar FK = 
            scalar(0.0962e6)*K+scalar(0.148e12)*sqr(K)
           +scalar(0.0141e18)*pow3(K);
        FlamK = 
            scalar(1.0)+FK*(scalar(1.0)-exp(-Tu/scalar(1.5)))
           +scalar(0.556)*(scalar(1.0)-exp(-scalar(23.9)*lambda))
           *exp(-Tu/scalar(1.5)); 
    }
    else 
    {
        scalar Flam = 
            scalar(10.32)*lambda+scalar(89.47)*sqr(lambda)+265.51*pow3(lambda);
            FlamK = scalar(1.0)+Flam*exp(-Tu/scalar(3.0));
     }

    return FTu*FlamK;

}

tmp<volScalarField> gammaReTheta::intermittencySep() const
{
    const volTensorField gradU(fvc::grad(U_));
    return FThetat()*min
    (
        s1_*Freattach()*max
        (
            sqr(y_)*sqrt(scalar(2))
           *mag(symm(gradU))
           /(scalar(3.235)*nu()*ReThetac())-scalar(1.0),
            scalar(0.0)
        ),
        scalar(2.0)
    );
}

tmp<volScalarField> gammaReTheta::F1(const volScalarField& CDkOmega) const
{
    volScalarField CDkOmegaPlus = max
    (
        CDkOmega,
        dimensionedScalar("1.0e-10", dimless/sqr(dimTime), 1.0e-10)
    )();

    volScalarField arg1 = min
    (
        max
        (
            (scalar(1)/betaStar_)*sqrt(k_)/(omega_*y_),
            scalar(500)*nu()/(sqr(y_)*omega_)
        ),
        (4*alphaOmega2_)*k_/(CDkOmegaPlus*sqr(y_))
    )();

    return max
    (
        tanh(pow4(arg1)),
	exp(-sqr(pow4(y_*sqrt(k_)/(scalar(120)*nu()))))
    );
}

tmp<volScalarField> gammaReTheta::F2() const
{
    tmp<volScalarField> arg2 = max
    (
        (scalar(2)/betaStar_)*sqrt(k_)/(omega_*y_),
        scalar(500)*nu()/(sqr(y_)*omega_)
    );

    return tanh(sqr(arg2));
}

gammaReTheta::gammaReTheta
(
    const volVectorField& U,
    const surfaceScalarField& phi,
    transportModel& lamTransportModel,
    const word& turbulenceModelName,
    const word& modelName
)
:
    RASModel(modelName, U, phi, lamTransportModel, turbulenceModelName),

    ca1_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "ca1",
            coeffDict_,
            2.0
        )
    ),
    ce1_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "ce1",
            coeffDict_,
            1.0
        )
    ),
    ca2_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "ca2",
            coeffDict_,
            0.06
        )
    ),
    ce2_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "ce2",
            coeffDict_,
            50.0
        )
    ),
    cThetat_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "cThetat",
            coeffDict_,
            0.03
        )
    ),
    sigmaf_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "sigmaf",
            coeffDict_,
            1.0
        )
    ),
    sigmaThetat_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "sigmaThetat",
            coeffDict_,
            2.0
        )
    ),
    s1_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "s1",
            coeffDict_,
            2.0
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
    intermittency1_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "intermittency1",
            coeffDict_,
            0.5532
        )
    ),
    intermittency2_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "intermittency2",
            coeffDict_,
            0.4403
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

    intermittency_
    (
        IOobject
        (
            "intermittency",
            runTime_.timeName(),
            mesh_,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
    mesh_
    ),
    ReThetaTilda_
    (
        IOobject
        (
            "ReThetaTilda",
            runTime_.timeName(),
            mesh_,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
    mesh_
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

tmp<volSymmTensorField> gammaReTheta::R() const
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


tmp<volSymmTensorField> gammaReTheta::devReff() const
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


tmp<fvVectorMatrix> gammaReTheta::divDevReff(volVectorField& U) const
{
    return
    (
      - fvm::laplacian(nuEff(), U)
      - fvc::div(nuEff()*dev(T(fvc::grad(U))))
    );
}


tmp<fvVectorMatrix> gammaReTheta::divDevRhoReff
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


bool gammaReTheta::read()
{
    if (RASModel::read())
    {
	ca1_.readIfPresent(coeffDict());
	ce1_.readIfPresent(coeffDict());
	ca2_.readIfPresent(coeffDict());
	ce2_.readIfPresent(coeffDict());
	cThetat_.readIfPresent(coeffDict());
	sigmaf_.readIfPresent(coeffDict());
	sigmaThetat_.readIfPresent(coeffDict());
	s1_.readIfPresent(coeffDict());
        alphaK1_.readIfPresent(coeffDict());
        alphaK2_.readIfPresent(coeffDict());
        alphaOmega1_.readIfPresent(coeffDict());
        alphaOmega2_.readIfPresent(coeffDict());
        // gamma1_.readIfPresent(coeffDict());
        // gamma2_.readIfPresent(coeffDict());
        intermittency1_.readIfPresent(coeffDict());
        intermittency2_.readIfPresent(coeffDict());
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


void gammaReTheta::correct()
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

    volScalarField const S2(2.0*magSqr(symm(fvc::grad(U_))));
    volScalarField G(GName(), nut_*S2);

    // Update omega and G at the wall
    omega_.boundaryField().updateCoeffs();

    volScalarField CDkOmega
    (
        (2*alphaOmega2_)*(fvc::grad(k_) & fvc::grad(omega_))/omega_
    );

    volScalarField F1( this->F1(CDkOmega));

    dimensionedScalar nutMin
    (
        "nutMin", 
        kMin_.dimensions()/omegaMin_.dimensions(), 
        SMALL
     );

    // Turbulent frequency equation
    tmp<fvScalarMatrix> omegaEqn
    (
        fvm::ddt(omega_)
      + fvm::div(phi_, omega_)
      - fvm::Sp(fvc::div(phi_), omega_)
      - fvm::laplacian(DomegaEff(F1), omega_)
     ==
        intermittency(F1)*S2
      - fvm::Sp(beta(F1)*omega_, omega_)
      - fvm::SuSp
        (
            (F1 - scalar(1))*CDkOmega/omega_,
            omega_
        )
    );

    omegaEqn().relax();

    omegaEqn().boundaryManipulate(omega_.boundaryField());

    solve(omegaEqn);
    bound(omega_, omegaMin_);


    volScalarField intermittencyEff = max
    (
        intermittency_,
       	intermittencySep()
    )();

    // Turbulent kinetic energy equation
    tmp<fvScalarMatrix> kEqn
    (
        fvm::ddt(k_)
      + fvm::div(phi_, k_)
      - fvm::Sp(fvc::div(phi_), k_)
      - fvm::laplacian(DkEff(F1), k_)
     ==
        G*intermittencyEff
      - fvm::Sp
        (
            min
            (
                max
                (
                    intermittencyEff,
                    scalar(0.1)
                ),
                scalar(1)
             )*betaStar_*omega_, 
             k_
        )
    );
    
    kEqn().relax();
    solve(kEqn);
    bound(k_, kMin_);


    // Re-calculate viscosity
    nut_ = min
    (
        a1_*k_/max(a1_*omega_, F2()*sqrt(S2)),
        sqrt(0.6)*k_/(sqrt(3.0)*sqrt(S2))
    );

    nut_.correctBoundaryConditions();


    // local transition onset momentum thickness Reynolds number
    volScalarField ReThetaField
    (
        IOobject
        (
            "ReThetaField",
            runTime_.timeName(),
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        ReThetaTilda_
    );
    ReTheta(ReThetaField);
    
    // Transition onset momentum thickness Reynolds number equation
    tmp<fvScalarMatrix> ReThetaTildaEqn
    (
        fvm::ddt(ReThetaTilda_)
      + fvm::div(phi_, ReThetaTilda_)
      - fvm::Sp(fvc::div(phi_), ReThetaTilda_)
      - fvm::laplacian(DReThetaTildaEff(), ReThetaTilda_)
     ==
        cThetat_*magSqr(U_)
      * (scalar(1.0)-FThetat())*ReThetaField/(scalar(500.0)*nu())
      - fvm::Sp
        (
            cThetat_*magSqr(U_)*(scalar(1.0)-FThetat())/(scalar(500.0)*nu()), 
            ReThetaTilda_
        )
    );

    ReThetaTildaEqn().relax();
    solve(ReThetaTildaEqn);

    bound(ReThetaTilda_,scalar(20));
  

    // Intermittency equation
    
    tmp<fvScalarMatrix> intermittencyEqn
    (
        fvm::ddt(intermittency_)
      + fvm::div(phi_, intermittency_)
      - fvm::Sp(fvc::div(phi_), intermittency_)
      - fvm::laplacian(DintermittencyEff(), intermittency_)
     ==
        Flength()*ca1_*sqrt(scalar(2))*mag(symm(fvc::grad(U_)))*sqrt(Fonset()*intermittency_)
      - fvm::Sp
        (
	    Flength()*ca1_*sqrt(scalar(2))*mag(symm(fvc::grad(U_)))*sqrt(Fonset()*intermittency_)*ce1_,
	    intermittency_
	)
      + ca2_*sqrt(scalar(2))*mag(skew(fvc::grad(U_)))*Fturb()*intermittency_
      - fvm::Sp
        (
            ce2_*ca2_*sqrt(scalar(2))*mag(skew(fvc::grad(U_)))*Fturb()*intermittency_,
            intermittency_
        )
    );

    intermittencyEqn().relax();
    solve(intermittencyEqn);

    bound(intermittency_,scalar(0));
}

}
}
}
