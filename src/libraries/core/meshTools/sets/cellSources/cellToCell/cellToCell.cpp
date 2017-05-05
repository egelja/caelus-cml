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

#include "cellToCell.hpp"
#include "polyMesh.hpp"
#include "cellSet.hpp"

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(cellToCell, 0);

addToRunTimeSelectionTable(topoSetSource, cellToCell, word);

addToRunTimeSelectionTable(topoSetSource, cellToCell, istream);

}


CML::topoSetSource::addToUsageTable CML::cellToCell::usage_
(
    cellToCell::typeName,
    "\n    Usage: cellToCell <cellSet>\n\n"
    "    Select all cells in the cellSet\n\n"
);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::cellToCell::cellToCell
(
    const polyMesh& mesh,
    const word& setName
)
:
    topoSetSource(mesh),
    setName_(setName)
{}


// Construct from dictionary
CML::cellToCell::cellToCell
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    topoSetSource(mesh),
    setName_(dict.lookup("set"))
{}


// Construct from Istream
CML::cellToCell::cellToCell
(
    const polyMesh& mesh,
    Istream& is
)
:
    topoSetSource(mesh),
    setName_(checkIs(is))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::cellToCell::~cellToCell()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::cellToCell::applyToSet
(
    const topoSetSource::setAction action,
    topoSet& set
) const
{
    if ((action == topoSetSource::ADD) || (action == topoSetSource::NEW))
    {
        Info<< "    Adding all elements of cellSet " << setName_ << " ..."
            << endl;

        // Load the set
        cellSet loadedSet(mesh_, setName_);

        set.addSet(loadedSet);
    }
    else if (action == topoSetSource::DELETE)
    {
        Info<< "    Removing all elements of cellSet " << setName_ << " ..."
            << endl;

        // Load the set
        cellSet loadedSet(mesh_, setName_);

        set.deleteSet(loadedSet);
    }
}


// ************************************************************************* //
