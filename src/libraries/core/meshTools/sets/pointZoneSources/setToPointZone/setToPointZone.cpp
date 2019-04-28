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

#include "setToPointZone.hpp"
#include "polyMesh.hpp"
#include "pointZoneSet.hpp"

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(setToPointZone, 0);

addToRunTimeSelectionTable(topoSetSource, setToPointZone, word);

addToRunTimeSelectionTable(topoSetSource, setToPointZone, istream);

}


CML::topoSetSource::addToUsageTable CML::setToPointZone::usage_
(
    setToPointZone::typeName,
    "\n    Usage: setToPointZone <pointSet>\n\n"
    "    Select all points in the pointSet.\n\n"
);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::setToPointZone::setToPointZone
(
    const polyMesh& mesh,
    const word& setName
)
:
    topoSetSource(mesh),
    setName_(setName)
{}


// Construct from dictionary
CML::setToPointZone::setToPointZone
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    topoSetSource(mesh),
    setName_(dict.lookup("set"))
{}


// Construct from Istream
CML::setToPointZone::setToPointZone
(
    const polyMesh& mesh,
    Istream& is
)
:
    topoSetSource(mesh),
    setName_(checkIs(is))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::setToPointZone::~setToPointZone()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::setToPointZone::applyToSet
(
    const topoSetSource::setAction action,
    topoSet& set
) const
{
    if (!isA<pointZoneSet>(set))
    {
        WarningInFunction
            << "Operation only allowed on a pointZoneSet." << endl;
    }
    else
    {
        pointZoneSet& fzSet = refCast<pointZoneSet>(set);

        if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
        {
            Info<< "    Adding all points from pointSet " << setName_
                << " ..." << endl;

            // Load the sets
            pointSet fSet(mesh_, setName_);

            // Start off from copy
            DynamicList<label> newAddressing(fzSet.addressing());

            forAllConstIter(pointSet, fSet, iter)
            {
                label pointI = iter.key();

                if (!fzSet.found(pointI))
                {
                    newAddressing.append(pointI);
                }
            }

            fzSet.addressing().transfer(newAddressing);
            fzSet.updateSet();
        }
        else if (action == topoSetSource::DELETE)
        {
            Info<< "    Removing all points from pointSet " << setName_
                << " ..." << endl;

            // Load the set
            pointSet loadedSet(mesh_, setName_);

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
