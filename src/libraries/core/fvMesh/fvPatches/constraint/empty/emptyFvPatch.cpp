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

#include "emptyFvPatch.hpp"
#include "fvBoundaryMesh.hpp"
#include "fvMesh.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(emptyFvPatch, 0);
addToRunTimeSelectionTable(fvPatch, emptyFvPatch, polyPatch);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

emptyFvPatch::emptyFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm)
:
    fvPatch(patch, bm),
    faceCells_
    (
        labelList::subList
        (
            boundaryMesh().mesh().faceOwner(), 0, patch.start()
        )
    )
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const labelUList& emptyFvPatch::faceCells() const
{
    return faceCells_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
