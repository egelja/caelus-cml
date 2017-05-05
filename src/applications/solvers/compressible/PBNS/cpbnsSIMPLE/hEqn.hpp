{
    Uf = phi/fvc::interpolate(rho);
    fvScalarMatrix hEqn
    (
        fvm::div(phi, h)
      - fvm::Sp(fvc::div(phi), h)
      - fvm::laplacian(turbulence->alphaEff(), h)
     ==
        fvc::div(Uf, p, "div(U,p)")
      - p*fvc::div(Uf)
      - (turbulence->devRhoReff() && fvc::grad(U))
      + fvOptions(rho, h)
    );

    hEqn.relax();

    fvOptions.constrain(hEqn);

    hEqn.solve();

    fvOptions.correct(h);

    thermo->correct();
}

