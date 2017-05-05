/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::wallDistReflection

Description
    Wall distance and reflection vector calculation. See wallDist.H

SourceFiles
    wallDistReflection.cpp

\*---------------------------------------------------------------------------*/

#ifndef wallDistReflection_H
#define wallDistReflection_H

#include "reflectionVectors.hpp"
#include "wallDistData.hpp"
#include "wallPointData.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class wallDistReflection Declaration
\*---------------------------------------------------------------------------*/

class wallDistReflection
:
    public reflectionVectors,
    public wallDistData<wallPointData<vector> >
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        wallDistReflection(const wallDistReflection&);

        //- Disallow default bitwise assignment
        void operator=(const wallDistReflection&);


public:

    // Constructors

        //- Construct from mesh and flag whether or not to correct wall.
        //  Calculate for all cells. correctWalls : correct wall (face&point)
        //  cells for correct distance, searching neighbours.
        wallDistReflection(const fvMesh& mesh, bool correctWalls = true);


    // Member Functions

        //- Correct for mesh geom/topo changes
        void correct();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
