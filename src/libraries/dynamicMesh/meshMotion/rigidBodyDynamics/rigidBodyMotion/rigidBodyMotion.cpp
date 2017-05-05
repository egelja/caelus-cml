/*---------------------------------------------------------------------------*\
Copyright (C) 2016 OpenFOAM Foundation
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

#include "rigidBodyMotion.hpp"
#include "rigidBodySolver.hpp"
#include "rigidBodyRestraint.hpp"
#include "septernion.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::RBD::rigidBodyMotion::initialize()
{
    // Calculate the initial body-state
    forwardDynamicsCorrection(rigidBodyModelState(*this));
    X00_ = X0_;

    // Update the body-state to correspond to the current joint-state
    forwardDynamicsCorrection(motionState_);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::RBD::rigidBodyMotion::rigidBodyMotion
(
    const Time& time
)
:
    rigidBodyModel(time),
    motionState_(*this),
    motionState0_(*this),
    aRelax_(1.0),
    aDamp_(1.0),
    report_(false),
    solver_(NULL)
{}

CML::RBD::rigidBodyMotion::rigidBodyMotion
(
    const Time& time,
    const dictionary& dict
)
:
    rigidBodyModel(time, dict),
    motionState_(*this, dict),
    motionState0_(motionState_),
    X00_(X0_.size()),
    aRelax_(dict.lookupOrDefault<scalar>("accelerationRelaxation", 1.0)),
    aDamp_(dict.lookupOrDefault<scalar>("accelerationDamping", 1.0)),
    report_(dict.lookupOrDefault<Switch>("report", false)),
    solver_(rigidBodySolver::New(*this, dict.subDict("solver")))
{
    if (dict.found("g"))
    {
        g() = vector(dict.lookup("g"));
    }

    initialize();
}


CML::RBD::rigidBodyMotion::rigidBodyMotion
(
    const Time& time,
    const dictionary& dict,
    const dictionary& stateDict
)
:
    rigidBodyModel(time, dict),
    motionState_(*this, stateDict),
    motionState0_(motionState_),
    X00_(X0_.size()),
    aRelax_(dict.lookupOrDefault<scalar>("accelerationRelaxation", 1.0)),
    aDamp_(dict.lookupOrDefault<scalar>("accelerationDamping", 1.0)),
    report_(dict.lookupOrDefault<Switch>("report", false)),
    solver_(rigidBodySolver::New(*this, dict.subDict("solver")))
{
    if (dict.found("g"))
    {
        g() = vector(dict.lookup("g"));
    }

    initialize();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::RBD::rigidBodyMotion::~rigidBodyMotion()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

CML::spatialTransform CML::RBD::rigidBodyMotion::X00
(
    const label bodyId
) const
{
    if (merged(bodyId))
    {
        const subBody& mBody = mergedBody(bodyId);
        return mBody.masterXT() & X00_[mBody.masterID()];
    }
    else
    {
        return X00_[bodyId];
    }
}


void CML::RBD::rigidBodyMotion::forwardDynamics
(
    rigidBodyModelState& state,
    const scalarField& tau,
    const Field<spatialVector>& fx
) const
{
    scalarField qDdotPrev = state.qDdot();
    rigidBodyModel::forwardDynamics(state, tau, fx);
    state.qDdot() = aDamp_*(aRelax_*state.qDdot() + (1 - aRelax_)*qDdotPrev);
}


void CML::RBD::rigidBodyMotion::solve
(
    scalar deltaT,
    const scalarField& tau,
    const Field<spatialVector>& fx
)
{
    motionState_.deltaT() = deltaT;

    if (motionState0_.deltaT() < SMALL)
    {
        motionState0_.deltaT() = deltaT;
    }

    if (Pstream::master())
    {
        solver_->solve(tau, fx);
    }

    Pstream::scatter(motionState_);

    // Update the body-state to correspond to the current joint-state
    forwardDynamicsCorrection(motionState_);
}


void CML::RBD::rigidBodyMotion::status(const label bodyID) const
{
    const spatialTransform CofR(X0(bodyID));
    const spatialVector vCofR(v(bodyID, Zero));

    Info<< "Rigid-body motion of the " << name(bodyID) << nl
        << "    Centre of rotation: " << CofR.r() << nl
        << "    Orientation: " << CofR.E() << nl
        << "    Linear velocity: " << vCofR.l() << nl
        << "    Angular velocity: " << vCofR.w()
        << endl;
}

void CML::RBD::rigidBodyMotion::writeState(const label bodyID, autoPtr<OFstream> &stateFilePtr)
{
    const spatialTransform CofR(X0(bodyID));
    const spatialVector vCofR(v(bodyID, Zero));
        
    stateFilePtr()
		<< token::SPACE << CofR.r()
        << token::SPACE << vCofR.l()
        << token::SPACE << vCofR.w()
        << token::SPACE << CofR.E()
        << endl;
}

void CML::RBD::rigidBodyMotion::writeRestState(List<autoPtr<OFstream> > &restraintStateFilePtrs)
{
    
    forAll(restraints_,ri)
    {
        restraints_[ri].writeState(restraintStateFilePtrs[ri]);
    }
}

CML::tmp<CML::pointField> CML::RBD::rigidBodyMotion::transformPoints
(
    const label bodyID,
    const pointField& initialPoints
) const
{
    // Calculate the transform from the initial state in the global frame
    // to the current state in the global frame
    spatialTransform X(X0(bodyID).inv() & X00(bodyID));

    tmp<pointField> tpoints(new pointField(initialPoints.size()));
    pointField& points = tpoints();

    forAll(points, i)
    {
        points[i] = X.transformPoint(initialPoints[i]);
    }

    return tpoints;
}


CML::tmp<CML::pointField> CML::RBD::rigidBodyMotion::transformPoints
(
    const label bodyID,
    const scalarField& weight,
    const pointField& initialPoints
) const
{
    // Calculate the transform from the initial state in the global frame
    // to the current state in the global frame
    spatialTransform X(X0(bodyID).inv() & X00(bodyID));

    // Calculate the septernion equivalent of the transformation for 'slerp'
    // interpolation
    septernion s(X);

    tmp<pointField> tpoints(new pointField(initialPoints));
    pointField& points = tpoints();

    forAll(points, i)
    {
        // Move non-stationary points
        if (weight[i] > SMALL)
        {
            // Use solid-body motion where weight = 1
            if (weight[i] > 1 - SMALL)
            {
                points[i] = X.transformPoint(initialPoints[i]);
            }
            // Slerp septernion interpolation
            else
            {
                points[i] =
                    slerp(septernion::I, s, weight[i])
                   .transformPoint(initialPoints[i]);
            }
        }
    }

    return tpoints;
}


CML::tmp<CML::pointField> CML::RBD::rigidBodyMotion::transformPoints
(
    const labelList& bodyIDs,
    const List<const scalarField*>& weights,
    const pointField& initialPoints
) const
{
    List<septernion> ss(bodyIDs.size() + 1);
    ss[bodyIDs.size()] = septernion::I;

    forAll(bodyIDs, bi)
    {
        const label bodyID = bodyIDs[bi];

        // Calculate the transform from the initial state in the global frame
        // to the current state in the global frame
        spatialTransform X(X0(bodyID).inv() & X00(bodyID));

        // Calculate the septernion equivalent of the transformation
        ss[bi] = septernion(X);
    }

    tmp<pointField> tpoints(new pointField(initialPoints));
    pointField& points = tpoints();

    List<scalar> w(ss.size());

    forAll(points, i)
    {
        // Initialize to 1 for the far-field weight
        scalar sum1mw = 1;

        forAll(bodyIDs, bi)
        {
            w[bi] = (*(weights[bi]))[i];
            sum1mw += w[bi]/(1 + SMALL - w[bi]);
        }

        // Calculate the limiter for wi/(1 - wi) to ensure the sum(wi) = 1
        scalar lambda = 1/sum1mw;

        // Limit wi/(1 - wi) and sum the resulting wi
        scalar sumw = 0;
        forAll(bodyIDs, bi)
        {
            w[bi] = lambda*w[bi]/(1 + SMALL - w[bi]);
            sumw += w[bi];
        }

        // Calculate the weight for the stationary far-field
        w[bodyIDs.size()] = 1 - sumw;

        points[i] = average(ss, w).transformPoint(initialPoints[i]);
    }

    return tpoints;
}


CML::septernion CML::RBD::rigidBodyMotion::transformation(const label bodyID) const
{
    // Calculate the transform from the initial state in the global frame
    // to the current state in the global frame
    spatialTransform X(X0(bodyID).inv() & X00(bodyID));

    // Calculate the septernion equivalent of the transformation
    return septernion(X);
}


// ************************************************************************* //
