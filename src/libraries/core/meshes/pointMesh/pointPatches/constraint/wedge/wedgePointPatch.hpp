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
    CML::wedgePointPatch

Description
    Wedge front and back plane patch.

SourceFiles
    wedgePointPatch.cpp

\*---------------------------------------------------------------------------*/

#ifndef wedgePointPatch_H
#define wedgePointPatch_H

#include "facePointPatch.hpp"
#include "wedgePolyPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class wedgePointPatch Declaration
\*---------------------------------------------------------------------------*/

class wedgePointPatch
:
    public facePointPatch
{

public:

    //- Runtime type information
    TypeName(wedgePolyPatch::typeName_());


    // Constructors

        //- Construct from polyPatch
        wedgePointPatch
        (
            const polyPatch& patch,
            const pointBoundaryMesh& bm
        )
        :
            facePointPatch(patch, bm)
        {}


    // Member Functions

        //- Return the constraint type this pointPatch implements.
        virtual const word& constraintType() const
        {
            return type();
        }

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
