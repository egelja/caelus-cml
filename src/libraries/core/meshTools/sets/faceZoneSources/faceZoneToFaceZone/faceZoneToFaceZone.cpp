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

#include "faceZoneToFaceZone.hpp"
#include "polyMesh.hpp"
#include "faceZoneSet.hpp"

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(faceZoneToFaceZone, 0);

addToRunTimeSelectionTable(topoSetSource, faceZoneToFaceZone, word);

addToRunTimeSelectionTable(topoSetSource, faceZoneToFaceZone, istream);

}


CML::topoSetSource::addToUsageTable CML::faceZoneToFaceZone::usage_
(
    faceZoneToFaceZone::typeName,
    "\n    Usage: faceZoneToFaceZone <faceZone>\n\n"
    "    Select all faces in the faceZone\n\n"
);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::faceZoneToFaceZone::faceZoneToFaceZone
(
    const polyMesh& mesh,
    const word& setName
)
:
    topoSetSource(mesh),
    setName_(setName)
{}


// Construct from dictionary
CML::faceZoneToFaceZone::faceZoneToFaceZone
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    topoSetSource(mesh),
    setName_(dict.lookup("zone"))
{}


// Construct from Istream
CML::faceZoneToFaceZone::faceZoneToFaceZone
(
    const polyMesh& mesh,
    Istream& is
)
:
    topoSetSource(mesh),
    setName_(checkIs(is))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::faceZoneToFaceZone::~faceZoneToFaceZone()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::faceZoneToFaceZone::applyToSet
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
        faceZoneSet& fSet = refCast<faceZoneSet>(set);

        if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
        {
            Info<< "    Adding all faces from faceZone " << setName_ << " ..."
                << endl;

            // Load the set
            faceZoneSet loadedSet(mesh_, setName_);

            DynamicList<label> newAddressing(fSet.addressing());
            DynamicList<bool> newFlipMap(fSet.flipMap());

            forAll(loadedSet.addressing(), i)
            {
                if (!fSet.found(loadedSet.addressing()[i]))
                {
                    newAddressing.append(loadedSet.addressing()[i]);
                    newFlipMap.append(loadedSet.flipMap()[i]);
                }
            }
            fSet.addressing().transfer(newAddressing);
            fSet.flipMap().transfer(newFlipMap);
            fSet.updateSet();
        }
        else if (action == topoSetSource::DELETE)
        {
            Info<< "    Removing all faces from faceZone " << setName_ << " ..."
                << endl;

            // Load the set
            faceZoneSet loadedSet(mesh_, setName_);

            DynamicList<label> newAddressing(fSet.addressing().size());
            DynamicList<bool> newFlipMap(fSet.flipMap().size());

            forAll(fSet.addressing(), i)
            {
                if (!loadedSet.found(fSet.addressing()[i]))
                {
                    newAddressing.append(fSet.addressing()[i]);
                    newFlipMap.append(fSet.flipMap()[i]);
                }
            }
            fSet.addressing().transfer(newAddressing);
            fSet.flipMap().transfer(newFlipMap);
            fSet.updateSet();
        }
    }
}


// ************************************************************************* //
