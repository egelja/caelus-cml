{
    p.boundaryField().updateCoeffs();

    volScalarField rAU(1.0/UEqn().A());
    U = rAU*UEqn().H();
    
    scalarField& VolI = Vol.internalField();
    VolI = mesh.V();
    Vol.correctBoundaryConditions();

    surfaceScalarField const volLeft =
        fvc::interpolate(Vol, left, "reconstruct")();

    surfaceScalarField const volRight =
        fvc::interpolate(Vol, right, "reconstruct")();

    surfaceScalarField const cVol(volLeft + volRight);

    surfaceScalarField const rAUfLeft
    (
        "rAUfLeft",
        fvc::interpolate
        (
            rAU*Vol,
            left,
            "reconstruct"
        )
    );

    surfaceScalarField const rAUfRight
    (
        "rAUfRight",
        fvc::interpolate
        (
            rAU*Vol,
            right,
            "reconstruct"
        )
    );

    surfaceScalarField const rAUf((rAUfLeft + rAUfRight)/cVol);
    
    UEqn.clear();

    phi = fvc::interpolate(U, "interpolate(HbyA)") & mesh.Sf();
    fvOptions.makeRelative(phi);
    adjustPhi(phi, U, p);

    // Non-orthogonal pressure corrector loop
    while (simple.correctNonOrthogonal())
    {
        fvScalarMatrix pEqn
        (
            fvm::laplacian(rAUf, p, "laplacian(rAUf,p)") == fvc::div(phi)
        );

        pEqn.setReference(pRefCell, pRefValue);

        pEqn.solve();

        if (simple.finalNonOrthogonalIter())
        {
            phi -= pEqn.flux();
        }
    }

    #include "continuityErrs.hpp"

    // Explicitly relax pressure for momentum corrector
    p.relax();

    // Momentum corrector
    U -= rAU*fvc::grad(p);
    U.correctBoundaryConditions();
    fvOptions.correct(U);
}
