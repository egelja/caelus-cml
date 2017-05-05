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
    CML::centredCFCFaceToCellStencilObject

Description

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef centredCFCFaceToCellStencilObject_H
#define centredCFCFaceToCellStencilObject_H

#include "extendedCentredFaceToCellStencil.hpp"
#include "CFCFaceToCellStencil.hpp"
#include "MeshObject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
              Class centredCFCFaceToCellStencilObject Declaration
\*---------------------------------------------------------------------------*/

class centredCFCFaceToCellStencilObject
:
    public MeshObject<fvMesh, centredCFCFaceToCellStencilObject>,
    public extendedCentredFaceToCellStencil
{

public:

    TypeName("centredCFCFaceToCellStencil");

    // Constructors

        //- Construct from uncompacted face stencil
        explicit centredCFCFaceToCellStencilObject
        (
            const fvMesh& mesh
        )
        :
            MeshObject<fvMesh, centredCFCFaceToCellStencilObject>(mesh),
            extendedCentredFaceToCellStencil(CFCFaceToCellStencil(mesh))
        {}


    //- Destructor
    virtual ~centredCFCFaceToCellStencilObject()
    {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
