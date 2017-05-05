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

#include "rigidBodyModelState.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::RBD::rigidBodyModelState::rigidBodyModelState
(
    const rigidBodyModel& model
)
:
    q_(model.nDoF(), Zero),
    qDot_(model.nDoF(), Zero),
    qDdot_(model.nDoF(), Zero),
    deltaT_(0)
{}


CML::RBD::rigidBodyModelState::rigidBodyModelState
(
    const rigidBodyModel& model,
    const dictionary& dict
)
:
    q_(dict.lookupOrDefault("q", scalarField(model.nDoF(), Zero))),
    qDot_(dict.lookupOrDefault("qDot", scalarField(model.nDoF(), Zero))),
    qDdot_(dict.lookupOrDefault("qDdot", scalarField(model.nDoF(), Zero))),
    deltaT_(dict.lookupOrDefault<scalar>("deltaT", 0))
{}


// ************************************************************************* //
