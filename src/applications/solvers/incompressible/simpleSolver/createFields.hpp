    Info<< "Reading field p\n" << endl;
    volScalarField p
    (
        IOobject
        (
            "p",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
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

    #include "createPhi.hpp"


    label pRefCell = 0;
    scalar pRefValue = 0.0;
    setRefCell(p, mesh.solutionDict().subDict("SIMPLE"), pRefCell, pRefValue);

    singlePhaseTransportModel laminarTransport(U, phi);

    autoPtr<incompressible::RASModel> turbulence
    (
        incompressible::RASModel::New(U, phi, laminarTransport)
    );

    volScalarField Vol
    (
        IOobject
        (
            "Vol",
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("vol",dimensionSet(0,3,0,0,0,0,0),scalar(0)),
        zeroGradientFvPatchScalarField::typeName
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
