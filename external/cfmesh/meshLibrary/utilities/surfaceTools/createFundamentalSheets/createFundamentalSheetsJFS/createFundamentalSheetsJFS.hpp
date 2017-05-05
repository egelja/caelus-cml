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
    createFundamentalSheetsJFS

Description
    Inserts sheets at the boundary of the mesh to capture all feature edges

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    createFundamentalSheetsJFS.cpp

\*---------------------------------------------------------------------------*/

#ifndef createFundamentalSheetsJFS_HPP
#define createFundamentalSheetsJFS_HPP

#include "polyMeshGenModifier.hpp"
#include "createFundamentalSheets.hpp"
#include "labelLongList.hpp"
#include "boolList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declarations
class meshSurfaceEngine;

/*---------------------------------------------------------------------------*\
                Class createFundamentalSheetsJFS Declaration
\*---------------------------------------------------------------------------*/

class createFundamentalSheetsJFS
:
    public createFundamentalSheets
{
    // Private member functions
        //- check if all cells have only one face at the boundary
        bool isTopologyOk() const;

        //- create inital sheet from all boundary faces of the surface mesh
        void createInitialSheet();

        //- create fundamental sheets for all feature edges
        void createSheetsAtFeatureEdges();

        //- Disallow default construct
        createFundamentalSheetsJFS();

        //- Disallow default bitwise copy construct
        createFundamentalSheetsJFS(const createFundamentalSheetsJFS&);

        //- Disallow default bitwise assignment
        void operator=(const createFundamentalSheetsJFS&);

public:

    // Runtime type information
        TypeName("Shepherd");

    // Constructors

        //- Construct from mesh data
        createFundamentalSheetsJFS
        (
            polyMeshGen& mesh,
            const bool createWrapperSheet = true
        );

    // Destructor

        ~createFundamentalSheetsJFS();

    // Member Functions
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
