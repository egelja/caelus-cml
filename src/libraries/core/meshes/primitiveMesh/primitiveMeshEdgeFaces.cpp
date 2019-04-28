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
#include "ListOps.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::labelListList& CML::primitiveMesh::edgeFaces() const
{
    if (!efPtr_)
    {
        if (debug)
        {
            Pout<< "primitiveMesh::edgeFaces() : calculating edgeFaces" << endl;

            if (debug == -1)
            {
                // For checking calls:abort so we can quickly hunt down
                // origin of call
                FatalErrorInFunction
                    << abort(FatalError);
            }
        }

        // Invert faceEdges
        efPtr_ = new labelListList(nEdges());
        invertManyToMany(nEdges(), faceEdges(), *efPtr_);
    }

    return *efPtr_;
}


const CML::labelList& CML::primitiveMesh::edgeFaces
(
    const label edgeI,
    DynamicList<label>& storage
) const
{
    if (hasEdgeFaces())
    {
        return edgeFaces()[edgeI];
    }
    else
    {
        // Use the fact that pointEdges are sorted in incrementing edge order
        const edge& e = edges()[edgeI];
        const labelList& pFaces0 = pointFaces()[e[0]];
        const labelList& pFaces1 = pointFaces()[e[1]];

        label i0 = 0;
        label i1 = 0;

        storage.clear();

        while (i0 < pFaces0.size() && i1 < pFaces1.size())
        {
            if (pFaces0[i0] < pFaces1[i1])
            {
                ++i0;
            }
            else if (pFaces0[i0] > pFaces1[i1])
            {
                ++i1;
            }
            else
            {
                // Equal. Append.
                storage.append(pFaces0[i0]);
                ++i0;
                ++i1;
            }
        }

        return storage;
    }
}


const CML::labelList& CML::primitiveMesh::edgeFaces(const label edgeI) const
{
    return edgeFaces(edgeI, labels_);
}


// ************************************************************************* //
