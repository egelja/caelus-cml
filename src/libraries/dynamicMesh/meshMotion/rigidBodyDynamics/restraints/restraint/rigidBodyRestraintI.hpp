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

#include "rigidBodyModel.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::point CML::RBD::restraint::bodyPoint
(
    const point& p
) const
{
    return (model_.X0(bodyID_).inv() && spatialVector(Zero, p)).l();
}


inline CML::spatialVector CML::RBD::restraint::bodyForce
(
    const vector& m,
    const vector& f
) const
{
    return (model_.X0(bodyID_).inv() & spatialVector(m, f));
}


inline CML::spatialVector CML::RBD::restraint::bodyPointVelocity
(
    const point& p
) const
{
    return model_.v(bodyID_, p);
}


// ************************************************************************* //
