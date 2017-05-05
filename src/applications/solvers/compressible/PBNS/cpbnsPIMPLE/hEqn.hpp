if (totalEnthalpyForm)
{
    volScalarField KE("KE", 0.5*magSqr(U));

    fvScalarMatrix hEqn
    (
        fvm::ddt(rho, h)
      + fvm::div(phi, h)
      + fvc::ddt(rho, KE) 
      + fvc::div(phi, KE)
      - fvm::laplacian(turbulence->alphaEff(), h)
     ==
        dpdt
      + fvc::div(turbulence->muEff()*(fvc::grad(U))&U) 
      + fvc::div(turbulence->muEff()*dev2(CML::T(fvc::grad(U)))&U)
      + fvOptions(rho,h)
    );

    hEqn.relax();
    fvOptions.constrain(hEqn);
    hEqn.solve();
    fvOptions.correct(h);

    thermo->correct();
}
else
{
    fvScalarMatrix hEqn
    (
        fvm::ddt(rho, h)
      + fvm::div(phi, h)
      - fvm::laplacian(turbulence->alphaEff(), h)
     ==
        dpdt
      + (U & fvc::grad(p))
      - (turbulence->devRhoReff() && CML::fvc::grad(U))
      + fvOptions(rho,h)
    );

    hEqn.relax();
    fvOptions.constrain(hEqn);
    hEqn.solve();
    fvOptions.correct(h);

    thermo->correct();
}

