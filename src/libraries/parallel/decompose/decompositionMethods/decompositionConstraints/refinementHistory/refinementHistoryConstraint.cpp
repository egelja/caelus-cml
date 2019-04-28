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

#include "refinementHistoryConstraint.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "syncTools.hpp"
#include "refinementHistory.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
defineTypeName(refinementHistoryConstraint);
addToRunTimeSelectionTable
(
    decompositionConstraint,
    refinementHistoryConstraint,
    dictionary
);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::refinementHistoryConstraint::refinementHistoryConstraint
(
    const dictionary& constraintsDict,
    const word& modelType
)
:
    decompositionConstraint(constraintsDict, typeName)
{
    if (decompositionConstraint::debug)
    {
        Info<< type() << " : setting constraints to preserve refinement history"
            << endl;
    }
}


CML::refinementHistoryConstraint::refinementHistoryConstraint()
:
    decompositionConstraint(dictionary(), typeName)
{
    if (decompositionConstraint::debug)
    {
        Info<< type() << " : setting constraints to refinement history"
            << endl;
    }
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void CML::refinementHistoryConstraint::add
(
    const polyMesh& mesh,
    boolList& blockedFace,
    PtrList<labelList>& specifiedProcessorFaces,
    labelList& specifiedProcessor,
    List<labelPair>& explicitConnections
) const
{
    autoPtr<const refinementHistory> storagePtr;
    refinementHistory const* refPtr = nullptr;

    if (mesh.foundObject<refinementHistory>("refinementHistory"))
    {
        if (decompositionConstraint::debug)
        {
            Info<< type() << " : found refinementHistory" << endl;
        }
        refPtr = &mesh.lookupObject<refinementHistory>("refinementHistory");
    }
    else
    {
        if (decompositionConstraint::debug)
        {
            Info<< type() << " : reading refinementHistory from time "
                << mesh.facesInstance() << endl;
        }
        storagePtr.reset
        (
            new refinementHistory
            (
                IOobject
                (
                    "refinementHistory",
                    mesh.facesInstance(),
                    polyMesh::meshSubDir,
                    mesh,
                    IOobject::READ_IF_PRESENT,
                    IOobject::NO_WRITE
                ),
                mesh.nCells()
            )
        );
    }

    const refinementHistory& history =
    (
        storagePtr.valid()
      ? storagePtr()
      : *refPtr
    );

    if (history.active())
    {
        // refinementHistory itself implements decompositionConstraint
        history.add
        (
            blockedFace,
            specifiedProcessorFaces,
            specifiedProcessor,
            explicitConnections
        );
    }
}


void CML::refinementHistoryConstraint::apply
(
    const polyMesh& mesh,
    const boolList& blockedFace,
    const PtrList<labelList>& specifiedProcessorFaces,
    const labelList& specifiedProcessor,
    const List<labelPair>& explicitConnections,
    labelList& decomposition
) const
{
    autoPtr<const refinementHistory> storagePtr;
    refinementHistory const* refPtr = nullptr;

    if (mesh.foundObject<refinementHistory>("refinementHistory"))
    {
        if (decompositionConstraint::debug)
        {
            Info<< type() << " : found refinementHistory" << endl;
        }
        refPtr = &mesh.lookupObject<refinementHistory>("refinementHistory");
    }
    else
    {
        if (decompositionConstraint::debug)
        {
            Info<< type() << " : reading refinementHistory from time "
                << mesh.facesInstance() << endl;
        }
        storagePtr.reset
        (
            new refinementHistory
            (
                IOobject
                (
                    "refinementHistory",
                    mesh.facesInstance(),
                    polyMesh::meshSubDir,
                    mesh,
                    IOobject::READ_IF_PRESENT,
                    IOobject::NO_WRITE
                ),
                mesh.nCells()
            )
        );
    }

    const refinementHistory& history =
    (
        storagePtr.valid()
      ? storagePtr()
      : *refPtr
    );

    if (history.active())
    {
        // refinementHistory itself implements decompositionConstraint
        history.apply
        (
            blockedFace,
            specifiedProcessorFaces,
            specifiedProcessor,
            explicitConnections,
            decomposition
        );
    }
}


// ************************************************************************* //
