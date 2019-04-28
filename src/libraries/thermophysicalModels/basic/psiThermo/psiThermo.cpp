/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "psiThermo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(psiThermo, 0);
    defineRunTimeSelectionTable(psiThermo, fvMesh);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::psiThermo::psiThermo(const fvMesh& mesh, const word& phaseName)
:
    fluidThermo(mesh, phaseName),

    psi_
    (
        IOobject
        (
            phasePropertyName("thermo:psi"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionSet(0, -2, 2, 0, 0)
    ),

    mu_
    (
        IOobject
        (
            phasePropertyName("thermo:mu"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionSet(1, -1, -1, 0, 0)
    )
{}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::psiThermo> CML::psiThermo::New
(
    const fvMesh& mesh,
    const word& phaseName
)
{
    return basicThermo::New<psiThermo>(mesh, phaseName);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::tmp<CML::volScalarField> CML::psiThermo::rho() const
{
    return p_*psi_;
}


CML::tmp<CML::scalarField> CML::psiThermo::rho(const label patchi) const
{
    return p_.boundaryField()[patchi]*psi_.boundaryField()[patchi];
}


void CML::psiThermo::correctRho(const CML::volScalarField& deltaRho)
{}


const CML::volScalarField& CML::psiThermo::psi() const
{
    return psi_;
}


const CML::volScalarField& CML::psiThermo::mu() const
{
    return mu_;
}


const CML::scalarField& CML::psiThermo::mu(const label patchi) const
{
    return mu_.boundaryField()[patchi];
}
