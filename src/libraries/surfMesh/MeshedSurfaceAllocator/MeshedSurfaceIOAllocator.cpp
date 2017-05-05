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

\*---------------------------------------------------------------------------*/

#include "MeshedSurfaceIOAllocator.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::MeshedSurfaceIOAllocator::MeshedSurfaceIOAllocator
(
    const IOobject& ioPoints,
    const IOobject& ioFaces,
    const IOobject& ioZones
)
:
    points_(ioPoints),
    faces_(ioFaces),
    zones_(ioZones)
{}


CML::MeshedSurfaceIOAllocator::MeshedSurfaceIOAllocator
(
    const IOobject& ioPoints,
    const pointField& points,
    const IOobject& ioFaces,
    const faceList& faces,
    const IOobject& ioZones,
    const surfZoneList& zones
)
:
    points_(ioPoints, points),
    faces_(ioFaces, faces),
    zones_(ioZones, zones)
{}


CML::MeshedSurfaceIOAllocator::MeshedSurfaceIOAllocator
(
    const IOobject& ioPoints,
    const Xfer<pointField>& points,
    const IOobject& ioFaces,
    const Xfer<faceList>& faces,
    const IOobject& ioZones,
    const Xfer<surfZoneList>& zones
)
:
    points_(ioPoints, points),
    faces_(ioFaces, faces),
    zones_(ioZones, zones)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::MeshedSurfaceIOAllocator::clear()
{
    points_.clear();
    faces_.clear();
    zones_.clear();
}


void CML::MeshedSurfaceIOAllocator::resetFaces
(
    const Xfer<List<face> >& faces,
    const Xfer<surfZoneList>& zones
)
{
    if (&faces)
    {
        faces_.transfer(faces());
    }

    if (&zones)
    {
        zones_.transfer(zones());
    }
}


void CML::MeshedSurfaceIOAllocator::reset
(
    const Xfer<pointField>& points,
    const Xfer<faceList>& faces,
    const Xfer<surfZoneList>& zones
)
{
    // Take over new primitive data.
    // Optimized to avoid overwriting data at all
    if (&points)
    {
        points_.transfer(points());
    }

    resetFaces(faces, zones);
}


void CML::MeshedSurfaceIOAllocator::reset
(
    const Xfer<List<point> >& points,
    const Xfer<faceList>& faces,
    const Xfer<surfZoneList>& zones
)
{
    // Take over new primitive data.
    // Optimized to avoid overwriting data at all
    if (&points)
    {
        points_.transfer(points());
    }

    resetFaces(faces, zones);
}


// ************************************************************************* //
