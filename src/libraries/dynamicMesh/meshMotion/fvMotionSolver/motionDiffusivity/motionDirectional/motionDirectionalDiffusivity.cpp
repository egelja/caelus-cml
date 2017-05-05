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

#include "motionDirectionalDiffusivity.hpp"
#include "surfaceInterpolate.hpp"
#include "zeroGradientFvPatchFields.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "velocityMotionSolver.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(motionDirectionalDiffusivity, 0);

    addToRunTimeSelectionTable
    (
        motionDiffusivity,
        motionDirectionalDiffusivity,
        Istream
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::motionDirectionalDiffusivity::motionDirectionalDiffusivity
(
    const fvMesh& mesh,
    Istream& mdData
)
:
    uniformDiffusivity(mesh, mdData),
    diffusivityVector_(mdData)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::motionDirectionalDiffusivity::~motionDirectionalDiffusivity()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::motionDirectionalDiffusivity::correct()
{
    static bool first = true;

    if (!first)
    {
        const volVectorField& cellMotionU =
            mesh().lookupObject<volVectorField>("cellMotionU");

        volVectorField D
        (
            IOobject
            (
                "D",
                mesh().time().timeName(),
                mesh()
            ),
            diffusivityVector_.y()*vector::one
          + (diffusivityVector_.x() - diffusivityVector_.y())*cellMotionU
           /(mag(cellMotionU) + dimensionedScalar("small", dimVelocity, SMALL)),
            zeroGradientFvPatchVectorField::typeName
        );
        D.correctBoundaryConditions();

        const surfaceVectorField n(mesh().Sf()/mesh().magSf());
        faceDiffusivity_ == (n & cmptMultiply(fvc::interpolate(D), n));
    }
    else
    {
        first = false;

        const velocityMotionSolver& mSolver =
            mesh().lookupObject<velocityMotionSolver>("dynamicMeshDict");

        const_cast<velocityMotionSolver&>(mSolver).solve();
        correct();
    }
}


// ************************************************************************* //
