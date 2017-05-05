/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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

#include "cloudScatter.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "thermoCloud.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    namespace radiation
    {
        defineTypeNameAndDebug(cloudScatter, 0);

        addToRunTimeSelectionTable
        (
            scatterModel,
            cloudScatter,
            dictionary
        );
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::radiation::cloudScatter::cloudScatter
(
    const dictionary& dict,
    const fvMesh& mesh
)
:
    scatterModel(dict, mesh),
    coeffsDict_(dict.subDict(typeName + "Coeffs")),
    cloudNames_(coeffsDict_.lookup("cloudNames"))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::radiation::cloudScatter::~cloudScatter()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::tmp<CML::volScalarField>
CML::radiation::cloudScatter::sigmaEff() const
{
    tmp<volScalarField> tsigma
    (
        new volScalarField
        (
            IOobject
            (
                "sigma",
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

    forAll(cloudNames_, i)
    {
        const thermoCloud& tc
        (
            mesh_.objectRegistry::lookupObject<thermoCloud>(cloudNames_[i])
        );

        tsigma() += tc.sigmap();
    }

    return 3.0*tsigma;
}


// ************************************************************************* //
