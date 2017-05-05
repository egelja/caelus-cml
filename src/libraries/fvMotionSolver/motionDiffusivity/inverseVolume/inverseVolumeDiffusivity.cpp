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

#include "inverseVolumeDiffusivity.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "patchWave.hpp"
#include "HashSet.hpp"
#include "surfaceInterpolate.hpp"
#include "zeroGradientFvPatchFields.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(inverseVolumeDiffusivity, 0);

    addToRunTimeSelectionTable
    (
        motionDiffusivity,
        inverseVolumeDiffusivity,
        Istream
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::inverseVolumeDiffusivity::inverseVolumeDiffusivity
(
    const fvMotionSolver& mSolver,
    Istream& mdData
)
:
    uniformDiffusivity(mSolver, mdData)
{
    correct();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::inverseVolumeDiffusivity::~inverseVolumeDiffusivity()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::inverseVolumeDiffusivity::correct()
{
    const fvMesh& mesh = mSolver().mesh();

    volScalarField V
    (
        IOobject
        (
            "V",
            mesh.time().timeName(),
            mesh
        ),
        mesh,
        dimless,
        zeroGradientFvPatchScalarField::typeName
    );

    V.internalField() = mesh.V();
    V.correctBoundaryConditions();

    faceDiffusivity_ = 1.0/fvc::interpolate(V);
}


// ************************************************************************* //
