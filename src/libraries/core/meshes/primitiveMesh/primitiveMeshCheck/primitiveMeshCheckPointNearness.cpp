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

#include "primitiveMesh.hpp"
#include "SortableList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

bool CML::primitiveMesh::checkPointNearness
(
    const bool report,
    const scalar reportDistSqr,
    labelHashSet* setPtr
) const
{
    const pointField& points = this->points();

    // Sort points
    SortableList<scalar> sortedMag(magSqr(points));

    label nClose = 0;

    for (label i = 1; i < sortedMag.size(); i++)
    {
        label pti = sortedMag.indices()[i];

        // Compare pti to any previous points with similar sortedMag
        for
        (
            label j = i-1;
            j >= 0 && (sortedMag[j] > sortedMag[i]-reportDistSqr);
            --j
        )
        {
            label prevPtI = sortedMag.indices()[j];

            if (magSqr(points[pti] - points[prevPtI]) < reportDistSqr)
            {
                //// Check if unconnected.
                //const labelList& pEdges = pointEdges()[pti];
                //
                //bool connected = false;
                //
                //forAll(pEdges, pEdgei)
                //{
                //    if (edges()[pEdges[pEdgei]].otherVertex(prevPtI) != -1)
                //    {
                //        connected = true;
                //        break;
                //    }
                //}
                //
                //if (!connected)
                {
                    nClose++;

                    if (setPtr)
                    {
                        setPtr->insert(pti);
                        setPtr->insert(prevPtI);
                    }
                }
            }
        }
    }

    reduce(nClose, sumOp<label>());

    if (nClose > 0)
    {
        if (report)
        {
            Info<< "  <<Points closer than " << CML::sqrt(reportDistSqr)
                << " together found, number: " << nClose
                << endl;
        }

        return true;
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //
