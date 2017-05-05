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

#include "coupledFacePointPatch.hpp"
#include "pointBoundaryMesh.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


namespace CML
{
    defineTypeNameAndDebug(coupledFacePointPatch, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::coupledFacePointPatch::coupledFacePointPatch
(
    const polyPatch& patch,
    const pointBoundaryMesh& bm
)
:
    facePointPatch(patch, bm),
    coupledPointPatch(bm),
    coupledPolyPatch_(refCast<const coupledPolyPatch>(patch))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::coupledFacePointPatch::~coupledFacePointPatch()
{}


// ************************************************************************* //
