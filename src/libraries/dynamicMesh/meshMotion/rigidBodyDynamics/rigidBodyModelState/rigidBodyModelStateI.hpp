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

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::scalarField& CML::RBD::rigidBodyModelState::q() const
{
    return q_;
}


inline const CML::scalarField& CML::RBD::rigidBodyModelState::qDot() const
{
    return qDot_;
}


inline const CML::scalarField& CML::RBD::rigidBodyModelState::qDdot() const
{
    return qDdot_;
}


inline CML::scalar CML::RBD::rigidBodyModelState::deltaT() const
{
    return deltaT_;
}


inline CML::scalarField& CML::RBD::rigidBodyModelState::q()
{
    return q_;
}


inline CML::scalarField& CML::RBD::rigidBodyModelState::qDot()
{
    return qDot_;
}


inline CML::scalarField& CML::RBD::rigidBodyModelState::qDdot()
{
    return qDdot_;
}


inline CML::scalar& CML::RBD::rigidBodyModelState::deltaT()
{
    return deltaT_;
}


// ************************************************************************* //
