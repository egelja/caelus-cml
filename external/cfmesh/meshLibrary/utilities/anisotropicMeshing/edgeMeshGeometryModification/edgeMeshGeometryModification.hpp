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
    edgeMeshGeometryModification

Description
    Modifies geometry of edge meshes according to the anisotropic sources
    given by the user

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    edgeMeshGeometryModification.cpp

\*---------------------------------------------------------------------------*/

#ifndef edgeMeshGeometryModification_HPP
#define edgeMeshGeometryModification_HPP

#include "coordinateModification.hpp"
#include "point.hpp"
#include "typeInfo.hpp"
#include "coordinateModifier.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class edgeMesh;
class dictionary;

/*---------------------------------------------------------------------------*\
                Class edgeMeshGeometryModification Declaration
\*---------------------------------------------------------------------------*/

class edgeMeshGeometryModification
{
    // Private data
        //- reference to edgeMesh
        const edgeMesh& edgeMesh_;

        //- length of box sides
        const dictionary& meshDict_;

        //- contruct coordinate modification
        coordinateModifier* coordinateModifierPtr_;

        //- is mofdification active
        bool modificationActive_;


    // Private member functions
        //- check existence of geometry modifiers
        void checkModification();

        //- disallow bitwise copy construct
        edgeMeshGeometryModification(const edgeMeshGeometryModification&);

        //- disallow bitwise assignment
        void operator=(const edgeMeshGeometryModification&);

public:

    // Constructors

        //- Construct from edgeMesh and dictionary
        edgeMeshGeometryModification(const edgeMesh&, const dictionary&);

    // Destructor
        ~edgeMeshGeometryModification();

    // Member Functions

        //- is geometry modification active
        bool activeModification() const;

        //- modify coordinates
        const edgeMesh* modifyGeometry() const;

        //- revert geometry modification
        const edgeMesh* revertGeometryModification() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
