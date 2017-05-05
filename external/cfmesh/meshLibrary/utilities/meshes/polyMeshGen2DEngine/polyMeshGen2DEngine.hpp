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
    polyMeshGen2DEngine

Description
    A simple engine which provides topological information of a 2D mesh
    and allows for maintaining consistency

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    polyMeshGen2DEngine.cpp

\*---------------------------------------------------------------------------*/

#ifndef polyMeshGen2DEngine_HPP
#define polyMeshGen2DEngine_HPP

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "polyMeshGen.hpp"
#include "boolList.hpp"
#include "labelList.hpp"
#include "boundBox.hpp"
#include "demandDrivenData.hpp"

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class polyMeshGen2DEngine
{
    // Private data
        //- const reference to the mesh
        const polyMeshGen& mesh_;

        //- bounding box
        boundBox bb_;

        //- which faces are not in the x-y plane
        mutable boolList* activeFacePtr_;

        //- labels of active faces
        mutable labelList* activeFaceLabelsPtr_;

        //- which points are in the x-y plane and at smallest z
        mutable boolList* zMinPointPtr_;

        //- labels of points at minimum z
        mutable labelList* zMinPointLabelsPtr_;

        //- labels of offset points of zMin points
        mutable labelList* zMinToZMaxPtr_;

        //- which points are in the x-y plane and at largest z
        mutable boolList* zMaxPointPtr_;

        //- labels of points at maximum z
        mutable labelList* zMaxPointLabelsPtr_;

        //- labels of zMin points of zMax points
        mutable labelList* zMaxToZMinPtr_;

    // Private member functions
        //- find active faces
        void findActiveFaces() const;

        //- find active face labels
        void findActiveFaceLabels() const;

        //- find points at minimum z
        void findZMinPoints() const;

        //- find labels of points at minimum z
        void findZMinPointLabels() const;

        //- find offset point to each zMin point
        void findZMinOffsetPoints() const;

        //- find points at maximum z
        void findZMaxPoints() const;

        //- find labels of points at minimum z
        void findZMaxPointLabels() const;

        //- find offset point to each zMin point
        void findZMaxOffsetPoints() const;

        //- disallow bitwise assignment
        void operator=(const polyMeshGen2DEngine&);

        //- disallow copy construct
        polyMeshGen2DEngine(const polyMeshGen2DEngine&);

public:

    // Constructors
        //- Construct from the const reference to the mesh
        polyMeshGen2DEngine(const polyMeshGen& mesh);

    // Destructor
        ~polyMeshGen2DEngine();

    // Member functions
        //- const access to active faces
        inline const boolList& activeFace() const;

        //- labels of active faces
        inline const labelList& activeFaceLabels() const;

        //- const access to an array of booleans which is true for points
        //- in the x-y plane with the smallest z coordinate
        inline const boolList& zMinPoints() const;

        //- labels of points in the x-y with the smallest z coordinate
        inline const labelList& zMinPointLabels() const;

        //- offset points of points at minimum z
        inline const labelList& zMinToZMax() const;

        //- const access to an array of booleans which is true for points
        //- in the x-y plane with the largest z coordinate
        inline const boolList& zMaxPoints() const;

        //- labels of points in the x-y with the largest z coordinate
        inline const labelList& zMaxPointLabels() const;

        //- offset points of points at maximum z
        inline const labelList& zMaxToZMin() const;

        //- correct x and y coordinates of offset points
        //- and unify the z coordinate
        void correctPoints();

        //- delete all dynamically allocated data
        void clearOut();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline const boolList& polyMeshGen2DEngine::activeFace() const
{
    if( !activeFacePtr_ )
        findActiveFaces();

    return *activeFacePtr_;
}

inline const labelList& polyMeshGen2DEngine::activeFaceLabels() const
{
    if( !activeFaceLabelsPtr_ )
        findActiveFaceLabels();

    return *activeFaceLabelsPtr_;
}

inline const boolList& polyMeshGen2DEngine::zMinPoints() const
{
    if( !zMinPointPtr_ )
        findZMinPoints();

    return *zMinPointPtr_;
}

inline const labelList& polyMeshGen2DEngine::zMinPointLabels() const
{
    if( !zMinPointLabelsPtr_ )
        findZMinPointLabels();

    return *zMinPointLabelsPtr_;
}

inline const labelList& polyMeshGen2DEngine::zMinToZMax() const
{
    if( !zMinToZMaxPtr_ )
        findZMinOffsetPoints();

    return *zMinToZMaxPtr_;
}

inline const boolList& polyMeshGen2DEngine::zMaxPoints() const
{
    if( !zMaxPointPtr_ )
        findZMaxPoints();

    return *zMaxPointPtr_;
}

inline const labelList& polyMeshGen2DEngine::zMaxPointLabels() const
{
    if( !zMaxPointLabelsPtr_ )
        findZMaxPointLabels();

    return *zMaxPointLabelsPtr_;
}

inline const labelList& polyMeshGen2DEngine::zMaxToZMin() const
{
    if( !zMaxToZMinPtr_ )
        findZMaxOffsetPoints();

    return *zMaxToZMinPtr_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
