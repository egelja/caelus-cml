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
    CML::pointBoundaryMesh

Description
    CML::pointBoundaryMesh

SourceFiles
    pointBoundaryMesh.cpp

\*---------------------------------------------------------------------------*/

#ifndef pointBoundaryMesh_H
#define pointBoundaryMesh_H

#include "pointPatchList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class pointMesh;
class polyBoundaryMesh;

/*---------------------------------------------------------------------------*\
                       Class pointBoundaryMesh Declaration
\*---------------------------------------------------------------------------*/

class pointBoundaryMesh
:
    public pointPatchList
{
    // Private data

        //- Reference to mesh
        const pointMesh& mesh_;


    // Private Member Functions

        //- Calculate the geometry for the patches (transformation tensors etc.)
        void calcGeometry();

        //- Disallow default bitwise copy construct
        pointBoundaryMesh(const pointBoundaryMesh&);

        //- Disallow default bitwise assignment
        void operator=(const pointBoundaryMesh&);


public:

    //- Declare friendship with pointMesh
    friend class pointMesh;


    // Constructors

        //- Construct from polyBoundaryMesh
        pointBoundaryMesh
        (
            const pointMesh&,
            const polyBoundaryMesh&
        );


    // Member functions

        //- Return the mesh reference
        const pointMesh& mesh() const
        {
            return mesh_;
        }

        //- Correct polyBoundaryMesh after moving points
        void movePoints(const pointField&);

        //- Correct polyBoundaryMesh after topology update
        void updateMesh();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
