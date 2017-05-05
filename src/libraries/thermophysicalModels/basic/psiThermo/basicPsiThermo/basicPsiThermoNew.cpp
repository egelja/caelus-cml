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

#include "basicPsiThermo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::basicPsiThermo> CML::basicPsiThermo::New
(
    const fvMesh& mesh
)
{
    // get model name, but do not register the dictionary
    // otherwise it is registered in the database twice
    const word modelType
    (
        IOdictionary
        (
            IOobject
            (
                "thermophysicalProperties",
                mesh.time().constant(),
                mesh,
                IOobject::MUST_READ_IF_MODIFIED,
                IOobject::NO_WRITE,
                false
            )
        ).lookup("thermoType")
    );

    Info<< "Selecting thermodynamics package " << modelType << endl;

    fvMeshConstructorTable::iterator cstrIter =
        fvMeshConstructorTablePtr_->find(modelType);

    if (cstrIter == fvMeshConstructorTablePtr_->end())
    {
        FatalErrorIn("basicPsiThermo::New(const fvMesh&)")
            << "Unknown basicPsiThermo type " << modelType << nl << nl
            << "Valid basicPsiThermo types are:" << nl
            << fvMeshConstructorTablePtr_->sortedToc() << nl
            << exit(FatalError);
    }

    return autoPtr<basicPsiThermo>(cstrIter()(mesh));
}


// ************************************************************************* //
