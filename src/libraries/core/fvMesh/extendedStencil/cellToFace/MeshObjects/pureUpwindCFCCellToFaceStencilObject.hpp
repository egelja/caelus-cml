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
    CML::pureUpwindCFCCellToFaceStencilObject

Description

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef pureUpwindCFCCellToFaceStencilObject_H
#define pureUpwindCFCCellToFaceStencilObject_H

#include "extendedUpwindCellToFaceStencil.hpp"
#include "CFCCellToFaceStencil.hpp"
#include "MeshObject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
            Class pureUpwindCFCCellToFaceStencilObject Declaration
\*---------------------------------------------------------------------------*/

class pureUpwindCFCCellToFaceStencilObject
:
    public MeshObject<fvMesh, pureUpwindCFCCellToFaceStencilObject>,
    public extendedUpwindCellToFaceStencil
{

public:

    TypeName("pureUpwindCFCCellToFaceStencil");

    // Constructors

        //- Construct from uncompacted face stencil
        explicit pureUpwindCFCCellToFaceStencilObject
        (
            const fvMesh& mesh
        )
        :
            MeshObject<fvMesh, pureUpwindCFCCellToFaceStencilObject>(mesh),
            extendedUpwindCellToFaceStencil(CFCCellToFaceStencil(mesh))
        {
            if (extendedCellToFaceStencil::debug)
            {
                Info<< "Generated pure upwind stencil " << type()
                    << nl << endl;
                writeStencilStats(Info, ownStencil(), ownMap());
            }
        }


    //- Destructor
    virtual ~pureUpwindCFCCellToFaceStencilObject()
    {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
