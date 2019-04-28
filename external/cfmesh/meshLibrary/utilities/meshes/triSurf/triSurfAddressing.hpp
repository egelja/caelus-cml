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
    triSurfAddressing

Description
    Provides topology addressing of the triangulated surface

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    triSurfAddressing.cpp

\*---------------------------------------------------------------------------*/

#ifndef triSurfAddressing_HPP
#define triSurfAddressing_HPP

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "VRWGraph.hpp"
#include "edgeLongList.hpp"
#include "triSurfFacets.hpp"
#include "labelledTri.hpp"
#include "DynList.hpp"

#include <map>

# ifdef USE_OMP
#include <omp.h>
# endif

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class triSurfAddressing
{
    // Private data
        //- const reference to the points
        const pointField& points_;

    // Topology addressing
        //- const reference to the facets
        const LongList<labelledTri>& facets_;

        //- facets connected to a point
        mutable VRWGraph* pointFacetsPtr_;

        //- edges in the triangulation
        mutable edgeLongList* edgesPtr_;

        //- labels of edges in the triangles
        mutable VRWGraph* facetEdgesPtr_;

        //- labels of triangles connected to an edge
        mutable VRWGraph* edgeFacetsPtr_;

        //- labels of edges connected to a point
        mutable VRWGraph* pointEdgesPtr_;

        //- facets connected tp a facet via edges
        mutable VRWGraph* facetFacetsEdgesPtr_;

    // Geometry data
        //- point normals
        mutable vectorField* pointNormalsPtr_;

        //- face normals
        mutable vectorField* facetNormalsPtr_;

        //- face centres
        mutable vectorField* facetCentresPtr_;

    // Private member functions
        //- calculate point-facets addressing
        void calculatePointFacets() const;

        //- calculate edges, facet-edges and edge-facets addressing
        void calculateEdges() const;

        //- calculate facet-edges addresing
        void calculateFacetEdges() const;

        //- calculate edge-facets addressing
        void calculateEdgeFacets() const;

        //- calculate point-edges addressing
        void calculatePointEdges() const;

        //- calculate facet-faceys addressing
        void calculateFacetFacetsEdges() const;

        //- calculate point normals
        void calculatePointNormals() const;

        //- calculate normals of facets
        void calculateFacetNormals() const;

        //- calculate centres of facets
        void calculateFacetCentres() const;

    // Disallow bitwise assignment
        void operator=(const triSurfAddressing&);

        triSurfAddressing(const triSurfAddressing&);

public:

    // Constructors
        //- Construct from surface triangles
        triSurfAddressing
        (
            const pointField& points,
            const LongList<labelledTri>& triangles
        );

    // Destructor
        ~triSurfAddressing();

    // Member functions
        //- return point-facets addressing
        inline const VRWGraph& pointFacets() const;

        //- return edges
        inline const LongList<edge>& edges() const;

        //- return facet-edges addressing
        inline const VRWGraph& facetEdges() const;

        //- return edge-facets addressing
        inline const VRWGraph& edgeFacets() const;

        //- return point-edges addressing
        inline const VRWGraph& pointEdges() const;

        //- return facet-facets addressing
        inline const VRWGraph& facetFacets() const;

        //- return point normals
        inline const pointField& pointNormals() const;

        //- return normals of facets
        inline const vectorField& facetNormals() const;

        //- return centres of facets
        inline const vectorField& facetCentres() const;

        //- delete all data
        void clearAddressing();

        //- delete geometry data
        void clearGeometry();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline const VRWGraph& triSurfAddressing::pointFacets() const
{
    if( !pointFacetsPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
              << "Cannot calculate pointFacets" << abort(FatalError);
        # endif

        calculatePointFacets();
    }

    return *pointFacetsPtr_;
}

inline const edgeLongList& triSurfAddressing::edges() const
{
    if( !edgesPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
              << "Cannot calculate edges" << abort(FatalError);
        # endif

        calculateEdges();
    }

    return *edgesPtr_;
}

inline const VRWGraph& triSurfAddressing::facetEdges() const
{
    if( !facetEdgesPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
              << "Cannot calculate facetEdges" << abort(FatalError);
        # endif

        calculateFacetEdges();
    }

    return *facetEdgesPtr_;
}

inline const VRWGraph& triSurfAddressing::edgeFacets() const
{
    if( !edgeFacetsPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
              << "Cannot calculate edgeFacets" << abort(FatalError);
        # endif

        calculateEdgeFacets();
    }

    return *edgeFacetsPtr_;
}

inline const VRWGraph& triSurfAddressing::pointEdges() const
{
    if( !pointEdgesPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
              << "Cannot calculate pointEdges" << abort(FatalError);
        # endif

        calculatePointEdges();
    }

    return *pointEdgesPtr_;
}

inline const VRWGraph& triSurfAddressing::facetFacets() const
{
    if( !facetFacetsEdgesPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
              << "Cannot calculate facetFacets" << abort(FatalError);
        # endif

        calculateFacetFacetsEdges();
    }

    return *facetFacetsEdgesPtr_;
}

inline const vectorField& triSurfAddressing::pointNormals() const
{
    if( !pointNormalsPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
              << "Cannot calculate pointNormals" << abort(FatalError);
        # endif

        calculatePointNormals();
    }

    return *pointNormalsPtr_;
}

inline const vectorField& triSurfAddressing::facetNormals() const
{
    if( !facetNormalsPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
              << "Cannot calculate facetNormals" << abort(FatalError);
        # endif

        calculateFacetNormals();
    }

    return *facetNormalsPtr_;
}

inline const vectorField& triSurfAddressing::facetCentres() const
{
    if( !facetCentresPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
              << "Cannot calculate facetCentres" << abort(FatalError);
        # endif

        calculateFacetCentres();
    }

    return *facetCentresPtr_;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
