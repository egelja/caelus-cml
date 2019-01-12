/*---------------------------------------------------------------------------*\
Copyright (C) Creative Fields, Ltd.
-------------------------------------------------------------------------------
License
    This file is part of cfMesh.

    cfMesh is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    cfMesh is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with cfMesh.  If not, see <http://www.gnu.org/licenses/>.

Author: Franjo Juretic (franjo.juretic@c-fields.com)

\*---------------------------------------------------------------------------*/

#include "triSurfaceCleanupDuplicates.hpp"
#include "meshOctree.hpp"
#include "demandDrivenData.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

triSurfaceCleanupDuplicates::triSurfaceCleanupDuplicates
(
    const meshOctree& octree,
    const scalar tol
)
:
    tolerance_(tol),
    surf_(const_cast<triSurf&>(octree.surface())),
    octree_(octree),
    newTriangleLabel_(),
    done_(false)
{}

triSurfaceCleanupDuplicates::~triSurfaceCleanupDuplicates()
{}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void triSurfaceCleanupDuplicates::mergeIdentities()
{
    if( Pstream::parRun() )
        FatalError << "Material detection does not run in parallel"
            << exit(FatalError);

    if( done_ )
    {
        WarningInFunction
            << "Operation is already performed" << endl;
        return;
    }

    newTriangleLabel_.setSize(surf_.size());
    forAll(newTriangleLabel_, triI)
        newTriangleLabel_[triI] = triI;

    bool finished;
    do
    {
        finished = true;

        if( checkDuplicateTriangles() )
            finished = false;
        if( mergeDuplicatePoints() )
            finished = false;
    } while( !finished );

    done_ = true;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
