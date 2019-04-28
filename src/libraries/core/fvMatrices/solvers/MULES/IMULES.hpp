/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2016 OpenFOAM Foundation
Copyright (C) 2016 OpenCFD Ltd
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

Global
    IMULES

Description
    IMULES: Multidimensional universal limiter for implicit solution.

    Solve a convective-only transport equation using an explicit universal
    multi-dimensional limiter applied to an implicit formulation requiring
    iteration to guarantee boundedness.  The number of iterations required
    to obtain boundedness increases with the Courant number of the simulation.

    It may be more efficient to use CMULES.

SourceFiles
    IMULES.cpp

\*---------------------------------------------------------------------------*/

#ifndef IMULES_HPP
#define IMULES_HPP

#include "MULES.hpp"
#include "profiling.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace MULES
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class RhoType, class SpType, class SuType>
void implicitSolve
(
    const RhoType& rho,
    volScalarField& gamma,
    const surfaceScalarField& phi,
    surfaceScalarField& phiCorr,
    const SpType& Sp,
    const SuType& Su,
    const scalar psiMax,
    const scalar psiMin
);

void implicitSolve
(
    volScalarField& gamma,
    const surfaceScalarField& phi,
    surfaceScalarField& phiCorr,
    const scalar psiMax,
    const scalar psiMin
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace MULES
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "gaussConvectionScheme.hpp"
#include "surfaceInterpolate.hpp"
#include "fvmDdt.hpp"
#include "fvmSup.hpp"
#include "fvcDiv.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace MULES
{
    template<class RhoType>
    inline tmp<surfaceScalarField> interpolate(const RhoType& rho)
    {
        NotImplemented;
        return tmp<surfaceScalarField>(nullptr);
    }

    template<>
    inline tmp<surfaceScalarField> interpolate(const volScalarField& rho)
    {
        return fvc::interpolate(rho);
    }
}
}


template<class RhoType, class SpType, class SuType>
void CML::MULES::implicitSolve
(
    const RhoType& rho,
    volScalarField& psi,
    const surfaceScalarField& phi,
    surfaceScalarField& phiPsi,
    const SpType& Sp,
    const SuType& Su,
    const scalar psiMax,
    const scalar psiMin
)
{
    addProfiling(solve, "MULES::implicitSolve");

    const fvMesh& mesh = psi.mesh();

    const dictionary& MULEScontrols = mesh.solverDict(psi.name());

    label maxIter
    (
        readLabel(MULEScontrols.lookup("maxIter"))
    );

    scalar maxUnboundedness
    (
        readScalar(MULEScontrols.lookup("maxUnboundedness"))
    );

    scalar CoCoeff
    (
        readScalar(MULEScontrols.lookup("CoCoeff"))
    );

    scalarField allCoLambda(mesh.nFaces());

    {
        slicedSurfaceScalarField CoLambda
        (
            IOobject
            (
                "CoLambda",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            mesh,
            dimless,
            allCoLambda,
            false   // Use slices for the couples
        );

        if (phi.dimensions() == dimDensity*dimVelocity*dimArea)
        {
            tmp<surfaceScalarField> Cof =
                mesh.time().deltaT()*mesh.surfaceInterpolation::deltaCoeffs()
               *mag(phi/interpolate(rho))/mesh.magSf();

            CoLambda == 1.0/max(CoCoeff*Cof, scalar(1));
        }
        else
        {
            tmp<surfaceScalarField> Cof =
                mesh.time().deltaT()*mesh.surfaceInterpolation::deltaCoeffs()
               *mag(phi)/mesh.magSf();

            CoLambda == 1.0/max(CoCoeff*Cof, scalar(1));
        }
    }

    scalarField allLambda(allCoLambda);

    slicedSurfaceScalarField lambda
    (
        IOobject
        (
            "lambda",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        ),
        mesh,
        dimless,
        allLambda,
        false   // Use slices for the couples
    );

    linear<scalar> CDs(mesh);
    upwind<scalar> UDs(mesh, phi);

    fvScalarMatrix psiConvectionDiffusion
    (
        fvm::ddt(rho, psi)
      + fv::gaussConvectionScheme<scalar>(mesh, phi, UDs).fvmDiv(phi, psi)
        //- fv::gaussLaplacianScheme<scalar, scalar>(mesh, CDs, snGrads)
        //.fvmLaplacian(Dpsif, psi)
      - fvm::Sp(Sp, psi)
      - Su
    );

    surfaceScalarField phiBD(psiConvectionDiffusion.flux());

    surfaceScalarField& phiCorr = phiPsi;
    phiCorr -= phiBD;

    for (label i=0; i<maxIter; i++)
    {
        if (i != 0 && i < 4)
        {
            allLambda = allCoLambda;
        }

        limiter
        (
            allLambda,
            1.0/mesh.time().deltaTValue(),
            rho,
            psi,
            phiBD,
            phiCorr,
            Sp,
            Su,
            psiMax,
            psiMin
        );

        solve
        (
            psiConvectionDiffusion + fvc::div(lambda*phiCorr),
            MULEScontrols
        );

        scalar maxPsiM1 = gMax(psi.internalField()) - 1.0;
        scalar minPsi = gMin(psi.internalField());

        scalar unboundedness = max(max(maxPsiM1, 0.0), -min(minPsi, 0.0));

        if (unboundedness < maxUnboundedness)
        {
            break;
        }
        else
        {
            Info<< "MULES: max(" << psi.name() << " - 1) = " << maxPsiM1
                << " min(" << psi.name() << ") = " << minPsi << endl;

            phiBD = psiConvectionDiffusion.flux();
        }
    }

    phiPsi = psiConvectionDiffusion.flux() + lambda*phiCorr;
}

#endif

// ************************************************************************* //
