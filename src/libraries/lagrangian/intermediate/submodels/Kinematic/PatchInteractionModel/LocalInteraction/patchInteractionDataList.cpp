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

#include "patchInteractionDataList.hpp"
#include "stringListOps.hpp"
#include "wallPolyPatch.hpp"

// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

CML::patchInteractionDataList::patchInteractionDataList()
:
    List<patchInteractionData>(),
    patchGroupIDs_()
{}


CML::patchInteractionDataList::patchInteractionDataList
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    List<patchInteractionData>(dict.lookup("patches")),
    patchGroupIDs_(this->size())
{
    const polyBoundaryMesh& bMesh = mesh.boundaryMesh();
    const wordList allPatchNames = bMesh.names();

    const List<patchInteractionData>& items = *this;
    forAllReverse(items, i)
    {
        const word& patchName = items[i].patchName();
        labelList patchIDs = findStrings(patchName, allPatchNames);

        if (patchIDs.empty())
        {
            WarningIn
            (
                "CML::patchInteractionDataList::patchInteractionDataList"
                "("
                    "const polyMesh&, "
                    "const dictionary&"
                ")"
            )   << "Cannot find any patch names matching " << patchName
                << endl;
        }

        patchGroupIDs_[i].transfer(patchIDs);
    }

    // check that all walls are specified
    DynamicList<word> badWalls;
    forAll(bMesh, patchI)
    {
        const polyPatch& pp = bMesh[patchI];
        if (isA<wallPolyPatch>(pp) && applyToPatch(pp.index()) < 0)
        {
            badWalls.append(pp.name());
        }
    }

    if (badWalls.size() > 0)
    {
        FatalErrorIn
        (
            "CML::patchInteractionDataList::patchInteractionDataList"
            "("
                "const polyMesh&, "
                "const dictionary&"
            ")"
        )    << "All wall patches must be specified when employing local patch "
            << "interaction. Please specify data for patches:" << nl
            << badWalls << nl << exit(FatalError);
    }
}


CML::patchInteractionDataList::patchInteractionDataList
(
    const patchInteractionDataList& pidl
)
:
    List<patchInteractionData>(pidl),
    patchGroupIDs_(pidl.patchGroupIDs_)
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

CML::label CML::patchInteractionDataList::applyToPatch(const label id) const
{
    forAll(patchGroupIDs_, groupI)
    {
        const labelList& patchIDs = patchGroupIDs_[groupI];
        forAll(patchIDs, patchI)
        {
            if (patchIDs[patchI] == id)
            {
                return groupI;
            }
        }
    }

    return -1;
}


// ************************************************************************* //
