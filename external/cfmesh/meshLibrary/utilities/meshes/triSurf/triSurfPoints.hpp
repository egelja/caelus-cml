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
    triSurfPoints

Description
    Point coordinates information for triangulated surface

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    triSurfPoints.C

\*---------------------------------------------------------------------------*/

#ifndef triSurfPoints_HPP
#define triSurfPoints_HPP

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "meshSubset.hpp"
#include "pointField.hpp"
#include "DynList.hpp"
#include "Map.hpp"

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class triSurfPoints
{
protected:

    // Protected data
        //- list of vertices
        pointField points_;

        //- map of point subsets
        Map<meshSubset> pointSubsets_;

    // Disallow bitwise assignment
        void operator=(const triSurfPoints&);

        triSurfPoints(const triSurfPoints&);

public:

    // Constructors
        //- Null constructor
        triSurfPoints();

        //- Construct from vertices
        triSurfPoints(const pointField& points);

    // Destructor
        ~triSurfPoints();

    // Member functions
        //- return the number of points
        inline label nPoints() const;

        //- access to points
        inline const pointField& points() const;

        //- append a vertex to the end of the list
        inline void appendVertex(const point& p);

        //- point subsets
        label addPointSubset(const word&);
        void removePointSubset(const label);
        word pointSubsetName(const label) const;
        label pointSubsetIndex(const word&) const;
        inline void addPointToSubset(const label, const label);
        inline void removePointFromSubset(const label, const label);
        inline void pointInSubsets(const label, DynList<label>&) const;
        inline void pointSubsetIndices(DynList<label>&) const;
        template<class ListType>
        inline void pointsInSubset(const label, ListType&) const;
        template<class ListType>
        inline void updatePointSubsets(const ListType&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline label triSurfPoints::nPoints() const
{
    return points_.size();
}

inline const pointField& triSurfPoints::points() const
{
    return points_;
}

inline void triSurfPoints::appendVertex(const point& p)
{
    const label s = points_.size();
    points_.setSize(s+1);
    points_[s] = p;
}

inline void triSurfPoints::addPointToSubset
(
    const label setI,
    const label pointI
)
{
    Map<meshSubset>::iterator it = pointSubsets_.find(setI);
    if( it == pointSubsets_.end() )
        return;

    it().addElement(pointI);
}

inline void triSurfPoints::removePointFromSubset
(
    const label setI,
    const label pointI
)
{
    Map<meshSubset>::iterator it = pointSubsets_.find(setI);
    if( it == pointSubsets_.end() )
        return;

    it().removeElement(pointI);
}

inline void triSurfPoints::pointInSubsets
(
    const label pointI,
    DynList<label>& pointSubsets
) const
{
    pointSubsets.clear();

    forAllConstIter
    (
        Map<meshSubset>,
        pointSubsets_,
        it
    )
    {
        if( it().contains(pointI) )
            pointSubsets.append(it.key());
    }
}

inline void triSurfPoints::pointSubsetIndices(DynList<label>& indices) const
{
    indices.clear();

    forAllConstIter
    (
        Map<meshSubset>,
        pointSubsets_,
        it
    )
        indices.append(it.key());
}

template<class ListType>
inline void triSurfPoints::pointsInSubset
(
    const label setI,
    ListType& pointLabels
) const
{
    pointLabels.clear();

    Map<meshSubset>::const_iterator it =
        pointSubsets_.find(setI);
    if( it == pointSubsets_.end() )
        return;

    it().containedElements(pointLabels);
}

template<class ListType>
inline void triSurfPoints::updatePointSubsets(const ListType& newNodeLabels)
{
    for
    (
        Map<meshSubset>::iterator it=pointSubsets_.begin();
        it!=pointSubsets_.end();
        ++it
    )
        it().updateSubset(newNodeLabels);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
