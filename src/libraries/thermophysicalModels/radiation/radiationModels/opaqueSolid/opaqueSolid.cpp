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

#include "opaqueSolid.hpp"
#include "physicoChemicalConstants.hpp"
#include "fvMesh.hpp"
#include "Time.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    namespace radiation
    {
        defineTypeNameAndDebug(opaqueSolid, 0);

        addToRadiationRunTimeSelectionTables(opaqueSolid);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::radiation::opaqueSolid::opaqueSolid(const volScalarField& T)
:
    radiationModel(typeName, T)
{}


CML::radiation::opaqueSolid::opaqueSolid
(
    const dictionary& dict,
    const volScalarField& T
)
:
    radiationModel(typeName, dict, T)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::radiation::opaqueSolid::read()
{
    return radiationModel::read();
}


void CML::radiation::opaqueSolid::calculate()
{}


CML::tmp<CML::volScalarField> CML::radiation::opaqueSolid::Rp() const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "Rp",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedScalar
            (
                "Rp",
                constant::physicoChemical::sigma.dimensions()/dimLength,
                0.0
            )
        )
    );
}


CML::tmp<CML::DimensionedField<CML::scalar, CML::volMesh> >
CML::radiation::opaqueSolid::Ru() const
{
    return tmp<CML::DimensionedField<CML::scalar, CML::volMesh> >
    (
        new DimensionedField<scalar, volMesh>
        (
            IOobject
            (
                "Ru",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedScalar
            (
                "Ru", dimMass/dimLength/pow3(dimTime), 0.0
            )
        )
    );
}
