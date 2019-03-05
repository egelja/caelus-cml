#include "createRDeltaT.hpp"    

Info<< "Reading thermophysical properties\n" << endl;

autoPtr<psiThermo> pThermo(psiThermo::New(mesh));
psiThermo& thermo = pThermo();
thermo.validate(args.executable(), "h");

volScalarField& p = thermo.p();

const volScalarField& psi = thermo.psi();
volScalarField& T = const_cast<volScalarField&>(thermo.T());

volScalarField rho
(
    IOobject
    (
        "rho",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    thermo.rho()
);

Info<< "Reading field U\n" << endl;
volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

#include "compressibleCreatePhi.hpp"

mesh.setFluxRequired(p.name());

dimensionedScalar rhoMax(pimple.dict().lookup("rhoMax"));
dimensionedScalar rhoMin(pimple.dict().lookup("rhoMin"));

Info<< "Creating turbulence model\n" << endl;
autoPtr<compressible::turbulenceModel> turbulence
(
    compressible::turbulenceModel::New
    (
        rho,
        U,
        phi,
        thermo
    )
);

tmp<surfaceScalarField> Uf = phi/fvc::interpolate(rho);

surfaceScalarField phiHat
(
    "phiHat",
    fvc::interpolate(psi)
   *(
        (fvc::interpolate(U) & mesh.Sf())
    )
);

