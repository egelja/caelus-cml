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

volScalarField::GeometricBoundaryField const& pGBF = p.boundaryField();
wordList const pBT = pGBF.types();
        
volScalarField pCorr
(
    IOobject
    (
        "pCorr",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar("zero", p.dimensions(), 0.0),
    pBT
);

forAll(pCorr.boundaryField(), patchi)
{
    if
    (
        isType<fixedValueFvPatchScalarField>
        (
            pCorr.boundaryField()[patchi]
        )
    )
    {
        fixedValueFvPatchScalarField& pCorrB =
            refCast<fixedValueFvPatchScalarField>
            (
                pCorr.boundaryField()[patchi]
            );
        
        pCorrB == scalarField(pCorrB.size(),scalar(0.0));
    }
}

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

surfaceVectorField Uf = (fvc::interpolate(U))();

label pRefCell = 0;
scalar pRefValue = 0.0;
if (simple.correctionForm())
{
    // Read values from dicitionary
    setRefCell(p, mesh.solutionDict().subDict("SIMPLE"), pRefCell, pRefValue);

    // Handle a non-zero pRefValue
    if (p.needReference())
    {
        p += dimensionedScalar("p", p.dimensions(), pRefValue - getRefCellValue(p, pRefCell));
    }

    // Change pRefValue to zero for correctionForm
    pRefValue = 0.0;
}
else
{
    setRefCell(p, mesh.solutionDict().subDict("SIMPLE"), pRefCell, pRefValue);
}
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

volScalarField rAU
(
    IOobject
    (
        "rAU",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    runTime.deltaT(),
    zeroGradientFvPatchScalarField::typeName
);

mesh.setFluxRequired(p.name());

mesh.setFluxRequired(pCorr.name());
