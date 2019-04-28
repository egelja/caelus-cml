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

#include "setToCellZone.hpp"
#include "polyMesh.hpp"
#include "cellZoneSet.hpp"

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(setToCellZone, 0);

addToRunTimeSelectionTable(topoSetSource, setToCellZone, word);

addToRunTimeSelectionTable(topoSetSource, setToCellZone, istream);

}


CML::topoSetSource::addToUsageTable CML::setToCellZone::usage_
(
    setToCellZone::typeName,
    "\n    Usage: setToCellZone <cellSet>\n\n"
    "    Select all cells in the cellSet.\n\n"
);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::setToCellZone::setToCellZone
(
    const polyMesh& mesh,
    const word& setName
)
:
    topoSetSource(mesh),
    setName_(setName)
{}


// Construct from dictionary
CML::setToCellZone::setToCellZone
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    topoSetSource(mesh),
    setName_(dict.lookup("set"))
{}


// Construct from Istream
CML::setToCellZone::setToCellZone
(
    const polyMesh& mesh,
    Istream& is
)
:
    topoSetSource(mesh),
    setName_(checkIs(is))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::setToCellZone::~setToCellZone()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::setToCellZone::applyToSet
(
    const topoSetSource::setAction action,
    topoSet& set
) const
{
    if (!isA<cellZoneSet>(set))
    {
        WarningInFunction
            << "Operation only allowed on a cellZoneSet." << endl;
    }
    else
    {
        cellZoneSet& fzSet = refCast<cellZoneSet>(set);

        if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
        {
            Info<< "    Adding all cells from cellSet " << setName_
                << " ..." << endl;

            // Load the sets
            cellSet fSet(mesh_, setName_);

            // Start off from copy
            DynamicList<label> newAddressing(fzSet.addressing());

            forAllConstIter(cellSet, fSet, iter)
            {
                label cellI = iter.key();

                if (!fzSet.found(cellI))
                {
                    newAddressing.append(cellI);
                }
            }

            fzSet.addressing().transfer(newAddressing);
            fzSet.updateSet();
        }
        else if (action == topoSetSource::DELETE)
        {
            Info<< "    Removing all cells from cellSet " << setName_
                << " ..." << endl;

            // Load the set
            cellSet loadedSet(mesh_, setName_);

            // Start off empty
            DynamicList<label> newAddressing(fzSet.addressing().size());

            forAll(fzSet.addressing(), i)
            {
                if (!loadedSet.found(fzSet.addressing()[i]))
                {
                    newAddressing.append(fzSet.addressing()[i]);
                }
            }
            fzSet.addressing().transfer(newAddressing);
            fzSet.updateSet();
        }
    }
}


// ************************************************************************* //
