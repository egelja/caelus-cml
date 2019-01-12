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

#include "displacementComponentLaplacianFvMotionSolver.hpp"
#include "motionDiffusivity.hpp"
#include "fvmLaplacian.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "mapPolyMesh.hpp"
#include "volPointInterpolation.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(displacementComponentLaplacianFvMotionSolver, 0);

    addToRunTimeSelectionTable
    (
        motionSolver,
        displacementComponentLaplacianFvMotionSolver,
        dictionary
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::displacementComponentLaplacianFvMotionSolver::
displacementComponentLaplacianFvMotionSolver
(
    const polyMesh& mesh,
    const IOdictionary& dict
)
:
    componentDisplacementMotionSolver(mesh, dict, type()),
    fvMotionSolverCore(mesh),
    cellDisplacement_
    (
        IOobject
        (
            "cellDisplacement" + cmptName_,
            mesh.time().timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        fvMesh_,
        dimensionedScalar
        (
            "cellDisplacement",
            pointDisplacement_.dimensions(),
            0
        ),
        cellMotionBoundaryTypes<scalar>(pointDisplacement_.boundaryField())
    ),
    pointLocation_(nullptr),
    diffusivityPtr_
    (
        motionDiffusivity::New(fvMesh_, coeffDict().lookup("diffusivity"))
    ),
    frozenPointsZone_
    (
        coeffDict().found("frozenPointsZone")
      ? fvMesh_.pointZones().findZoneID(coeffDict().lookup("frozenPointsZone"))
      : -1
    )
{
    Switch applyPointLocation
    (
        coeffDict().lookupOrDefault
        (
            "applyPointLocation",
            true
        )
    );

    if (applyPointLocation)
    {
        pointLocation_.reset
        (
            new pointVectorField
            (
                IOobject
                (
                    "pointLocation",
                    fvMesh_.time().timeName(),
                    fvMesh_,
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                pointMesh::New(fvMesh_)
            )
        );

        //if (debug)
        {
            Info<< "displacementComponentLaplacianFvMotionSolver :"
                << " Read pointVectorField "
                << pointLocation_().name()
                << " to be used for boundary conditions on points."
                << nl
                << "Boundary conditions:"
                << pointLocation_().boundaryField().types() << endl;
        }
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::displacementComponentLaplacianFvMotionSolver::
~displacementComponentLaplacianFvMotionSolver()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::tmp<CML::pointField>
CML::displacementComponentLaplacianFvMotionSolver::curPoints() const
{
    volPointInterpolation::New(fvMesh_).interpolate
    (
        cellDisplacement_,
        pointDisplacement_
    );

    if (pointLocation_.valid())
    {
        if (debug)
        {
            Info<< "displacementComponentLaplacianFvMotionSolver : applying "
                << " boundary conditions on " << pointLocation_().name()
                << " to new point location."
                << endl;
        }

        // Apply pointLocation_ b.c. to mesh points.

        pointLocation_().internalField() = fvMesh_.points();

        pointLocation_().internalField().replace
        (
            cmpt_,
            points0_ + pointDisplacement_.internalField()
        );

        pointLocation_().correctBoundaryConditions();

        // Implement frozen points
        if (frozenPointsZone_ != -1)
        {
            const pointZone& pz = fvMesh_.pointZones()[frozenPointsZone_];

            forAll(pz, i)
            {
                label pointI = pz[i];

                pointLocation_()[pointI][cmpt_] = points0_[pointI];
            }
        }

        twoDCorrectPoints(pointLocation_().internalField());

        return tmp<pointField>(pointLocation_().internalField());
    }
    else
    {
        tmp<pointField> tcurPoints(new pointField(fvMesh_.points()));
        pointField& curPoints = tcurPoints();

        curPoints.replace
        (
            cmpt_,
            points0_ + pointDisplacement_.internalField()
        );

        // Implement frozen points
        if (frozenPointsZone_ != -1)
        {
            const pointZone& pz = fvMesh_.pointZones()[frozenPointsZone_];

            forAll(pz, i)
            {
                label pointI = pz[i];

                curPoints[pointI][cmpt_] = points0_[pointI];
            }
        }

        twoDCorrectPoints(curPoints);

        return tcurPoints;
    }
}


void CML::displacementComponentLaplacianFvMotionSolver::solve()
{
    // The points have moved so before interpolation update
    // the motionSolver accordingly
    movePoints(fvMesh_.points());

    diffusivityPtr_->correct();
    pointDisplacement_.boundaryField().updateCoeffs();

    CML::solve
    (
        fvm::laplacian
        (
            diffusivityPtr_->operator()(),
            cellDisplacement_,
            "laplacian(diffusivity,cellDisplacement)"
        )
    );
}


void CML::displacementComponentLaplacianFvMotionSolver::updateMesh
(
    const mapPolyMesh& mpm
)
{
    componentDisplacementMotionSolver::updateMesh(mpm);

    // Update diffusivity. Note two stage to make sure old one is de-registered
    // before creating/registering new one.
    diffusivityPtr_.reset(nullptr);
    diffusivityPtr_ = motionDiffusivity::New
    (
        fvMesh_,
        coeffDict().lookup("diffusivity")
    );
}


// ************************************************************************* //
