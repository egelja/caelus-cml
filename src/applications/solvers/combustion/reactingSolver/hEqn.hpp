    fvScalarMatrix hEqn
    (
        fvm::ddt(rho, he)
      + divOperator().fvmDiv(phi, he)
      + EkMatDer
      - fvm::laplacian(turbulence->alphaEff(), he)
     ==
        dpdt
      + fvc::div(turbulence->muEff()*(fvc::grad(U))&U) 
      + fvc::div(turbulence->muEff()*dev2(CML::T(fvc::grad(U)))&U)
      + fvOptions(rho,he)
      + Qdot
    );

    fvOptions.constrain(hEqn);
    hEqn.relax();
    hEqn.solve();
    fvOptions.correct(he);

    thermo.correct();


