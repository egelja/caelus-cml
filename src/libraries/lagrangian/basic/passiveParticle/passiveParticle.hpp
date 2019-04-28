/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::passiveParticle

Description
    Copy of base particle

SourceFiles
    passiveParticle.hpp

\*---------------------------------------------------------------------------*/

#ifndef passiveParticle_H
#define passiveParticle_H

#include "particle.hpp"
#include "IOstream.hpp"
#include "autoPtr.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class passiveParticle Declaration
\*---------------------------------------------------------------------------*/

class passiveParticle
:
    public particle
{

public:

    // Constructors

        //- Construct from components
        passiveParticle
        (
            const polyMesh& mesh,
            const barycentric& coordinates,
            const label celli,
            const label tetFacei,
            const label tetPti
        )
        :
            particle(mesh, coordinates, celli, tetFacei, tetPti)
        {}


        //- Construct from a position and a cell, searching for the rest of the
        //  required topology
        passiveParticle
        (
            const polyMesh& mesh,
            const vector& position,
            const label celli
        )
        :
            particle(mesh, position, celli)
        {}

        //- Construct from Istream
        passiveParticle
        (
            const polyMesh& mesh,
            Istream& is,
            bool readFields = true
        )
        :
            particle(mesh, is, readFields)
        {}

        //- Construct as copy
        passiveParticle(const passiveParticle& p)
        :
            particle(p)
        {}

        //- Construct and return a clone
        virtual autoPtr<particle> clone() const
        {
            return autoPtr<particle>(new passiveParticle(*this));
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
