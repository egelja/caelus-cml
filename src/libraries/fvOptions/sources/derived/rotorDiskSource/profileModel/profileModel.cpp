/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2013 OpenFOAM Foundation
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

#include "profileModel.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(profileModel, 0);
    defineRunTimeSelectionTable(profileModel, dictionary);
}


// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

bool CML::profileModel::readFromFile() const
{
    return fName_ != fileName::null;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::profileModel::profileModel(const dictionary& dict, const word& name)
:
    dict_(dict),
    name_(name),
    fName_(fileName::null)
{
    dict.readIfPresent("fileName", fName_);
}

// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

CML::profileModel::~profileModel()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::word& CML::profileModel::name() const
{
    return name_;
}


CML::autoPtr<CML::profileModel> CML::profileModel::New
(
    const dictionary& dict
)
{
    const word& modelName(dict.dictName());

    const word modelType(dict.lookup("type"));

    Info<< "    - creating " << modelType << " profile " << modelName << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(modelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown profile model type " << modelType
            << nl << nl
            << "Valid model types are :" << nl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<profileModel>(cstrIter()(dict, modelName));
}


// ************************************************************************* //
