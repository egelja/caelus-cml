Info<< "Reading thermophysical properties\n" << endl;

autoPtr<psiReactionThermo> pThermo(psiReactionThermo::New(mesh));
psiReactionThermo& thermo = pThermo();
thermo.validate(args.executable(), "h");

//    psiChemistryModel& chemistry = combustionModel->pChemistry();

volScalarField& p = thermo.p();
volScalarField& he = thermo.he();

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

volScalarField T
(
    IOobject
    (
        "T",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    thermo.T()
);

volScalarField const& psi = thermo.psi();

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

basicSpecieMixture& composition = thermo.composition();
PtrList<volScalarField>& Y = composition.Y();
word inertSpecie(thermo.lookup("inertSpecie"));
const label inertIndex(composition.species()[inertSpecie]);

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

Info<< "Creating reaction model\n" << endl;
autoPtr<CombustionModel<psiReactionThermo> > reaction
(
    CombustionModel<psiReactionThermo>::New(thermo, turbulence())
);

surfaceScalarField phiHat
(
    "phiHat",
    fvc::interpolate(psi)
   *(
        (fvc::interpolate(U) & mesh.Sf())
    )
);

volScalarField dpdt("dpdt", fvc::ddt(p));

Info<< "Creating field kinetic energy K\n" << endl;
volScalarField Ek("Ek", 0.5*magSqr(U));

volScalarField EkMatDer("EkMatDer", fvc::ddt(rho,Ek)+fvc::div(phi,Ek));

multivariateSurfaceInterpolationScheme<scalar>::fieldTable fields;

forAll(Y, i)
{
    fields.add(Y[i]);
}

fields.add(thermo.he());
 
volScalarField Qdot
(
    IOobject
    (
        "Qdot",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    mesh,
    dimensionedScalar("Qdot", dimEnergy/dimVolume/dimTime, 0.0)
);
