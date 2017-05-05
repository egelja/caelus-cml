    Info<< "Reading thermophysical properties\n" << endl;

    autoPtr<basicPsiThermo> thermo
    (
        basicPsiThermo::New(mesh)
    );

    volScalarField& p = thermo->p();
    volScalarField& h = thermo->h();
    const volScalarField& psi = thermo->psi();
    volScalarField& T = const_cast<volScalarField&>(thermo->T());

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
        thermo->rho()
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
            thermo()
        )
    );

    tmp<surfaceScalarField> Uf = phi/fvc::interpolate(rho);

    surfaceScalarField phid
    (
        "phid",
        fvc::interpolate(psi)
       *(
            (fvc::interpolate(U) & mesh.Sf())
        )
    );


    surfaceScalarField phis
    (
        "phis",
        fvc::interpolate(psi)
       *(
            (fvc::interpolate(U) & mesh.Sf())
        )
    );

    surfaceScalarField phil
    (
        "phil",
        fvc::interpolate(psi)
       *(
            (fvc::interpolate(U) & mesh.Sf())
        )
    );

    surfaceScalarField left
    (
        IOobject
        (
            "left",
            runTime.timeName(),
            mesh
        ),
        mesh,
        dimensionedScalar("left", dimless, 1.0)
    );

    surfaceScalarField right
    (
        IOobject
        (
            "right",
            runTime.timeName(),
            mesh
        ),
        mesh,
        dimensionedScalar("right", dimless, -1.0)
    );
