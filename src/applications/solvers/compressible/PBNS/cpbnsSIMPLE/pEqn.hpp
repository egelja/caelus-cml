rho = thermo->rho();
rho = max(rho, rhoMin);
rho = min(rho, rhoMax);
rho.relax();
volScalarField R = ( thermo->Cp() - thermo->Cv() )();
R.correctBoundaryConditions();
T.relax();
thermo->rho() = thermo->rho() - psi*p;
thermo->rho()->correctBoundaryConditions();

volScalarField rAU(1.0/UEqn().A());
volVectorField UStar("UStar", U);
UStar = rAU*UEqn().H();

UEqn.clear();

#include "compressibleFlux.hpp"

bool closedVolume = false;

while (simple.correctNonOrthogonal())
{
    fvScalarMatrix pEqn
    (
        fvm::div(phid, p) - fvm::laplacian(rho*rAU, p) 
     == fvOptions(psi, p, rho.name())
    );

    pEqn.relax(mesh.equationRelaxationFactor("p"));

    fvOptions.constrain(pEqn);

    pEqn.solve();

    if (simple.finalNonOrthogonalIter())
    {
        phi == pEqn.flux();
    }
}

thermo->rho() = thermo->rho() + psi*p;
thermo->rho()->correctBoundaryConditions();

rho = thermo->rho();
rho = max(rho, rhoMin);
rho = min(rho, rhoMax);
rho.relax();
T.relax();

Info<< "rho max/min : " << max(rho).value()
    << " " << min(rho).value() << endl;

U = UStar - rAU*fvc::grad(p);
U.correctBoundaryConditions();
fvOptions.correct(U);

dimensionedScalar finalMass = fvc::domainIntegrate(psi*p);

if (closedVolume)
{
    p += (initialMass - finalMass)/finalMass;
}





