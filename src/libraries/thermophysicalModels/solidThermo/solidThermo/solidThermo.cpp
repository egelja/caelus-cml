/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "solidThermo.hpp"
#include "fvMesh.hpp"


/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */

namespace CML
{
    defineTypeNameAndDebug(solidThermo, 0);
    defineRunTimeSelectionTable(solidThermo, fvMesh);
    defineRunTimeSelectionTable(solidThermo, dictionary);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::solidThermo::solidThermo
(
    const fvMesh& mesh,
    const word& phaseName
)
:
    basicThermo(mesh, phaseName),
    rho_
    (
        IOobject
        (
            phasePropertyName("thermo:rho"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimDensity
    )
{}


CML::solidThermo::solidThermo
(
    const fvMesh& mesh,
    const dictionary& dict,
    const word& phaseName
)
:
    basicThermo(mesh, dict, phaseName),
    rho_
    (
        IOobject
        (
            phasePropertyName("thermo:rho"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimDensity
    )
{}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::solidThermo> CML::solidThermo::New
(
    const fvMesh& mesh,
    const word& phaseName
)
{
    return basicThermo::New<solidThermo>(mesh, phaseName);
}


CML::autoPtr<CML::solidThermo> CML::solidThermo::New
(
    const fvMesh& mesh,
    const dictionary& dict,
    const word& phaseName
)
{
    return basicThermo::New<solidThermo>(mesh, dict, phaseName);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::tmp<CML::volScalarField> CML::solidThermo::rho() const
{
    return rho_;
}


CML::tmp<CML::scalarField> CML::solidThermo::rho(const label patchi) const
{
    return rho_.boundaryField()[patchi];
}


CML::volScalarField& CML::solidThermo::rho()
{
    return rho_;
}


bool CML::solidThermo::read()
{
    return regIOobject::read();
}
