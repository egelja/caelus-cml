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
    CML::geomDecomp

Description
    Geometrical domain decomposition

SourceFiles
    geomDecomp.cpp

\*---------------------------------------------------------------------------*/

#ifndef geomDecomp_H
#define geomDecomp_H

#include "decompositionMethod.hpp"
#include "Vector_.hpp"

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class geomDecomp Declaration
\*---------------------------------------------------------------------------*/

class geomDecomp
:
    public decompositionMethod
{

protected:

    // Protected data

        const dictionary& geomDecomDict_;

        Vector<label> n_;
        scalar delta_;
        tensor rotDelta_;

public:

    // Constructors

        //- Construct given the decomposition dictionary
        //  and the derived type name
        geomDecomp
        (
            const dictionary& decompositionDict,
            const word& derivedType
        );

        //- Return for every coordinate the wanted processor number.
        virtual labelList decompose
        (
            const pointField& points,
            const scalarField& pointWeights
        ) = 0;

        //- Like decompose but with uniform weights on the points
        virtual labelList decompose(const pointField&) = 0;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
