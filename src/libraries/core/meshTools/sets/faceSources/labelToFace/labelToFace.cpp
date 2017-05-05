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

#include "labelToFace.hpp"
#include "polyMesh.hpp"

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(labelToFace, 0);

addToRunTimeSelectionTable(topoSetSource, labelToFace, word);

addToRunTimeSelectionTable(topoSetSource, labelToFace, istream);

}


CML::topoSetSource::addToUsageTable CML::labelToFace::usage_
(
    labelToFace::typeName,
    "\n    Usage: labelToFace (i0 i1 .. in)\n\n"
    "    Select faces by label\n\n"
);


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::labelToFace::combine(topoSet& set, const bool add) const
{
    forAll(labels_, labelI)
    {
        addOrDelete(set, labels_[labelI], add);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::labelToFace::labelToFace
(
    const polyMesh& mesh,
    const labelList& labels
)
:
    topoSetSource(mesh),
    labels_(labels)
{}


// Construct from dictionary
CML::labelToFace::labelToFace
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    topoSetSource(mesh),
    labels_(dict.lookup("value"))
{}


// Construct from Istream
CML::labelToFace::labelToFace
(
    const polyMesh& mesh,
    Istream& is
)
:
    topoSetSource(mesh),
    labels_(checkIs(is))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::labelToFace::~labelToFace()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::labelToFace::applyToSet
(
    const topoSetSource::setAction action,
    topoSet& set
) const
{
    if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
    {
        Info<< "    Adding faces mentioned in dictionary" << " ..." << endl;

        combine(set, true);
    }
    else if (action == topoSetSource::DELETE)
    {
        Info<< "    Removing faces mentioned dictionary" << " ..." << endl;

        combine(set, false);
    }
}


// ************************************************************************* //
