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
    createFundamentalSheetsFJ

Description
    Inserts sheets at the boundary of the mesh to capture all feature edges.
    An initial o-layer is inserted first, and followed by the insertion of
    additional sheets which exit the domain at feature edges. The additional
    sheets are inserted such that they exit at other patches next the patch
    they are extruded at.

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    createFundamentalSheetsFJ.C

\*---------------------------------------------------------------------------*/

#ifndef createFundamentalSheetsFJ_HPP
#define createFundamentalSheetsFJ_HPP

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
                Class createFundamentalSheetsFJ Declaration
\*---------------------------------------------------------------------------*/

class createFundamentalSheetsFJ
:
    public createFundamentalSheets
{
    // Private member functions
        //- create inital sheet from all boundary faces of the surface mesh
        void createInitialSheet();

        //- create fundamental sheets for all feature edges
        void createSheetsAtFeatureEdges();

        //- Disallow default construct
        createFundamentalSheetsFJ();

        //- Disallow default bitwise copy construct
        createFundamentalSheetsFJ(const createFundamentalSheetsFJ&);

        //- Disallow default bitwise assignment
        void operator=(const createFundamentalSheetsFJ&);

public:

    // Runtime type information
        TypeName("modifiedPartitions");

    // Constructors

        //- Construct from mesh data
        createFundamentalSheetsFJ
        (
            polyMeshGen& mesh,
            const bool createWrapperSheet = true
        );

    // Destructor

        ~createFundamentalSheetsFJ();

    // Member Functions
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
