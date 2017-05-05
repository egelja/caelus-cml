/*---------------------------------------------------------------------------*\
Copyright (C) 2014 OpenFOAM Foundation
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
    CML::unmappedPassiveParticleCloud

Description
    passiveParticleCloud but with autoMap disabled

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef unmappedPassiveParticleCloud_HPP
#define unmappedPassiveParticleCloud_HPP

#include "passiveParticleCloud.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                Class unmappedPassiveParticleCloud Declaration
\*---------------------------------------------------------------------------*/

class unmappedPassiveParticleCloud
:
    public passiveParticleCloud
{

public:

    // Constructors

        //- Construct given mesh
        unmappedPassiveParticleCloud
        (
            const polyMesh& mesh,
            const word& cloudName = "defaultCloud",
            bool readFields = true
        )
        :
            passiveParticleCloud(mesh, cloudName, readFields)
        {}

        //- Construct from mesh, cloud name, and a list of particles
        unmappedPassiveParticleCloud
        (
            const polyMesh& mesh,
            const word& cloudName,
            const IDLList<passiveParticle>& particles
        )
        :
            passiveParticleCloud(mesh, cloudName, particles)
        {}

        //- Destructor
        virtual ~unmappedPassiveParticleCloud()
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
