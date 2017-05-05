/*---------------------------------------------------------------------------*\
Copyright (C) 2014-2017 Applied CCM
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

\*---------------------------------------------------------------------------*/

#include "staticCompression.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(staticCompression, 0);
    addToRunTimeSelectionTable(dynCompressionModel, staticCompression, dictionary);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::staticCompression::staticCompression
(
    const fvMesh& mesh,
    const volVectorField& U,
    const surfaceScalarField& phi,
    const volScalarField& alpha1
)
:
    dynCompressionModel(mesh, U, phi, alpha1),
    cAlphaf_
    (
        IOobject
        (
            "cAlphaf",
            mesh_.time().timeName(),
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh_,
        dimensionedScalar("cAlphaf",dimless, cAlpha0_)
    ),
    cAlpha_
    (
        IOobject
        (
            "cAlpha",
            mesh_.time().timeName(),
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh_,
        dimensionedScalar("cAlphaf",dimless, cAlpha0_)
    )
{
}

// ************************************************************************* //
