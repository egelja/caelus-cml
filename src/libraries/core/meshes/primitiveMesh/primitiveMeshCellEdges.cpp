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

#include "primitiveMesh.hpp"
#include "DynamicList.hpp"
#include "ListOps.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::primitiveMesh::calcCellEdges() const
{
    // Loop through all faces and mark up cells with edges of the face.
    // Check for duplicates

    if (debug)
    {
        Pout<< "primitiveMesh::calcCellEdges() : "
            << "calculating cellEdges"
            << endl;

        if (debug == -1)
        {
            // For checking calls:abort so we can quickly hunt down
            // origin of call
            FatalErrorInFunction
                << abort(FatalError);
        }
    }

    // It is an error to attempt to recalculate cellEdges
    // if the pointer is already set
    if (cePtr_)
    {
        FatalErrorInFunction
            << "cellEdges already calculated"
            << abort(FatalError);
    }
    else
    {
        // Set up temporary storage
        List<DynamicList<label, edgesPerCell_> > ce(nCells());


        // Get reference to faceCells and faceEdges
        const labelList& own = faceOwner();
        const labelList& nei = faceNeighbour();
        const labelListList& fe = faceEdges();

        // loop through the list again and add edges; checking for duplicates
        forAll(own, faceI)
        {
            DynamicList<label, edgesPerCell_>& curCellEdges = ce[own[faceI]];

            const labelList& curEdges = fe[faceI];

            forAll(curEdges, edgeI)
            {
                if (findIndex(curCellEdges, curEdges[edgeI]) == -1)
                {
                    // Add the edge
                    curCellEdges.append(curEdges[edgeI]);
                }
            }
        }

        forAll(nei, faceI)
        {
            DynamicList<label, edgesPerCell_>& curCellEdges = ce[nei[faceI]];

            const labelList& curEdges = fe[faceI];

            forAll(curEdges, edgeI)
            {
                if (findIndex(curCellEdges, curEdges[edgeI]) == -1)
                {
                    // add the edge
                    curCellEdges.append(curEdges[edgeI]);
                }
            }
        }

        cePtr_ = new labelListList(ce.size());
        labelListList& cellEdgeAddr = *cePtr_;

        // reset the size
        forAll(ce, cellI)
        {
            cellEdgeAddr[cellI].transfer(ce[cellI]);
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::labelListList& CML::primitiveMesh::cellEdges() const
{
    if (!cePtr_)
    {
        calcCellEdges();
    }

    return *cePtr_;
}


// ************************************************************************* //
