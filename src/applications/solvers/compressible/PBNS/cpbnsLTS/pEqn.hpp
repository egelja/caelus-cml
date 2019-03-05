volScalarField rAU(1.0/UEqn.A());

rho = thermo.rho();
rho = max(rho, rhoMin);
rho = min(rho, rhoMax);
rho.relax();
rho.correctBoundaryConditions(); // DWS Added
volScalarField R = ( thermo.Cp() - thermo.Cv() )();
R.correctBoundaryConditions();
T.relax();
//thermo.rho() = thermo.rho() - psi*p;
//thermo.rho().correctBoundaryConditions();


volVectorField HbyA(constrainHbyA(rAU*UEqn.H(), U, p));

if (pimple.nCorrPISO() <= 1)
{
    tUEqn.clear();
}

#include "compressibleFlux_cpbnsLTS.hpp"

while (pimple.correctNonOrthogonal())
{
    fvScalarMatrix pEqn
    (
        fvm::ddt(psi, p)
      + fvm::div(phiHat, p)
      - fvm::laplacian(rho*rAU, p)
    );

    pEqn.solve(mesh.solver(p.select(pimple.finalInnerIter())));

    if (pimple.finalNonOrthogonalIter())
    {
        phi == pEqn.flux();
    }
}

//thermo.rho() = thermo.rho() + psi*p;
//thermo.rho().correctBoundaryConditions();

#include "rhoEqn.hpp"
#include "compressibleContinuityErrs.hpp"

p.relax();

rho = thermo.rho();
rho = max(rho, rhoMin);
rho = min(rho, rhoMax);
rho.relax();
rho.correctBoundaryConditions(); //DWS -added
T.relax();
Info<< "rho max/min : " << max(rho).value()
    << " " << min(rho).value() << endl;

U = HbyA - rAU*fvc::grad(p);
U.correctBoundaryConditions();
fvOptions.correct(U);
