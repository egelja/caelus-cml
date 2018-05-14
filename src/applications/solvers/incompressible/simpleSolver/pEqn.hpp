{
    p.boundaryField().updateCoeffs();
    volScalarField pHat = p;

    scalarField& VolI = Vol.internalField();
    VolI = mesh.V();
    Vol.correctBoundaryConditions();

    surfaceScalarField const volLeft =
        fvc::interpolate(Vol, left, "reconstruct")();

    surfaceScalarField const volRight =
        fvc::interpolate(Vol, right, "reconstruct")();

    surfaceScalarField const cVol(volLeft + volRight);
           
    if (simple.consistent())
    {
        rAU = 1.0/UEqn().Ac();
    }
    else
    {
        rAU = 1.0/UEqn().A();
    }

    surfaceScalarField const rAUfLeft
    (
        "rAUfLeft",
        fvc::interpolate(rAU*Vol, left, "reconstruct")
    );

    surfaceScalarField const rAUfRight
    (
        "rAUfRight",
        fvc::interpolate(rAU*Vol, right, "reconstruct")
    );

    surfaceScalarField const rAUf((rAUfLeft + rAUfRight)/cVol);

    if (simple.correctionForm())
    {
        surfaceVectorField const e(mesh.delta()/mag(mesh.delta()));
        
        CML::fv::orthogonalSnGrad<scalar> const nFaceGradient(mesh);
    
        surfaceVectorField const faceAvgGradP(fvc::interpolate(fvc::grad(p)));
    
        surfaceVectorField const faceGradP
        (
            faceAvgGradP
          - (faceAvgGradP & e)*e
          + nFaceGradient.snGrad(p)*e
        );
    
        Uf = (fvc::interpolate(U))();

        scalar const alphaU = mesh.equationRelaxationFactor("U");

        phi = (Uf & mesh.Sf())
            - ((rAUf*(faceGradP - faceAvgGradP)) & mesh.Sf())
            + (scalar(1) - alphaU)*(phi.prevIter()-(Uf.prevIter() & mesh.Sf()));
    }
    else
    {
        if (simple.consistent())
        {
            phi = fvc::interpolate(U + rAU*fvc::grad(p), "interpolate(HbyA)")
                & mesh.Sf();
        }
        else
        {
            U = rAU*UEqn().H();
            phi = fvc::interpolate(U, "interpolate(HbyA)") & mesh.Sf();
        }
    }

    UEqn.clear();

    MRF.makeRelative(phi);
    adjustPhi(phi, U, p);

    if (simple.correctionForm())
    {
        pCorr *= scalar(0);
        pCorr.correctBoundaryConditions();
    }

    // Update pressure BCs to ensure flux consistency
    constrainPressure(p, U, phi, rAU, MRF);

    // Non-orthogonal pressure corrector loop
    while (simple.correctNonOrthogonal())
    {
        fvScalarMatrix pEqn
        (
            fvm::laplacian
            (
                rAUf,
                simple.correctionForm() ? pCorr : p,
                "laplacian(rAUf,p)"
            )
          ==
            fvc::div(phi)
        );
 
        pEqn.setReference(pRefCell, pRefValue); //NOTE pRefValue should always be zero for correctedForm.

        pEqn.solve();

        if (simple.finalNonOrthogonalIter())
        {
            phi -= pEqn.flux();
        }
    }
    
    #include "continuityErrs.hpp"

    if (simple.consistent())
    {
        if (simple.correctionForm())
        {
            // Correct pressure with pressure correction
            p += pCorr;
            p.correctBoundaryConditions();

            // Momentum corrector
            U -= rAU*fvc::grad(pCorr);
        }
        else
        {
            // Momentum corrector
            U -= rAU*fvc::grad(p-pHat);
        }
    }
    else
    {
        if (simple.correctionForm())
        {
            scalar const alphaP = mesh.fieldRelaxationFactor("pCorr");
            // Correct pressure with relaxed pressure correction
            p += alphaP*pCorr;
            p.correctBoundaryConditions();

            // Momentum corrector
            U -= rAU*fvc::grad(pCorr);
        }
        else
        {
            // Explicitly relax pressure for momentum corrector
            p.relax();
            U -= rAU*fvc::grad(p);
        }
    }
    
    U.correctBoundaryConditions();
    fvOptions.correct(U);
}
