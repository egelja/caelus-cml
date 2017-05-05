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
    triSurfaceCopyParts

Description
    Copies user-selected surface parts into a new surface mesh

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    triSurfaceCopyParts.cpp

\*---------------------------------------------------------------------------*/

#ifndef triSurfaceCopyParts_HPP
#define triSurfaceCopyParts_HPP

#include "triSurf.hpp"
#include "boolList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                Class triSurfaceCopyParts Declaration
\*---------------------------------------------------------------------------*/

class triSurfaceCopyParts
{
    // Private data
        //- reference to triSurf
        const triSurf& surf_;

    // Private member functions
        //- mark facets which shall be copied
        void markFacetsForCopying(const wordList&, boolList&) const;

        //- copies data to a new surface mesh
        void copySurfaceMesh(const boolList&, triSurf&) const;

        //- Disallow default bitwise copy construct
        triSurfaceCopyParts(const triSurfaceCopyParts&);

        //- Disallow default bitwise assignment
        void operator=(const triSurfaceCopyParts&);

public:

    // Constructors

        //- Construct from octree
        triSurfaceCopyParts(const triSurf& surface);

    // Destructor

        ~triSurfaceCopyParts();


    // Member Functions

        //- copies selected patches/subsets to an already created mesh
        void copySurface(const wordList&, triSurf&) const;

        //- creates a new surface mesh and copies selected patches/subsets
        triSurf* copySurface(const wordList&) const;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
