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

#include "setToFaceZone.hpp"
#include "polyMesh.hpp"
#include "faceZoneSet.hpp"

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(setToFaceZone, 0);

addToRunTimeSelectionTable(topoSetSource, setToFaceZone, word);

addToRunTimeSelectionTable(topoSetSource, setToFaceZone, istream);

}


CML::topoSetSource::addToUsageTable CML::setToFaceZone::usage_
(
    setToFaceZone::typeName,
    "\n    Usage: setToFaceZone <faceSet>\n\n"
    "    Select all faces in the faceSet."
    " Sets flipMap.\n\n"
);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::setToFaceZone::setToFaceZone
(
    const polyMesh& mesh,
    const word& setName
)
:
    topoSetSource(mesh),
    setName_(setName)
{}


// Construct from dictionary
CML::setToFaceZone::setToFaceZone
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    topoSetSource(mesh),
    setName_(dict.lookup("faceSet"))
{}


// Construct from Istream
CML::setToFaceZone::setToFaceZone
(
    const polyMesh& mesh,
    Istream& is
)
:
    topoSetSource(mesh),
    setName_(checkIs(is))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::setToFaceZone::~setToFaceZone()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::setToFaceZone::applyToSet
(
    const topoSetSource::setAction action,
    topoSet& set
) const
{
    if (!isA<faceZoneSet>(set))
    {
        WarningInFunction
            << "Operation only allowed on a faceZoneSet." << endl;
    }
    else
    {
        faceZoneSet& fzSet = refCast<faceZoneSet>(set);

        if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
        {
            Info<< "    Adding all faces from faceSet " << setName_
                << " ..." << endl;

            // Load the sets
            faceSet fSet(mesh_, setName_);

            // Start off from copy
            DynamicList<label> newAddressing(fzSet.addressing());
            DynamicList<bool> newFlipMap(fzSet.flipMap());

            forAllConstIter(faceSet, fSet, iter)
            {
                label faceI = iter.key();

                if (!fzSet.found(faceI))
                {
                    newAddressing.append(faceI);
                    newFlipMap.append(false);
                }
            }

            fzSet.addressing().transfer(newAddressing);
            fzSet.flipMap().transfer(newFlipMap);
            fzSet.updateSet();
        }
        else if (action == topoSetSource::DELETE)
        {
            Info<< "    Removing all faces from faceSet " << setName_
                << " ..." << endl;

            // Load the set
            faceSet loadedSet(mesh_, setName_);

            // Start off empty
            DynamicList<label> newAddressing(fzSet.addressing().size());
            DynamicList<bool> newFlipMap(fzSet.flipMap().size());

            forAll(fzSet.addressing(), i)
            {
                if (!loadedSet.found(fzSet.addressing()[i]))
                {
                    newAddressing.append(fzSet.addressing()[i]);
                    newFlipMap.append(fzSet.flipMap()[i]);
                }
            }
            fzSet.addressing().transfer(newAddressing);
            fzSet.flipMap().transfer(newFlipMap);
            fzSet.updateSet();
        }
    }
}


// ************************************************************************* //
