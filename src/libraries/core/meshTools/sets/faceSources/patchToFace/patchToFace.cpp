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

#include "patchToFace.hpp"
#include "polyMesh.hpp"

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(patchToFace, 0);

addToRunTimeSelectionTable(topoSetSource, patchToFace, word);

addToRunTimeSelectionTable(topoSetSource, patchToFace, istream);

}


CML::topoSetSource::addToUsageTable CML::patchToFace::usage_
(
    patchToFace::typeName,
    "\n    Usage: patchToFace patch\n\n"
    "    Select all faces in the patch. Note:accepts wildcards for patch.\n\n"
);


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::patchToFace::combine(topoSet& set, const bool add) const
{
    bool hasMatched = false;

    forAll(mesh_.boundaryMesh(), patchI)
    {
        const polyPatch& pp = mesh_.boundaryMesh()[patchI];

        if (patchName_.match(pp.name()))
        {
            Info<< "    Found matching patch " << pp.name()
                << " with " << pp.size() << " faces." << endl;

            hasMatched = true;


            for
            (
                label faceI = pp.start();
                faceI < pp.start() + pp.size();
                faceI++
            )
            {
                addOrDelete(set, faceI, add);
            }
        }
    }

    if (!hasMatched)
    {
        WarningInFunction
            << "Cannot find any patch named " << patchName_ << endl
            << "Valid names are " << mesh_.boundaryMesh().names() << endl;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::patchToFace::patchToFace
(
    const polyMesh& mesh,
    const word& patchName
)
:
    topoSetSource(mesh),
    patchName_(patchName)
{}


// Construct from dictionary
CML::patchToFace::patchToFace
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    topoSetSource(mesh),
    patchName_(dict.lookup("name"))
{}


// Construct from Istream
CML::patchToFace::patchToFace
(
    const polyMesh& mesh,
    Istream& is
)
:
    topoSetSource(mesh),
    patchName_(checkIs(is))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::patchToFace::~patchToFace()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::patchToFace::applyToSet
(
    const topoSetSource::setAction action,
    topoSet& set
) const
{
    if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
    {
        Info<< "    Adding all faces of patch " << patchName_ << " ..." << endl;

        combine(set, true);
    }
    else if (action == topoSetSource::DELETE)
    {
        Info<< "    Removing all faces of patch " << patchName_ << " ..."
            << endl;

        combine(set, false);
    }
}


// ************************************************************************* //
