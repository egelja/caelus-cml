/*---------------------------------------------------------------------------*\
Copyright (C) 2015 OpenFOAM Foundation
Copyright (C) 2015 OpenCFD Ltd.
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

#include "decompositionConstraint.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
defineTypeNameAndDebug(decompositionConstraint, 1);
defineRunTimeSelectionTable(decompositionConstraint, dictionary);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::decompositionConstraint::decompositionConstraint
(
    const dictionary& constraintsDict,
    const word& type
)
:
    coeffDict_(constraintsDict)
{}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::decompositionConstraint>
CML::decompositionConstraint::New
(
    const dictionary& dict,
    const word& modelType
)
{
    Info<< "Selecting decompositionConstraint " << modelType << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(modelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown decompositionConstraint type "
            << modelType << nl << nl
            << "Valid decompositionConstraint types:" << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return autoPtr<decompositionConstraint>
    (
        cstrIter()(dict, modelType)
    );
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::decompositionConstraint::~decompositionConstraint()
{}


// ************************************************************************* //
