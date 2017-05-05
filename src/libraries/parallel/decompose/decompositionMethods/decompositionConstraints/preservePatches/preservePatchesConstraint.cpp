/*---------------------------------------------------------------------------*\
Copyright (C) 2015 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "preservePatchesConstraint.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "syncTools.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace decompositionConstraints
{
defineTypeName(preservePatchesConstraint);
addToRunTimeSelectionTable
(
    decompositionConstraint,
    preservePatchesConstraint,
    dictionary
);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::decompositionConstraints::preservePatchesConstraint::
preservePatchesConstraint
(
    const dictionary& constraintsDict,
    const word& modelType
)
:
    decompositionConstraint(constraintsDict, typeName),
    patches_(coeffDict_.lookup("patches"))
{
    if (decompositionConstraint::debug)
    {
        Info<< type() << " : adding constraints to keep owner of faces"
            << " in patches " << patches_
            << " on same processor. This only makes sense for cyclics." << endl;
    }
}


CML::decompositionConstraints::preservePatchesConstraint::
preservePatchesConstraint
(
    const wordReList& patches
)
:
    decompositionConstraint(dictionary(), typeName),
    patches_(patches)
{
    if (decompositionConstraint::debug)
    {
        Info<< type() << " : adding constraints to keep owner of faces"
            << " in patches " << patches_
            << " on same processor. This only makes sense for cyclics." << endl;
    }
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void CML::decompositionConstraints::preservePatchesConstraint::add
(
    const polyMesh& mesh,
    boolList& blockedFace,
    PtrList<labelList>& specifiedProcessorFaces,
    labelList& specifiedProcessor,
    List<labelPair>& explicitConnections
) const
{
    const polyBoundaryMesh& pbm = mesh.boundaryMesh();

    blockedFace.setSize(mesh.nFaces(), true);

    const labelList patchIDs(pbm.patchSet(patches_).sortedToc());

    label nUnblocked = 0;

    forAll(patchIDs, i)
    {
        const polyPatch& pp = pbm[patchIDs[i]];

        forAll(pp, i)
        {
            if (blockedFace[pp.start() + i])
            {
                blockedFace[pp.start() + i] = false;
                nUnblocked++;
            }
        }
    }

    if (decompositionConstraint::debug & 2)
    {
        reduce(nUnblocked, sumOp<label>());
        Info<< type() << " : unblocked " << nUnblocked << " faces" << endl;
    }

    syncTools::syncFaceList(mesh, blockedFace, andEqOp<bool>());
}


void CML::decompositionConstraints::preservePatchesConstraint::apply
(
    const polyMesh& mesh,
    const boolList& blockedFace,
    const PtrList<labelList>& specifiedProcessorFaces,
    const labelList& specifiedProcessor,
    const List<labelPair>& explicitConnections,
    labelList& decomposition
) const
{
    // If the decomposition has not enforced the constraint do it over
    // here.

    // Synchronise decomposition on patchIDs
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    const polyBoundaryMesh& pbm = mesh.boundaryMesh();

    labelList destProc(mesh.nFaces()-mesh.nInternalFaces(), labelMax);

    forAll(pbm, patchI)
    {
        const polyPatch& pp = pbm[patchI];

        const labelUList& faceCells = pp.faceCells();

        forAll(faceCells, i)
        {
            label bFaceI = pp.start()+i-mesh.nInternalFaces();
            destProc[bFaceI] = decomposition[faceCells[i]];
        }
    }

    syncTools::syncBoundaryFaceList(mesh, destProc, minEqOp<label>());


    // Override if differing
    // ~~~~~~~~~~~~~~~~~~~~~

    const labelList patchIDs(pbm.patchSet(patches_).sortedToc());

    label nChanged = 0;

    forAll(patchIDs, i)
    {
        const polyPatch& pp = pbm[patchIDs[i]];

        const labelUList& faceCells = pp.faceCells();

        forAll(faceCells, i)
        {
            label bFaceI = pp.start()+i-mesh.nInternalFaces();

            if (decomposition[faceCells[i]] != destProc[bFaceI])
            {
                decomposition[faceCells[i]] = destProc[bFaceI];
                nChanged++;
            }
        }
    }

    if (decompositionConstraint::debug & 2)
    {
        reduce(nChanged, sumOp<label>());
        Info<< type() << " : changed decomposition on " << nChanged
            << " cells" << endl;
    }
}


// ************************************************************************* //
