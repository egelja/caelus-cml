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

#include "hCombustionThermo.hpp"
#include "fvMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::hCombustionThermo> CML::hCombustionThermo::New
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
        FatalErrorIn("hCombustionThermo::New(const fvMesh&)")
            << "Unknown hCombustionThermo type "
            << modelType << nl << nl
            << "Valid hCombustionThermo types are:" << nl
            << fvMeshConstructorTablePtr_->sortedToc() << nl
            << exit(FatalError);
    }

    return autoPtr<hCombustionThermo>(cstrIter()(mesh));
}


CML::autoPtr<CML::hCombustionThermo> CML::hCombustionThermo::NewType
(
    const fvMesh& mesh,
    const word& thermoType
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


    if (modelType.find(thermoType) == string::npos)
    {
        wordList allModels = fvMeshConstructorTablePtr_->sortedToc();
        DynamicList<word> validModels;
        forAll(allModels, i)
        {
            if (allModels[i].find(thermoType) != string::npos)
            {
                validModels.append(allModels[i]);
            }
        }

        FatalErrorIn
        (
            "autoPtr<hCombustionThermo> hCombustionThermo::NewType"
            "("
                "const fvMesh&, "
                "const word&"
            ")"
        )   << "Inconsistent thermo package selected:" << nl << nl
            << modelType << nl << nl << "Please select a "
            << "thermo package based on " << thermoType
            << ". Valid options include:" << nl << validModels << nl
            << exit(FatalError);
    }

    Info<< "Selecting thermodynamics package " << modelType << endl;

    fvMeshConstructorTable::iterator cstrIter =
        fvMeshConstructorTablePtr_->find(modelType);

    if (cstrIter == fvMeshConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "autoPtr<hCombustionThermo> hCombustionThermo::NewType"
            "("
                "const fvMesh&, "
                "const word&"
            ")"
        )   << "Unknown hCombustionThermo type "
            << modelType << nl << nl
            << "Valid hCombustionThermo types are:" << nl
            << fvMeshConstructorTablePtr_->sortedToc() << nl
            << exit(FatalError);
    }

    return autoPtr<hCombustionThermo>(cstrIter()(mesh));
}


// ************************************************************************* //
