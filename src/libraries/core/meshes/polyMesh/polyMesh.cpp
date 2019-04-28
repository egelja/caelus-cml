/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
Copyright (C) 2015-2016 Applied CCM
Copyright (C) 2016 OpenCFD Ltd.
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

#include "polyMesh.hpp"
#include "Time.hpp"
#include "cellIOList.hpp"
#include "SubList.hpp"
#include "wedgePolyPatch.hpp"
#include "emptyPolyPatch.hpp"
#include "globalMeshData.hpp"
#include "processorPolyPatch.hpp"
#include "OSspecific.hpp"
#include "polyMeshTetDecomposition.hpp"
#include "indexedOctree.hpp"
#include "treeDataCell.hpp"
#include "SubField.hpp"

#include "pointMesh.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(polyMesh, 0);

    word polyMesh::defaultRegion = "region0";
    word polyMesh::meshSubDir = "polyMesh";
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::polyMesh::calcDirections() const
{
    for (direction cmpt=0; cmpt<vector::nComponents; cmpt++)
    {
        solutionD_[cmpt] = 1;
    }

    // Knock out empty and wedge directions. Note:they will be present on all
    // domains.

    label nEmptyPatches = 0;
    label nWedgePatches = 0;

    vector emptyDirVec = Zero;
    vector wedgeDirVec = Zero;

    forAll(boundaryMesh(), patchi)
    {
        if (boundaryMesh()[patchi].size())
        {
            if (isA<emptyPolyPatch>(boundaryMesh()[patchi]))
            {
                nEmptyPatches++;
                emptyDirVec += sum(cmptMag(boundaryMesh()[patchi].faceAreas()));
            }
            else if (isA<wedgePolyPatch>(boundaryMesh()[patchi]))
            {
                const wedgePolyPatch& wpp = refCast<const wedgePolyPatch>
                (
                    boundaryMesh()[patchi]
                );

                nWedgePatches++;
                wedgeDirVec += cmptMag(wpp.centreNormal());
            }
        }
    }

    reduce(nEmptyPatches, maxOp<label>());
    reduce(nWedgePatches, maxOp<label>());

    if (nEmptyPatches)
    {
        reduce(emptyDirVec, sumOp<vector>());

        emptyDirVec /= mag(emptyDirVec);

        for (direction cmpt=0; cmpt<vector::nComponents; cmpt++)
        {
            if (emptyDirVec[cmpt] > 1e-6)
            {
                solutionD_[cmpt] = -1;
            }
            else
            {
                solutionD_[cmpt] = 1;
            }
        }
    }


    // Knock out wedge directions

    geometricD_ = solutionD_;

    if (nWedgePatches)
    {
        reduce(wedgeDirVec, sumOp<vector>());

        wedgeDirVec /= mag(wedgeDirVec);

        for (direction cmpt=0; cmpt<vector::nComponents; cmpt++)
        {
            if (wedgeDirVec[cmpt] > 1e-6)
            {
                geometricD_[cmpt] = -1;
            }
            else
            {
                geometricD_[cmpt] = 1;
            }
        }
    }
}


CML::autoPtr<CML::labelIOList> CML::polyMesh::readTetBasePtIs() const
{
    IOobject io
    (
        "tetBasePtIs",
        instance(),
        meshSubDir,
        *this,
        IOobject::READ_IF_PRESENT,
        IOobject::NO_WRITE
    );

    if (io.headerOk())
    {
        return autoPtr<labelIOList>(new labelIOList(io));
    }
    else
    {
        return autoPtr<labelIOList>(nullptr);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::polyMesh::polyMesh(const IOobject& io, const bool defectCorr, const scalar areaSwitch)
:
    objectRegistry(io),
    primitiveMesh(defectCorr, areaSwitch),
    points_
    (
        IOobject
        (
            "points",
            time().findInstance(meshDir(), "points"),
            meshSubDir,
            *this,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
    faces_
    (
        IOobject
        (
            "faces",
            time().findInstance(meshDir(), "faces"),
            meshSubDir,
            *this,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
    owner_
    (
        IOobject
        (
            "owner",
            faces_.instance(),
            meshSubDir,
            *this,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE
        )
    ),
    neighbour_
    (
        IOobject
        (
            "neighbour",
            faces_.instance(),
            meshSubDir,
            *this,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE
        )
    ),
    clearedPrimitives_(false),
    boundary_
    (
        IOobject
        (
            "boundary",
            time().findInstance(meshDir(), "boundary"),
            meshSubDir,
            *this,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        ),
        *this
    ),
    bounds_(points_),
    geometricD_(Zero),
    solutionD_(Zero),
    tetBasePtIsPtr_(readTetBasePtIs()),
    cellTreePtr_(nullptr),
    pointZones_
    (
        IOobject
        (
            "pointZones",
            time().findInstance
            (
                meshDir(),
                "pointZones",
                IOobject::READ_IF_PRESENT
            ),
            meshSubDir,
            *this,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE
        ),
        *this
    ),
    faceZones_
    (
        IOobject
        (
            "faceZones",
            time().findInstance
            (
                meshDir(),
                "faceZones",
                IOobject::READ_IF_PRESENT
            ),
            meshSubDir,
            *this,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE
        ),
        *this
    ),
    cellZones_
    (
        IOobject
        (
            "cellZones",
            time().findInstance
            (
                meshDir(),
                "cellZones",
                IOobject::READ_IF_PRESENT
            ),
            meshSubDir,
            *this,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE
        ),
        *this
    ),
    globalMeshDataPtr_(nullptr),
    moving_(false),
    topoChanging_(false),
    curMotionTimeIndex_(-1),
    oldPointsPtr_(nullptr),
    oldCellCentresPtr_(nullptr),
    storeOldCellCentres_(false)
{
    if (exists(owner_.objectPath()))
    {
        initMesh();
    }
    else
    {
        cellCompactIOList cLst
        (
            IOobject
            (
                "cells",
                time().findInstance(meshDir(), "cells"),
                meshSubDir,
                *this,
                IOobject::MUST_READ,
                IOobject::NO_WRITE
            )
        );

        // Set the primitive mesh
        initMesh(cLst);

        owner_.write();
        neighbour_.write();
    }

    // Calculate topology for the patches (processor-processor comms etc.)
    boundary_.updateMesh();

    // Calculate the geometry for the patches (transformation tensors etc.)
    boundary_.calcGeometry();

    // Warn if global empty mesh
    if (returnReduce(nPoints(), sumOp<label>()) == 0)
    {
        WarningInFunction
            << "no points in mesh" << endl;
    }
    if (returnReduce(nCells(), sumOp<label>()) == 0)
    {
        WarningInFunction
            << "no cells in mesh" << endl;
    }

    // Initialise demand-driven data
    calcDirections();
}


CML::polyMesh::polyMesh
(
    const IOobject& io,
    const Xfer<pointField>& points,
    const Xfer<faceList>& faces,
    const Xfer<labelList>& owner,
    const Xfer<labelList>& neighbour,
    const bool syncPar,
    const bool defectCorr,
    const scalar areaSwitch
)
:
    objectRegistry(io),
    primitiveMesh(defectCorr, areaSwitch),
    points_
    (
        IOobject
        (
            "points",
            instance(),
            meshSubDir,
            *this,
            io.readOpt(),
            IOobject::AUTO_WRITE
        ),
        points
    ),
    faces_
    (
        IOobject
        (
            "faces",
            instance(),
            meshSubDir,
            *this,
            io.readOpt(),
            IOobject::AUTO_WRITE
        ),
        faces
    ),
    owner_
    (
        IOobject
        (
            "owner",
            instance(),
            meshSubDir,
            *this,
            io.readOpt(),
            IOobject::AUTO_WRITE
        ),
        owner
    ),
    neighbour_
    (
        IOobject
        (
            "neighbour",
            instance(),
            meshSubDir,
            *this,
            io.readOpt(),
            IOobject::AUTO_WRITE
        ),
        neighbour
    ),
    clearedPrimitives_(false),
    boundary_
    (
        IOobject
        (
            "boundary",
            instance(),
            meshSubDir,
            *this,
            io.readOpt(),
            IOobject::AUTO_WRITE
        ),
        *this,
        polyPatchList()
    ),
    bounds_(points_, syncPar),
    geometricD_(Zero),
    solutionD_(Zero),
    tetBasePtIsPtr_(readTetBasePtIs()),
    cellTreePtr_(nullptr),
    pointZones_
    (
        IOobject
        (
            "pointZones",
            instance(),
            meshSubDir,
            *this,
            io.readOpt(),
            IOobject::NO_WRITE
        ),
        *this,
        PtrList<pointZone>()
    ),
    faceZones_
    (
        IOobject
        (
            "faceZones",
            instance(),
            meshSubDir,
            *this,
            io.readOpt(),
            IOobject::NO_WRITE
        ),
        *this,
        PtrList<faceZone>()
    ),
    cellZones_
    (
        IOobject
        (
            "cellZones",
            instance(),
            meshSubDir,
            *this,
            io.readOpt(),
            IOobject::NO_WRITE
        ),
        *this,
        PtrList<cellZone>()
    ),
    globalMeshDataPtr_(nullptr),
    moving_(false),
    topoChanging_(false),
    curMotionTimeIndex_(-1),
    oldPointsPtr_(nullptr),
    oldCellCentresPtr_(nullptr),
    storeOldCellCentres_(false)
{
    // Check if the faces and cells are valid
    forAll(faces_, facei)
    {
        const face& curFace = faces_[facei];

        if (min(curFace) < 0 || max(curFace) > points_.size())
        {
            FatalErrorInFunction
                << "Face " << facei << "contains vertex labels out of range: "
                << curFace << " Max point index = " << points_.size()
                << abort(FatalError);
        }
    }

    // Set the primitive mesh
    initMesh();
}


CML::polyMesh::polyMesh
(
    const IOobject& io,
    const Xfer<pointField>& points,
    const Xfer<faceList>& faces,
    const Xfer<cellList>& cells,
    const bool syncPar,
    const bool defectCorr,
    const scalar areaSwitch
)
:
    objectRegistry(io),
    primitiveMesh(defectCorr, areaSwitch),
    points_
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
        points
    ),
    faces_
    (
        IOobject
        (
            "faces",
            instance(),
            meshSubDir,
            *this,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        faces
    ),
    owner_
    (
        IOobject
        (
            "owner",
            instance(),
            meshSubDir,
            *this,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        0
    ),
    neighbour_
    (
        IOobject
        (
            "neighbour",
            instance(),
            meshSubDir,
            *this,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        0
    ),
    clearedPrimitives_(false),
    boundary_
    (
        IOobject
        (
            "boundary",
            instance(),
            meshSubDir,
            *this,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        *this,
        0
    ),
    bounds_(points_, syncPar),
    geometricD_(Zero),
    solutionD_(Zero),
    tetBasePtIsPtr_(readTetBasePtIs()),
    cellTreePtr_(nullptr),
    pointZones_
    (
        IOobject
        (
            "pointZones",
            instance(),
            meshSubDir,
            *this,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        *this,
        0
    ),
    faceZones_
    (
        IOobject
        (
            "faceZones",
            instance(),
            meshSubDir,
            *this,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        *this,
        0
    ),
    cellZones_
    (
        IOobject
        (
            "cellZones",
            instance(),
            meshSubDir,
            *this,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        *this,
        0
    ),
    globalMeshDataPtr_(nullptr),
    moving_(false),
    topoChanging_(false),
    curMotionTimeIndex_(-1),
    oldPointsPtr_(nullptr),
    oldCellCentresPtr_(nullptr),
    storeOldCellCentres_(false)
{
    // Check if faces are valid
    forAll(faces_, facei)
    {
        const face& curFace = faces_[facei];

        if (min(curFace) < 0 || max(curFace) > points_.size())
        {
            FatalErrorInFunction
                << "Face " << facei << "contains vertex labels out of range: "
                << curFace << " Max point index = " << points_.size()
                << abort(FatalError);
        }
    }

    // transfer in cell list
    cellList cLst(cells);

    // Check if cells are valid
    forAll(cLst, celli)
    {
        const cell& curCell = cLst[celli];

        if (min(curCell) < 0 || max(curCell) > faces_.size())
        {
            FatalErrorInFunction
                << "Cell " << celli << "contains face labels out of range: "
                << curCell << " Max face index = " << faces_.size()
                << abort(FatalError);
        }
    }

    // Set the primitive mesh
    initMesh(cLst);
}


void CML::polyMesh::resetPrimitives
(
    const Xfer<pointField>& points,
    const Xfer<faceList>& faces,
    const Xfer<labelList>& owner,
    const Xfer<labelList>& neighbour,
    const labelList& patchSizes,
    const labelList& patchStarts,
    const bool validBoundary
)
{
    // Clear addressing. Keep geometric props and updateable props for mapping.
    clearAddressing(true);

    // Take over new primitive data.
    // Optimized to avoid overwriting data at all
    if (notNull(points))
    {
        points_.transfer(points());
        bounds_ = boundBox(points_, validBoundary);
    }

    if (notNull(faces))
    {
        faces_.transfer(faces());
    }

    if (notNull(owner))
    {
        owner_.transfer(owner());
    }

    if (notNull(neighbour))
    {
        neighbour_.transfer(neighbour());
    }


    // Reset patch sizes and starts
    forAll(boundary_, patchi)
    {
        boundary_[patchi] = polyPatch
        (
            boundary_[patchi],
            boundary_,
            patchi,
            patchSizes[patchi],
            patchStarts[patchi]
        );
    }


    // Flags the mesh files as being changed
    setInstance(time().timeName());

    // Check if the faces and cells are valid
    forAll(faces_, facei)
    {
        const face& curFace = faces_[facei];

        if (min(curFace) < 0 || max(curFace) > points_.size())
        {
            FatalErrorInFunction
                << "Face " << facei << " contains vertex labels out of range: "
                << curFace << " Max point index = " << points_.size()
                << abort(FatalError);
        }
    }


    // Set the primitive mesh from the owner_, neighbour_.
    // Works out from patch end where the active faces stop.
    initMesh();


    if (validBoundary)
    {
        // Note that we assume that all the patches stay the same and are
        // correct etc. so we can already use the patches to do
        // processor-processor comms.

        // Calculate topology for the patches (processor-processor comms etc.)
        boundary_.updateMesh();

        // Calculate the geometry for the patches (transformation tensors etc.)
        boundary_.calcGeometry();

        // Warn if global empty mesh
        if
        (
            (returnReduce(nPoints(), sumOp<label>()) == 0)
         || (returnReduce(nCells(), sumOp<label>()) == 0)
        )
        {
            FatalErrorInFunction
                << "no points or no cells in mesh" << endl;
        }
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::polyMesh::~polyMesh()
{
    clearOut();
    resetMotion();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::fileName& CML::polyMesh::dbDir() const
{
    if (objectRegistry::dbDir() == defaultRegion)
    {
        return parent().dbDir();
    }
    else
    {
        return objectRegistry::dbDir();
    }
}


CML::fileName CML::polyMesh::meshDir() const
{
    return dbDir()/meshSubDir;
}


const CML::fileName& CML::polyMesh::pointsInstance() const
{
    return points_.instance();
}


const CML::fileName& CML::polyMesh::facesInstance() const
{
    return faces_.instance();
}


const CML::Vector<CML::label>& CML::polyMesh::geometricD() const
{
    if (geometricD_.x() == 0)
    {
        calcDirections();
    }

    return geometricD_;
}


CML::label CML::polyMesh::nGeometricD() const
{
    return cmptSum(geometricD() + Vector<label>::one)/2;
}


const CML::Vector<CML::label>& CML::polyMesh::solutionD() const
{
    if (solutionD_.x() == 0)
    {
        calcDirections();
    }

    return solutionD_;
}


CML::label CML::polyMesh::nSolutionD() const
{
    return cmptSum(solutionD() + Vector<label>::one)/2;
}


const CML::labelIOList& CML::polyMesh::tetBasePtIs() const
{
    if (tetBasePtIsPtr_.empty())
    {
        if (debug)
        {
            WarningInFunction
                << "Forcing storage of base points."
                << endl;
        }

        tetBasePtIsPtr_.reset
        (
            new labelIOList
            (
                IOobject
                (
                    "tetBasePtIs",
                    instance(),
                    meshSubDir,
                    *this,
                    IOobject::READ_IF_PRESENT,
                    IOobject::NO_WRITE
                ),
                polyMeshTetDecomposition::findFaceBasePts(*this)
            )
        );
    }

    return tetBasePtIsPtr_();
}


const CML::indexedOctree<CML::treeDataCell>&
CML::polyMesh::cellTree() const
{
    if (cellTreePtr_.empty())
    {
        cellTreePtr_.reset
        (
            new indexedOctree<treeDataCell>
            (
                treeDataCell
                (
                    false,      // not cache bb
                    *this,
                    FACEDIAGTETS   // use tet-decomposition for any inside test
                ),
                treeBoundBox(points()).extend(1e-4),
                8,              // maxLevel
                10,             // leafsize
                5.0             // duplicity
            )
        );
    }

    return cellTreePtr_();
}


// Add boundary patches. Constructor helper
void CML::polyMesh::addPatches
(
    const List<polyPatch*>& p,
    const bool validBoundary
)
{
    if (boundaryMesh().size())
    {
        FatalErrorInFunction
            << "boundary already exists"
            << abort(FatalError);
    }

    // Reset valid directions
    geometricD_ = Zero;
    solutionD_ = Zero;

    boundary_.setSize(p.size());

    // Copy the patch pointers
    forAll(p, pI)
    {
        boundary_.set(pI, p[pI]);
    }

    // parallelData depends on the processorPatch ordering so force
    // recalculation. Problem: should really be done in removeBoundary but
    // there is some info in parallelData which might be interesting in between
    // removeBoundary and addPatches.
    globalMeshDataPtr_.clear();

    if (validBoundary)
    {
        // Calculate topology for the patches (processor-processor comms etc.)
        boundary_.updateMesh();

        // Calculate the geometry for the patches (transformation tensors etc.)
        boundary_.calcGeometry();

        boundary_.checkDefinition();
    }
}


// Add mesh zones. Constructor helper
void CML::polyMesh::addZones
(
    const List<pointZone*>& pz,
    const List<faceZone*>& fz,
    const List<cellZone*>& cz
)
{
    if (pointZones().size() || faceZones().size() || cellZones().size())
    {
        FatalErrorInFunction
            << "point, face or cell zone already exists"
            << abort(FatalError);
    }

    // Point zones
    if (pz.size())
    {
        pointZones_.setSize(pz.size());

        // Copy the zone pointers
        forAll(pz, pI)
        {
            pointZones_.set(pI, pz[pI]);
        }

        pointZones_.writeOpt() = IOobject::AUTO_WRITE;
    }

    // Face zones
    if (fz.size())
    {
        faceZones_.setSize(fz.size());

        // Copy the zone pointers
        forAll(fz, fI)
        {
            faceZones_.set(fI, fz[fI]);
        }

        faceZones_.writeOpt() = IOobject::AUTO_WRITE;
    }

    // Cell zones
    if (cz.size())
    {
        cellZones_.setSize(cz.size());

        // Copy the zone pointers
        forAll(cz, cI)
        {
            cellZones_.set(cI, cz[cI]);
        }

        cellZones_.writeOpt() = IOobject::AUTO_WRITE;
    }
}


const CML::pointField& CML::polyMesh::points() const
{
    if (clearedPrimitives_)
    {
        FatalErrorInFunction
            << "points deallocated"
            << abort(FatalError);
    }

    return points_;
}


bool CML::polyMesh::upToDatePoints(const regIOobject& io) const
{
    return io.upToDate(points_);
}


void CML::polyMesh::setUpToDatePoints(regIOobject& io) const
{
    io.eventNo() = points_.eventNo()+1;
}


const CML::faceList& CML::polyMesh::faces() const
{
    if (clearedPrimitives_)
    {
        FatalErrorInFunction
            << "faces deallocated"
            << abort(FatalError);
    }

    return faces_;
}


const CML::labelList& CML::polyMesh::faceOwner() const
{
    return owner_;
}


const CML::labelList& CML::polyMesh::faceNeighbour() const
{
    return neighbour_;
}


// Return old mesh motion points
const CML::pointField& CML::polyMesh::oldPoints() const
{
    if (!moving_)
    {
        return points_;
    }

    if (oldPointsPtr_.empty())
    {
        FatalErrorInFunction
            << "Old points have not been stored"
            << exit(FatalError);
    }

    return oldPointsPtr_();
}


const CML::pointField& CML::polyMesh::oldCellCentres() const
{
    storeOldCellCentres_ = true;

    if (!moving_)
    {
        return cellCentres();
    }

    if (oldCellCentresPtr_.empty())
    {
        FatalErrorInFunction
            << "Old cell centres have not been stored"
            << exit(FatalError);
    }

    return oldCellCentresPtr_();
}



CML::tmp<CML::scalarField> CML::polyMesh::movePoints
(
    const pointField& newPoints
)
{
    if (debug)
    {
        InfoInFunction
            << "Moving points for time " << time().value()
            << " index " << time().timeIndex() << endl;
    }

    moving(true);

    // Pick up old points and cell centres
    if (curMotionTimeIndex_ != time().timeIndex())
    {
        oldPointsPtr_.clear();
        oldPointsPtr_.reset(new pointField(points_));
        if (storeOldCellCentres_)
        {
            oldCellCentresPtr_.clear();
            oldCellCentresPtr_.reset(new pointField(cellCentres()));
        }
        curMotionTimeIndex_ = time().timeIndex();
    }

    points_ = newPoints;

    bool moveError = false;
    if (debug)
    {
        // Check mesh motion
        if (checkMeshMotion(points_, true))
        {
            moveError = true;

            InfoInFunction
                << "Moving the mesh with given points will "
                << "invalidate the mesh." << nl
                << "Mesh motion should not be executed." << endl;
        }
    }

    points_.writeOpt() = IOobject::AUTO_WRITE;
    points_.instance() = time().timeName();
    points_.eventNo() = getEvent();

    if (tetBasePtIsPtr_.valid())
    {
        tetBasePtIsPtr_().writeOpt() = IOobject::AUTO_WRITE;
        tetBasePtIsPtr_().instance() = time().timeName();
        tetBasePtIsPtr_().eventNo() = getEvent();
    }

    tmp<scalarField> sweptVols = primitiveMesh::movePoints
    (
        points_,
        oldPoints()
    );

    // Adjust parallel shared points
    if (globalMeshDataPtr_.valid())
    {
        globalMeshDataPtr_().movePoints(points_);
    }

    // Force recalculation of all geometric data with new points

    bounds_ = boundBox(points_);
    boundary_.movePoints(points_);

    pointZones_.movePoints(points_);
    faceZones_.movePoints(points_);
    cellZones_.movePoints(points_);

    // Cell tree might become invalid
    cellTreePtr_.clear();

    // Reset valid directions (could change with rotation)
    geometricD_ = Zero;
    solutionD_ = Zero;


    // Hack until proper callbacks. Below are all the polyMeh MeshObjects with a
    // movePoints function.

    // pointMesh
    if (thisDb().foundObject<pointMesh>(pointMesh::typeName))
    {
        const_cast<pointMesh&>
        (
            thisDb().lookupObject<pointMesh>
            (
                pointMesh::typeName
            )
        ).movePoints(points_);
    }

    return sweptVols;
}


// Reset motion by deleting old points
void CML::polyMesh::resetMotion() const
{
    curMotionTimeIndex_ = -1;
    oldPointsPtr_.clear();
    oldCellCentresPtr_.clear();
}


// Return parallel info
const CML::globalMeshData& CML::polyMesh::globalData() const
{
    if (globalMeshDataPtr_.empty())
    {
        if (debug)
        {
            Pout<< "polyMesh::globalData() const : "
                << "Constructing parallelData from processor topology" << nl
                << "This needs the patch faces to be correctly matched"
                << endl;
        }
        // Construct globalMeshData using processorPatch information only.
        globalMeshDataPtr_.reset(new globalMeshData(*this));
    }

    return globalMeshDataPtr_();
}


// Remove all files and some subdirs (eg, sets)
void CML::polyMesh::removeFiles(const fileName& instanceDir) const
{
    fileName meshFilesPath = thisDb().path()/instanceDir/meshDir();

    rm(meshFilesPath/"points");
    rm(meshFilesPath/"faces");
    rm(meshFilesPath/"owner");
    rm(meshFilesPath/"neighbour");
    rm(meshFilesPath/"cells");
    rm(meshFilesPath/"boundary");
    rm(meshFilesPath/"pointZones");
    rm(meshFilesPath/"faceZones");
    rm(meshFilesPath/"cellZones");
    rm(meshFilesPath/"meshModifiers");
    rm(meshFilesPath/"parallelData");

    // remove subdirectories
    if (isDir(meshFilesPath/"sets"))
    {
        rmDir(meshFilesPath/"sets");
    }
}


void CML::polyMesh::removeFiles() const
{
    removeFiles(instance());
}


void CML::polyMesh::findCellFacePt
(
    const point& p,
    label& celli,
    label& tetFacei,
    label& tetPti
) const
{
    celli = -1;
    tetFacei = -1;
    tetPti = -1;

    const indexedOctree<treeDataCell>& tree = cellTree();

    // Find point inside cell
    celli = tree.findInside(p);

    if (celli != -1)
    {
        // Check the nearest cell to see if the point is inside.
        findTetFacePt(celli, p, tetFacei, tetPti);
    }
}


void CML::polyMesh::findTetFacePt
(
    const label celli,
    const point& p,
    label& tetFacei,
    label& tetPti
) const
{
    const polyMesh& mesh = *this;

    tetIndices tet(polyMeshTetDecomposition::findTet(mesh, celli, p));
    tetFacei = tet.face();
    tetPti = tet.tetPt();
}


bool CML::polyMesh::pointInCell
(
    const point& p,
    label celli,
    const cellRepresentation decompMode
) const
{
    switch (decompMode)
    {
        case FACEPLANES:
        {
            return primitiveMesh::pointInCell(p, celli);
        }
        break;

        case FACECENTRETETS:
        {
            // only test that point is on inside of plane defined by cell face
            // triangles
            const cell& cFaces = cells()[celli];

            forAll(cFaces, cFacei)
            {
                label facei = cFaces[cFacei];
                const face& f = faces_[facei];
                const point& fc = faceCentres()[facei];
                bool isOwn = (owner_[facei] == celli);

                forAll(f, fp)
                {
                    label pointi;
                    label nextPointi;

                    if (isOwn)
                    {
                        pointi = f[fp];
                        nextPointi = f.nextLabel(fp);
                    }
                    else
                    {
                        pointi = f.nextLabel(fp);
                        nextPointi = f[fp];
                    }

                    triPointRef faceTri
                    (
                        points()[pointi],
                        points()[nextPointi],
                        fc
                    );

                    vector proj = p - faceTri.centre();

                    if ((faceTri.area() & proj) > 0)
                    {
                        return false;
                    }
                }
            }
            return true;
        }
        break;

        case FACEDIAGTETS:
        {
            // only test that point is on inside of plane defined by cell face
            // triangles
            const cell& cFaces = cells()[celli];

            forAll(cFaces, cFacei)
            {
                label facei = cFaces[cFacei];
                const face& f = faces_[facei];

                for (label tetPti = 1; tetPti < f.size() - 1; tetPti++)
                {
                    // Get tetIndices of face triangle
                    tetIndices faceTetIs(celli, facei, tetPti);

                    triPointRef faceTri = faceTetIs.faceTri(*this);

                    vector proj = p - faceTri.centre();

                    if ((faceTri.area() & proj) > 0)
                    {
                        return false;
                    }
                }
            }

            return true;
        }
        break;

        case CELL_TETS:
        {
            label tetFacei;
            label tetPti;

            findTetFacePt(celli, p, tetFacei, tetPti);

            return tetFacei != -1;
        }
        break;
    }

    return false;
}


CML::label CML::polyMesh::findCell
(
    const point& p,
    const cellRepresentation decompMode
) const
{
    if
    (
        Pstream::parRun()
     && decompMode == FACEDIAGTETS
    )
    {
        // Force construction of face-diagonal decomposition before testing
        // for zero cells.
        //
        // If parallel running a local domain might have zero cells so never
        // construct the face-diagonal decomposition which uses parallel
        // transfers.
        (void)tetBasePtIs();
    }

    if (nCells() == 0)
    {
        return -1;
    }

    // Find the nearest cell centre to this location
    label celli = findNearestCell(p);

    // If point is in the nearest cell return
    if (pointInCell(p, celli, decompMode))
    {
        return celli;
    }
    else
    {
        // Point is not in the nearest cell so search all cells

        for (label celli = 0; celli < nCells(); celli++)
        {
            if (pointInCell(p, celli, decompMode))
            {
                return celli;
            }
        }

        return -1;
    }
}


// ************************************************************************* //
