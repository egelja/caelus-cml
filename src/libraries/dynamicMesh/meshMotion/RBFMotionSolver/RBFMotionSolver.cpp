/*---------------------------------------------------------------------------*\
Copyright (C) 2009 Frank Bos
Copyright (C) 2016 Applied CCM Pty Ltd
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "RBFMotionSolver.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "transformField.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(RBFMotionSolver, 0);

    addToRunTimeSelectionTable
    (
        motionSolver,
        RBFMotionSolver,
        dictionary
    );
}

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::RBFMotionSolver::makeControlIDs()
{
    // Points that are neither on moving nor on static patches
    // will be marked with 0
    labelList markedPoints(mesh().nPoints(), 0);

    // Mark all points on moving patches with 1
    label nMovingPoints = 0;

    forAll (movingPatches_, patchI)
    {
        // Find the patch in boundary
        label patchIndex =
            mesh().boundaryMesh().findPatchID(movingPatches_[patchI]);

        if (patchIndex < 0)
        {
            FatalErrorIn("void RBFMotionSolver::makeControlIDs()")
                << "Patch " << movingPatches_[patchI] << " not found.  "
                << "valid patch names: " << mesh().boundaryMesh().names()
                << abort(FatalError);
        }

        const labelList& mp = mesh().boundaryMesh()[patchIndex].meshPoints();

        forAll (mp, i)
        {
            markedPoints[mp[i]] = 1;
            nMovingPoints++;
        }
    }

    // Mark moving points and select control points from moving patches
    movingIDs_.setSize(nMovingPoints);

    Info<< "Total points on moving boundaries: " << nMovingPoints << endl;

    const pointField& points = mesh().points();

    // Re-use counter to count moving points
    // Note: the control points also hold static points in the second part
    // of the list if static patches are included in the RBF
    nMovingPoints = 0;

    // Count moving points first
    forAll (markedPoints, i)
    {
        if (markedPoints[i] == 1)
        {
            // Grab internal point
            movingIDs_[nMovingPoints] = i;
            nMovingPoints++;
        }
    }

    movingIDs_.setSize(nMovingPoints);

    // Actual location of moving points will be set later on request
    movingPoints_.setSize(nMovingPoints, vector::zero);

    // Mark all points on static patches with -1
    label nStaticPoints = 0;

    forAll (staticPatches_, patchI)
    {
        // Find the patch in boundary
        label patchIndex =
            mesh().boundaryMesh().findPatchID(staticPatches_[patchI]);

        if (patchIndex < 0)
        {
            FatalErrorIn("void RBFMotionSolver::makeControlPoints()")
                << "Patch " << staticPatches_[patchI] << " not found.  "
                << "valid patch names: " << mesh().boundaryMesh().names()
                << abort(FatalError);
        }

        const labelList& mp = mesh().boundaryMesh()[patchIndex].meshPoints();

        forAll (mp, i)
        {
            markedPoints[mp[i]] = -1;
            nStaticPoints++;
        }
    }

    Info<< "Total points on static boundaries: " << nStaticPoints << endl;
    staticIDs_.setSize(nStaticPoints);

    // Re-use counter
    nStaticPoints = 0;

    // Count total number of control points
    forAll (markedPoints, i)
    {
        if (markedPoints[i] == -1)
        {
            staticIDs_[nStaticPoints] = i;
            nStaticPoints++;
        }
    }

    staticIDs_.setSize(nStaticPoints);

    // Control IDs also potentially include points on static patches
    controlIDs_.setSize(movingIDs_.size() + staticIDs_.size());
    motion_.setSize(controlIDs_.size(), vector::zero);

    label nControlPoints = 0;

    forAll (movingPatches_, patchI)
    {
        // Find the patch in boundary
        label patchIndex =
            mesh().boundaryMesh().findPatchID(movingPatches_[patchI]);

        const labelList& mp = mesh().boundaryMesh()[patchIndex].meshPoints();

        for
        (
            label pickedPoint = 0;
            pickedPoint < mp.size();
            pickedPoint += coarseningRatio_
        )
        {
            // Pick point as control point
            controlIDs_[nControlPoints] = mp[pickedPoint];

            // Mark the point as picked
            markedPoints[mp[pickedPoint]] = 2;
            nControlPoints++;
        }
    }

    Info<< "Selected " << nControlPoints
        << " control points on moving boundaries" << endl;

    if (includeStaticPatches_)
    {
        forAll (staticPatches_, patchI)
        {
            // Find the patch in boundary
            label patchIndex =
                mesh().boundaryMesh().findPatchID(staticPatches_[patchI]);

            const labelList& mp =
                mesh().boundaryMesh()[patchIndex].meshPoints();

            for
            (
                label pickedPoint = 0;
                pickedPoint < mp.size();
                pickedPoint += coarseningRatio_
            )
            {
                // Pick point as control point
                controlIDs_[nControlPoints] = mp[pickedPoint];

                // Mark the point as picked
                markedPoints[mp[pickedPoint]] = 2;
                nControlPoints++;
            }
        }

        Info<< "Selected " << nControlPoints
            << " total control points" << endl;
    }

    // Resize control IDs
    controlIDs_.setSize(nControlPoints);

    // Pick up point locations
    controlPoints_.setSize(nControlPoints);

    // Set control points
    forAll (controlIDs_, i)
    {
        controlPoints_[i] = points[controlIDs_[i]];
    }

    // Pick up all internal points
    internalIDs_.setSize(points.size());
    internalPoints_.setSize(points.size());

    // Count internal points
    label nInternalPoints = 0;

    forAll (markedPoints, i)
    {
        if (markedPoints[i] == 0)
        {
            // Grab internal point
            internalIDs_[nInternalPoints] = i;
            internalPoints_[nInternalPoints] = points[i];
            nInternalPoints++;
        }
    }

    Info << "Number of internal points: " << nInternalPoints << endl;

    // Resize the lists
    internalIDs_.setSize(nInternalPoints);
    internalPoints_.setSize(nInternalPoints);
}


void CML::RBFMotionSolver::setMovingPoints() const
{
    const pointField& points = mesh().points();

    // Set moving points
    forAll (movingIDs_, i)
    {
        movingPoints_[i] = points[movingIDs_[i]];
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::RBFMotionSolver::RBFMotionSolver
(
    const polyMesh& mesh,
    const IOdictionary& dict
)
:
    motionSolver(mesh,dict,typeName),
    SBMFPtr_(solidBodyMotionFunction::New(coeffDict(), mesh.time(), true)),
    movingPatches_(coeffDict().lookup("movingPatches")),
    staticPatches_(coeffDict().lookup("staticPatches")),
    coarseningRatio_(readLabel(coeffDict().lookup("coarseningRatio"))),
    includeStaticPatches_(coeffDict().lookup("includeStaticPatches")),
    frozenInterpolation_(coeffDict().lookup("frozenInterpolation")),
    movingIDs_(0),
    movingPoints_(0),
    staticIDs_(0),
    controlIDs_(0),
    controlPoints_(0),
    internalIDs_(0),
    internalPoints_(0),
    motion_(0),
    interpolation_
    (
        coeffDict().subDict("interpolation"),
        controlPoints_,
        internalPoints_
    )
{
    makeControlIDs();
    undisplacedPoints_ = movingPoints();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::RBFMotionSolver::~RBFMotionSolver()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::RBFMotionSolver::setMotion(const vectorField& m)
{
    if (m.size() != movingIDs_.size())
    {
        FatalErrorIn
        (
            "void RBFMotionSolver::setMotion(const vectorField& m)"
        )   << "Incorrect size of motion points: m = " << m.size()
            << " movingIDs = " << movingIDs_.size()
            << abort(FatalError);
    }

    // Motion of static points is zero and moving points are first
    // in the list.
    motion_ = vector::zero;

    forAll (m, i)
    {
        motion_[i] = m[i];
    }

    if (!frozenInterpolation_)
    {
        // Set control points
        const pointField& points = mesh().points();

        forAll (controlIDs_, i)
        {
            controlPoints_[i] = points[controlIDs_[i]];
        }

        // Re-calculate interpolation
        interpolation_.movePoints();
    }
}


const CML::vectorField& CML::RBFMotionSolver::movingPoints() const
{
    // Update moving points based on current mesh
    setMovingPoints();

    return movingPoints_;
}


CML::tmp<CML::pointField> CML::RBFMotionSolver::curPoints() const
{
    // Prepare new points: same as old point
    tmp<pointField> tcurPoints
    (
        new vectorField(mesh().nPoints(), vector::zero)
    );
    pointField& curPoints = tcurPoints();

    // Add motion to existing points

    // 1. Insert prescribed motion of moving points
    forAll (movingIDs_, i)
    {
        curPoints[movingIDs_[i]] = motion_[i];
    }

    // 2. Insert zero motion of static points
    forAll (staticIDs_, i)
    {
        curPoints[staticIDs_[i]] = vector::zero;
    }

    // Set motion of control
    vectorField motionOfControl(controlIDs_.size());

    // 2. Capture positions of control points
    forAll (controlIDs_, i)
    {
        motionOfControl[i] = curPoints[controlIDs_[i]];
    }

    // Call interpolation
    vectorField interpolatedMotion(interpolation_.interpolate(motionOfControl));

    // 3. Insert RBF interpolated motion
    forAll (internalIDs_, i)
    {
        curPoints[internalIDs_[i]] = interpolatedMotion[i];
    }

    // 4. Add old point positions
    curPoints += mesh().points();

    twoDCorrectPoints(tcurPoints());

    return tcurPoints;
}


void CML::RBFMotionSolver::solve()
{
    // Motion is a vectorField of all moving boundary points
    vectorField motion(movingPoints().size(), vector::zero);

    vectorField oldPoints = movingPoints();
    vectorField newPoints = oldPoints;

    // Transform oldPoints into newPoints
    CML::transformPoints(newPoints, SBMFPtr_().transformation(), oldPoints);

    // Work out change in position
    motion = newPoints - oldPoints;

    setMotion(motion);

}


void CML::RBFMotionSolver::movePoints(const pointField&)
{
    // No local data to update
}


void CML::RBFMotionSolver::updateMesh(const mapPolyMesh&)
{
    // Recalculate control point IDs
    makeControlIDs();
}


// ************************************************************************* //
