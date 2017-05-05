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

#include "mapDistribute.hpp"
#include "extendedCentredCellToFaceStencil.hpp"
#include "cellToFaceStencil.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::extendedCentredCellToFaceStencil::extendedCentredCellToFaceStencil
(
    const cellToFaceStencil& stencil
)
:
    extendedCellToFaceStencil(stencil.mesh()),
    stencil_(stencil)
{
    // Calculate distribute map (also renumbers elements in stencil)
    List<Map<label> > compactMap(Pstream::nProcs());
    mapPtr_.reset
    (
        new mapDistribute
        (
            stencil.globalNumbering(),
            stencil_,
            compactMap
        )
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Per face which elements of the stencil to keep.
void CML::extendedCentredCellToFaceStencil::compact()
{
    boolList isInStencil(map().constructSize(), false);

    forAll(stencil_, faceI)
    {
        const labelList& stencilCells = stencil_[faceI];

        forAll(stencilCells, i)
        {
            isInStencil[stencilCells[i]] = true;
        }
    }

    mapPtr_().compact(isInStencil, Pstream::msgType());
}


// ************************************************************************* //
