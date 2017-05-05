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

#include "boxToPoint.hpp"
#include "polyMesh.hpp"

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(boxToPoint, 0);

addToRunTimeSelectionTable(topoSetSource, boxToPoint, word);

addToRunTimeSelectionTable(topoSetSource, boxToPoint, istream);

}


CML::topoSetSource::addToUsageTable CML::boxToPoint::usage_
(
    boxToPoint::typeName,
    "\n    Usage: boxToPoint ((minx miny minz) (maxx maxy maxz))\n\n"
    "    Select all points with coordinate within bounding box\n\n"
);


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::boxToPoint::combine(topoSet& set, const bool add) const
{
    const pointField& pts = mesh_.points();

    forAll(pts, pointI)
    {
        if (bb_.contains(pts[pointI]))
        {
            addOrDelete(set, pointI, add);
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::boxToPoint::boxToPoint
(
    const polyMesh& mesh,
    const treeBoundBox& bb
)
:
    topoSetSource(mesh),
    bb_(bb)
{}


// Construct from dictionary
CML::boxToPoint::boxToPoint
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    topoSetSource(mesh),
    bb_(dict.lookup("box"))
{}


// Construct from Istream
CML::boxToPoint::boxToPoint
(
    const polyMesh& mesh,
    Istream& is
)
:
    topoSetSource(mesh),
    bb_(checkIs(is))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::boxToPoint::~boxToPoint()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::boxToPoint::applyToSet
(
    const topoSetSource::setAction action,
    topoSet& set
) const
{
    if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
    {
        Info<< "    Adding points that are within box " << bb_ << " ..."
            << endl;

        combine(set, true);
    }
    else if (action == topoSetSource::DELETE)
    {
        Info<< "    Removing points that are within box " << bb_ << " ..."
            << endl;

        combine(set, false);
    }
}


// ************************************************************************* //
