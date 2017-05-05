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
    CML::nonuniformTransformCyclicFvPatch

Description
    Cyclic-plane patch.

SourceFiles
    nonuniformTransformCyclicFvPatch.cpp

\*---------------------------------------------------------------------------*/

#ifndef nonuniformTransformCyclicFvPatch_H
#define nonuniformTransformCyclicFvPatch_H

#include "cyclicFvPatch.hpp"
#include "nonuniformTransformCyclicPolyPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
            Class nonuniformTransformCyclicFvPatch Declaration
\*---------------------------------------------------------------------------*/

class nonuniformTransformCyclicFvPatch
:
    public cyclicFvPatch
{

public:

    //- Runtime type information
    TypeName(nonuniformTransformCyclicPolyPatch::typeName_());


    // Constructors

        //- Construct from polyPatch
        nonuniformTransformCyclicFvPatch
        (
            const polyPatch& patch,
            const fvBoundaryMesh& bm
        )
        :
            cyclicFvPatch(patch, bm)
        {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
