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

#include "cellToPoint.hpp"
#include "polyMesh.hpp"
#include "cellSet.hpp"

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(cellToPoint, 0);
    addToRunTimeSelectionTable(topoSetSource, cellToPoint, word);
    addToRunTimeSelectionTable(topoSetSource, cellToPoint, istream);

    template<>
    const char* CML::NamedEnum
    <
        CML::cellToPoint::cellAction,
        1
    >::names[] =
    {
        "all"
    };
}


CML::topoSetSource::addToUsageTable CML::cellToPoint::usage_
(
    cellToPoint::typeName,
    "\n    Usage: cellToPoint <cellSet> all\n\n"
    "    Select all points of cells in the cellSet\n\n"
);

const CML::NamedEnum<CML::cellToPoint::cellAction, 1>
    CML::cellToPoint::cellActionNames_;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::cellToPoint::combine(topoSet& set, const bool add) const
{
    // Load the set
    cellSet loadedSet(mesh_, setName_);

    // Add all point from cells in loadedSet
    forAllConstIter(cellSet, loadedSet, iter)
    {
        const label cellI = iter.key();
        const labelList& cFaces = mesh_.cells()[cellI];

        forAll(cFaces, cFaceI)
        {
            const face& f = mesh_.faces()[cFaces[cFaceI]];

            forAll(f, fp)
            {
                addOrDelete(set, f[fp], add);
            }
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::cellToPoint::cellToPoint
(
    const polyMesh& mesh,
    const word& setName,
    const cellAction option
)
:
    topoSetSource(mesh),
    setName_(setName),
    option_(option)
{}


// Construct from dictionary
CML::cellToPoint::cellToPoint
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    topoSetSource(mesh),
    setName_(dict.lookup("set")),
    option_(cellActionNames_.read(dict.lookup("option")))
{}


// Construct from Istream
CML::cellToPoint::cellToPoint
(
    const polyMesh& mesh,
    Istream& is
)
:
    topoSetSource(mesh),
    setName_(checkIs(is)),
    option_(cellActionNames_.read(checkIs(is)))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::cellToPoint::~cellToPoint()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::cellToPoint::applyToSet
(
    const topoSetSource::setAction action,
    topoSet& set
) const
{
    if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
    {
        Info<< "    Adding from " << setName_ << " ..." << endl;

        combine(set, true);
    }
    else if (action == topoSetSource::DELETE)
    {
        Info<< "    Removing from " << setName_ << " ..." << endl;

        combine(set, false);
    }
}


// ************************************************************************* //
