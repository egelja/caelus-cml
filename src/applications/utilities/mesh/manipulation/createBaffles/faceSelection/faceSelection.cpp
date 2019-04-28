/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2018 OpenFOAM Foundation
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

#include "faceSelection.hpp"
#include "fvMesh.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(faceSelection, 0);
    defineRunTimeSelectionTable(faceSelection, dictionary);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::faceSelection::faceSelection
(
    const word& name,
    const fvMesh& mesh,
    const dictionary& dict
)
:
    name_(name),
    mesh_(mesh),
    dict_(dict),
    flip_(dict.lookupOrDefault("flip", false))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::faceSelection::~faceSelection()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::autoPtr<CML::faceSelection> CML::faceSelection::New
(
    const word& name,
    const fvMesh& mesh,
    const dictionary& dict
)
{
    const word sampleType(dict.lookup("type"));

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(sampleType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown faceSelection type "
            << sampleType << nl << nl
            << "Valid faceSelection types : " << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<faceSelection>(cstrIter()(name, mesh, dict));
}


void CML::faceSelection::select
(
    const label zoneID,
    labelList& faceToZoneID,
    boolList& faceToFlip
) const
{
    if (flip_)
    {
        forAll(faceToZoneID, facei)
        {
            if (faceToZoneID[facei] == zoneID)
            {
                faceToFlip[facei] = !faceToFlip[facei];
            }
        }
    }
}


// ************************************************************************* //
