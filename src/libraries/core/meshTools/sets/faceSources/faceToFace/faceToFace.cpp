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

#include "faceToFace.hpp"
#include "polyMesh.hpp"
#include "faceSet.hpp"

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(faceToFace, 0);

addToRunTimeSelectionTable(topoSetSource, faceToFace, word);

addToRunTimeSelectionTable(topoSetSource, faceToFace, istream);

}


CML::topoSetSource::addToUsageTable CML::faceToFace::usage_
(
    faceToFace::typeName,
    "\n    Usage: faceToFace <faceSet>\n\n"
    "    Select all faces in the faceSet\n\n"
);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::faceToFace::faceToFace
(
    const polyMesh& mesh,
    const word& setName
)
:
    topoSetSource(mesh),
    setName_(setName)
{}


// Construct from dictionary
CML::faceToFace::faceToFace
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    topoSetSource(mesh),
    setName_(dict.lookup("set"))
{}


// Construct from Istream
CML::faceToFace::faceToFace
(
    const polyMesh& mesh,
    Istream& is
)
:
    topoSetSource(mesh),
    setName_(checkIs(is))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::faceToFace::~faceToFace()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::faceToFace::applyToSet
(
    const topoSetSource::setAction action,
    topoSet& set
) const
{
    if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
    {
        Info<< "    Adding all faces from faceSet " << setName_ << " ..."
            << endl;

        // Load the set
        faceSet loadedSet(mesh_, setName_);

        set.addSet(loadedSet);
    }
    else if (action == topoSetSource::DELETE)
    {
        Info<< "    Removing all faces from faceSet " << setName_ << " ..."
            << endl;

        // Load the set
        faceSet loadedSet(mesh_, setName_);

        set.deleteSet(loadedSet);
    }
}


// ************************************************************************* //
