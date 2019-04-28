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
    meshSurfaceEngine

Description
    Calculates surface of the mesh

SourceFiles
    meshSurfaceEngine.cpp

\*---------------------------------------------------------------------------*/

#ifndef meshSurfaceEngine_HPP
#define meshSurfaceEngine_HPP

#include "polyMeshGenModifier.hpp"
#include "SubList.hpp"
#include "boolList.hpp"
#include "DynList.hpp"
#include "VRWGraph.hpp"
#include "triangle.hpp"
#include "Map.hpp"

#include "demandDrivenData.hpp"

# ifdef USE_OMP
#include <omp.h>
# endif

// #define DEBUGSearch


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class meshSurfaceEngine Declaration
\*---------------------------------------------------------------------------*/

class meshSurfaceEngine
{
    // Private data
        //- reference to the mesh
        polyMeshGen& mesh_;

        //- number of active patch
        const label activePatch_;

        //- boundary points
        mutable labelList* boundaryPointsPtr_;

        //- boundary faces
        mutable faceList::subList* boundaryFacesPtr_;

        //- patches boundary faces are in
        mutable labelList* boundaryFacePatchPtr_;

        //- face owners
        mutable labelList* boundaryFaceOwnersPtr_;

        //- point faces addressing
        mutable VRWGraph* pointFacesPtr_;
        mutable VRWGraph* pointInFacePtr_;

        //- point-patches addressing
        mutable VRWGraph* pointPatchesPtr_;

        //- pointBoundaryPoint addressing
        mutable labelList* bppPtr_;

        //- point points addressing
        mutable VRWGraph* pointPointsPtr_;

        //- edges
        mutable edgeList* edgesPtr_;

        //- boundary point-edges addressing
        mutable VRWGraph* bpEdgesPtr_;

        //- edge faces addressing
        mutable VRWGraph* edgeFacesPtr_;

        //- face edges addressing
        mutable VRWGraph* faceEdgesPtr_;

        //- edge-patches addressing
        mutable VRWGraph* edgePatchesPtr_;

        //- face-faces addressing
        mutable VRWGraph* faceFacesPtr_;

        //- point normals
        mutable vectorField* pointNormalsPtr_;

        //- face normals
        mutable vectorField* faceNormalsPtr_;

        //- face centres
        mutable vectorField* faceCentresPtr_;

    // Private data for parallel execution

        //- global boundary point label
        mutable labelList* globalBoundaryPointLabelPtr_;

        //- global boundary point to local addressing
        mutable Map<label>* globalBoundaryPointToLocalPtr_;

        //- boundary point-processors addressing
        mutable VRWGraph* bpProcsPtr_;

        //- neighbour processors for communication when sending point data
        mutable DynList<label>* bpNeiProcsPtr_;

        //- global boundary edge label
        mutable labelList* globalBoundaryEdgeLabelPtr_;

        //- global boundary edge to local addressing
        mutable Map<label>* globalBoundaryEdgeToLocalPtr_;

        //- boundary edge-processors addressing
        mutable VRWGraph* beProcsPtr_;

        //- neighbour processors for communication when sending edge data
        mutable DynList<label>* beNeiProcsPtr_;

        //- processor containing other face and face-patch addressing
        mutable Map<label>* otherEdgeFaceAtProcPtr_;
        mutable Map<label>* otherEdgeFacePatchPtr_;

        //- global label for boundary faces
        mutable labelList* globalBoundaryFaceLabelPtr_;

    // Private member functions

        //- calculate boundary nodes, faces and addressing
        void calculateBoundaryFaces() const;
        void calculateBoundaryOwners() const;
        void calculateBoundaryNodes() const;

        void calculateBoundaryFacePatches() const;

        void calculatePointFaces() const;

        void calculatePointPatches() const;

        void calculatePointPoints() const;

        void calculatePointNormals() const;
        void calculateFaceNormals() const;
        void calculateFaceCentres() const;

        void updatePointNormalsAtProcBoundaries() const;

        void calculateEdgesAndAddressing() const;
        void calculateFaceEdgesAddressing() const;
        void calculateEdgeFacesAddressing() const;
        void calculateEdgePatchesAddressing() const;
        void calculateFaceFacesAddressing() const;

    // Private member functions for parallel runs

        void calcGlobalBoundaryPointLabels() const;
        void calcGlobalBoundaryEdgeLabels() const;
        void calcAddressingForProcEdges() const;
        void calcGlobalBoundaryFaceLabels() const;

    // Private member functions

        void clearOut();

        //- Disallow default bitwise copy construct
        meshSurfaceEngine(const meshSurfaceEngine&);

        //- Disallow default bitwise assignment
        void operator=(const meshSurfaceEngine&);

public:

        friend class meshSurfaceEngineModifier;

    // Constructors

        //- Construct from mesh
        meshSurfaceEngine(polyMeshGen& mesh);

        //- Construct from mesh and patch
        meshSurfaceEngine(polyMeshGen &mesh, const label patchI);

    // Destructor

        ~meshSurfaceEngine();

    // Member Functions
        inline const polyMeshGen& mesh() const;
        inline const pointFieldPMG& points() const;
        inline const faceListPMG& faces() const;
        inline const cellListPMG& cells() const;

        inline const labelList& boundaryPoints() const;

        inline const faceList::subList& boundaryFaces() const;

        //- patch label for each boundary face
        inline const labelList& boundaryFacePatches() const;

        inline const labelList& faceOwners() const;

        inline const VRWGraph& pointFaces() const;
        inline const VRWGraph& pointInFaces() const;

        //inline const VRWGraph& pointPatches() const;

        inline const VRWGraph& pointPoints() const;

        inline const vectorField& pointNormals() const;

        inline const vectorField& faceNormals() const;

        inline const vectorField& faceCentres() const;

        inline const labelList& bp() const;

        inline const edgeList& edges() const;

        inline const VRWGraph& boundaryPointEdges() const;

        inline const VRWGraph& edgeFaces() const;

        inline const VRWGraph& faceEdges() const;

        inline const VRWGraph& edgePatches() const;

        inline const VRWGraph& faceFaces() const;

    // Functions for parallel runs
        //- global boundary point label
        inline const labelList& globalBoundaryPointLabel() const;

        //- global point label to local label. Only for processors points
        inline const Map<label>& globalToLocalBndPointAddressing() const;

        //- processors which contain the vertex
        inline const VRWGraph& bpAtProcs() const;

        //- communication matrix for sending point data
        inline const DynList<label>& bpNeiProcs() const;

        //- global boundary edge label
        inline const labelList& globalBoundaryEdgeLabel() const;

        //- global boundary edge label to local label. Only for processor edges
        inline const Map<label>& globalToLocalBndEdgeAddressing() const;

        //- processors which contain the edges
        inline const VRWGraph& beAtProcs() const;

        //- communication matrix for sending edge data
        inline const DynList<label>& beNeiProcs() const;

        //- patch label and processor label at which the other surface face
        //- sharing processor edge is located. Only for processor edges
        inline const Map<label>& otherEdgeFaceAtProc() const;
        inline const Map<label>& otherEdgeFacePatch() const;

        //- global boundary face label
        inline const labelList& globalBoundaryFaceLabel() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline const polyMeshGen& meshSurfaceEngine::mesh() const
{
    return mesh_;
}

inline const pointFieldPMG& meshSurfaceEngine::points() const
{
    return mesh_.points();
}

inline const faceListPMG& meshSurfaceEngine::faces() const
{
    return mesh_.faces();
}

inline const cellListPMG& meshSurfaceEngine::cells() const
{
    return mesh_.cells();
}

inline const labelList& meshSurfaceEngine::bp() const
{
    if( !bppPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateBoundaryFaces();
        calculateBoundaryNodes();
    }

    return *bppPtr_;
}

inline const labelList& meshSurfaceEngine::boundaryPoints() const
{
    if( !boundaryPointsPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateBoundaryNodes();
    }

    return *boundaryPointsPtr_;
}

inline const faceList::subList& meshSurfaceEngine::boundaryFaces() const
{
    if( !boundaryFacesPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateBoundaryFaces();
    }

    return *boundaryFacesPtr_;
}

inline const labelList& meshSurfaceEngine::boundaryFacePatches() const
{
    if( !boundaryFacePatchPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateBoundaryFacePatches();
    }

    return *boundaryFacePatchPtr_;
}

inline const labelList& meshSurfaceEngine::faceOwners() const
{
    if( !boundaryFaceOwnersPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateBoundaryOwners();
    }

    return *boundaryFaceOwnersPtr_;
}

inline const VRWGraph& meshSurfaceEngine::pointFaces() const
{
    if( !pointFacesPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculatePointFaces();
    }

    return *pointFacesPtr_;
}

inline const VRWGraph& meshSurfaceEngine::pointInFaces() const
{
    if( !pointInFacePtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculatePointFaces();
    }

    return *pointInFacePtr_;
}
/*
inline const VRWGraph& meshSurfaceEngine::pointPatches() const
{
    if( !pointPatchesPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculatePointPatches();
    }

    return *pointPatchesPtr_;
}
*/

inline const VRWGraph& meshSurfaceEngine::pointPoints() const
{
    if( !pointPointsPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculatePointPoints();
    }

    return *pointPointsPtr_;
}

inline const vectorField& meshSurfaceEngine::pointNormals() const
{
    if( !pointNormalsPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculatePointNormals();
    }

    return *pointNormalsPtr_;
}

inline const vectorField& meshSurfaceEngine::faceNormals() const
{
    if( !faceNormalsPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateFaceNormals();
    }

    return *faceNormalsPtr_;
}

inline const vectorField& meshSurfaceEngine::faceCentres() const
{
    if( !faceCentresPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateFaceCentres();
    }

    return *faceCentresPtr_;
}

inline const edgeList& meshSurfaceEngine::edges() const
{
    if( !edgesPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateEdgesAndAddressing();
    }

    return *edgesPtr_;
}

inline const VRWGraph& meshSurfaceEngine::boundaryPointEdges() const
{
    if( !bpEdgesPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateEdgesAndAddressing();
    }

    return *bpEdgesPtr_;
}

inline const VRWGraph& meshSurfaceEngine::edgeFaces() const
{
    if( !edgeFacesPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateEdgeFacesAddressing();
    }

    return *edgeFacesPtr_;
}

inline const VRWGraph& meshSurfaceEngine::faceEdges() const
{
    if( !faceEdgesPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateFaceEdgesAddressing();
    }

    return *faceEdgesPtr_;
}

inline const VRWGraph& meshSurfaceEngine::edgePatches() const
{
    if( !edgePatchesPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateEdgePatchesAddressing();
    }

    return *edgePatchesPtr_;
}

inline const VRWGraph& meshSurfaceEngine::faceFaces() const
{
    if( !faceFacesPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateFaceFacesAddressing();
    }

    return *faceFacesPtr_;
}

inline const labelList& meshSurfaceEngine::globalBoundaryPointLabel() const
{
    if( !globalBoundaryPointLabelPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcGlobalBoundaryPointLabels();
    }

    return *globalBoundaryPointLabelPtr_;
}

inline const Map<label>&
meshSurfaceEngine::globalToLocalBndPointAddressing() const
{
    if( !globalBoundaryPointToLocalPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcGlobalBoundaryPointLabels();
    }

    return *globalBoundaryPointToLocalPtr_;
}

inline const VRWGraph& meshSurfaceEngine::bpAtProcs() const
{
    if( !globalBoundaryPointLabelPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcGlobalBoundaryPointLabels();
    }

    return *bpProcsPtr_;
}

inline const DynList<label>& meshSurfaceEngine::bpNeiProcs() const
{
    if( !bpNeiProcsPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcGlobalBoundaryPointLabels();
    }

    return *bpNeiProcsPtr_;
}

inline const labelList& meshSurfaceEngine::globalBoundaryEdgeLabel() const
{
    if( !globalBoundaryEdgeLabelPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcGlobalBoundaryEdgeLabels();
    }

    return *globalBoundaryEdgeLabelPtr_;
}

inline const Map<label>&
meshSurfaceEngine::globalToLocalBndEdgeAddressing() const
{
    if( !globalBoundaryEdgeToLocalPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcGlobalBoundaryEdgeLabels();
    }

    return *globalBoundaryEdgeToLocalPtr_;
}

inline const VRWGraph& meshSurfaceEngine::beAtProcs() const
{
    if( !globalBoundaryEdgeLabelPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcGlobalBoundaryEdgeLabels();
    }

    return *beProcsPtr_;
}

inline const DynList<label>& meshSurfaceEngine::beNeiProcs() const
{
    if( !beNeiProcsPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcGlobalBoundaryEdgeLabels();
    }

    return *beNeiProcsPtr_;
}

inline const Map<label>& meshSurfaceEngine::otherEdgeFaceAtProc() const
{
    if( !otherEdgeFaceAtProcPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcAddressingForProcEdges();
    }

    return *otherEdgeFaceAtProcPtr_;
}

inline const Map<label>& meshSurfaceEngine::otherEdgeFacePatch() const
{
    if( !otherEdgeFacePatchPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcAddressingForProcEdges();
    }

    return *otherEdgeFacePatchPtr_;
}

inline const labelList& meshSurfaceEngine::globalBoundaryFaceLabel() const
{
    if( !globalBoundaryFaceLabelPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcGlobalBoundaryFaceLabels();
    }

    return *globalBoundaryFaceLabelPtr_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
