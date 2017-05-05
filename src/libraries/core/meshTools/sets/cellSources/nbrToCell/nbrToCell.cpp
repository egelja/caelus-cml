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

#include "nbrToCell.hpp"
#include "polyMesh.hpp"

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(nbrToCell, 0);

addToRunTimeSelectionTable(topoSetSource, nbrToCell, word);

addToRunTimeSelectionTable(topoSetSource, nbrToCell, istream);

}


CML::topoSetSource::addToUsageTable CML::nbrToCell::usage_
(
    nbrToCell::typeName,
    "\n    Usage: nbrToCell <nNeighbours>\n\n"
    "    Select all cells with <= nNeighbours neighbouring cells\n\n"
);


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::nbrToCell::combine(topoSet& set, const bool add) const
{
    const cellList& cells = mesh().cells();
    const polyBoundaryMesh& patches = mesh_.boundaryMesh();

    boolList isCoupled(mesh_.nFaces()-mesh_.nInternalFaces(), false);

    forAll(patches, patchI)
    {
        const polyPatch& pp = patches[patchI];

        if (pp.coupled())
        {
            label faceI = pp.start();
            forAll(pp, i)
            {
                isCoupled[faceI-mesh_.nInternalFaces()] = true;
                faceI++;
            }
        }
    }

    forAll(cells, cellI)
    {
        const cell& cFaces = cells[cellI];

        label nNbrCells = 0;

        forAll(cFaces, i)
        {
            label faceI = cFaces[i];

            if (mesh_.isInternalFace(faceI))
            {
                nNbrCells++;
            }
            else if (isCoupled[faceI-mesh_.nInternalFaces()])
            {
                nNbrCells++;
            }
        }

        if (nNbrCells <= minNbrs_)
        {
            addOrDelete(set, cellI, add);
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::nbrToCell::nbrToCell
(
    const polyMesh& mesh,
    const label minNbrs
)
:
    topoSetSource(mesh),
    minNbrs_(minNbrs)
{}


// Construct from dictionary
CML::nbrToCell::nbrToCell
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    topoSetSource(mesh),
    minNbrs_(readLabel(dict.lookup("neighbours")))
{}


// Construct from Istream
CML::nbrToCell::nbrToCell
(
    const polyMesh& mesh,
    Istream& is
)
:
    topoSetSource(mesh),
    minNbrs_(readLabel(checkIs(is)))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::nbrToCell::~nbrToCell()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::nbrToCell::applyToSet
(
    const topoSetSource::setAction action,
    topoSet& set
) const
{
    if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
    {
        Info<< "    Adding cells with only " << minNbrs_ << " or less"
                " neighbouring cells" << " ..." << endl;

        combine(set, true);
    }
    else if (action == topoSetSource::DELETE)
    {
        Info<< "    Removing cells with only " << minNbrs_ << " or less"
                " neighbouring cells" << " ..." << endl;

        combine(set, false);
    }
}


// ************************************************************************* //
