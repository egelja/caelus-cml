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
    triSurf

Description
    A class for triangulated surface used in the meshing process.
    It is derived from points and facets with some additional subsets.
    Subsets are vert useful for local mesh refinement.

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    triSurf.cpp

\*---------------------------------------------------------------------------*/

#ifndef triSurf_HPP
#define triSurf_HPP

#include "triSurfPoints.hpp"
#include "triSurfFacets.hpp"
#include "triSurfFeatureEdges.hpp"
#include "triSurfAddressing.hpp"
#include <map>
#include "DynList.hpp"
#include "labelLongList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Froward declarations
class triSurfModifier;

/*---------------------------------------------------------------------------*\
                        Class triSurf Declaration
\*---------------------------------------------------------------------------*/

class triSurf
:
    public triSurfPoints,
    public triSurfFacets,
    public triSurfFeatureEdges,
    public triSurfAddressing
{
    // Private member functions
        void readFromFTR(const fileName&);
        void writeToFTR(const fileName&) const;

        void readFromFMS(const fileName&);
        void writeToFMS(const fileName&) const;

        inline LongList<labelledTri>& accessToFacets();
        inline geometricSurfacePatchList& accessToPatches();

        void topologyCheck();

        //- Disallow default bitwise assignment
        void operator=(const triSurf&);

public:

    // Friend classes
        friend class triSurfModifer;

    // Constructors

        //- Default construct
        triSurf();

        //- Construct from parts
        triSurf
        (
            const LongList<labelledTri>& triangles,
            const geometricSurfacePatchList& patches,
            const edgeLongList& featureEdges_,
            const pointField& points
        );

        //- Read from file
        triSurf(const fileName& fName);

    // Destructor

        ~triSurf();


    // Member Functions
        //- read and write the surface
        void readSurface(const fileName&);
        void writeSurface(const fileName&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline LongList<labelledTri>& triSurf::accessToFacets()
{
    return triSurfFacets::triangles_;
}

inline geometricSurfacePatchList& triSurf::accessToPatches()
{
    return triSurfFacets::patches_;
}

/*
inline label triSurf::numberOfFaceGroups() const
{
    if( !faceGroupPtr_ )
        calculateFaceGroups();

    return nFaceGroups_;
}

inline const labelLongList& triSurf::faceGroups() const
{
    if( !faceGroupPtr_ )
        calculateFaceGroups();

    return *faceGroupPtr_;
}
*/

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
