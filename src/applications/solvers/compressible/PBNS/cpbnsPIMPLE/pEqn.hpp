volScalarField rAU(1.0/UEqn().A());

rho = thermo->rho();
rho = max(rho, rhoMin);
rho = min(rho, rhoMax);
rho.relax();
thermo->rho() = thermo->rho() - psi*p;
thermo->rho()->correctBoundaryConditions();

U = rAU*UEqn().H();

if (pimple.nCorrPISO() <= 1)
{
    UEqn.clear();
}

#include "compressibleFlux.hpp"

while (pimple.correctNonOrthogonal())
{
    fvScalarMatrix pEqn
    (
        fvm::ddt(psi, p)
      + fvm::div(phid, p)
      - fvm::laplacian(rho*rAU, p)
    );

    pEqn.solve(mesh.solver(p.select(pimple.finalInnerIter())));

    if (pimple.finalNonOrthogonalIter())
    {
        phi == pEqn.flux();
    }
}

thermo->rho() = thermo->rho() + psi*p;
thermo->rho()->correctBoundaryConditions();

#include "rhoEqn.hpp"
#include "compressibleContinuityErrs.hpp"

p.relax();

rho = thermo->rho();
rho = max(rho, rhoMin);
rho = min(rho, rhoMax);
rho.relax();
Info<< "rho max/min : " << max(rho).value()
    << " " << min(rho).value() << endl;

U -= rAU*fvc::grad(p);
U.correctBoundaryConditions();

dpdt = fvc::ddt(p);
