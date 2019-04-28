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

#include "normalToFace.hpp"
#include "polyMesh.hpp"
#include "faceSet.hpp"

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(normalToFace, 0);

addToRunTimeSelectionTable(topoSetSource, normalToFace, word);

addToRunTimeSelectionTable(topoSetSource, normalToFace, istream);

}


CML::topoSetSource::addToUsageTable CML::normalToFace::usage_
(
    normalToFace::typeName,
    "\n    Usage: normalToFace (nx ny nz) <tol>\n\n"
    "    Select faces with normal aligned to unit vector (nx ny nz)\n"
    "    to within tol\n"
);


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::normalToFace::setNormal()
{
    normal_ /= mag(normal_) + VSMALL;

    Info<< "    normalToFace : Normalized vector to " << normal_ << endl;

    if (tol_ < -1 || tol_ > 1)
    {
        FatalErrorInFunction
            << "tolerance not within range -1..1 : " << tol_
            << exit(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::normalToFace::normalToFace
(
    const polyMesh& mesh,
    const vector& normal,
    const scalar tol
)
:
    topoSetSource(mesh),
    normal_(normal),
    tol_(tol)
{
    setNormal();
}


// Construct from dictionary
CML::normalToFace::normalToFace(const polyMesh& mesh, const dictionary& dict)
:
    topoSetSource(mesh),
    normal_(dict.lookup("normal")),
    tol_(readScalar(dict.lookup("cos")))
{
    setNormal();
}


// Construct from Istream
CML::normalToFace::normalToFace(const polyMesh& mesh, Istream& is)
:
    topoSetSource(mesh),
    normal_(checkIs(is)),
    tol_(readScalar(checkIs(is)))
{
    setNormal();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::normalToFace::~normalToFace()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::normalToFace::applyToSet
(
    const topoSetSource::setAction action,
    topoSet& set
) const
{
    if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
    {
        Info<< "    Adding faces according to normal being aligned with "
            << normal_ << " (to within " << tol_ << ") ..." << endl;

        forAll(mesh_.faceAreas(), faceI)
        {
            vector n = mesh_.faceAreas()[faceI];
            n /= mag(n) + VSMALL;

            if (mag(1 - (n & normal_)) < tol_)
            {
                set.insert(faceI);
            }
        }
    }
    else if (action == topoSetSource::DELETE)
    {
        Info<< "    Removing faces according to normal being aligned with "
            << normal_ << " (to within " << tol_ << ") ..." << endl;


        DynamicList<label> toBeRemoved(set.size()/10);

        forAllConstIter(topoSet, set, iter)
        {
            const label faceI = iter.key();

            vector n = mesh_.faceAreas()[faceI];
            n /= mag(n) + VSMALL;

            if (mag(1 - (n & normal_)) < tol_)
            {
                toBeRemoved.append(faceI);
            }
        }

        forAll(toBeRemoved, i)
        {
            set.erase(toBeRemoved[i]);
        }
    }
}


// ************************************************************************* //
