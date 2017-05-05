tmp<fvVectorMatrix> UEqn
(
    fvm::ddt(rho, U)
  + fvm::div(phi, U)
  + turbulence->divDevRhoReff(U)
 ==
    fvOptions(rho,U)
);

fvOptions.constrain(UEqn());

UEqn().relax();

if (pimple.momentumPredictor())
{
    solve(UEqn() == -fvc::grad(p));
}

fvOptions.correct(U);

volScalarField rAU(1.0/UEqn().A());

