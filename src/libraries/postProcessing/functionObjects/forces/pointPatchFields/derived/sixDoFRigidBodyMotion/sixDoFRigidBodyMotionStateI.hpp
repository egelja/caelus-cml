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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //
inline const CML::point& CML::sixDoFRigidBodyMotionState::centreOfMass() const
{
    return centreOfMass_;
}


inline const CML::tensor& CML::sixDoFRigidBodyMotionState::Q() const
{
    return Q_;
}


inline const CML::vector& CML::sixDoFRigidBodyMotionState::v() const
{
    return v_;
}


inline const CML::vector& CML::sixDoFRigidBodyMotionState::a() const
{
    return a_;
}


inline const CML::vector& CML::sixDoFRigidBodyMotionState::pi() const
{
    return pi_;
}


inline const CML::vector& CML::sixDoFRigidBodyMotionState::tau() const
{
    return tau_;
}


inline CML::point& CML::sixDoFRigidBodyMotionState::centreOfMass()
{
    return centreOfMass_;
}


inline CML::tensor& CML::sixDoFRigidBodyMotionState::Q()
{
    return Q_;
}


inline CML::vector& CML::sixDoFRigidBodyMotionState::v()
{
    return v_;
}


inline CML::vector& CML::sixDoFRigidBodyMotionState::a()
{
    return a_;
}


inline CML::vector& CML::sixDoFRigidBodyMotionState::pi()
{
    return pi_;
}


inline CML::vector& CML::sixDoFRigidBodyMotionState::tau()
{
    return tau_;
}


// ************************************************************************* //
