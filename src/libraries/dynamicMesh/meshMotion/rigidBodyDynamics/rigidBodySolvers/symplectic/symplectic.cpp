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

#include "symplectic.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace RBD
{
namespace rigidBodySolvers
{
    defineTypeNameAndDebug(symplectic, 0);
    addToRunTimeSelectionTable(rigidBodySolver, symplectic, dictionary);
}
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::RBD::rigidBodySolvers::symplectic::symplectic
(
    rigidBodyMotion& body,
    const dictionary& dict
)
:
    rigidBodySolver(body)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::RBD::rigidBodySolvers::symplectic::~symplectic()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::RBD::rigidBodySolvers::symplectic::solve
(
    const scalarField& tau,
    const Field<spatialVector>& fx
)
{
    // First simplectic step:
    //     Half-step for linear and angular velocities
    //     Update position and orientation
    qDot() = qDot0() + 0.5*deltaT0()*qDdot();
    q() = q0() + deltaT()*qDot();

    correctQuaternionJoints();

    // Update the body-state prior to the evaluation of the restraints
    model_.forwardDynamicsCorrection(state());

    // Accumulate the restraint forces
    scalarField rtau(tau);
    Field<spatialVector> rfx(fx);
    model_.applyRestraints(rtau, rfx);

    // Calculate the body acceleration for the given state
    // and restraint forces
    model_.forwardDynamics(state(), rtau, rfx);

    // Second simplectic step:
    //     Complete update of linear and angular velocities
    qDot() += 0.5*deltaT()*qDdot();
}


// ************************************************************************* //
