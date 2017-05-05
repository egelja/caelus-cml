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

#include "fvMotionSolver.hpp"
#include "volPointInterpolation.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(fvMotionSolver, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::fvMotionSolver::fvMotionSolver(const polyMesh& mesh)
:
    motionSolver(mesh),
    fvMesh_(refCast<const fvMesh>(mesh))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::fvMotionSolver::~fvMotionSolver()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::fvMotionSolver::movePoints(const pointField& p)
{
    // Movement of pointMesh and volPointInterpolation done by polyMesh,fvMesh
}


void CML::fvMotionSolver::updateMesh(const mapPolyMesh& mpm)
{
    motionSolver::updateMesh(mpm);
    // Update of pointMesh and volPointInterpolation done by polyMesh,fvMesh
}


// ************************************************************************* //
