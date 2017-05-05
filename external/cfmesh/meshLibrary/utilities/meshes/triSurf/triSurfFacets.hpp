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
    triSurfFacets

Description
    Facets for the triangulated surface

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    triSurfFacets.cpp

\*---------------------------------------------------------------------------*/

#ifndef triSurfFacets_HPP
#define triSurfFacets_HPP

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "triSurfAddressing.hpp"
#include "meshSubset.hpp"
#include "geometricSurfacePatchList.hpp"
#include "LongList.hpp"
#include "labelledTri.hpp"
#include "DynList.hpp"
#include "Map.hpp"

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class triSurfFacets
{
protected:

    // Protected data
        //- list of triangles
        LongList<labelledTri> triangles_;

        //- list of boundary patches and their properties
        geometricSurfacePatchList patches_;

        //- map of point subsets
        Map<meshSubset> facetSubsets_;

    // Disallow bitwise assignment
        void operator=(const triSurfFacets&);

        triSurfFacets(const triSurfFacets&);

public:

    // Constructors
        //- Null constructor
        triSurfFacets();

        //- Construct from components without the boundary
        triSurfFacets(const LongList<labelledTri>& triangles);

        //- Construct from components
        triSurfFacets
        (
            const LongList<labelledTri>& triangles,
            const geometricSurfacePatchList& patches
        );

    // Destructor
        ~triSurfFacets();

    // Member functions
        //- return the number of triangles
        inline label size() const;

        //- access to facets
        inline const LongList<labelledTri>& facets() const;

        //- access to patches
        inline const geometricSurfacePatchList& patches() const;

        //- return list of patches in the boundary
        wordList patchNames() const;

        //- return a list of patch indices corresponding to the given
        // name, expanding regular expressions
        labelList findPatches(const word& patchName) const;

        //- append a triangle to the end of the list
        inline void appendTriangle(const labelledTri& tria);

        //- point subsets
        label addFacetSubset(const word&);
        void removeFacetSubset(const label);
        word facetSubsetName(const label) const;
        label facetSubsetIndex(const word&) const;
        inline void addFacetToSubset(const label, const label);
        inline void removeFacetFromSubset(const label, const label);
        inline void facetInSubsets(const label, DynList<label>&) const;
        inline void facetSubsetIndices(DynList<label>&) const;
        template<class ListType>
        inline void facetsInSubset(const label, ListType&) const;
        template<class ListType>
        inline void updateFacetsSubsets(const ListType&);

    // Operators

        //- access to a triangle
        inline const labelledTri& operator[](const label) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline label triSurfFacets::size() const
{
    return triangles_.size();
}

inline const LongList<labelledTri>& triSurfFacets::facets() const
{
    return triangles_;
}

inline const geometricSurfacePatchList& triSurfFacets::patches() const
{
    return patches_;
}

inline void triSurfFacets::appendTriangle(const labelledTri& tri)
{
    triangles_.append(tri);
}

inline void triSurfFacets::addFacetToSubset
(
    const label setI,
    const label triI
)
{
    Map<meshSubset>::iterator it = facetSubsets_.find(setI);
    if( it == facetSubsets_.end() )
        return;

    it().addElement(triI);
}

inline void triSurfFacets::removeFacetFromSubset
(
    const label setI,
    const label triI
)
{
    Map<meshSubset>::iterator it = facetSubsets_.find(setI);
    if( it == facetSubsets_.end() )
        return;

    it().removeElement(triI);
}

inline void triSurfFacets::facetInSubsets
(
    const label triI,
    DynList<label>& facetSubsets
) const
{
    facetSubsets.clear();

    forAllConstIter
    (
        Map<meshSubset>,
        facetSubsets_,
        it
    )
    {
        if( it().contains(triI) )
            facetSubsets.append(it.key());
    }
}

inline void triSurfFacets::facetSubsetIndices(DynList<label>& indices) const
{
    indices.clear();

    forAllConstIter
    (
        Map<meshSubset>,
        facetSubsets_,
        it
    )
        indices.append(it.key());
}

template<class ListType>
inline void triSurfFacets::facetsInSubset
(
    const label setI,
    ListType& facetsLabels
) const
{
    facetsLabels.clear();

    Map<meshSubset>::const_iterator it = facetSubsets_.find(setI);
    if( it == facetSubsets_.end() )
        return;

    it().containedElements(facetsLabels);
}

template<class ListType>
inline void triSurfFacets::updateFacetsSubsets(const ListType& newFacetsLabels)
{
    for
    (
        Map<meshSubset>::iterator it=facetSubsets_.begin();
        it!=facetSubsets_.end();
        ++it
    )
        it().updateSubset(newFacetsLabels);
}

inline const labelledTri& triSurfFacets::operator[](const label triI) const
{
    return triangles_[triI];
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
