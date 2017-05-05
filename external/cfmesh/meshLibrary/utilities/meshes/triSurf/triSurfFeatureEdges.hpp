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
    triSurfFeatureEdges

Description
    User-selected list of feature edges used in the meshing process

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    triSurfFeatureEdges.C

\*---------------------------------------------------------------------------*/

#ifndef triSurfFeatureEdges_HPP
#define triSurfFeatureEdges_HPP

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "edgeLongList.hpp"
#include "meshSubset.hpp"
#include "Map.hpp"

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class triSurfFeatureEdges
{
protected:

    // Protected data
        //- list of feature edges
        edgeLongList featureEdges_;

        //- map of edge subsets
        Map<meshSubset> featureEdgeSubsets_;

    // Disallow bitwise assignment
        void operator=(const triSurfFeatureEdges&);

        triSurfFeatureEdges(const triSurfFeatureEdges&);

public:

    // Constructors
        //- Null constructor
        triSurfFeatureEdges();

        //- Construct from feature edges
        triSurfFeatureEdges(const edgeLongList& triangles);

    // Destructor
        ~triSurfFeatureEdges();

    // Member functions
        //- return the number of feature edges
        inline label nFeatureEdges() const;

        //- access to feature edges
        inline const edgeLongList& featureEdges() const;

        //- append an edge to the end of the list
        inline void appendFeatureEdge(const edge& e);

        //- point subsets
        label addEdgeSubset(const word&);
        void removeEdgeSubset(const label);
        word edgeSubsetName(const label) const;
        label edgeSubsetIndex(const word&) const;
        inline void addEdgeToSubset(const label, const label);
        inline void removeEdgeFromSubset(const label, const label);
        inline void edgeInSubsets(const label, DynList<label>&) const;
        inline void edgeSubsetIndices(DynList<label>&) const;
        template<class ListType>
        inline void edgesInSubset(const label, ListType&) const;
        template<class ListType>
        inline void updateEdgeSubsets(const ListType&);

    // Operators
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline label triSurfFeatureEdges::nFeatureEdges() const
{
    return featureEdges_.size();
}

inline const edgeLongList& triSurfFeatureEdges::featureEdges() const
{
    return featureEdges_;
}

inline void triSurfFeatureEdges::appendFeatureEdge(const edge& e)
{
    featureEdges_.append(e);
}

inline void triSurfFeatureEdges::addEdgeToSubset
(
    const label setI,
    const label eI
)
{
    Map<meshSubset>::iterator it = featureEdgeSubsets_.find(setI);
    if( it == featureEdgeSubsets_.end() )
        return;

    it().addElement(eI);
}

inline void triSurfFeatureEdges::removeEdgeFromSubset
(
    const label setI,
    const label eI
)
{
    Map<meshSubset>::iterator it = featureEdgeSubsets_.find(setI);
    if( it == featureEdgeSubsets_.end() )
        return;

    it().removeElement(eI);
}

inline void triSurfFeatureEdges::edgeInSubsets
(
    const label eI,
    DynList<label>& edgeSubsets
) const
{
    edgeSubsets.clear();

    forAllConstIter
    (
        Map<meshSubset>,
        featureEdgeSubsets_,
        it
    )
    {
        if( it().contains(eI) )
            edgeSubsets.append(it.key());
    }
}

inline void triSurfFeatureEdges::edgeSubsetIndices
(
    DynList<label>& indices
) const
{
    indices.clear();

    forAllConstIter
    (
        Map<meshSubset>,
        featureEdgeSubsets_,
        it
    )
        indices.append(it.key());
}

template<class ListType>
inline void triSurfFeatureEdges::edgesInSubset
(
    const label setI,
    ListType& edgeLabels
) const
{
    edgeLabels.clear();

    Map<meshSubset>::const_iterator it = featureEdgeSubsets_.find(setI);
    if( it == featureEdgeSubsets_.end() )
        return;

    it().containedElements(edgeLabels);
}

template<class ListType>
inline void triSurfFeatureEdges::updateEdgeSubsets
(
    const ListType& newEdgeLabels
)
{
    for
    (
        Map<meshSubset>::iterator it=featureEdgeSubsets_.begin();
        it!=featureEdgeSubsets_.end();
        ++it
    )
        it().updateSubset(newEdgeLabels);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
