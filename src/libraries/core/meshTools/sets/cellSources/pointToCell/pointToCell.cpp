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

#include "pointToCell.hpp"
#include "polyMesh.hpp"
#include "pointSet.hpp"

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(pointToCell, 0);
    addToRunTimeSelectionTable(topoSetSource, pointToCell, word);
    addToRunTimeSelectionTable(topoSetSource, pointToCell, istream);

    template<>
    const char* CML::NamedEnum
    <
        CML::pointToCell::pointAction,
        1
    >::names[] =
    {
        "any"
    };
}


CML::topoSetSource::addToUsageTable CML::pointToCell::usage_
(
    pointToCell::typeName,
    "\n    Usage: pointToCell <pointSet> any\n\n"
    "    Select all cells with any point in the pointSet\n\n"
);

const CML::NamedEnum<CML::pointToCell::pointAction, 1>
    CML::pointToCell::pointActionNames_;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::pointToCell::combine(topoSet& set, const bool add) const
{
    // Load the set
    pointSet loadedSet(mesh_, setName_);


    // Handle any selection
    if (option_ == ANY)
    {
        forAllConstIter(pointSet, loadedSet, iter)
        {
            const label pointI = iter.key();
            const labelList& pCells = mesh_.pointCells()[pointI];

            forAll(pCells, pCellI)
            {
                addOrDelete(set, pCells[pCellI], add);
            }
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::pointToCell::pointToCell
(
    const polyMesh& mesh,
    const word& setName,
    const pointAction option
)
:
    topoSetSource(mesh),
    setName_(setName),
    option_(option)
{}


// Construct from dictionary
CML::pointToCell::pointToCell
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    topoSetSource(mesh),
    setName_(dict.lookup("set")),
    option_(pointActionNames_.read(dict.lookup("option")))
{}


// Construct from Istream
CML::pointToCell::pointToCell
(
    const polyMesh& mesh,
    Istream& is
)
:
    topoSetSource(mesh),
    setName_(checkIs(is)),
    option_(pointActionNames_.read(checkIs(is)))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::pointToCell::~pointToCell()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::pointToCell::applyToSet
(
    const topoSetSource::setAction action,
    topoSet& set
) const
{
    if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
    {
        Info<< "    Adding cells according to pointSet " << setName_
            << " ..." << endl;

        combine(set, true);
    }
    else if (action == topoSetSource::DELETE)
    {
        Info<< "    Removing cells according to pointSet " << setName_
            << " ..." << endl;

        combine(set, false);
    }
}


// ************************************************************************* //
