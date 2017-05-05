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
    CML::passiveParticleCloud

Description
    A Cloud of passive particles

SourceFiles
    passiveParticleCloud.cpp

\*---------------------------------------------------------------------------*/

#ifndef passiveParticleCloud_H
#define passiveParticleCloud_H

#include "Cloud.hpp"
#include "passiveParticle.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                   Class passiveParticleCloud Declaration
\*---------------------------------------------------------------------------*/

class passiveParticleCloud
:
    public Cloud<passiveParticle>
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        passiveParticleCloud(const passiveParticleCloud&);

        //- Disallow default bitwise assignment
        void operator=(const passiveParticleCloud&);


public:

    // Constructors

        //- Construct given mesh
        passiveParticleCloud
        (
            const polyMesh&,
            const word& cloudName = "defaultCloud",
            bool readFields = true
        );

        //- Construct from mesh, cloud name, and a list of particles
        passiveParticleCloud
        (
            const polyMesh& mesh,
            const word& cloudName,
            const IDLList<passiveParticle>& particles
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
