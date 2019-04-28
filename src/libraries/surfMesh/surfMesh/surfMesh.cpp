/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
Copyright (C) 2015 Applied CCM
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
#include "MeshedSurfaceProxy.hpp"

#include "Time.hpp"
#include "OSspecific.hpp"
#include "MeshedSurface.hpp"
#include "demandDrivenData.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


namespace CML
{
    defineTypeNameAndDebug(surfMesh, 0);
}


CML::word CML::surfMesh::meshSubDir = "surfMesh";

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// void CML::surfMesh::oneZone()
// {
//     word zoneName;
//
//     surfZoneList& zones = Allocator::storedIOZones();
//     if (zones.size())
//     {
//         zoneName = zones[0].name();
//     }
//     if (zoneName.empty())
//     {
//         zoneName = "zone0";
//     }
//
//     // set single default zone
//     zones.setSize(1);
//     zones[0] = surfZone
//     (
//         zoneName,
//         nFaces(),       // zone size
//         0,              // zone start
//         0               // zone index
//     );
// }


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::surfMesh::surfMesh(const IOobject& io, const word& surfName)
:
    surfaceRegistry(io.db(), (surfName.size() ? surfName : io.name())),
    Allocator
    (
        IOobject
        (
            "points",
            time().findInstance(meshDir(), "points"),
            meshSubDir,
            *this,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        ),
        IOobject
        (
            "faces",
            time().findInstance(meshDir(), "faces"),
            meshSubDir,
            *this,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        ),
        IOobject
        (
            "surfZones",
            time().findInstance(meshDir(), "surfZones"),
            meshSubDir,
            *this,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
    MeshReference(this->storedIOFaces(), this->storedIOPoints())
{}


CML::surfMesh::surfMesh
(
    const IOobject& io,
    const Xfer<pointField>& pointLst,
    const Xfer<faceList>& faceLst,
    const word& surfName
)
:
    surfaceRegistry(io.db(), (surfName.size() ? surfName : io.name())),
    Allocator
    (
        IOobject
        (
            "points",
            instance(),
            meshSubDir,
            *this,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        pointLst,
        IOobject
        (
            "faces",
            instance(),
            meshSubDir,
            *this,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        faceLst,
        IOobject
        (
            "surfZones",
            instance(),
            meshSubDir,
            *this,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        Xfer<surfZoneList>()
    ),
    MeshReference(this->storedIOFaces(), this->storedIOPoints())
{}


CML::surfMesh::surfMesh
(
    const IOobject& io,
    const Xfer<MeshedSurface<face> >& surf,
    const word& surfName
)
:
    surfaceRegistry(io.db(), (surfName.size() ? surfName : io.name())),
    Allocator
    (
        IOobject
        (
            "points",
            instance(),
            meshSubDir,
            *this,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        pointField(),
        IOobject
        (
            "faces",
            instance(),
            meshSubDir,
            *this,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        faceList(),
        IOobject
        (
            "surfZones",
            instance(),
            meshSubDir,
            *this,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        surfZoneList()
    ),
    MeshReference(this->storedIOFaces(), this->storedIOPoints())
{
    if (debug)
    {
        Info<<"IOobject: " << io.path() << nl
            <<" name: " << io.name()
            <<" instance: " << io.instance()
            <<" local: " << io.local()
            <<" dbDir: " << io.db().dbDir() << endl;
        Info<<"creating surfMesh at instance " << instance() << endl;
        Info<<"timeName: " << instance() << endl;
    }

    // We can also send Xfer<..>::null just to initialize without allocating
    if (notNull(surf))
    {
        transfer(surf());
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::surfMesh::~surfMesh()
{
    // clearOut();
    // resetMotion();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::surfMesh::updatePointsRef()
{
    // assign the reference to the points (this is truly ugly)
    reinterpret_cast<SubField<point>&>
    (
        const_cast<Field<point>&>(MeshReference::points())
    ) = reinterpret_cast<SubField<point>&>(this->storedPoints());
}


void CML::surfMesh::updateFacesRef()
{
    // assign the reference to the faces
    static_cast<UList<face>&>(*this) = this->storedFaces();
}


void CML::surfMesh::updateRefs()
{
    this->updatePointsRef();
    this->updateFacesRef();
}


void CML::surfMesh::resetPrimitives
(
    const Xfer<pointField>& points,
    const Xfer<faceList>& faces,
    const Xfer<surfZoneList>& zones,
    const bool validate
)
{
    // Clear addressing.
    MeshReference::clearGeom();

    Allocator::reset(points, faces, zones);
    this->updateRefs();

    if (validate)
    {
        checkZones();
    }
}


void CML::surfMesh::transfer
(
    MeshedSurface<face>& surf
)
{
    // Clear addressing.
    MeshReference::clearGeom();

    this->storedIOPoints().transfer(surf.storedPoints());
    this->storedIOFaces().transfer(surf.storedFaces());
    this->storedIOZones().transfer(surf.storedZones());

    this->updateRefs();
}


CML::Xfer<CML::MeshedSurface<CML::face> > CML::surfMesh::xfer()
{
    Xfer<MeshedSurface<face> > xf;

    xf().storedPoints().transfer(this->storedPoints());
    xf().storedFaces().transfer(this->storedFaces());
    xf().storedZones().transfer(this->storedZones());

    // is this needed?
    this->updateRefs();

    // Clear addressing.
    MeshReference::clearGeom();

    return xf;
}


CML::fileName CML::surfMesh::meshDir() const
{
    return dbDir()/meshSubDir;
}


const CML::fileName& CML::surfMesh::pointsInstance() const
{
    return this->storedIOPoints().instance();
}


const CML::fileName& CML::surfMesh::facesInstance() const
{
    return this->storedIOFaces().instance();
}


CML::label CML::surfMesh::nPoints() const
{
    return this->points().size();
}


CML::label CML::surfMesh::nFaces() const
{
    return this->faces().size();
}


const CML::pointField& CML::surfMesh::points() const
{
    return this->storedIOPoints();
}


const CML::faceList& CML::surfMesh::faces() const
{
    return this->storedIOFaces();
}


void CML::surfMesh::checkZones()
{
    // extra safety, ensure we have at some zones
    // and they cover all the faces - fix start silently
    surfZoneList& zones = Allocator::storedIOZones();

    if (zones.size() <= 1)
    {
        removeZones();
    }
    else
    {
        label count = 0;
        forAll(zones, zoneI)
        {
            zones[zoneI].start() = count;
            count += zones[zoneI].size();
        }

        if (count < nFaces())
        {
            WarningInFunction
                << "more faces " << nFaces() << " than zones " << count
                << " ... extending final zone"
                << endl;

            zones.last().size() += count - nFaces();
        }
        else if (count > size())
        {
            FatalErrorInFunction
                << "more zones " << count << " than faces " << nFaces()
                << exit(FatalError);
        }
    }
}


// Add boundary patches. Constructor helper
void CML::surfMesh::addZones
(
    const surfZoneList& srfZones,
    const bool validate
)
{
    surfZoneList& zones = Allocator::storedIOZones();

    forAll(zones, zoneI)
    {
        zones[zoneI] = surfZone(srfZones[zoneI], zoneI);
    }

    if (validate)
    {
        checkZones();
    }
}


// Remove all files and some subdirs (eg, sets)
void CML::surfMesh::removeFiles(const fileName& instanceDir) const
{
    fileName meshFilesPath = db().path()/instanceDir/meshSubDir;

    rm(meshFilesPath/"points");
    rm(meshFilesPath/"faces");
    rm(meshFilesPath/"surfZones");
}

void CML::surfMesh::removeFiles() const
{
    removeFiles(instance());
}


void CML::surfMesh::write(const fileName& name, const surfMesh& surf)
{
    MeshedSurfaceProxy<face>
    (
        surf.points(),
        surf.faces(),
        surf.surfZones()
    ).write(name);
}


void CML::surfMesh::write(const fileName& name)
{
    write(name, *this);
}


// ************************************************************************* //
