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

#include "boundaryToFace.hpp"
#include "polyMesh.hpp"

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(boundaryToFace, 0);

addToRunTimeSelectionTable(topoSetSource, boundaryToFace, word);

addToRunTimeSelectionTable(topoSetSource, boundaryToFace, istream);

}


CML::topoSetSource::addToUsageTable CML::boundaryToFace::usage_
(
    boundaryToFace::typeName,
    "\n    Usage: boundaryToFace\n\n"
    "    Select all boundary faces\n\n"
);


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::boundaryToFace::combine(topoSet& set, const bool add) const
{
    for
    (
        label faceI = mesh().nInternalFaces();
        faceI < mesh().nFaces();
        faceI++
    )
    {
        addOrDelete(set, faceI, add);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::boundaryToFace::boundaryToFace(const polyMesh& mesh)
:
    topoSetSource(mesh)
{}


// Construct from dictionary
CML::boundaryToFace::boundaryToFace(const polyMesh& mesh, const dictionary&)
:
    topoSetSource(mesh)
{}


// Construct from Istream
CML::boundaryToFace::boundaryToFace
(
    const polyMesh& mesh,
    Istream& is
)
:
    topoSetSource(mesh)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::boundaryToFace::~boundaryToFace()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::boundaryToFace::applyToSet
(
    const topoSetSource::setAction action,
    topoSet& set
) const
{
    if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
    {
        Info<< "    Adding all boundary faces ..." << endl;

        combine(set, true);
    }
    else if (action == topoSetSource::DELETE)
    {
        Info<< "    Removing all boundary faces ..." << endl;

        combine(set, false);
    }
}


// ************************************************************************* //
