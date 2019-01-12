{
    volScalarField& he = thermo.he();

    Uf = phi/fvc::interpolate(rho);

    fvScalarMatrix EEqn
    (
        fvm::ddt(rho, he) + fvm::div(phi, he)
      - fvm::laplacian(turbulence->alphaEff(), he)
     ==
        fvc::ddt(p)
      + fvc::div(Uf(), p, "div(U,p)")
      - p*fvc::div(Uf())
      - (turbulence->devRhoReff() && CML::fvc::grad(U))
      + fvOptions(rho,he)
    );

    EEqn.relax();

    fvOptions.constrain(EEqn);

    EEqn.solve();

    fvOptions.correct(he);

    thermo.correct();
}
