    fvScalarMatrix hEqn
    (
        fvm::ddt(rho, h)
      + fvm::div(phi, h)
      + EkMatDer
      - fvm::laplacian(turbulence->alphaEff(), h)
     ==
        dpdt
      + fvc::div(turbulence->muEff()*(fvc::grad(U))&U) 
      + fvc::div(turbulence->muEff()*dev2(CML::T(fvc::grad(U)))&U)
      + fvOptions(rho,h)
    );

    fvOptions.constrain(hEqn);
    hEqn.solve();
    fvOptions.correct(h);

    thermo->correct();


