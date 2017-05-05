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

#include "labelToCell.hpp"
#include "polyMesh.hpp"

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(labelToCell, 0);

addToRunTimeSelectionTable(topoSetSource, labelToCell, word);

addToRunTimeSelectionTable(topoSetSource, labelToCell, istream);

}


CML::topoSetSource::addToUsageTable CML::labelToCell::usage_
(
    labelToCell::typeName,
    "\n    Usage: labelToCell (i0 i1 .. in)\n\n"
    "    Select cells by cellLabel\n\n"
);


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::labelToCell::combine(topoSet& set, const bool add) const
{
    forAll(labels_, labelI)
    {
        addOrDelete(set, labels_[labelI], add);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::labelToCell::labelToCell
(
    const polyMesh& mesh,
    const labelList& labels
)
:
    topoSetSource(mesh),
    labels_(labels)
{}


// Construct from dictionary
CML::labelToCell::labelToCell
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    topoSetSource(mesh),
    labels_(dict.lookup("value"))
{}


// Construct from Istream
CML::labelToCell::labelToCell
(
    const polyMesh& mesh,
    Istream& is
)
:
    topoSetSource(mesh),
    labels_(checkIs(is))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::labelToCell::~labelToCell()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::labelToCell::applyToSet
(
    const topoSetSource::setAction action,
    topoSet& set
) const
{
    if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
    {
        Info<< "    Adding cells mentioned in dictionary" << " ..." << endl;

        combine(set, true);
    }
    else if (action == topoSetSource::DELETE)
    {
        Info<< "    Removing cells mentioned in dictionary" << " ..." << endl;

        combine(set, false);
    }
}


// ************************************************************************* //
