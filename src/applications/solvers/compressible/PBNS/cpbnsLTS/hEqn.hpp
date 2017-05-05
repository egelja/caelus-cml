{
    Uf = phi/fvc::interpolate(rho);
    fvScalarMatrix hEqn
    (
        fvm::ddt(rho, h)
      + fvm::div(phi, h)
      - fvm::laplacian(turbulence->alphaEff(), h)
     ==
        fvc::ddt(p)
      + fvc::div(Uf(), p, "div(U,p)")
      - p*fvc::div(Uf())
      - (turbulence->devRhoReff() && CML::fvc::grad(U))
      + fvOptions(rho,h)
    );

    hEqn.relax();
    fvOptions.constrain(hEqn);
    hEqn.solve();
    fvOptions.correct(h);

    thermo->correct();
}
