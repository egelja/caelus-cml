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
    CML::centredCPCCellToFaceStencilObject

Description

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef centredCPCCellToFaceStencilObject_H
#define centredCPCCellToFaceStencilObject_H

#include "extendedCentredCellToFaceStencil.hpp"
#include "CPCCellToFaceStencil.hpp"
#include "MeshObject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
              Class centredCPCCellToFaceStencilObject Declaration
\*---------------------------------------------------------------------------*/

class centredCPCCellToFaceStencilObject
:
    public MeshObject<fvMesh, centredCPCCellToFaceStencilObject>,
    public extendedCentredCellToFaceStencil
{

public:

    TypeName("centredCPCCellToFaceStencil");

    // Constructors

        //- Construct from uncompacted face stencil
        explicit centredCPCCellToFaceStencilObject
        (
            const fvMesh& mesh
        )
        :
            MeshObject<fvMesh, centredCPCCellToFaceStencilObject>(mesh),
            extendedCentredCellToFaceStencil(CPCCellToFaceStencil(mesh))
        {
            if (extendedCellToFaceStencil::debug)
            {
                Info<< "Generated centred stencil " << type()
                    << nl << endl;
                writeStencilStats(Info, stencil(), map());
            }
        }


    //- Destructor
    virtual ~centredCPCCellToFaceStencilObject()
    {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
