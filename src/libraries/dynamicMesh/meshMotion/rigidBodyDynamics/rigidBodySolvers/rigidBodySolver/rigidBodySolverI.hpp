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

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

inline CML::RBD::rigidBodyModelState& CML::RBD::rigidBodySolver::state()
{
    return model_.motionState_;
}


inline CML::scalarField& CML::RBD::rigidBodySolver::q()
{
    return state().q();
}


inline CML::scalarField& CML::RBD::rigidBodySolver::qDot()
{
    return state().qDot();
}


inline CML::scalarField& CML::RBD::rigidBodySolver::qDdot()
{
    return state().qDdot();
}


inline CML::scalar CML::RBD::rigidBodySolver::deltaT() const
{
    return model_.motionState_.deltaT();
}


inline const CML::RBD::rigidBodyModelState&
CML::RBD::rigidBodySolver::state0() const
{
    return model_.motionState0_;
}

inline const CML::scalarField& CML::RBD::rigidBodySolver::q0() const
{
    return state0().q();
}


inline const CML::scalarField& CML::RBD::rigidBodySolver::qDot0() const
{
    return state0().qDot();
}


inline const CML::scalarField& CML::RBD::rigidBodySolver::qDdot0() const
{
    return state0().qDdot();
}


inline CML::scalar CML::RBD::rigidBodySolver::deltaT0() const
{
    return model_.motionState0_.deltaT();
}


// ************************************************************************* //
