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

#include "isotropicKSolidThermo.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(isotropicKSolidThermo, 0);
    addToRunTimeSelectionTable
    (
        basicSolidThermo,
        isotropicKSolidThermo,
        mesh
    );

    addToRunTimeSelectionTable
    (
        basicSolidThermo,
        isotropicKSolidThermo,
        dictionary
    );

}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::isotropicKSolidThermo::isotropicKSolidThermo
(
    const fvMesh& mesh,
    const dictionary& dict
)
:
    interpolatedSolidThermo(mesh, typeName + "Coeffs", dict),
    K_
    (
        IOobject
        (
            "K",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimTime/(dimLength*dimTemperature)
    ),
    KValues_ (Field<scalar>(subDict(typeName + "Coeffs").lookup("KValues")))
{
    correct();
}


CML::isotropicKSolidThermo::isotropicKSolidThermo(const fvMesh& mesh)
:
    interpolatedSolidThermo(mesh, typeName + "Coeffs"),
    K_
    (
        IOobject
        (
            "K",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimTime/(dimLength*dimTemperature)
    ),
    KValues_ (Field<scalar>(subDict(typeName + "Coeffs").lookup("KValues")))
{
    correct();
}


void CML::isotropicKSolidThermo::correct()
{
    // Correct K
    K_.internalField() = interpolateXY
    (
        T_.internalField(),
        TValues_,
        KValues_
    );

    forAll(K_.boundaryField(), patchI)
    {
        K_.boundaryField()[patchI] == interpolateXY
        (
            T_.boundaryField()[patchI],
            TValues_,
            KValues_
        );
    }

    interpolatedSolidThermo::calculate();
}


CML::tmp<CML::volScalarField> CML::isotropicKSolidThermo::K() const
{
    return K_;
}


CML::tmp<CML::scalarField> CML::isotropicKSolidThermo::K
(
    const label patchI
) const
{
    return K_.boundaryField()[patchI];
}


bool CML::isotropicKSolidThermo::read()
{
    KValues_  = Field<scalar>(subDict(typeName + "Coeffs").lookup("KValues"));
    return true;
}


bool CML::isotropicKSolidThermo::writeData(Ostream& os) const
{
    os.writeKeyword("KValues") << KValues_ << token::END_STATEMENT << nl;
    bool ok = interpolatedSolidThermo::writeData(os);

    return ok && os.good();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::isotropicKSolidThermo::~isotropicKSolidThermo()
{}


// ************************************************************************* //
