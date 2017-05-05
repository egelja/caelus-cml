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

#include "Newmark.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace RBD
{
namespace rigidBodySolvers
{
    defineTypeNameAndDebug(Newmark, 0);
    addToRunTimeSelectionTable(rigidBodySolver, Newmark, dictionary);
}
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::RBD::rigidBodySolvers::Newmark::Newmark
(
    rigidBodyMotion& body,
    const dictionary& dict
)
:
    rigidBodySolver(body),
    gamma_(dict.lookupOrDefault<scalar>("gamma", 0.5)),
    beta_
    (
        max
        (
            0.25*sqr(gamma_ + 0.5),
            dict.lookupOrDefault<scalar>("beta", 0.25)
        )
    )
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::RBD::rigidBodySolvers::Newmark::~Newmark()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::RBD::rigidBodySolvers::Newmark::solve
(
    const scalarField& tau,
    const Field<spatialVector>& fx
)
{
    // Accumulate the restraint forces
    scalarField rtau(tau);
    Field<spatialVector> rfx(fx);
    model_.applyRestraints(rtau, rfx);

    // Calculate the accelerations for the given state and forces
    model_.forwardDynamics(state(), rtau, rfx);

    // Correct velocity
    qDot() = qDot0()
      + deltaT()*(gamma_*qDdot() + (1 - gamma_)*qDdot0());

    // Correct position
    q() = q0()
      + deltaT()*qDot0()
      + sqr(deltaT())*(beta_*qDdot() + (0.5 - beta_)*qDdot0());

    correctQuaternionJoints();
}


// ************************************************************************* //
