/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "surfMesh.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::surfMesh::setInstance(const fileName& inst)
{
    if (debug)
    {
        Info<< "void surfMesh::setInstance(const fileName& inst) : "
            << "Resetting file instance to " << inst << endl;
    }

    instance() = inst;

    storedIOPoints().writeOpt() = IOobject::AUTO_WRITE;
    storedIOPoints().instance() = inst;

    storedIOFaces().writeOpt() = IOobject::AUTO_WRITE;
    storedIOFaces().instance() = inst;

    storedIOZones().writeOpt() = IOobject::AUTO_WRITE;
    storedIOZones().instance() = inst;
}


CML::surfMesh::readUpdateState CML::surfMesh::readUpdate()
{
    if (debug)
    {
        Info<< "surfMesh::readUpdateState surfMesh::readUpdate() : "
            << "Updating mesh based on saved data." << endl;
    }

    // Find point and face instances
    fileName pointsInst(time().findInstance(meshDir(), "points"));
    fileName facesInst(time().findInstance(meshDir(), "faces"));

    if (debug)
    {
        Info<< "Points instance: old = " << pointsInstance()
            << " new = " << pointsInst << nl
            << "Faces instance: old = " << facesInstance()
            << " new = " << facesInst << endl;
    }

    if (facesInst != facesInstance())
    {
        // Topological change
        if (debug)
        {
            Info<< "Topological change" << endl;
        }

        clearOut();

        // Set instance to new instance.
        // Note points instance can differ from faces instance.
        setInstance(facesInst);
        storedIOPoints().instance() = pointsInst;

        storedIOPoints() = pointIOField
        (
            IOobject
            (
                "points",
                pointsInst,
                meshSubDir,
                *this,
                IOobject::MUST_READ,
                IOobject::NO_WRITE,
                false
            )
        );

        storedFaces() = faceCompactIOList
        (
            IOobject
            (
                "faces",
                facesInst,
                meshSubDir,
                *this,
                IOobject::MUST_READ,
                IOobject::NO_WRITE,
                false
            )
        );

        // Reset the surface zones
        surfZoneIOList newZones
        (
            IOobject
            (
                "surfZones",
                facesInst,
                meshSubDir,
                *this,
                IOobject::MUST_READ,
                IOobject::NO_WRITE,
                false
            )
        );

        // Check that zone types and names are unchanged
        bool zonesChanged = false;

        surfZoneList& zones = this->storedIOZones();
        if (zones.size() != newZones.size())
        {
            zonesChanged = true;
        }
        else
        {
            forAll(zones, zoneI)
            {
                if (zones[zoneI].name() != newZones[zoneI].name())
                {
                    zonesChanged = true;
                    break;
                }
            }
        }

        zones.transfer(newZones);

        if (zonesChanged)
        {
            WarningInFunction
                << "Number of zones has changed.  This may have "
                << "unexpected consequences.  Proceed with care." << endl;

            return surfMesh::TOPO_PATCH_CHANGE;
        }
        else
        {
            return surfMesh::TOPO_CHANGE;
        }

    }
    else if (pointsInst != pointsInstance())
    {
        // Points moved
        if (debug)
        {
            Info<< "Point motion" << endl;
        }

        clearGeom();
        storedIOPoints().instance() = pointsInst;

        storedIOPoints() = pointIOField
        (
            IOobject
            (
                "points",
                pointsInst,
                meshSubDir,
                *this,
                IOobject::MUST_READ,
                IOobject::NO_WRITE,
                false
            )
        );

        return surfMesh::POINTS_MOVED;
    }
    else
    {
        if (debug)
        {
            Info<< "No change" << endl;
        }
    }

    return surfMesh::UNCHANGED;
}


// ************************************************************************* //
