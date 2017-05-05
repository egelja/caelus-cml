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

#include "faceToPoint.hpp"
#include "polyMesh.hpp"
#include "faceSet.hpp"

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(faceToPoint, 0);
    addToRunTimeSelectionTable(topoSetSource, faceToPoint, word);
    addToRunTimeSelectionTable(topoSetSource, faceToPoint, istream);

    template<>
    const char* CML::NamedEnum
    <
        CML::faceToPoint::faceAction,
        1
    >::names[] =
    {
        "all"
    };
}


CML::topoSetSource::addToUsageTable CML::faceToPoint::usage_
(
    faceToPoint::typeName,
    "\n    Usage: faceToPoint <faceSet> all\n\n"
    "    Select all points of faces in the faceSet\n\n"
);

const CML::NamedEnum<CML::faceToPoint::faceAction, 1>
    CML::faceToPoint::faceActionNames_;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::faceToPoint::combine(topoSet& set, const bool add) const
{
    // Load the set
    faceSet loadedSet(mesh_, setName_);

    // Add all points from faces in loadedSet
    forAllConstIter(faceSet, loadedSet, iter)
    {
        const face& f = mesh_.faces()[iter.key()];

        forAll(f, fp)
        {
            addOrDelete(set, f[fp], add);
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::faceToPoint::faceToPoint
(
    const polyMesh& mesh,
    const word& setName,
    const faceAction option
)
:
    topoSetSource(mesh),
    setName_(setName),
    option_(option)
{}


// Construct from dictionary
CML::faceToPoint::faceToPoint
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    topoSetSource(mesh),
    setName_(dict.lookup("set")),
    option_(faceActionNames_.read(dict.lookup("option")))
{}


// Construct from Istream
CML::faceToPoint::faceToPoint
(
    const polyMesh& mesh,
    Istream& is
)
:
    topoSetSource(mesh),
    setName_(checkIs(is)),
    option_(faceActionNames_.read(checkIs(is)))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::faceToPoint::~faceToPoint()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::faceToPoint::applyToSet
(
    const topoSetSource::setAction action,
    topoSet& set
) const
{
    if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
    {
        Info<< "    Adding points from face in faceSet " << setName_
            << " ..." << endl;

        combine(set, true);
    }
    else if (action == topoSetSource::DELETE)
    {
        Info<< "    Removing points from face in faceSet " << setName_
            << " ..." << endl;

        combine(set, false);
    }
}


// ************************************************************************* //
