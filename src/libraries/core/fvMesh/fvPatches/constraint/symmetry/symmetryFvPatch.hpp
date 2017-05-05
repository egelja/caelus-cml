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
    CML::symmetryFvPatch

Description
    Symmetry-plane patch.

SourceFiles
    symmetryFvPatch.cpp

\*---------------------------------------------------------------------------*/

#ifndef symmetryFvPatch_H
#define symmetryFvPatch_H

#include "fvPatch.hpp"
#include "symmetryPolyPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class symmetryFvPatch Declaration
\*---------------------------------------------------------------------------*/

class symmetryFvPatch
:
    public fvPatch
{

public:

    //- Runtime type information
    TypeName(symmetryPolyPatch::typeName_());


    // Constructors

        //- Construct from polyPatch
        symmetryFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm)
        :
            fvPatch(patch, bm)
        {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
