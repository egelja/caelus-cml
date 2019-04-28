/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2015 OpenFOAM Foundation
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

#include "interpolationWeights.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(interpolationWeights, 0);
defineRunTimeSelectionTable(interpolationWeights, word);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

interpolationWeights::interpolationWeights
(
    const scalarField& samples
)
:
    samples_(samples)
{}


// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

autoPtr<interpolationWeights> interpolationWeights::New
(
    const word& type,
    const scalarField& samples
)
{
    if (debug)
    {
        InfoInFunction
            << "Selecting interpolationWeights "
            << type << endl;
    }

    wordConstructorTable::iterator cstrIter =
        wordConstructorTablePtr_->find(type);

    if (cstrIter == wordConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown interpolationWeights type "
            << type
            << endl << endl
            << "Valid interpolationWeights types are :" << endl
            << wordConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<interpolationWeights>(cstrIter()(samples));
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

interpolationWeights::~interpolationWeights()
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
