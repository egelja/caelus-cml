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
    CML::centredCECCellToFaceStencilObject

Description

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef centredCECCellToFaceStencilObject_H
#define centredCECCellToFaceStencilObject_H

#include "extendedCentredCellToFaceStencil.hpp"
#include "CECCellToFaceStencil.hpp"
#include "MeshObject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
              Class centredCECCellToFaceStencilObject Declaration
\*---------------------------------------------------------------------------*/

class centredCECCellToFaceStencilObject
:
    public MeshObject<fvMesh, centredCECCellToFaceStencilObject>,
    public extendedCentredCellToFaceStencil
{

public:

    TypeName("centredCECCellToFaceStencil");

    // Constructors

        //- Construct from uncompacted face stencil
        explicit centredCECCellToFaceStencilObject
        (
            const fvMesh& mesh
        )
        :
            MeshObject<fvMesh, centredCECCellToFaceStencilObject>(mesh),
            extendedCentredCellToFaceStencil(CECCellToFaceStencil(mesh))
        {
            if (extendedCellToFaceStencil::debug)
            {
                Info<< "Generated centred stencil " << type()
                    << nl << endl;
                writeStencilStats(Info, stencil(), map());
            }
        }


    //- Destructor
    virtual ~centredCECCellToFaceStencilObject()
    {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
