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

#include "basicChemistryModel.hpp"
#include "fvMesh.hpp"
#include "Time.hpp"

/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */

namespace CML
{
    defineTypeNameAndDebug(basicChemistryModel, 0);
}

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

void CML::basicChemistryModel::correct()
{
    // do nothing
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::basicChemistryModel::basicChemistryModel(const fvMesh& mesh)
:
    IOdictionary
    (
        IOobject
        (
            "chemistryProperties",
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    ),
    mesh_(mesh),
    chemistry_(lookup("chemistry")),
    deltaTChemIni_(readScalar(lookup("initialChemicalTimeStep"))),
    deltaTChem_
    (
        IOobject
        (
            "deltaTChem",
            mesh.time().constant(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("deltaTChem0", dimTime, deltaTChemIni_)
    )
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::basicChemistryModel::~basicChemistryModel()
{}


// ************************************************************************* //
