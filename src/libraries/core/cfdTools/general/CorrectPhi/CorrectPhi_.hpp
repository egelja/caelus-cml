/*---------------------------------------------------------------------------*\
Copyright (C) 2015-2016 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::CorrectPhi

Description
    Flux correction functions to ensure continuity.

    Required during start-up, restart, mesh-motion etc. when non-conservative
    fluxes may adversely affect the prediction-part of the solution algorithm
    (the part before the first pressure solution which would ensure continuity).
    This is particularly important for VoF and other multi-phase solver in
    which non-conservative fluxes cause unboundedness of the phase-fraction.


\*---------------------------------------------------------------------------*/

#ifndef CorrectPhi_HPP
#define CorrectPhi_HPP

#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"

#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "fvScalarMatrix.hpp"
#include "fvmDdt.hpp"
#include "fvmLaplacian.hpp"
#include "fvcDiv.hpp"
#include "fixedValueFvPatchFields.hpp"
#include "zeroGradientFvPatchFields.hpp"
#include "adjustPhi.hpp"
#include "fvcMeshPhi.hpp"
#include "pimpleControl.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    class pimpleControl;

    //- If the mesh is moving correct the velocity BCs on the moving walls to
    //  ensure the corrected fluxes and velocity are consistent
    void correctUphiBCs
    (
        volVectorField& U,
        surfaceScalarField& phi
    );

    //- If the mesh is moving correct the velocity BCs on the moving walls to
    //  ensure the corrected fluxes and velocity are consistent
    void correctUphiBCs
    (
        const volScalarField& rho,
        volVectorField& U,
        surfaceScalarField& phi
    );

    template<class RAUfType, class DivUType>
    void CorrectPhi
    (
        volVectorField& U,
        surfaceScalarField& phi,
        const volScalarField& p,
        const RAUfType& rAUf,
        const DivUType& divU,
        pimpleControl& pimple
    );

    template<class RAUfType, class DivRhoUType>
    void CorrectPhi
    (
        volVectorField& U,
        surfaceScalarField& phi,
        const volScalarField& p,
        const volScalarField& rho,
        const volScalarField& psi,
        const RAUfType& rAUf,
        const DivRhoUType& divRhoU,
        pimpleControl& pimple
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class RAUfType, class DivUType>
void CML::CorrectPhi
(
    volVectorField& U,
    surfaceScalarField& phi,
    const volScalarField& p,
    const RAUfType& rAUf,
    const DivUType& divU,
    pimpleControl& pimple
)
{
    const fvMesh& mesh = U.mesh();
    const Time& runTime = mesh.time();

    correctUphiBCs(U, phi);

    // Initialize BCs list for pcorr to zero-gradient
    wordList pcorrTypes
    (
        p.boundaryField().size(),
        zeroGradientFvPatchScalarField::typeName
    );

    // Set BCs of pcorr to fixed-value for patches at which p is fixed
    forAll(p.boundaryField(), patchi)
    {
        if (p.boundaryField()[patchi].fixesValue())
        {
            pcorrTypes[patchi] = fixedValueFvPatchScalarField::typeName;
        }
    }

    volScalarField pcorr
    (
        IOobject
        (
            "pcorr",
            runTime.timeName(),
            mesh
        ),
        mesh,
        dimensionedScalar("pcorr", p.dimensions(), 0.0),
        pcorrTypes
    );

    if (pcorr.needReference())
    {
        fvc::makeRelative(phi, U);
        adjustPhi(phi, U, pcorr);
        fvc::makeAbsolute(phi, U);
    }

     mesh.setFluxRequired(pcorr.name());

    while (pimple.correctNonOrthogonal())
    {
        // Solve for pcorr such that the divergence of the corrected flux
        // matches the divU provided (from previous iteration, time-step...)
        fvScalarMatrix pcorrEqn
        (
            fvm::laplacian(rAUf, pcorr) == fvc::div(phi) - divU
        );

        pcorrEqn.setReference(0, 0);

        pcorrEqn.solve
        (
            mesh.solver(pcorr.select(pimple.finalNonOrthogonalIter()))
        );

        if (pimple.finalNonOrthogonalIter())
        {
            phi -= pcorrEqn.flux();
        }
    }
}


template<class RAUfType, class DivRhoUType>
void CML::CorrectPhi
(
    volVectorField& U,
    surfaceScalarField& phi,
    const volScalarField& p,
    const volScalarField& rho,
    const volScalarField& psi,
    const RAUfType& rAUf,
    const DivRhoUType& divRhoU,
    pimpleControl& pimple
)
{
    const fvMesh& mesh = U.mesh();
    const Time& runTime = mesh.time();

    correctUphiBCs(rho, U, phi);

    // Initialize BCs list for pcorr to zero-gradient
    wordList pcorrTypes
    (
        p.boundaryField().size(),
        zeroGradientFvPatchScalarField::typeName
    );

    // Set BCs of pcorr to fixed-value for patches at which p is fixed
    forAll(p.boundaryField(), patchi)
    {
        if (p.boundaryField()[patchi].fixesValue())
        {
            pcorrTypes[patchi] = fixedValueFvPatchScalarField::typeName;
        }
    }

    volScalarField pcorr
    (
        IOobject
        (
            "pcorr",
            runTime.timeName(),
            mesh
        ),
        mesh,
        dimensionedScalar("pcorr", p.dimensions(), 0.0),
        pcorrTypes
    );

     mesh.setFluxRequired(pcorr.name());

    while (pimple.correctNonOrthogonal())
    {
        // Solve for pcorr such that the divergence of the corrected flux
        // matches the divRhoU provided (from previous iteration, time-step...)
        fvScalarMatrix pcorrEqn
        (
            fvm::ddt(psi, pcorr)
          + fvc::div(phi)
          - fvm::laplacian(rAUf, pcorr)
         ==
            divRhoU
        );

        pcorrEqn.solve
        (
            mesh.solver(pcorr.select(pimple.finalNonOrthogonalIter()))
        );

        if (pimple.finalNonOrthogonalIter())
        {
            phi += pcorrEqn.flux();
        }
    }
}


#endif

// ************************************************************************* //
