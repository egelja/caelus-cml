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

#include "particle.hpp"
#include "transform.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

CML::label CML::particle::particleCount_ = 0;

const CML::scalar CML::particle::trackingCorrectionTol = 1e-5;

const CML::scalar CML::particle::lambdaDistanceToleranceCoeff = 1e-12; //1e3*SMALL;

const CML::scalar CML::particle::minStepFractionTol = 1e-10; //1e5*SMALL;

namespace CML
{
    defineTypeNameAndDebug(particle, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::particle::particle
(
    const polyMesh& mesh,
    const vector& position,
    const label cellI,
    const label tetFaceI,
    const label tetPtI
)
:
    mesh_(mesh),
    position_(position),
    cellI_(cellI),
    faceI_(-1),
    stepFraction_(0.0),
    tetFaceI_(tetFaceI),
    tetPtI_(tetPtI),
    origProc_(Pstream::myProcNo()),
    origId_(getNewParticleID())
{}


CML::particle::particle
(
    const polyMesh& mesh,
    const vector& position,
    const label cellI,
    bool doCellFacePt
)
:
    mesh_(mesh),
    position_(position),
    cellI_(cellI),
    faceI_(-1),
    stepFraction_(0.0),
    tetFaceI_(-1),
    tetPtI_(-1),
    origProc_(Pstream::myProcNo()),
    origId_(getNewParticleID())
{
    if (doCellFacePt)
    {
        initCellFacePt();
    }
}


CML::particle::particle(const particle& p)
:
    mesh_(p.mesh_),
    position_(p.position_),
    cellI_(p.cellI_),
    faceI_(p.faceI_),
    stepFraction_(p.stepFraction_),
    tetFaceI_(p.tetFaceI_),
    tetPtI_(p.tetPtI_),
    origProc_(p.origProc_),
    origId_(p.origId_)
{}


CML::particle::particle(const particle& p, const polyMesh& mesh)
:
    mesh_(mesh),
    position_(p.position_),
    cellI_(p.cellI_),
    faceI_(p.faceI_),
    stepFraction_(p.stepFraction_),
    tetFaceI_(p.tetFaceI_),
    tetPtI_(p.tetPtI_),
    origProc_(p.origProc_),
    origId_(p.origId_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::particle::transformProperties(const tensor&)
{}


void CML::particle::transformProperties(const vector&)
{}


CML::scalar CML::particle::wallImpactDistance(const vector&) const
{
    Info<< "particle::wallImpactDistance" << endl;

    return 0.0;
}


// * * * * * * * * * * * * * * Friend Operators * * * * * * * * * * * * * * //

bool CML::operator==(const particle& pA, const particle& pB)
{
    return (pA.origProc() == pB.origProc() && pA.origId() == pB.origId());
}


bool CML::operator!=(const particle& pA, const particle& pB)
{
    return !(pA == pB);
}


// ************************************************************************* //
