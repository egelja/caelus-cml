#include "SpalartAllmarasDES.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{
namespace incompressible
{
namespace LESModels
{

defineTypeNameAndDebug(SpalartAllmarasDES, 0);
addToRunTimeSelectionTable(LESModel, SpalartAllmarasDES, dictionary);

void SpalartAllmarasDES::updateSubGridScaleFields()
{
    nuSgs_.internalField() = fv1()*nuTilda_.internalField();
    nuSgs_.correctBoundaryConditions();
}

tmp<volScalarField> SpalartAllmarasDES::chi() const
{
    return nuTilda_/nu();
}


tmp<volScalarField> SpalartAllmarasDES::fv1() const
{
    const volScalarField chi3(pow3(chi()));
    return chi3/(chi3 + pow3(Cv1_));
}


tmp<volScalarField> SpalartAllmarasDES::fv2() const
{
    return (scalar(1.0) - chi()/(scalar(1.0)+chi()*fv1()));
}

// Magnitude of vorticity 
tmp<volScalarField> SpalartAllmarasDES::S(const volTensorField& gradU) const
{
    return sqrt(2.0)*mag(skew(gradU));
}

tmp<volScalarField> SpalartAllmarasDES::STilda
(
    const volScalarField& S,
    const volScalarField& dTilda
) const
{
    return S + fv2()*nuTilda_/sqr(kappa_*dTilda);
}

tmp<volScalarField> SpalartAllmarasDES::r
(
    const volScalarField& visc,
    const volScalarField& S,
    const volScalarField& dTilda
) const
{
    return min
    (
        visc
       /(
           max
           (
               S,
               dimensionedScalar("SMALL", S.dimensions(), SMALL)
           )
          *sqr(kappa_*dTilda)
        ),
        scalar(10)
    );
}


tmp<volScalarField> SpalartAllmarasDES::fw
(
    const volScalarField& S,
    const volScalarField& dTilda
) const
{
    const volScalarField r(this->r(nuTilda_, S, dTilda));
    const volScalarField g(r + Cw2_*(pow6(r) - r));

    return g*pow((1 + pow6(Cw3_))/(pow6(g) + pow6(Cw3_)), 1.0/6.0);
}


tmp<volScalarField> SpalartAllmarasDES::dTilda(const volScalarField&) const
{
    return min(CDES_*delta(), y_);
}

SpalartAllmarasDES::SpalartAllmarasDES
(
    const volVectorField& U,
    const surfaceScalarField& phi,
    transportModel& transport,
    const word& turbulenceModelName,
    const word& modelName
)
:
    LESModel(modelName, U, phi, transport, turbulenceModelName),

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
    Cv1_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Cv1",
            coeffDict_,
            7.1
        )
    ),
    Cv2_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Cv2",
            coeffDict_,
            5.0
        )
    ),
    CDES_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "CDES",
            coeffDict_,
            0.65
        )
    ),
    ck_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "ck",
            coeffDict_,
            0.07
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

    y_(mesh_),

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
    )
{
    updateSubGridScaleFields();
}


void SpalartAllmarasDES::correct(const tmp<volTensorField>& gradU)
{
    LESModel::correct(gradU);

    if (mesh_.changing())
    {
        y_.correct();
        y_.boundaryField() = max(y_.boundaryField(), VSMALL);
    }

    const volScalarField S(this->S(gradU));
    const volScalarField dTilda(this->dTilda(S));
    const volScalarField STilda(this->STilda(S, dTilda));

    tmp<fvScalarMatrix> nuTildaEqn
    (
        fvm::ddt(nuTilda_)
      + fvm::div(phi(), nuTilda_)
      - fvm::laplacian
        (
            (nuTilda_ + nu())/sigmaNut_,
            nuTilda_,
            "laplacian(DnuTildaEff,nuTilda)"
        )
      - Cb2_/sigmaNut_*magSqr(fvc::grad(nuTilda_))
     ==
        Cb1_*STilda*nuTilda_
      - fvm::Sp(Cw1_*fw(STilda, dTilda)*nuTilda_/sqr(dTilda), nuTilda_)
    );

    nuTildaEqn().relax();
    nuTildaEqn().solve();

    bound(nuTilda_, dimensionedScalar("zero", nuTilda_.dimensions(), 0.0));
    nuTilda_.correctBoundaryConditions();

    updateSubGridScaleFields();
}


tmp<volScalarField> SpalartAllmarasDES::k() const
{
    return sqr(nuSgs()/ck_/dTilda(S(fvc::grad(U()))));
}


tmp<volScalarField> SpalartAllmarasDES::epsilon() const
{
    return 2*nuEff()*magSqr(symm(fvc::grad(U())));
}


tmp<volSymmTensorField> SpalartAllmarasDES::B() const
{
    return ((2.0/3.0)*I)*k() - nuSgs()*twoSymm(fvc::grad(U()));
}


tmp<volSymmTensorField> SpalartAllmarasDES::devReff() const
{
    return -nuEff()*dev(twoSymm(fvc::grad(U())));
}


tmp<fvVectorMatrix> SpalartAllmarasDES::divDevReff(volVectorField& U) const
{
    return
    (
      - fvm::laplacian(nuEff(), U) - fvc::div(nuEff()*dev(T(fvc::grad(U))))
    );
}

tmp<fvVectorMatrix> SpalartAllmarasDES::divDevRhoReff
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


bool SpalartAllmarasDES::read()
{
    if (LESModel::read())
    {
        sigmaNut_.readIfPresent(coeffDict());
        kappa_.readIfPresent(*this);
        Cb1_.readIfPresent(coeffDict());
        Cb2_.readIfPresent(coeffDict());
        Cv1_.readIfPresent(coeffDict());
        Cv2_.readIfPresent(coeffDict());
        CDES_.readIfPresent(coeffDict());
        ck_.readIfPresent(coeffDict());
        Cw1_ = Cb1_/sqr(kappa_) + (1.0 + Cb2_)/sigmaNut_;
        Cw2_.readIfPresent(coeffDict());
        Cw3_.readIfPresent(coeffDict());

        return true;
    }
    else
    {
        return false;
    }
}


}
}
}


