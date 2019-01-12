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

#include "solidParticleCloud.hpp"
#include "fvMesh.hpp"
#include "volFields.hpp"
#include "interpolationCellPoint.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::solidParticleCloud::solidParticleCloud
(
    const fvMesh& mesh,
    const word& cloudName,
    bool readFields
)
:
    Cloud<solidParticle>(mesh, cloudName, false),
    mesh_(mesh),
    particleProperties_
    (
        IOobject
        (
            "particleProperties",
            mesh_.time().constant(),
            mesh_,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    ),
    rhop_(dimensionedScalar(particleProperties_.lookup("rhop")).value()),
    e_(dimensionedScalar(particleProperties_.lookup("e")).value()),
    mu_(dimensionedScalar(particleProperties_.lookup("mu")).value())
{
    if (readFields)
    {
        solidParticle::readFields(*this);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::solidParticleCloud::move(const dimensionedVector& g)
{
    const volScalarField& rho = mesh_.lookupObject<const volScalarField>("rho");
    const volVectorField& U = mesh_.lookupObject<const volVectorField>("U");
    const volScalarField& nu = mesh_.lookupObject<const volScalarField>("nu");

    interpolationCellPoint<scalar> rhoInterp(rho);
    interpolationCellPoint<vector> UInterp(U);
    interpolationCellPoint<scalar> nuInterp(nu);

    solidParticle::trackingData
        td(*this, rhoInterp, UInterp, nuInterp, g.value());

    Cloud<solidParticle>::move(*this, td, mesh_.time().deltaTValue());
}


// ************************************************************************* //
