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
    CML::cyclicSlipPointPatch

Description
    Cyclic patch with slip constraint

SourceFiles
    cyclicSlipPointPatch.cpp

\*---------------------------------------------------------------------------*/

#ifndef cyclicSlipPointPatch_H
#define cyclicSlipPointPatch_H

#include "cyclicPointPatch.hpp"
#include "cyclicSlipPolyPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class cyclicSlipPointPatch Declaration
\*---------------------------------------------------------------------------*/

class cyclicSlipPointPatch
:
    public cyclicPointPatch
{

public:

    //- Runtime type information
    TypeName(cyclicSlipPolyPatch::typeName_());


    // Constructors

        //- Construct from components
        cyclicSlipPointPatch
        (
            const polyPatch& patch,
            const pointBoundaryMesh& bm
        )
        :
            cyclicPointPatch(patch, bm)
        {}


    // Destructor

        virtual ~cyclicSlipPointPatch()
        {}


    // Member Functions

        //- Return point unit normals.
        virtual const vectorField& pointNormals() const;

        //- Accumulate the effect of constraint direction of this patch
        virtual void applyConstraint
        (
            const label pointi,
            pointConstraint&
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
