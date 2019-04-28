/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "shapeToCell.hpp"
#include "polyMesh.hpp"
#include "unitConversion.hpp"
#include "hexMatcher.hpp"
#include "cellFeatures.hpp"

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(shapeToCell, 0);

addToRunTimeSelectionTable(topoSetSource, shapeToCell, word);

addToRunTimeSelectionTable(topoSetSource, shapeToCell, istream);

}


CML::topoSetSource::addToUsageTable CML::shapeToCell::usage_
(
    shapeToCell::typeName,
    "\n    Usage: shapeToCell tet|pyr|prism|hex|tetWedge|wedge|splitHex\n\n"
    "    Select all cells of given cellShape.\n"
    "    (splitHex hardcoded with internal angle < 10 degrees)\n"
);


// Angle for polys to be considered splitHexes.
CML::scalar CML::shapeToCell::featureCos = CML::cos(degToRad(10.0));


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::shapeToCell::combine(topoSet& set, const bool add) const
{
    if (type_ == "splitHex")
    {
        for (label cellI = 0; cellI < mesh_.nCells(); cellI++)
        {
            cellFeatures superCell(mesh_, featureCos, cellI);

            if (hexMatcher().isA(superCell.faces()))
            {
                addOrDelete(set, cellI, add);
            }
        }
    }
    else
    {
        const cellModel& wantedModel = *(cellModeller::lookup(type_));

        const cellShapeList& cellShapes = mesh_.cellShapes();

        forAll(cellShapes, cellI)
        {
            if (cellShapes[cellI].model() == wantedModel)
            {
                addOrDelete(set, cellI, add);
            }
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::shapeToCell::shapeToCell
(
    const polyMesh& mesh,
    const word& type
)
:
    topoSetSource(mesh),
    type_(type)
{
    if (!cellModeller::lookup(type_) && (type_ != "splitHex"))
    {
        FatalErrorInFunction
            << "Illegal cell type " << type_ << exit(FatalError);
    }
}


// Construct from dictionary
CML::shapeToCell::shapeToCell
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    topoSetSource(mesh),
    type_(dict.lookup("type"))
{
    if (!cellModeller::lookup(type_) && (type_ != "splitHex"))
    {
        FatalErrorInFunction
            << "Illegal cell type " << type_ << exit(FatalError);
    }
}


// Construct from Istream
CML::shapeToCell::shapeToCell
(
    const polyMesh& mesh,
    Istream& is
)
:
    topoSetSource(mesh),
    type_(checkIs(is))
{
    if (!cellModeller::lookup(type_) && (type_ != "splitHex"))
    {
        FatalErrorInFunction
            << "Illegal cell type " << type_ << exit(FatalError);
    }
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::shapeToCell::~shapeToCell()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::shapeToCell::applyToSet
(
    const topoSetSource::setAction action,
    topoSet& set
) const
{
    if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
    {
        Info<< "    Adding all cells of type " << type_ << " ..." << endl;

        combine(set, true);
    }
    else if (action == topoSetSource::DELETE)
    {
        Info<< "    Removing all cells of type " << type_ << " ..." << endl;

        combine(set, false);
    }
}


// ************************************************************************* //
