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
    CML::upwindFECCellToFaceStencilObject

Description

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef upwindFECCellToFaceStencilObject_H
#define upwindFECCellToFaceStencilObject_H

#include "extendedUpwindCellToFaceStencil.hpp"
#include "FECCellToFaceStencil.hpp"
#include "MeshObject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
              Class upwindFECCellToFaceStencilObject Declaration
\*---------------------------------------------------------------------------*/

class upwindFECCellToFaceStencilObject
:
    public MeshObject<fvMesh, upwindFECCellToFaceStencilObject>,
    public extendedUpwindCellToFaceStencil
{

public:

    TypeName("upwindFECCellToFaceStencil");

    // Constructors

        //- Construct from uncompacted face stencil
        explicit upwindFECCellToFaceStencilObject
        (
            const fvMesh& mesh,
            const bool pureUpwind,
            const scalar minOpposedness
        )
        :
            MeshObject<fvMesh, upwindFECCellToFaceStencilObject>(mesh),
            extendedUpwindCellToFaceStencil
            (
                FECCellToFaceStencil(mesh),
                pureUpwind,
                minOpposedness
            )
        {
            if (extendedCellToFaceStencil::debug)
            {
                Info<< "Generated off-centred stencil " << type()
                    << nl << endl;
                writeStencilStats(Info, ownStencil(), ownMap());
            }
        }


    //- Destructor
    virtual ~upwindFECCellToFaceStencilObject()
    {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
