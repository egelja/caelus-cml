volScalarField const R = ( thermo->Cp() - thermo->Cv() )();

rho.boundaryField() = p.boundaryField()/(R.boundaryField()*T.boundaryField());

rho.correctBoundaryConditions();

rhoU.boundaryField() = rho.boundaryField()*U.boundaryField();

h = thermo->Cp()/thermo->Cv()*(rhoE - 0.5*magSqr(rhoU)/rho)/(rho);

h.correctBoundaryConditions();

T.correctBoundaryConditions();

p = rho*R*T;

p.correctBoundaryConditions();

thermo->correct();

rhoE.boundaryField() = rho.boundaryField()
                     * (h.boundaryField() + 0.5*magSqr(U.boundaryField())) 
                     - p.boundaryField();

T = thermo->T();
T.correctBoundaryConditions();
