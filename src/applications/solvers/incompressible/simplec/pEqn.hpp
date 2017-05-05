{
    p.boundaryField().updateCoeffs();
    pHat.boundaryField().updateCoeffs();

    volScalarField AI(1/UEqn().Ac());

    UEqn.clear();

    phi = fvc::interpolate
    (
        U + AI*fvc::grad(p), 
        "interpolate(HbyA)"
    ) & mesh.Sf();

    fvOptions.makeRelative(phi);
    adjustPhi(phi, U, p);

    // Non-orthogonal pressure corrector loop
    while (simplec.correctNonOrthogonal())
    {
        fvScalarMatrix pEqn
        (
            fvm::laplacian(AI, p) == fvc::div(phi)
        );

        pEqn.setReference(pRefCell, pRefValue);

        pEqn.solve();

        if (simplec.finalNonOrthogonalIter())
        {
            phi -= pEqn.flux();
        }
    }

    #include "continuityErrs.hpp"

    // Momentum corrector
    U -= AI*fvc::grad(p-pHat);
    U.correctBoundaryConditions();
    fvOptions.correct(U);
}
