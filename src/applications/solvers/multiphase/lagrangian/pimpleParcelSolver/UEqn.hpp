// Solve the Momentum equation
MRF.correctBoundaryVelocity(U);

tmp<fvVectorMatrix> UEqn
(
    fvm::ddt(U) + fvm::div(phi, U)
  + MRF.DDt(U)
  + turbulence->divDevReff(U)
 ==
    fvOptions(U)
);

for (label i = 0; i < nClouds; ++i)
{
    UEqn() == 1/rhoInf*kinematicClouds[i].SU(U);
}

UEqn().relax();

fvOptions.constrain(UEqn());

volScalarField rAU(1.0/UEqn().A());

if (pimple.momentumPredictor())
{
    solve(UEqn() == -fvc::grad(p));

    fvOptions.correct(U);
}
