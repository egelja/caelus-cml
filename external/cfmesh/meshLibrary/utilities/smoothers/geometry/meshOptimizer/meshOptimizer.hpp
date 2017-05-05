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
    meshOptimizer

Description
    Mesh smoothing without any topological changes

SourceFiles
    meshOptimizer.cpp

\*---------------------------------------------------------------------------*/

#ifndef meshOptimizer_HPP
#define meshOptimizer_HPP

#include "polyMeshGen.hpp"
#include "boolList.hpp"
#include "VRWGraph.hpp"
#include "DynList.hpp"
#include "partTet.hpp"
#include "HashSet.hpp"
#include "boundBox.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declarations
class meshOctree;
class meshSurfaceEngine;
class plane;
class partTetMesh;

/*---------------------------------------------------------------------------*\
                        Class meshOptimizer Declaration
\*---------------------------------------------------------------------------*/

class meshOptimizer
{
    // Private data
        //- reference to the mesh
        polyMeshGen& mesh_;

        //- location of vertex (internal, boundary, edge, corner)
        List<direction> vertexLocation_;

        //- locked faces which shall not be changed
        labelLongList lockedFaces_;

        //- mesh surface
        mutable meshSurfaceEngine* msePtr_;

        //- enforce constraints
        bool enforceConstraints_;

        //- name of the subset contaning tangled points
        word badPointsSubsetName_;

    // Private member functions
        //- return mesh surface
        const meshSurfaceEngine& meshSurface() const;
        void clearSurface();

        //- find problematic faces
        label findBadFaces(labelHashSet&, const boolList&) const;
        label findLowQualityFaces(labelHashSet&, const boolList&) const;

        //- mark point locations
        void calculatePointLocations();

    // Nested classes

        class laplaceSmoother
        {
            // Private data
                //- reference to the mesh
                polyMeshGen& mesh_;

                //- location of vertex (internal, boundary, edge, corner)
                const List<direction>& vertexLocation_;

            // Private member functions
                //- smooth the node using the laplacian smoother
                //- new position is the average of the neighbouring vertices
                void laplacian(const labelLongList&, const label);
                void laplacianSurface(const labelLongList&, const label);

                void laplacianParallel
                (
                    const labelLongList& procPoints,
                    const bool smoothOnlySurfaceNodes = false
                );

                //- smooth the node using the laplacian smoother
                //- new position is the average of the centres of faces attached
                //- to the vertex
                void laplacianPC(const labelLongList&, const label);
                void laplacianPCParallel(const labelLongList& procPoints);

                //- smooth the node using the laplacian smoother
                //- new position is the average of the centres of faces attached
                //- to the vertex
                void laplacianWPC(const labelLongList&, const label);
                void laplacianWPCParallel(const labelLongList& procPoints);

                //- update geometry after smoothing
                void updateMeshGeometry(const labelLongList& smoothPoints);

                //- Disallow default bitwise copy construct
                laplaceSmoother(const laplaceSmoother&);

                //- Disallow default bitwise assignment
                void operator=(const laplaceSmoother&);

        public:

            // Constructor

                //- Construct from mesh and vertex locations
                laplaceSmoother(polyMeshGen&, const List<direction>&);

            // Destructor

                ~laplaceSmoother();

            // Member Functions
                //- new position is the average of the neighbouring vertices
                void optimizeLaplacian(const label nIterations = 1);
                void optimizeLaplacian
                (
                    const labelHashSet& badFaces,
                    const label nIterations = 1
                );

                //- new position of surface point is the average of
                //- the neighbouring surface vertices
                void optimizeSurfaceLaplacian
                (
                    const labelHashSet& badFaces,
                    const label nIterations = 1
                );

                //- new positions are the average of the centres of the cells
                //- adjacent to the vertex
                void optimizeLaplacianPC(const label nIterations = 1);
                void optimizeLaplacianPC
                (
                    const labelHashSet& badFaces,
                    const label nIterations = 1
                );

                //- new positions are the average of the centres of the cells
                //- adjacent to the vertex weighted by cell volumes
                void optimizeLaplacianWPC(const label nIterations = 1);
                void optimizeLaplacianWPC
                (
                    const labelHashSet& badFaces,
                    const label nIterations = 1
                );
        };

        //- Disallow default bitwise copy construct
        meshOptimizer(const meshOptimizer&);

        //- Disallow default bitwise assignment
        void operator=(const meshOptimizer&);

    // enumerators
        enum vertexType_
        {
            INSIDE = 1,
            BOUNDARY = 2,
            EDGE = 4,
            CORNER = 8,
            PARALLELBOUNDARY = 16,
            LOCKED = 32
        };

public:

    // Constructors

        //- Construct from mesh
        meshOptimizer(polyMeshGen& mesh);


    // Destructor

        ~meshOptimizer();

    // Member Functions
        //- Flag stopping the meshing process if it is not possible
        //- to untangle the surface without sacrificing geometry constraints
        //- Points which must be moved away from the required position are
        //- stored into a point subset
        void enforceConstraints(const word subsetName="badPoints");

        //- lock the cells which shall not be modified
        template<class labelListType>
        inline void lockCells(const labelListType&);

        //- lock cells which shall not be modified
        //- contained inside the specified subset
        void lockCellsInSubset(const word& subsetName);

        //- lock the faces whih shall not be modified
        template<class labelListType>
        inline void lockFaces(const labelListType&);

        //- lock faces which shall not be modified
        //- stored in a face subset
        void lockFacesInSubset(const word& subsetName);

        //- lock points which shall not be moved
        template<class labelListType>
        inline void lockPoints(const labelListType&);

        //- lock points which shall not be modified
        //- given as a point subset
        void lockPointsInSubset(const word& subsetName);

        //- reset to default constraints
        void removeUserConstraints();

        //- smooth surface vertices
        void optimizeSurface(const meshOctree&);

        //- performs mesh untangling based on detected negative normals
        //- a global iteration consists of maxNumIterations iterations when
        //- only internal points are moved and maxNumSurfaceIterations when
        //- surface points are moved
        void untangleMeshFV
        (
            const label maxNumGlobalIterations = 10,
            const label maxNumIterations = 50,
            const label maxNumSurfaceIterations = 2,
            const bool relaxedCheck = false
        );

        //- performs optimisation of boundary layer cells
        //- it is applied to the first boundary layer, only
        void optimizeBoundaryLayer(const bool addBufferLayer = true);

        //- performs untangling of boundary layer cells
        //- if the user requests for it
        void untangleBoundaryLayer();

        //- performs mesh optimisation for faces with high non-orthogonality
        //- and skewness
        void optimizeLowQualityFaces(const label maxNumIterations = 10);

        //- perform optimization of cells near the boundary
        //- the use shall specify the number of iterations and the number
        //- of cell layers which shall be taken into account
        void optimizeMeshNearBoundaries
        (
            const label maxNumIterations = 10,
            const label numLayersOfCells = 2
        );

        //- final optimisation
        void optimizeMeshFV
        (
            const label numLaplaceIterations = 5,
            const label maxNumGlobalIterations = 10,
            const label maxNumIterations = 50,
            const label maxNumSurfaceIterations = 2
        );

        //- greedy optimisation until the mesh can be improved
        void optimizeMeshFVBestQuality
        (
            const label maxNumIterations = 50,
            const scalar threshold = 0.1
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class labelListType>
inline void meshOptimizer::lockCells(const labelListType& l)
{
    boolList lockedFace(mesh_.faces().size(), false);
    const cellListPMG& cells = mesh_.cells();
    forAll(l, lcI)
    {
        const cell& c = cells[l[lcI]];

        forAll(c, fI)
            lockedFace[c[fI]] = true;
    }

    if( Pstream::parRun() )
    {
        const PtrList<processorBoundaryPatch>& procBoundaries =
            mesh_.procBoundaries();

        forAll(procBoundaries, patchI)
        {
            labelLongList dataToSend;

            const label start = procBoundaries[patchI].patchStart();
            const label end = start+procBoundaries[patchI].patchSize();

            for(label faceI=start;faceI<end;++faceI)
                if( lockedFace[faceI] )
                    dataToSend.append(faceI-start);

            OPstream toOtherProc
            (
                Pstream::blocking,
                procBoundaries[patchI].neiProcNo(),
                dataToSend.byteSize()
            );

            toOtherProc << dataToSend;
        }

        forAll(procBoundaries, patchI)
        {
            const label start = procBoundaries[patchI].patchStart();

            IPstream fromOtherProc
            (
                Pstream::blocking,
                procBoundaries[patchI].neiProcNo()
            );

            labelList receivedData;
            fromOtherProc >> receivedData;

            forAll(receivedData, i)
                lockedFace[start+receivedData[i]];
        }
    }

    //- Finally, mark locked points and faces
    const faceListPMG& faces = mesh_.faces();
    forAll(lockedFace, faceI)
    {
        if( lockedFace[faceI] )
        {
            lockedFaces_.append(faceI);

            const face& f = faces[faceI];

            forAll(f, pI)
                vertexLocation_[f[pI]] |= LOCKED;
        }
    }

    # ifdef DEBUGSmoothing
    const label lockedFacesI = mesh_.addFaceSubset("lockedFaces");
    forAll(lockedFaces_, lfI)
        mesh_.addFaceToSubset(lockedFacesI, lockedFaces_[lfI]);

    const label lockPointsI = mesh_.addPointSubset("lockedPoints");
    forAll(vertexLocation_, pointI)
        if( vertexLocation_[pointI] & LOCKED )
            mesh_.addPointToSubset(lockPointsI, pointI);
    # endif
}

template<class labelListType>
inline void meshOptimizer::lockFaces(const labelListType& lf)
{
    boolList lockedFace(mesh_.faces().size(), false);
    forAll(lf, lfI)
    {
        lockedFace[lf[lfI]] = true;
    }

    if( Pstream::parRun() )
    {
        const PtrList<processorBoundaryPatch>& procBoundaries =
            mesh_.procBoundaries();

        forAll(procBoundaries, patchI)
        {
            labelLongList dataToSend;

            const label start = procBoundaries[patchI].patchStart();
            const label end = start+procBoundaries[patchI].patchSize();

            for(label faceI=start;faceI<end;++faceI)
                if( lockedFace[faceI] )
                    dataToSend.append(faceI-start);

            OPstream toOtherProc
            (
                Pstream::blocking,
                procBoundaries[patchI].neiProcNo(),
                dataToSend.byteSize()
            );

            toOtherProc << dataToSend;
        }

        forAll(procBoundaries, patchI)
        {
            const label start = procBoundaries[patchI].patchStart();

            IPstream fromOtherProc
            (
                Pstream::blocking,
                procBoundaries[patchI].neiProcNo()
            );

            labelList receivedData;
            fromOtherProc >> receivedData;

            forAll(receivedData, i)
                lockedFace[start+receivedData[i]];
        }
    }

    //- Finally, mark locked points and faces
    const faceListPMG& faces = mesh_.faces();
    forAll(lockedFace, faceI)
    {
        if( lockedFace[faceI] )
        {
            lockedFaces_.append(faceI);

            const face& f = faces[faceI];

            forAll(f, pI)
                vertexLocation_[f[pI]] |= LOCKED;
        }
    }

    # ifdef DEBUGSmoothing
    const label lockedFacesI = mesh_.addFaceSubset("lockedFaces");
    forAll(lockedFaces_, lfI)
        mesh_.addFaceToSubset(lockedFacesI, lockedFaces_[lfI]);

    const label lockPointsI = mesh_.addPointSubset("lockedPoints");
    forAll(vertexLocation_, pointI)
        if( vertexLocation_[pointI] & LOCKED )
            mesh_.addPointToSubset(lockPointsI, pointI);
    # endif
}

template<class labelListType>
inline void meshOptimizer::lockPoints(const labelListType& lp)
{
    forAll(lp, lpI)
        vertexLocation_[lp[lpI]] |= LOCKED;

    if( Pstream::parRun() )
    {
        const PtrList<processorBoundaryPatch>& procBoundaries =
            mesh_.procBoundaries();
        const faceListPMG& faces = mesh_.faces();

        forAll(procBoundaries, patchI)
        {
            labelLongList dataToSend;

            const label start = procBoundaries[patchI].patchStart();
            const label end = start+procBoundaries[patchI].patchSize();

            for(label faceI=start;faceI<end;++faceI)
            {
                const face& f = faces[faceI];

                forAll(f, pI)
                {
                    if( vertexLocation_[pI] & LOCKED )
                    {
                        // send the face lable and the location in the face
                        dataToSend.append(faceI-start);
                        dataToSend.append((f.size()-pI)%f.size());
                    }
                }
            }

            OPstream toOtherProc
            (
                Pstream::blocking,
                procBoundaries[patchI].neiProcNo(),
                dataToSend.byteSize()
            );

            toOtherProc << dataToSend;
        }

        forAll(procBoundaries, patchI)
        {
            const label start = procBoundaries[patchI].patchStart();

            IPstream fromOtherProc
            (
                Pstream::blocking,
                procBoundaries[patchI].neiProcNo()
            );

            labelList receivedData;
            fromOtherProc >> receivedData;

            label counter(0);
            while( counter < receivedData.size() )
            {
                const face& f = faces[start+receivedData[counter++]];
                vertexLocation_[f[receivedData[counter++]]] |= LOCKED;
            }
        }
    }

    # ifdef DEBUGSmoothing
    const label lockPointsI = mesh_.addPointSubset("lockedPoints");
    forAll(vertexLocation_, pointI)
        if( vertexLocation_[pointI] & LOCKED )
            mesh_.addPointToSubset(lockPointsI, pointI);
    # endif
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
