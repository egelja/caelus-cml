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
#include "degenerateMatcher.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::primitiveMesh::calcCellShapes() const
{
    if (debug)
    {
        Pout<< "primitiveMesh::calcCellShapes() : calculating cellShapes"
            << endl;
    }

    // It is an error to attempt to recalculate faceCells
    // if the pointer is already set
    if (cellShapesPtr_)
    {
        FatalErrorInFunction
            << "cellShapes already calculated"
            << abort(FatalError);
    }
    else
    {
        cellShapesPtr_ = new cellShapeList(nCells());
        cellShapeList& cellShapes = *cellShapesPtr_;

        forAll(cellShapes, celli)
        {
            cellShapes[celli] = degenerateMatcher::match(*this, celli);
        }
    }
}


// ************************************************************************* //
