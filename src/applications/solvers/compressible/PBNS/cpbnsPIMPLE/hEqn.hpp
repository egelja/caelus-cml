    
    volScalarField& he = thermo.he();

    fvScalarMatrix hEqn
    (
        fvm::ddt(rho, he)
      + fvm::div(phi, he)
      + EkMatDer
      - fvm::laplacian(turbulence->alphaEff(), he)
     ==
        dpdt
      + fvc::div(turbulence->muEff()*(fvc::grad(U))&U) 
      + fvc::div(turbulence->muEff()*dev2(CML::T(fvc::grad(U)))&U)
      + fvOptions(rho,he)
    );

    fvOptions.constrain(hEqn);
    hEqn.solve();
    fvOptions.correct(he);

    thermo.correct();


