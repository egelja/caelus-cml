rho = thermo.rho();
rho = max(rho, rhoMin);
rho = min(rho, rhoMax);
T = thermo.T();

volVectorField HbyA(constrainHbyA(rAU*UEqn.H(), U, p));

#include "compressibleFlux_reacting.hpp"

if (pimple.nCorrPISO() <= 1) tUEqn.clear();

#include "compressibleFlux_reacting.hpp"

while (pimple.correctNonOrthogonal())
{
    fvScalarMatrix pEqn
    (
        fvm::ddt(psi, p)
      + fvm::div(phiHat, p)
      - fvm::laplacian(rAU*rho, p)
    );

    pEqn.solve(mesh.solver(p.select(pimple.finalInnerIter())));

    if (pimple.finalNonOrthogonalIter())
    {
        phi == pEqn.flux();
    }
}

#include "rhoEqn.hpp"
#include "compressibleContinuityErrs.hpp"

p.relax();

rho = thermo.rho();
rho = max(rho, rhoMin);
rho = min(rho, rhoMax);
rho.relax();
T = thermo.T();
T.relax();
Info<< "rho max/min : " << max(rho).value()
    << " " << min(rho).value() << endl;

U = HbyA - rAU*fvc::grad(p);
U.correctBoundaryConditions();
fvOptions.correct(U);

dpdt = fvc::ddt(p);


