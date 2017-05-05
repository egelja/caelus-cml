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
    triSurfModifier

Description
    A class allowing non-const access to the member of the surface mesh

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    triSurfModifier.cpp

\*---------------------------------------------------------------------------*/

#ifndef triSurfModifier_HPP
#define triSurfModifier_HPP

#include "triSurf.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class triSurfModifier Declaration
\*---------------------------------------------------------------------------*/

class triSurfModifier
{
    // Private data
        //- reference to the surface mesh
        triSurf& surface_;

    // Disallow bitwise assignment
        //- Default construct
        triSurfModifier();

        //- Disallow default bitwise assignment
        void operator=(const triSurfModifier&);

public:

    // Constructors

        //- Construct from surface mesh
        triSurfModifier(triSurf& surface);

    // Destructor

        ~triSurfModifier();


    // Member Functions
        //- non-const access to points
        inline pointField& pointsAccess();

        //- access to facets
        inline LongList<labelledTri>& facetsAccess();

        //- non-const access to feature edges
        inline edgeLongList& featureEdgesAccess();

        //- access to patches
        inline geometricSurfacePatchList& patchesAccess();
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline pointField& triSurfModifier::pointsAccess()
{
    return const_cast<pointField&>(surface_.points());
}

inline LongList<labelledTri>& triSurfModifier::facetsAccess()
{
    return const_cast<LongList<labelledTri>&>(surface_.facets());
}

inline edgeLongList& triSurfModifier::featureEdgesAccess()
{
    return const_cast<edgeLongList&>(surface_.featureEdges());
}

inline geometricSurfacePatchList& triSurfModifier::patchesAccess()
{
    return const_cast<geometricSurfacePatchList&>(surface_.patches());
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
