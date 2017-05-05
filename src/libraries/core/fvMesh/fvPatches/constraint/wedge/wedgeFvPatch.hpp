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
    CML::wedgeFvPatch

Description
    Wedge front and back plane patch.

SourceFiles
    wedgeFvPatch.cpp

\*---------------------------------------------------------------------------*/

#ifndef wedgeFvPatch_H
#define wedgeFvPatch_H

#include "fvPatch.hpp"
#include "wedgePolyPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class wedgeFvPatch Declaration
\*---------------------------------------------------------------------------*/

class wedgeFvPatch
:
    public fvPatch
{
    // Private data

        const wedgePolyPatch& wedgePolyPatch_;


public:

    //- Runtime type information
    TypeName(wedgePolyPatch::typeName_());


    // Constructors

        //- Construct from polyPatch
        wedgeFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm);


    // Member functions

        // Access

            //- Return face transformation tensor
            const tensor& faceT() const
            {
                return wedgePolyPatch_.faceT();
            }

            //- Return neighbour-cell transformation tensor
            const tensor& cellT() const
            {
                return wedgePolyPatch_.cellT();
            }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
