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

#include "pyrolysisModel.hpp"
#include "fvMesh.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace pyrolysisModels
{

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

autoPtr<pyrolysisModel> pyrolysisModel::New(const fvMesh& mesh)
{
    // get model name, but do not register the dictionary
    const word modelType
    (
        IOdictionary
        (
            IOobject
            (
                "pyrolysisProperties",
                mesh.time().constant(),
                mesh,
                IOobject::MUST_READ,
                IOobject::NO_WRITE,
                false
            )
        ).lookup("pyrolysisModel")
    );

    Info<< "Selecting pyrolysisModel " << modelType << endl;

    meshConstructorTable::iterator cstrIter =
        meshConstructorTablePtr_->find(modelType);

    if (cstrIter == meshConstructorTablePtr_->end())
    {
        FatalErrorIn("pyrolysisModel::New(const fvMesh&)")
            << "Unknown pyrolysisModel type " << modelType
            << nl << nl << "Valid pyrolisisModel types are:" << nl
            << meshConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<pyrolysisModel>(cstrIter()(modelType, mesh));
}


autoPtr<pyrolysisModel> pyrolysisModel::New
(
    const fvMesh& mesh,
    const dictionary& dict
)
{

    const word modelType = dict.lookup("pyrolysisModel");

    Info<< "Selecting pyrolysisModel " << modelType << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(modelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorIn("pyrolysisModel::New(const fvMesh&, const dictionary&)")
            << "Unknown pyrolysisModel type " << modelType
            << nl << nl << "Valid pyrolisisModel types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<pyrolysisModel>(cstrIter()(modelType, mesh, dict));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // End namespace regionModels
} // End namespace CML

// ************************************************************************* //
