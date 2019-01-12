/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
Copyright (C) 2016 Applied CCM
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

#include "primitiveMesh.hpp"
#include "demandDrivenData.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


namespace CML
{
    defineTypeNameAndDebug(primitiveMesh, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::primitiveMesh::primitiveMesh(const bool defectCorr, const scalar areaSwitch)
:
    nInternalPoints_(0),    // note: points are considered ordered on empty mesh
    nPoints_(0),
    nInternal0Edges_(-1),
    nInternal1Edges_(-1),
    nInternalEdges_(-1),
    nEdges_(-1),
    nInternalFaces_(0),
    nFaces_(0),
    nCells_(0),

    cellShapesPtr_(nullptr),
    edgesPtr_(nullptr),
    ccPtr_(nullptr),
    ecPtr_(nullptr),
    pcPtr_(nullptr),

    cfPtr_(nullptr),
    efPtr_(nullptr),
    pfPtr_(nullptr),

    cePtr_(nullptr),
    fePtr_(nullptr),
    pePtr_(nullptr),
    ppPtr_(nullptr),
    cpPtr_(nullptr),

    labels_(0),

    cellCentresPtr_(nullptr),
    cellCentresGeometricPtr_(nullptr),
    faceCentresPtr_(nullptr),
    cellVolumesPtr_(nullptr),
    faceAreasPtr_(nullptr),
    defectCorr_(defectCorr),
    areaSwitch_(areaSwitch)
{}


// Construct from components
// WARNING: ASSUMES CORRECT ORDERING OF DATA.
CML::primitiveMesh::primitiveMesh
(
    const label nPoints,
    const label nInternalFaces,
    const label nFaces,
    const label nCells,
    const bool defectCorr,
    const scalar areaSwitch
)
:
    nInternalPoints_(-1),
    nPoints_(nPoints),
    nEdges_(-1),
    nInternalFaces_(nInternalFaces),
    nFaces_(nFaces),
    nCells_(nCells),

    cellShapesPtr_(nullptr),
    edgesPtr_(nullptr),
    ccPtr_(nullptr),
    ecPtr_(nullptr),
    pcPtr_(nullptr),

    cfPtr_(nullptr),
    efPtr_(nullptr),
    pfPtr_(nullptr),

    cePtr_(nullptr),
    fePtr_(nullptr),
    pePtr_(nullptr),
    ppPtr_(nullptr),
    cpPtr_(nullptr),

    labels_(0),

    cellCentresPtr_(nullptr),
    cellCentresGeometricPtr_(nullptr),
    faceCentresPtr_(nullptr),
    cellVolumesPtr_(nullptr),
    faceAreasPtr_(nullptr),
    defectCorr_(defectCorr),
    areaSwitch_(areaSwitch)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::primitiveMesh::~primitiveMesh()
{
    clearOut();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::primitiveMesh::calcPointOrder
(
    label& nInternalPoints,
    labelList& oldToNew,
    const faceList& faces,
    const label nInternalFaces,
    const label nPoints
)
{
    // Internal points are points that are not used by a boundary face.

    // Map from old to new position
    oldToNew.setSize(nPoints);
    oldToNew = -1;


    // 1. Create compact addressing for boundary points. Start off by indexing
    // from 0 inside oldToNew. (shifted up later on)

    label nBoundaryPoints = 0;
    for (label faceI = nInternalFaces; faceI < faces.size(); faceI++)
    {
        const face& f = faces[faceI];

        forAll(f, fp)
        {
            label pointI = f[fp];

            if (oldToNew[pointI] == -1)
            {
                oldToNew[pointI] = nBoundaryPoints++;
            }
        }
    }

    // Now we know the number of boundary and internal points

    nInternalPoints = nPoints - nBoundaryPoints;

    // Move the boundary addressing up
    forAll(oldToNew, pointI)
    {
        if (oldToNew[pointI] != -1)
        {
            oldToNew[pointI] += nInternalPoints;
        }
    }


    // 2. Compact the internal points. Detect whether internal and boundary
    // points are mixed.

    label internalPointI = 0;

    bool ordered = true;

    for (label faceI = 0; faceI < nInternalFaces; faceI++)
    {
        const face& f = faces[faceI];

        forAll(f, fp)
        {
            label pointI = f[fp];

            if (oldToNew[pointI] == -1)
            {
                if (pointI >= nInternalPoints)
                {
                    ordered = false;
                }
                oldToNew[pointI] = internalPointI++;
            }
        }
    }

    return ordered;
}


void CML::primitiveMesh::reset
(
    const label nPoints,
    const label nInternalFaces,
    const label nFaces,
    const label nCells
)
{
    clearOut();

    nPoints_ = nPoints;
    nEdges_ = -1;
    nInternal0Edges_ = -1;
    nInternal1Edges_ = -1;
    nInternalEdges_ = -1;

    nInternalFaces_ = nInternalFaces;
    nFaces_ = nFaces;
    nCells_ = nCells;

    // Check if points are ordered
    label nInternalPoints;
    labelList pointMap;

    bool isOrdered = calcPointOrder
    (
        nInternalPoints,
        pointMap,
        faces(),
        nInternalFaces_,
        nPoints_
    );

    if (isOrdered)
    {
        nInternalPoints_ = nInternalPoints;
    }
    else
    {
        nInternalPoints_ = -1;
    }

    if (debug)
    {
        Pout<< "primitiveMesh::reset : mesh reset to"
            << " nInternalPoints:" << nInternalPoints_
            << " nPoints:" << nPoints_
            << " nEdges:" << nEdges_
            << " nInternalFaces:" << nInternalFaces_
            << " nFaces:" << nFaces_
            << " nCells:" << nCells_
            << endl;
    }
}


void CML::primitiveMesh::reset
(
    const label nPoints,
    const label nInternalFaces,
    const label nFaces,
    const label nCells,
    cellList& clst
)
{
    reset
    (
        nPoints,
        nInternalFaces,
        nFaces,
        nCells
    );

    cfPtr_ = new cellList(clst, true);
}


void CML::primitiveMesh::reset
(
    const label nPoints,
    const label nInternalFaces,
    const label nFaces,
    const label nCells,
    const Xfer<cellList>& clst
)
{
    reset
    (
        nPoints,
        nInternalFaces,
        nFaces,
        nCells
    );

    cfPtr_ = new cellList(clst);
}


CML::tmp<CML::scalarField> CML::primitiveMesh::movePoints
(
    const pointField& newPoints,
    const pointField& oldPoints
)
{
    if (newPoints.size() <  nPoints() || oldPoints.size() < nPoints())
    {
        FatalErrorInFunction
            << "Cannot move points: size of given point list smaller "
            << "than the number of active points"
            << abort(FatalError);
    }

    // Create swept volumes
    const faceList& f = faces();

    tmp<scalarField> tsweptVols(new scalarField(f.size()));
    scalarField& sweptVols = tsweptVols();

    forAll(f, faceI)
    {
        sweptVols[faceI] = f[faceI].sweptVol(oldPoints, newPoints);
    }

    // Force recalculation of all geometric data with new points
    clearGeom();

    return tsweptVols;
}


const CML::cellShapeList& CML::primitiveMesh::cellShapes() const
{
    if (!cellShapesPtr_)
    {
        calcCellShapes();
    }

    return *cellShapesPtr_;
}


// ************************************************************************* //
