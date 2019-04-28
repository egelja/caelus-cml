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

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::primitiveMesh::calcPointPoints() const
{
    if (debug)
    {
        Pout<< "primitiveMesh::calcPointPoints() : "
            << "calculating pointPoints"
            << endl;

        if (debug == -1)
        {
            // For checking calls:abort so we can quickly hunt down
            // origin of call
            FatalErrorInFunction
                << abort(FatalError);
        }
    }

    // It is an error to attempt to recalculate pointPoints
    // if the pointer is already set
    if (ppPtr_)
    {
        FatalErrorInFunction
            << "pointPoints already calculated"
            << abort(FatalError);
    }
    else
    {
        const edgeList& e = edges();
        const labelListList& pe = pointEdges();

        ppPtr_ = new labelListList(pe.size());
        labelListList& pp = *ppPtr_;

        forAll(pe, pointI)
        {
            pp[pointI].setSize(pe[pointI].size());

            forAll(pe[pointI], ppi)
            {
                if (e[pe[pointI][ppi]].start() == pointI)
                {
                    pp[pointI][ppi] = e[pe[pointI][ppi]].end();
                }
                else if (e[pe[pointI][ppi]].end() == pointI)
                {
                    pp[pointI][ppi] = e[pe[pointI][ppi]].start();
                }
                else
                {
                    FatalErrorInFunction
                        << "something wrong with edges"
                        << abort(FatalError);
                }
            }
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::labelListList& CML::primitiveMesh::pointPoints() const
{
    if (!ppPtr_)
    {
        calcPointPoints();
    }

    return *ppPtr_;
}


const CML::labelList& CML::primitiveMesh::pointPoints
(
    const label pointI,
    DynamicList<label>& storage
) const
{
    if (hasPointPoints())
    {
        return pointPoints()[pointI];
    }
    else
    {
        const edgeList& edges = this->edges();
        const labelList& pEdges = pointEdges()[pointI];

        storage.clear();

        if (pEdges.size() > storage.capacity())
        {
            storage.setCapacity(pEdges.size());
        }

        forAll(pEdges, i)
        {
            storage.append(edges[pEdges[i]].otherVertex(pointI));
        }

        return storage;
    }
}


const CML::labelList& CML::primitiveMesh::pointPoints
(
    const label pointI
) const
{
    return pointPoints(pointI, labels_);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// ************************************************************************* //
