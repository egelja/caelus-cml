/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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

#include "basicSolidThermo.hpp"
#include "fvMesh.hpp"
#include "HashTable.hpp"

/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */

namespace CML
{
    defineTypeNameAndDebug(basicSolidThermo, 0);
    defineRunTimeSelectionTable(basicSolidThermo, mesh);
    defineRunTimeSelectionTable(basicSolidThermo, dictionary);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::basicSolidThermo::basicSolidThermo(const fvMesh& mesh)
:
    IOdictionary
    (
        IOobject
        (
            "solidThermophysicalProperties",
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
    mesh_(mesh),
    T_
    (
        IOobject
        (
            "T",
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    ),
    rho_
    (
        IOobject
        (
            "rho",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimMass/dimVolume
    ),
    kappa_
    (
        IOobject
        (
            "kappa",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimless/dimLength
    ),
    sigmaS_
    (
        IOobject
        (
            "sigmaS",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimless/dimLength
    ),
    emissivity_
    (
        IOobject
        (
            "emissivity",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimless
    )

{}


CML::basicSolidThermo::basicSolidThermo
(
    const fvMesh& mesh,
    const dictionary& dict
)
:
    IOdictionary
    (
        IOobject
        (
            "solidThermophysicalProperties",
            mesh.time().constant(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        dict
    ),
    mesh_(mesh),
    T_
    (
        IOobject
        (
            "T",
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    ),
    rho_
    (
        IOobject
        (
            "rho",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimMass/dimVolume
    ),
    kappa_
    (
        IOobject
        (
            "kappa",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimless/dimLength
    ),
    sigmaS_
    (
        IOobject
        (
            "sigmaS",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimless/dimLength
    ),
    emissivity_
    (
        IOobject
        (
            "emissivity",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimless
    )
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::basicSolidThermo::~basicSolidThermo()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::volScalarField& CML::basicSolidThermo::T()
{
    return T_;
}


const CML::volScalarField& CML::basicSolidThermo::T() const
{
    return T_;
}


const CML::volScalarField& CML::basicSolidThermo::rho() const
{
    return rho_;
}


CML::volScalarField& CML::basicSolidThermo::rho()
{
    return rho_;
}


const CML::volScalarField& CML::basicSolidThermo::kappa() const
{
    return kappa_;
}


const CML::volScalarField& CML::basicSolidThermo::sigmaS() const
{
    return sigmaS_;
}


const CML::volScalarField& CML::basicSolidThermo::emissivity() const
{
    return emissivity_;
}


CML::basicSolidMixture& CML::basicSolidThermo::composition()
{
    notImplemented("basicSolidThermo::composition()");
    return *reinterpret_cast<basicSolidMixture*>(0);
}


const CML::basicSolidMixture& CML::basicSolidThermo::composition() const
{
    notImplemented("basicSolidThermo::composition() const");
    return *reinterpret_cast<const basicSolidMixture*>(0);
}


CML::tmp<CML::volScalarField> CML::basicSolidThermo::hs() const
{
    notImplemented("basicSolidThermo::hs() const");
    return volScalarField::null();
}


CML::tmp<CML::scalarField> CML::basicSolidThermo::hs(const label patchI)
const
{
    notImplemented("basicSolidThermo::hs(const label) const");
    return scalarField::null();
}


bool CML::basicSolidThermo::read()
{
    return regIOobject::read();
}


bool CML::basicSolidThermo::writeData(Ostream& os) const
{
    return true;
}


// * * * * * * * * * * * * * * IOStream operators  * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const basicSolidThermo& s)
{
    s.writeData(os);
    return os;
}


// ************************************************************************* //
