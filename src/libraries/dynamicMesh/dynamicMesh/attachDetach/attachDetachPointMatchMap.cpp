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

#include "attachDetach.hpp"
#include "polyMesh.hpp"
#include "primitiveMesh.hpp"
#include "primitiveFacePatch.hpp"
#include "polyTopoChanger.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

const CML::Map<CML::label>&
CML::attachDetach::pointMatchMap() const
{
    if (!pointMatchMapPtr_)
    {
        calcPointMatchMap();
    }

    return *pointMatchMapPtr_;
}


void CML::attachDetach::calcPointMatchMap() const
{
    if (debug)
    {
        Pout<< "void attachDetach::calcPointMatchMap() const "
            << " for object " << name() << " : "
            << "Calculating point matching" << endl;
    }

    if (pointMatchMapPtr_)
    {
        FatalErrorInFunction
            << "Point match map already calculated for object " << name()
            << abort(FatalError);
    }

    const polyMesh& mesh = topoChanger().mesh();
    const faceList& faces = mesh.faces();

    const polyPatch& masterPatch = mesh.boundaryMesh()[masterPatchID_.index()];
    const polyPatch& slavePatch = mesh.boundaryMesh()[slavePatchID_.index()];

    // Create the reverse patch out of the slave patch
    primitiveFacePatch reverseSlavePatch
    (
        faceList(slavePatch.size()),
        mesh.points()
    );

    const label slavePatchStart = slavePatch.start();

    forAll(reverseSlavePatch, faceI)
    {
        reverseSlavePatch[faceI] =
            faces[slavePatchStart + faceI].reverseFace();
    }

    // Create point merge list and remove merged points
    const labelList& masterMeshPoints = masterPatch.meshPoints();
    const labelList& slaveMeshPoints = reverseSlavePatch.meshPoints();

    const faceList& masterLocalFaces = masterPatch.localFaces();
    const faceList& slaveLocalFaces = reverseSlavePatch.localFaces();

    pointMatchMapPtr_ = new Map<label>(2*slaveMeshPoints.size());
    Map<label>& removedPointMap = *pointMatchMapPtr_;

    forAll(masterLocalFaces, faceI)
    {
        const face& curMasterPoints = masterLocalFaces[faceI];
        const face& curSlavePoints = slaveLocalFaces[faceI];

        forAll(curMasterPoints, pointI)
        {
            // If the master and slave point labels are the same, the
            // point remains.  Otherwise, the slave point is removed and
            // replaced by the master
            if
            (
                masterMeshPoints[curMasterPoints[pointI]]
             != slaveMeshPoints[curSlavePoints[pointI]]
            )
            {
                // Pout<< "Matching slave point "
                //     << slaveMeshPoints[curSlavePoints[pointI]]
                //     << " with "
                //     << masterMeshPoints[curMasterPoints[pointI]]
                //     << endl;

                // Grab the addressing
                removedPointMap.insert
                (
                    slaveMeshPoints[curSlavePoints[pointI]],
                    masterMeshPoints[curMasterPoints[pointI]]
                );
            }
        }
    }

    if (debug)
    {
        Pout<< "void attachDetach::calcPointMatchMap() const "
            << " for object " << name() << " : "
            << "Finished calculating point matching" << endl;
    }
}


// ************************************************************************* //
