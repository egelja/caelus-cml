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

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

inline bool CML::RBD::rigidBodyMotion::report() const
{
    return report_;
}


inline const CML::RBD::rigidBodyModelState&
CML::RBD::rigidBodyMotion::state() const
{
    return motionState_;
}


inline CML::RBD::rigidBodyModelState&
CML::RBD::rigidBodyMotion::state()
{
    return motionState_;
}


inline void CML::RBD::rigidBodyMotion::newTime()
{
    motionState0_ = motionState_;
}


// ************************************************************************* //
