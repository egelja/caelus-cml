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
    CML::indexedParticleCloud

Description
    A Cloud of particles carrying an additional index.

SourceFiles
    indexedParticleCloud.cpp

\*---------------------------------------------------------------------------*/

#ifndef indexedParticleCloud_H
#define indexedParticleCloud_H

#include "Cloud.hpp"
#include "indexedParticle.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class indexedParticleCloud Declaration
\*---------------------------------------------------------------------------*/

class indexedParticleCloud
:
    public Cloud<indexedParticle>
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        indexedParticleCloud(const indexedParticleCloud&);

        //- Disallow default bitwise assignment
        void operator=(const indexedParticleCloud&);


public:

    // Constructors

        //- Construct given mesh
        indexedParticleCloud
        (
            const polyMesh&,
            const word& cloudName = "defaultCloud",
            bool readFields = true
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
