{  
    p.boundaryField().updateCoeffs();

    scalarField& VolI = Vol.internalField();
    VolI = mesh.V();
    Vol.correctBoundaryConditions();

    surfaceScalarField const volLeft =
        fvc::interpolate(Vol, left, "reconstruct")();

    surfaceScalarField const volRight =
        fvc::interpolate(Vol, right, "reconstruct")();

    surfaceScalarField const cVol(volLeft + volRight);

    volScalarField const rAU(1.0/UEqn().A());
    
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
   
    scalar const omegaU = mesh.equationRelaxationFactor("U");

    phi = (Uf&mesh.Sf())
        - ((rAUf*(faceGradP - faceAvgGradP))&mesh.Sf())
        + (scalar(1) - omegaU)*(phi.prevIter()-(Uf.prevIter()&mesh.Sf()));
    
    UEqn.clear();

    fvOptions.makeRelative(phi);
    adjustPhi(phi, U, p);

    pCorr *= scalar(0);
    pCorr.correctBoundaryConditions();

    // Non-orthogonal pressure corrector loop
    while (simple.correctNonOrthogonal())
    {
        fvScalarMatrix pCorrEqn
        (
            fvm::laplacian(rAUf, pCorr, "laplacian(rAUf,p)") == fvc::div(phi)
        );

        pCorrEqn.setReference(pRefCell, pRefValue);

        pCorrEqn.solve();

        if (simple.finalNonOrthogonalIter())
        {
            phi -= pCorrEqn.flux();
        }
    }

    #include "continuityErrs.hpp"

    scalar const omegaP = mesh.fieldRelaxationFactor("pCorr");

    p += omegaP*pCorr;
    p.correctBoundaryConditions();

    // Momentum corrector
    U -= rAU*fvc::grad(pCorr);
    U.correctBoundaryConditions();
    fvOptions.correct(U);
}
