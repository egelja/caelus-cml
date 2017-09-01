    // Calculate and cache mu for the porous media
    volScalarField mu(mixture.mu());

    surfaceScalarField muEff
    (
        "muEff",
        mixture.muf()
      + fvc::interpolate(rho*turbulence->nut())
    );

    fvVectorMatrix UEqn
    (
        pm->ddt(rho, U)
        + 1.0/porosity*fvm::div(rhoPhi/(porosityFace), U)
        - fvm::laplacian(muEff/porosityFace, U)
        - (fvc::grad(U) & (1.0/porosity*fvc::grad(muEff)))
     ==
        fvOptions(rho, U)
    );

    UEqn.relax();

    pm->addResistance(UEqn);

    fvOptions.constrain(UEqn);

    if (pimple.momentumPredictor())
    {
        solve
        (
            UEqn
         ==
            fvc::reconstruct
            (
                (
                    interface.surfaceTensionForce()
                  - ghf*fvc::snGrad(rho)
                  - fvc::snGrad(p_rgh)
                ) * mesh.magSf()
            )
        );

        fvOptions.correct(U);
    }
