/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::indexedParticle

Description
    Adds label index to base particle

SourceFiles
    indexedParticle.hpp

\*---------------------------------------------------------------------------*/

#ifndef indexedParticle_H
#define indexedParticle_H

#include "particle.hpp"
#include "IOstream.hpp"
#include "autoPtr.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class indexedParticle Declaration
\*---------------------------------------------------------------------------*/

class indexedParticle
:
    public particle
{
    // Private data

        label index_;


public:

    // Constructors

        //- Construct from components
        indexedParticle
        (
            const polyMesh& mesh,
            const barycentric& coordinates,
            const label celli,
            const label tetFacei,
            const label tetPti,
            const label index = 0
        )
        :
            particle(mesh, coordinates, celli, tetFacei, tetPti),
            index_(index)
        {}

        //- Construct from Istream
        indexedParticle
        (
            const polyMesh& mesh,
            Istream& is,
            bool readFields = true
        )
        :
            particle(mesh, is, readFields)
        {}

        //- Construct as a copy
        indexedParticle(const indexedParticle& p)
        :
            particle(p)
        {}

        //- Construct and return a clone
        virtual autoPtr<particle> clone() const
        {
            return autoPtr<particle>(new indexedParticle(*this));
        }


    // Member functions

        label index() const
        {
            return index_;
        }

        label& index()
        {
            return index_;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
