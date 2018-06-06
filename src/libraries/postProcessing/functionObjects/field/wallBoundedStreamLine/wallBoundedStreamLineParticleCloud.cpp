/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "wallBoundedStreamLineParticleCloud.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTemplateTypeNameAndDebug(Cloud<wallBoundedStreamLineParticle>, 0);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::wallBoundedStreamLineParticleCloud::wallBoundedStreamLineParticleCloud
(
    const polyMesh& mesh,
    const word& cloudName,
    bool readFields
)
:
    Cloud<wallBoundedStreamLineParticle>(mesh, cloudName, false)
{
    if (readFields)
    {
        wallBoundedStreamLineParticle::readFields(*this);
    }
}


CML::wallBoundedStreamLineParticleCloud::wallBoundedStreamLineParticleCloud
(
    const polyMesh& mesh,
    const word& cloudName,
    const IDLList<wallBoundedStreamLineParticle>& particles
)
:
    Cloud<wallBoundedStreamLineParticle>(mesh, cloudName, particles)
{}


// ************************************************************************* //