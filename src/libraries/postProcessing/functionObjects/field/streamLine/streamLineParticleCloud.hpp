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
    CML::streamLineParticleCloud

Description
    A Cloud of streamLine particles

SourceFiles
    streamLineCloud.cpp

\*---------------------------------------------------------------------------*/

#ifndef streamLineCloud_H
#define streamLineCloud_H

#include "Cloud.hpp"
#include "streamLineParticle.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class streamLineCloud Declaration
\*---------------------------------------------------------------------------*/

class streamLineParticleCloud
:
    public Cloud<streamLineParticle>
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        streamLineParticleCloud(const streamLineParticleCloud&);

        //- Disallow default bitwise assignment
        void operator=(const streamLineParticleCloud&);


public:

    //- Type of parcel the cloud was instantiated for
    typedef streamLineParticle parcelType;

    // Constructors

        //- Construct given mesh
        streamLineParticleCloud
        (
            const polyMesh&,
            const word& cloudName = "defaultCloud",
            bool readFields = true
        );

        //- Construct from mesh, cloud name, and a list of particles
        streamLineParticleCloud
        (
            const polyMesh& mesh,
            const word& cloudName,
            const IDLList<streamLineParticle>& particles
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
