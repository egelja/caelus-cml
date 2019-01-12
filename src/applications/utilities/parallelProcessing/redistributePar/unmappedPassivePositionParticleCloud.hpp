/*---------------------------------------------------------------------------*\
Copyright (C) 2015 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::unmappedPassivePositionParticleCloud

Description
    passivePositionParticleCloud but with autoMap and writing disabled.
    Only used for its objectRegistry to store lagrangian fields

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef unmappedPassivePositionParticleCloud_HPP
#define unmappedPassivePositionParticleCloud_HPP

#include "passivePositionParticleCloud.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                Class unmappedPassiveParticleCloud Declaration
\*---------------------------------------------------------------------------*/

class unmappedPassivePositionParticleCloud
:
    public passivePositionParticleCloud
{

public:

    // Constructors

        //- Construct given mesh
        unmappedPassivePositionParticleCloud
        (
            const polyMesh& mesh,
            const word& cloudName = "defaultCloud",
            bool readFields = true
        )
        :
            passivePositionParticleCloud(mesh, cloudName, readFields)
        {}

        //- Construct from mesh, cloud name, and a list of particles
        unmappedPassivePositionParticleCloud
        (
            const polyMesh& mesh,
            const word& cloudName,
            const IDLList<passivePositionParticle>& particles
        )
        :
            passivePositionParticleCloud(mesh, cloudName, particles)
        {}

        //- Destructor
        virtual ~unmappedPassivePositionParticleCloud()
        {}

        //- Switch off remapping of cells of particles when
        //  mesh topology changes
        virtual void autoMap(const mapPolyMesh&)
        {}

        //- Switch off writing the objects
        virtual bool writeObject
        (
            IOstream::streamFormat fmt,
            IOstream::versionNumber ver,
            IOstream::compressionType cmp
        ) const
        {
            return true;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
