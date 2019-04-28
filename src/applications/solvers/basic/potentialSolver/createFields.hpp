    Info<< "Reading field p\n" << endl;
    volScalarField p
    (
        IOobject
        (
            "p",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        ),
        mesh
    );

    p = dimensionedScalar("zero", p.dimensions(), 0.0);


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

    U = dimensionedVector("0", U.dimensions(), vector::zero);

    surfaceScalarField phi
    (
        IOobject
        (
            "phi",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        fvc::flux(U)
    );

    if (args.optionFound("initialiseUBCs"))
    {
        U.correctBoundaryConditions();
        phi = fvc::flux(U);
    }


    label pRefCell = 0;
    scalar pRefValue = 0.0;
    setRefCell
    (
        p,
        potentialFlow,
        pRefCell,
        pRefValue
    );

    mesh.setFluxRequired(p.name());
