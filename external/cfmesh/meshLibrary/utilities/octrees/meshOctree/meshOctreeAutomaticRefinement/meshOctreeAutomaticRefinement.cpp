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

#include "meshOctreeAutomaticRefinement.hpp"
#include "demandDrivenData.hpp"
#include "triSurfacePartitioner.hpp"
#include "triSurfaceCurvatureEstimator.hpp"
#include "meshOctreeAddressing.hpp"
#include "IOdictionary.hpp"
#include "triSurf.hpp"

// #define DEBUGSearch

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

void meshOctreeAutomaticRefinement::createOctreeAddressing() const
{
    octreeAddressingPtr_ =
        new meshOctreeAddressing(octree_, meshDict_, useDATABoxes_);
}

const meshOctreeAddressing& meshOctreeAutomaticRefinement::octreeAddressing()
const
{
    if( !octreeAddressingPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
              << "Cannot calculate addressing!" << abort(FatalError);
        # endif

        createOctreeAddressing();
    }

    return *octreeAddressingPtr_;
}

void meshOctreeAutomaticRefinement::createSurfacePartitioner() const
{
    partitionerPtr_ = new triSurfacePartitioner(octree_.surface());
}

const triSurfacePartitioner& meshOctreeAutomaticRefinement::partitioner() const
{
    if( !partitionerPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
              << "Cannot calculate addressing!" << abort(FatalError);
        # endif

        createSurfacePartitioner();
    }

    return *partitionerPtr_;
}

void meshOctreeAutomaticRefinement::createCurvatureEstimator() const
{
    curvaturePtr_ = new triSurfaceCurvatureEstimator(octree_.surface());
}

const triSurfaceCurvatureEstimator& meshOctreeAutomaticRefinement::curvature()
const
{
    if( !curvaturePtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
              << "Cannot calculate addressing!" << abort(FatalError);
        # endif

        createCurvatureEstimator();
    }

    return *curvaturePtr_;
}

void meshOctreeAutomaticRefinement::setMaxRefLevel()
{
    const boundBox& rootBox = octree_.rootBox();
    const scalar size = rootBox.max().x() - rootBox.min().x();
    maxRefLevel_ = 0;

    if( meshDict_.found("minCellSize") )
    {
        const scalar maxSize(readScalar(meshDict_.lookup("maxCellSize")));
        scalar cs(readScalar(meshDict_.lookup("minCellSize")));
        cs *= (1.0 + SMALL);

        if( cs > maxSize )
            return;

        bool finished;
        do
        {
            finished = false;

            const scalar lSize = size / pow(label(2), label(maxRefLevel_));

            if( lSize < cs )
            {
                finished = true;
            }
            else
            {
                ++maxRefLevel_;
            }
        } while( !finished );

        useDATABoxes_ = true;

        Info << "Requested min cell size corresponds to octree level "
            << label(maxRefLevel_) << endl;
    }
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from surface and IOdictionary
meshOctreeAutomaticRefinement::meshOctreeAutomaticRefinement
(
    meshOctree& mo,
    const IOdictionary& dict,
    bool useDATABoxes
)
:
    octree_(mo),
    meshDict_(dict),
    useDATABoxes_(useDATABoxes),
    hexRefinement_(false),
    octreeAddressingPtr_(nullptr),
    partitionerPtr_(nullptr),
    curvaturePtr_(nullptr),
    maxRefLevel_(0)
{
    if( !useDATABoxes_ && dict.found("keepCellsIntersectingBoundary") )
    {
        useDATABoxes_ = readBool(dict.lookup("keepCellsIntersectingBoundary"));
    }

    //- calculate maximum allowed refinement level from the minimum cell size
    setMaxRefLevel();
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

meshOctreeAutomaticRefinement::~meshOctreeAutomaticRefinement()
{
    deleteDemandDrivenData(octreeAddressingPtr_);
    deleteDemandDrivenData(curvaturePtr_);
    deleteDemandDrivenData(partitionerPtr_);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
