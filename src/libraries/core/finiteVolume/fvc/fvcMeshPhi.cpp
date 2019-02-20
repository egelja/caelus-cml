/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2017 OpenFOAM Foundation
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

#include "fvcMeshPhi.hpp"
#include "fvMesh.hpp"
#include "ddtScheme.hpp"
#include "surfaceInterpolate.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::tmp<CML::surfaceScalarField> CML::fvc::meshPhi
(
    const volVectorField& vf
)
{
    return fv::ddtScheme<vector>::New
    (
        vf.mesh(),
        vf.mesh().ddtScheme("ddt(" + vf.name() + ')')
    )().meshPhi(vf);
}


CML::tmp<CML::surfaceScalarField> CML::fvc::meshPhi
(
    const dimensionedScalar& rho,
    const volVectorField& vf
)
{
    return fv::ddtScheme<vector>::New
    (
        vf.mesh(),
        vf.mesh().ddtScheme("ddt(" + rho.name() + ',' + vf.name() + ')')
    )().meshPhi(vf);
}


CML::tmp<CML::surfaceScalarField> CML::fvc::meshPhi
(
    const volScalarField& rho,
    const volVectorField& vf
)
{
    return fv::ddtScheme<vector>::New
    (
        vf.mesh(),
        vf.mesh().ddtScheme("ddt(" + rho.name() + ',' + vf.name() + ')')
    )().meshPhi(vf);
}


void CML::fvc::makeRelative
(
    surfaceScalarField& phi,
    const volVectorField& U
)
{
    if (phi.mesh().moving())
    {
        phi -= fvc::meshPhi(U);
    }
}

void CML::fvc::makeRelative
(
    surfaceScalarField& phi,
    const dimensionedScalar& rho,
    const volVectorField& U
)
{
    if (phi.mesh().moving())
    {
        phi -= rho*fvc::meshPhi(rho, U);
    }
}

void CML::fvc::makeRelative
(
    surfaceScalarField& phi,
    const volScalarField& rho,
    const volVectorField& U
)
{
    if (phi.mesh().moving())
    {
        phi -= fvc::interpolate(rho)*fvc::meshPhi(rho, U);
    }
}


void CML::fvc::makeAbsolute
(
    surfaceScalarField& phi,
    const volVectorField& U
)
{
    if (phi.mesh().moving())
    {
        phi += fvc::meshPhi(U);
    }
}

void CML::fvc::makeAbsolute
(
    surfaceScalarField& phi,
    const dimensionedScalar& rho,
    const volVectorField& U
)
{
    if (phi.mesh().moving())
    {
        phi += rho*fvc::meshPhi(rho, U);
    }
}

void CML::fvc::makeAbsolute
(
    surfaceScalarField& phi,
    const volScalarField& rho,
    const volVectorField& U
)
{
    if (phi.mesh().moving())
    {
        phi += fvc::interpolate(rho)*fvc::meshPhi(rho, U);
    }
}


CML::tmp<CML::surfaceScalarField> CML::fvc::relative
(
    const tmp<surfaceScalarField>& tphi,
    const volVectorField& U
)
{
    if (tphi().mesh().moving())
    {
        return tphi - fvc::meshPhi(U);
    }
    else
    {
        return tmp<surfaceScalarField>(tphi, true);
    }
}


CML::tmp<CML::surfaceScalarField> CML::fvc::relative
(
    const tmp<surfaceScalarField>& tphi,
    const volScalarField& rho,
    const volVectorField& U
)
{
    if (tphi().mesh().moving())
    {
        return tphi - fvc::interpolate(rho)*fvc::meshPhi(rho, U);
    }
    else
    {
        return tmp<surfaceScalarField>(tphi, true);
    }
}


CML::tmp<CML::surfaceScalarField> CML::fvc::absolute
(
    const tmp<surfaceScalarField>& tphi,
    const volVectorField& U
)
{
    if (tphi().mesh().moving())
    {
        return tphi + fvc::meshPhi(U);
    }
    else
    {
        return tmp<surfaceScalarField>(tphi, true);
    }
}


CML::tmp<CML::surfaceScalarField> CML::fvc::absolute
(
    const tmp<surfaceScalarField>& tphi,
    const volScalarField& rho,
    const volVectorField& U
)
{
    if (tphi().mesh().moving())
    {
        return tphi + fvc::interpolate(rho)*fvc::meshPhi(rho, U);
    }
    else
    {
        return tmp<surfaceScalarField>(tphi, true);
    }
}


void CML::fvc::correctUf
(
    autoPtr<surfaceVectorField>& UfPtr,
    const volVectorField& U,
    const surfaceScalarField& phi
)
{
    const fvMesh& mesh = U.mesh();

    if (mesh.dynamic())
    {
        UfPtr() = fvc::interpolate(U);
        surfaceVectorField n(mesh.Sf()/mesh.magSf());
        UfPtr() += n*(phi/mesh.magSf() - (n & UfPtr()));
    }
}


void CML::fvc::correctRhoUf
(
    autoPtr<surfaceVectorField>& rhoUfPtr,
    const volScalarField& rho,
    const volVectorField& U,
    const surfaceScalarField& phi
)
{
    const fvMesh& mesh = U.mesh();

    if (mesh.dynamic())
    {
        rhoUfPtr() = fvc::interpolate(rho*U);
        surfaceVectorField n(mesh.Sf()/mesh.magSf());
        rhoUfPtr() += n*(fvc::absolute(phi, rho, U)/mesh.magSf() - (n & rhoUfPtr()));
    }
}


// ************************************************************************* //
