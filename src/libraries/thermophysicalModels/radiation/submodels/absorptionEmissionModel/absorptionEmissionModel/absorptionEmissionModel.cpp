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

#include "absorptionEmissionModel.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    namespace radiation
    {
        defineTypeNameAndDebug(absorptionEmissionModel, 0);
        defineRunTimeSelectionTable(absorptionEmissionModel, dictionary);
    }
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::radiation::absorptionEmissionModel::absorptionEmissionModel
(
    const dictionary& dict,
    const fvMesh& mesh
)
:
    dict_(dict),
    mesh_(mesh)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::tmp<CML::volScalarField>
CML::radiation::absorptionEmissionModel::a(const label bandI) const
{
    return aDisp(bandI) + aCont(bandI);
}


CML::tmp<CML::volScalarField>
CML::radiation::absorptionEmissionModel::aCont(const label bandI) const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "aCont",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            mesh_,
            dimensionedScalar("zero", dimless/dimLength, 0.0)
        )
    );
}


CML::tmp<CML::volScalarField>
CML::radiation::absorptionEmissionModel::aDisp(const label bandI) const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "aDisp",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            mesh_,
            dimensionedScalar("zero", dimless/dimLength, 0.0)
        )
    );
}


CML::tmp<CML::volScalarField>
CML::radiation::absorptionEmissionModel::e(const label bandI) const
{
    return eDisp(bandI) + eCont(bandI);
}


CML::tmp<CML::volScalarField>
CML::radiation::absorptionEmissionModel::eCont(const label bandI) const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "eCont",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            mesh_,
            dimensionedScalar("zero", dimless/dimLength, 0.0)
        )
    );
}


CML::tmp<CML::volScalarField>
CML::radiation::absorptionEmissionModel::eDisp(const label bandI) const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "eDisp",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            mesh_,
            dimensionedScalar("zero", dimless/dimLength, 0.0)
        )
    );
}


CML::tmp<CML::volScalarField>
CML::radiation::absorptionEmissionModel::E(const label bandI) const
{
    return EDisp(bandI) + ECont(bandI);
}


CML::tmp<CML::volScalarField>
CML::radiation::absorptionEmissionModel::ECont(const label bandI) const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "ECont",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            mesh_,
            dimensionedScalar("zero", dimMass/dimLength/pow3(dimTime), 0.0)
        )
    );
}


CML::tmp<CML::volScalarField>
CML::radiation::absorptionEmissionModel::EDisp(const label bandI) const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "EDisp",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            mesh_,
            dimensionedScalar("zero", dimMass/dimLength/pow3(dimTime), 0.0)
        )
    );
}


CML::label CML::radiation::absorptionEmissionModel::nBands() const
{
    return pTraits<label>::one;
}


const CML::Vector2D<CML::scalar>&
CML::radiation::absorptionEmissionModel::bands(const label n) const
{
    return Vector2D<scalar>::one;
}


bool CML::radiation::absorptionEmissionModel::isGrey() const
{
    return false;
}


void CML::radiation::absorptionEmissionModel::correct
(
    volScalarField& a,
    PtrList<volScalarField>& aj
) const
{
    a = this->a();
    aj[0] =  a;
}
