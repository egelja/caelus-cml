/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2016 OpenFOAM Foundation
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

#include "pointConstraints.hpp"
#include "emptyPointPatch.hpp"
#include "polyMesh.hpp"
#include "pointMesh.hpp"
#include "globalMeshData.hpp"
#include "twoDPointCorrector.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(pointConstraints, 0);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::pointConstraints::makePatchPatchAddressing()
{
    if (debug)
    {
        Pout<< "pointConstraints::makePatchPatchAddressing() : "
            << "constructing boundary addressing"
            << endl << incrIndent;
    }

    const pointMesh& pMesh = mesh();
    const polyMesh& mesh = pMesh();

    const pointBoundaryMesh& pbm = pMesh.boundary();
    const polyBoundaryMesh& bm = mesh.boundaryMesh();


    // First count the total number of patch-patch points

    label nPatchPatchPoints = 0;

    forAll(pbm, patchi)
    {
        if (!isA<emptyPointPatch>(pbm[patchi]) && !pbm[patchi].coupled())
        {
            const labelList& bp = bm[patchi].boundaryPoints();

            nPatchPatchPoints += bp.size();

            if (debug)
            {
                Pout<< indent << "On patch:" << pbm[patchi].name()
                    << " nBoundaryPoints:" << bp.size() << endl;
            }
        }
    }

    if (debug)
    {
        Pout<< indent << "Found nPatchPatchPoints:" << nPatchPatchPoints
            << endl;
    }


    // Go through all patches and mark up the external edge points
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // From meshpoint to index in patchPatchPointConstraints_.
    Map<label> patchPatchPointSet(2*nPatchPatchPoints);

    // Constraints (initialised to unconstrained)
    patchPatchPointConstraints_.setSize(nPatchPatchPoints);
    patchPatchPointConstraints_ = pointConstraint();

    // From constraint index to mesh point
    labelList patchPatchPoints(nPatchPatchPoints);

    label pppi = 0;

    forAll(pbm, patchi)
    {
        if (!isA<emptyPointPatch>(pbm[patchi]) && !pbm[patchi].coupled())
        {
            const labelList& bp = bm[patchi].boundaryPoints();
            const labelList& meshPoints = pbm[patchi].meshPoints();

            forAll(bp, pointi)
            {
                label ppp = meshPoints[bp[pointi]];

                Map<label>::iterator iter = patchPatchPointSet.find(ppp);

                label constraintI = -1;

                if (iter == patchPatchPointSet.end())
                {
                    patchPatchPointSet.insert(ppp, pppi);
                    patchPatchPoints[pppi] = ppp;
                    constraintI = pppi++;
                }
                else
                {
                    constraintI = iter();
                }

                // Apply to patch constraints
                pbm[patchi].applyConstraint
                (
                    bp[pointi],
                    patchPatchPointConstraints_[constraintI]
                );
            }
        }
    }

    if (debug)
    {
        Pout<< indent << "Have (local) constrained points:"
            << nPatchPatchPoints << endl;
    }


    // Extend set with constraints across coupled points
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    {
        const globalMeshData& gd = mesh.globalData();
        const labelListList& globalPointSlaves = gd.globalPointSlaves();
        const mapDistribute& globalPointSlavesMap = gd.globalPointSlavesMap();
        const Map<label>& cpPointMap = gd.coupledPatch().meshPointMap();
        const labelList& cpMeshPoints = gd.coupledPatch().meshPoints();

        // Constraints on coupled points
        List<pointConstraint> constraints
        (
            globalPointSlavesMap.constructSize()
        );

        // Copy from patchPatch constraints into coupledConstraints.
        forAll(pbm, patchi)
        {
            if (!isA<emptyPointPatch>(pbm[patchi]) && !pbm[patchi].coupled())
            {
                const labelList& bp = bm[patchi].boundaryPoints();
                const labelList& meshPoints = pbm[patchi].meshPoints();

                forAll(bp, pointi)
                {
                    label ppp = meshPoints[bp[pointi]];

                    Map<label>::const_iterator fnd = cpPointMap.find(ppp);
                    if (fnd != cpPointMap.end())
                    {
                        // Can just copy (instead of apply) constraint
                        // will already be consistent across multiple patches.
                        constraints[fnd()] = patchPatchPointConstraints_
                        [
                            patchPatchPointSet[ppp]
                        ];
                    }
                }
            }
        }

        // Exchange data
        globalPointSlavesMap.distribute(constraints);

        // Combine master with slave constraints
        forAll(globalPointSlaves, pointi)
        {
            const labelList& slaves = globalPointSlaves[pointi];

            // Combine master constraint with slave constraints
            forAll(slaves, i)
            {
                constraints[pointi].combine(constraints[slaves[i]]);
            }
            // Duplicate master constraint into slave slots
            forAll(slaves, i)
            {
                constraints[slaves[i]] = constraints[pointi];
            }
        }

        // Send back
        globalPointSlavesMap.reverseDistribute
        (
            cpMeshPoints.size(),
            constraints
        );

        // Add back into patchPatch constraints
        forAll(constraints, coupledPointi)
        {
            if (constraints[coupledPointi].first() != 0)
            {
                label meshPointi = cpMeshPoints[coupledPointi];

                Map<label>::iterator iter = patchPatchPointSet.find(meshPointi);

                label constraintI = -1;

                if (iter == patchPatchPointSet.end())
                {
                    //Pout<< indent << "on meshpoint:" << meshPointi
                    //    << " coupled:" << coupledPointi
                    //    << " at:" << mesh.points()[meshPointi]
                    //    << " have new constraint:"
                    //    << constraints[coupledPointi]
                    //    << endl;

                    // Allocate new constraint
                    if (patchPatchPoints.size() <= pppi)
                    {
                        patchPatchPoints.setSize(pppi+100);
                    }
                    patchPatchPointSet.insert(meshPointi, pppi);
                    patchPatchPoints[pppi] = meshPointi;
                    constraintI = pppi++;
                }
                else
                {
                    //Pout<< indent << "on meshpoint:" << meshPointi
                    //    << " coupled:" << coupledPointi
                    //    << " at:" << mesh.points()[meshPointi]
                    //    << " have possibly extended constraint:"
                    //    << constraints[coupledPointi]
                    //    << endl;

                    constraintI = iter();
                }

                // Combine (new or existing) constraint with one
                // on coupled.
                patchPatchPointConstraints_[constraintI].combine
                (
                    constraints[coupledPointi]
                );
            }
        }
    }



    nPatchPatchPoints = pppi;
    patchPatchPoints.setSize(nPatchPatchPoints);
    patchPatchPointConstraints_.setSize(nPatchPatchPoints);


    if (debug)
    {
        Pout<< indent << "Have (global) constrained points:"
            << nPatchPatchPoints << endl;
    }


    // Copy out all non-trivial constraints
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    patchPatchPointConstraintPoints_.setSize(nPatchPatchPoints);
    patchPatchPointConstraintTensors_.setSize(nPatchPatchPoints);

    label nConstraints = 0;

    forAll(patchPatchPointConstraints_, i)
    {
        // Note: check for more than zero constraints. (could check for
        //       more than one constraint but what about coupled points that
        //       inherit the constraintness)
        if (patchPatchPointConstraints_[i].first() != 0)
        {
            patchPatchPointConstraintPoints_[nConstraints] =
                patchPatchPoints[i];

            patchPatchPointConstraintTensors_[nConstraints] =
                patchPatchPointConstraints_[i].constraintTransformation();

            nConstraints++;
        }
    }

    if (debug)
    {
        Pout<< indent << "Have non-trivial constrained points:"
            << nConstraints << endl;
    }

    patchPatchPointConstraints_.setSize(nConstraints);
    patchPatchPointConstraintPoints_.setSize(nConstraints);
    patchPatchPointConstraintTensors_.setSize(nConstraints);


    if (debug)
    {
        Pout<< decrIndent
            << "pointConstraints::makePatchPatchAddressing() : "
            << "finished constructing boundary addressing"
            << endl;
    }
}


// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

CML::pointConstraints::pointConstraints(const pointMesh& pm)
:
    MeshObject<pointMesh, pointConstraints>(pm)
{
    if (debug)
    {
        Pout<< "pointConstraints::pointConstraints(const pointMesh&): "
            << "Constructing from pointMesh " << pm.name()
            << endl;
    }

    makePatchPatchAddressing();
}


// * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * * //

CML::pointConstraints::~pointConstraints()
{
    if (debug)
    {
        Pout<< "pointConstraints::~pointConstraints()" << endl;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::pointConstraints::updateMesh(const mapPolyMesh&)
{
    makePatchPatchAddressing();
}


bool CML::pointConstraints::movePoints()
{
    return true;
}


void CML::pointConstraints::constrainDisplacement
(
    pointVectorField& pf,
    const bool overrideFixedValue
) const
{
    // Override constrained pointPatchField types with the constraint value.
    // This relies on only constrained pointPatchField implementing the evaluate
    // function
    pf.correctBoundaryConditions();

    // Sync any dangling points
    syncUntransformedData
    (
        pf.mesh()(),
        pf.internalField(),
        maxMagSqrEqOp<vector>()
    );

    // Apply multiple constraints on edge/corner points
    constrainCorners(pf);

    // Apply any 2D motion constraints (or should they go before
    // corner constraints?)
    twoDPointCorrector::New(mesh()()).correctDisplacement
    (
        mesh()().points(),
        pf.internalField()
    );

    if (overrideFixedValue)
    {
        setPatchFields(pf);
    }
}


template<>
void CML::pointConstraints::constrainCorners<CML::scalar>
(
    GeometricField<scalar, pointPatchField, pointMesh>& pf
) const
{}


template<>
void CML::pointConstraints::constrainCorners<CML::label>
(
    GeometricField<label, pointPatchField, pointMesh>& pf
) const
{}


// ************************************************************************* //
