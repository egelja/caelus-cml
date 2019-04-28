/*---------------------------------------------------------------------------*\
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

#include "VLESSpalartAllmaras.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{
namespace incompressible
{
namespace VLESModels
{

defineTypeNameAndDebug(SpalartAllmarasVLES, 0);
addToRunTimeSelectionTable(VLESModel, SpalartAllmarasVLES, dictionary);

tmp<volScalarField> SpalartAllmarasVLES::F1() const
{

    volScalarField const CDkOmega
    (
        (2*0.856)*(fvc::grad(k()) 
        & 
        fvc::grad(epsilon()/k()/0.09))/(epsilon()/k()/0.09)
    );

    tmp<volScalarField> CDkOmegaPlus = max
    (
        CDkOmega,
        dimensionedScalar("1.0e-10", dimless/sqr(dimTime), 1.0e-10)
    );

    tmp<volScalarField> arg1 = min
    (
        max
        (
            (scalar(1)/0.09)*sqrt(k())/(epsilon()/k()*d_/0.09),
            scalar(500)*nu()/(sqr(d_)*epsilon()/k()/0.09)
        ),
        (4*0.856)*k()/(CDkOmegaPlus*sqr(d_))
    );

    return tanh(pow4(arg1));
}


tmp<volScalarField> SpalartAllmarasVLES::chi() const
{
    return nuTilda_/nu();
}


tmp<volScalarField> 
SpalartAllmarasVLES::fv1(const volScalarField& chi) const
{
    volScalarField const chi3(pow3(chi));
    return chi3/(chi3 + pow3(Cv1_));
}


tmp<volScalarField> SpalartAllmarasVLES::fv2
(
    volScalarField const& chi,
    volScalarField const& fv1
) const
{
    return (scalar(1.0) - chi/(scalar(1.0)+chi*fv1));
}


tmp<volScalarField> 
SpalartAllmarasVLES::fw(volScalarField const& Stilda) const
{
    volScalarField r
    (
        min
        (
            nuTilda_
           /(
               max
               (
                   Stilda,
                   dimensionedScalar("SMALL", Stilda.dimensions(), SMALL)
               )
              *sqr(kappa_*d_)
            ),
            scalar(10.0)
        )
    );
    r.boundaryField() == 0.0;

    volScalarField const g(r + Cw2_*(pow6(r) - r));

    return g*pow((1.0 + pow6(Cw3_))/(pow6(g) + pow6(Cw3_)), 1.0/6.0);
}

SpalartAllmarasVLES::SpalartAllmarasVLES
(
    volVectorField const& U,
    surfaceScalarField const& phi,
    transportModel& transport,
    word const& turbulenceModelName,
    word const& modelName
) :
    VLESModel(modelName, U, phi, transport, turbulenceModelName),
    curvatureCorrection_
    (
        coeffDict_.lookupOrDefault<Switch>
        (
            "curvatureCorrection", 
            false
        )
    ),
    delayed_(coeffDict_.lookupOrDefault<Switch>("delayed", true)),
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
    sigmaNut_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "sigmaNut",
            coeffDict_,
            0.66666
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

    Cb1_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Cb1",
            coeffDict_,
            0.1355
        )
    ),
    Cb2_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Cb2",
            coeffDict_,
            0.622
        )
    ),
    Cw1_(Cb1_/sqr(kappa_) + (1.0 + Cb2_)/sigmaNut_),
    Cw2_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Cw2",
            coeffDict_,
            0.3
        )
    ),
    Cw3_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Cw3",
            coeffDict_,
            2.0
        )
    ),
    Cv1_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Cv1",
            coeffDict_,
            7.1
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
            12.0
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
    Cx_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Cx",
            coeffDict_,
            0.61
        )
    ),
    nuTilda_
    (
        IOobject
        (
            "nuTilda",
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
    nuSgs_
    (
        IOobject
        (
            "nuSgs",
            runTime_.timeName(),
            mesh_,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        mesh_,
        dimensionedScalar("nuSgs",nut_.dimensions(),0)
    ),
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
        dimensionedScalar("one", dimless, 1),
        calculatedFvPatchScalarField::typeName
    ),
    d_(mesh_)
{
    if (curvatureCorrection_)
    {
        Info<<" Curvature correction modification enabled" << endl;
    }

    printCoeffs();
}

tmp<volScalarField> SpalartAllmarasVLES::DnuTildaEff() const
{
    return tmp<volScalarField>
    (
        new volScalarField("DnuTildaEff", (nuTilda_ + nu())/sigmaNut_)
    );
}


tmp<volScalarField> SpalartAllmarasVLES::k() const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "k",
                runTime_.timeName(),
                mesh_
            ),
            (
                sqrt
                (
                    epsilon()*nut_/0.09
                    +
                    dimensionedScalar
                    (
                        "sk",
                        dimensionSet(0,4,-4,0,0,0,0),
                        scalar(VSMALL)
                    )
                )
            )
        )
    );
}


tmp<volScalarField> SpalartAllmarasVLES::epsilon() const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "epsilon",
                runTime_.timeName(),
                mesh_
            ),
            (
                scalar(2.0)*nuEff()*magSqr(symm(fvc::grad(U())))
                +
                dimensionedScalar
                (
                    "se",
                    dimensionSet(0,2,-3,0,0,0,0),
                    scalar(VSMALL)
                )
            )
        )
    );
}


tmp<volSymmTensorField> SpalartAllmarasVLES::R() const
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
            ((2.0/3.0)*I)*(sqrt(epsilon()*nut_/0.09)) 
            - nut()*twoSymm(fvc::grad(U_))
        )
    )*Fr_;
}


bool SpalartAllmarasVLES::read()
{
    if (VLESModel::read())
    {
        curvatureCorrection_.readIfPresent("curvatureCorrection", coeffDict());
        delayed_.readIfPresent("delayed", coeffDict());
        sigmaNut_.readIfPresent(coeffDict());
        kappa_.readIfPresent(coeffDict());
        Cb1_.readIfPresent(coeffDict());
        Cb2_.readIfPresent(coeffDict());
        Cw1_ = Cb1_/sqr(kappa_) + (1.0 + Cb2_)/sigmaNut_;
        Cw2_.readIfPresent(coeffDict());
        Cw3_.readIfPresent(coeffDict());
        Cv1_.readIfPresent(coeffDict());
        Cr1_.readIfPresent(coeffDict());
        Cr2_.readIfPresent(coeffDict());
        Cr3_.readIfPresent(coeffDict());
        Cx_.readIfPresent(coeffDict());

        return true;
    }
    else
    {
        return false;
    }
}


void SpalartAllmarasVLES::correct()
{
    VLESModel::correct();

    if (!turbulence_)
    {
        // Re-calculate viscosity
        nut_ = nuTilda_*fv1(this->chi());
        nut_.correctBoundaryConditions();

        return;
    }

    if (mesh_.changing())
    {
        d_.correct();
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
            << "Case is not 3D or 2D, LES is not applicable"
            << exit(FatalError);
    }

    volScalarField const chi(this->chi());
    volScalarField const fv1(this->fv1(chi));
    volTensorField const Omegaij(skew(fvc::grad(this->U_)));
    volScalarField const sqrOmega(2*magSqr(Omegaij));

    nut_ = fv1*nuTilda_;
    nut_.correctBoundaryConditions();

    tmp<volScalarField> const Li = pow(k(),3.0/2.0)/epsilon();

    tmp<volScalarField> const Lk = pow(nu(),3.0/4.0)/pow(epsilon(),1.0/4.0);

    if (delayed_)
    {
        Fr_ = min
        (
            scalar(1.0),
            pow
            (
                (scalar(1.0)-(1-F1())*exp(-0.002*Lc/Lk()))
                /
                (scalar(1.0)-(1-F1())*exp(-0.002*Li()/Lk())),
                2.0
            )
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

    // Curvature correction terms
    if (curvatureCorrection_)
    {
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
       fr1_ = 
       (scalar(1.0) + Cr1_)*scalar(2.0)*rStar/(scalar(1.0) + rStar)
            *(scalar(1.0)-Cr3_*atan(Cr2_*rTilda)) - Cr1_;
    }

    volScalarField const Stilda
    (
        sqrt(sqrOmega) + fv2(chi, fv1)*nuTilda_/sqr(kappa_*d_)
    );

    tmp<fvScalarMatrix> nuTildaEqn
    (
        fvm::ddt(nuTilda_)
      + fvm::div(phi_, nuTilda_)
      - fvm::laplacian(DnuTildaEff(), nuTilda_)
      - Cb2_/sigmaNut_*magSqr(fvc::grad(nuTilda_))
     ==
        fr1_*Cb1_*Stilda*nuTilda_
      - fvm::Sp(Cw1_*fw(Stilda)*nuTilda_/sqr(d_), nuTilda_)
    );

    nuTildaEqn().relax();

    mesh_.updateFvMatrix(nuTildaEqn());
    solve(nuTildaEqn);
    bound(nuTilda_, dimensionedScalar("0", nuTilda_.dimensions(), 0.0));
    nuTilda_.correctBoundaryConditions();

    nut_ = fv1*nuTilda_;
    nut_.correctBoundaryConditions();

    // Re-calculate viscosity
    nuSgs_ = Fr_*nut_;
    nuSgs_.correctBoundaryConditions();
}


}
}
}


