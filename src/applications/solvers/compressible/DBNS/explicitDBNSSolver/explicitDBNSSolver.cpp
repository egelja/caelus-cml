/*---------------------------------------------------------------------------*\
Copyright (C) 2015 - 2016 Applied CCM
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS.  If not, see <http://www.gnu.org/licenses/>.

Application

    explicitDBNSSolver

Description

    Explicit density-based Navier-Stokes solver for high speed flows. The
    solution algorithm is based on reconstruction-average-evolution (RAE)
    algorithm that utulizes a Rimeann solver for the reconstruction part
    of the algorithm. No flux preconditioning is used so the solver is
    strictly applicable only to high speed flows. Evolution part of the 
    algorithm uses low-storage Runge-Kutta mutistage scheme while the
    averaging is performed through a projection on C^0 function space
    used in cell-centered finite vlumes.

References

  [1] "Density Based Navier-Stokes Solver for Transonic Flows", O. Borm,
      A. Jemcov, and H.-P. Kau, 6th OpenFOAM Workshop, PennState University,
      USA, June 14, 2011

Author

    Aleksandar Jemcov



\*---------------------------------------------------------------------------*/
#include "fvCFD.hpp"
#include "psiThermo.hpp"
#include "compressibleTurbulenceModel.hpp"
#include "bound.hpp"
#include "numericFlux.hpp"
#include "mdLimiter.hpp"
#include "dbRegistrator.hpp"
#include "riemannSolver.hpp"


int main(int argc, char *argv[])
{

#   include "setRootCase.hpp"
#   include "createTime.hpp"
#   include "createMesh.hpp"
#   include "createTimeControls.hpp"
#   include "createFields.hpp"
#   include "createNumericFlux.hpp"

    lduMatrix::debug = 0;

    Info<< "\nStarting time loop\n" << endl;

    while (runTime.run())
    {
#       include "cfl.hpp"
#       include "readTimeControls.hpp"
#       include "setDeltaT.hpp"

        runTime++;

        Info<< "\n Time = " << runTime.value() << nl << endl;

        // Update primitive boundaries
        U.correctBoundaryConditions();
        T.correctBoundaryConditions();
        p.correctBoundaryConditions();

        // Runge-Kutta multistage time loop
        forAll (beta, i)
        {
            Info << "Runge-Kutta stage " << i + 1 << endl;
            riemannSolver->update();
            volScalarField rhoResidual  = 
                (fvc::div(riemannSolver->rhoFlux()))();
            volVectorField rhoUResidual = 
                (fvc::div(riemannSolver->rhoUFlux()))();
            volScalarField rhoEResidual = 
                (fvc::div(riemannSolver->rhoEFlux()))();

            solve
            (
                fvm::ddt(1.0/beta[i], rho) == - rhoResidual
            );

            scalar const  rhoL2 =
                CML::sqrt(gSum(sqr(rhoResidual.internalField())))
                /mesh.nCells();

            Info << "rho residual: "<< rhoL2 << "    ";

            rhoUResidual -= 
                fvc::div(turbulence->muEff()*dev2(CML::T(fvc::grad(U))));
            rhoUResidual -= fvc::laplacian(turbulence->muEff(),U);
            scalar const c = scalar(2.0/3.0);
            rhoUResidual -= fvc::div(c*I*rho*turbulence->k());

            solve
            (
                fvm::ddt(1.0/beta[i], rhoU) == -rhoUResidual
            );

            U.dimensionedInternalField() = rhoU.dimensionedInternalField()
              / rho.dimensionedInternalField();
            U.correctBoundaryConditions();

            scalar const rhoUL2 =
                CML::sqrt(gSum(magSqr(rhoUResidual.internalField())))
                /mesh.nCells();

            Info << "rhoU residual: "<< rhoUL2 << "    ";

            volScalarField k = (thermo.Cp()*turbulence->alphaEff())();
            k.correctBoundaryConditions();
            rhoEResidual -= fvc::laplacian(k,T);
            rhoEResidual -= 
                fvc::div(turbulence->muEff()*(fvc::grad(U))&U);
            rhoEResidual -=  
                fvc::div(turbulence->muEff()*dev2(CML::T(fvc::grad(U)))&U);
            scalar const sigmaK = scalar(0.6);
            volScalarField muk = (turbulence->mu()+sigmaK*turbulence->mut())();
            rhoEResidual -= fvc::laplacian(muk,turbulence->k());
            rhoEResidual -= fvc::div((c*I*rho*turbulence->k())&U);

            solve
            (
                fvm::ddt(1.0/beta[i], rhoE) == - rhoEResidual
            );

            scalar const rhoEL2 =
                CML::sqrt(gSum(sqr(rhoEResidual.internalField())))
                /mesh.nCells();

            Info << "rhoE residual: "<< rhoEL2 << endl;            

#           include "updateFields.hpp"

        }

        phi = riemannSolver->rhoFlux();

        turbulence->correct();

        runTime.write();

        #include "reportTimeStats.hpp"

    }

    Info<< "\n end \n";

    return(0);
}

