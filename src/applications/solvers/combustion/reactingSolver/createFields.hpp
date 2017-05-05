    Info<< "Reading thermophysical properties\n" << endl;

    autoPtr<combustionModels::psiChemistryCombustionModel> combustionModel
    (
        combustionModels::psiChemistryCombustionModel::New(mesh)
    );

    psiChemistryModel& chemistry = combustionModel->pChemistry();

    hsCombustionThermo& thermo = chemistry.thermo();

    volScalarField& p = thermo.p();
    volScalarField& h = thermo.hs();

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

    basicMultiComponentMixture& composition = thermo.composition();
    PtrList<volScalarField>& Y = composition.Y();
    word inertSpecie(thermo.lookup("inertSpecie"));

    #include "compressibleCreatePhi.hpp"

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

    combustionModel->setTurbulence(turbulence());

    surfaceScalarField phiHat
    (
        "phiHat",
        fvc::interpolate(psi)
       *(
            (fvc::interpolate(U) & mesh.Sf())
        )
    );

    volScalarField dpdt("dpdt", fvc::ddt(p));

    volScalarField Ek("Ek", 0.5*magSqr(U));

    volScalarField EkMatDer("EkMatDer", fvc::ddt(rho,Ek)+fvc::div(phi,Ek));

    multivariateSurfaceInterpolationScheme<scalar>::fieldTable fields;

    forAll(Y, i)
    {
        fields.add(Y[i]);
    }

    fields.add(h);
 
    volScalarField dQ
    (
        IOobject
        (
            "dQ",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionedScalar("dQ", dimEnergy/dimTime, 0.0)
    );
