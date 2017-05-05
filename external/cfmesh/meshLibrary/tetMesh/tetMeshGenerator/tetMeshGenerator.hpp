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

Class
    tetMeshGenerator

Description
    Creates cartesian mesh from the octree

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    tetMeshGenerator.cpp

\*---------------------------------------------------------------------------*/

#ifndef tetMeshGenerator_HPP
#define tetMeshGenerator_HPP

#include "polyMeshGen.hpp"
#include "IOdictionary.hpp"
#include "workflowControls.hpp"
//#include "volFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declarations
class triSurf;
class meshOctree;
class Time;

/*---------------------------------------------------------------------------*\
                           Class tetMeshGenerator Declaration
\*---------------------------------------------------------------------------*/

class tetMeshGenerator
{
    // Private data
        //- reference to Time
        const Time& runTime_;

        //- pointer to the surface
        const triSurf* surfacePtr_;

        //- pointer to the modified surface mesh
        const triSurf* modSurfacePtr_;

        //- IOdictionary containing information about cell sizes, etc..
        IOdictionary meshDict_;

        //- pointer to the octree
        meshOctree* octreePtr_;

        //- mesh
        polyMeshGen mesh_;

        //- workflow controller
        workflowControls controller_;

    // Private member functions
        //- create cartesian mesh
        void createTetMesh();

        //- prepare mesh surface
        void surfacePreparation();

        //- map mesh to the surface and untangle surface
        void mapMeshToSurface();

        //- capture edges and corners
        void extractPatches();

        //- capture edges and corners
        void mapEdgesAndCorners();

        //- optimise surface mesh
        void optimiseMeshSurface();

        //- add boundary layers
        void generateBoundaryLayers();

        //- mesh optimisation
        void optimiseFinalMesh();

        //- re-project points back on the surface mesh after back-scaling
        void projectSurfaceAfterBackScaling();

        //- refine boundary layers
        void refBoundaryLayers();

        //- replace boundaries
        void replaceBoundaries();

        //- renumber the mesh
        void renumberMesh();

        //- generate mesh
        void generateMesh();

        //- Disallow default bitwise copy construct
        tetMeshGenerator(const tetMeshGenerator&);

        //- Disallow default bitwise assignment
        void operator=(const tetMeshGenerator&);

public:

    // Constructors

        //- Construct from time
        tetMeshGenerator(const Time&);

    // Destructor

        ~tetMeshGenerator();


    // Member Functions

        //- write the mesh
        void writeMesh() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
