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

#include "rigidBodySolver.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace RBD
{

    defineTypeNameAndDebug(rigidBodySolver, 0);
    defineRunTimeSelectionTable(rigidBodySolver, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::RBD::rigidBodySolver::rigidBodySolver(rigidBodyMotion& body)
:
    model_(body)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::RBD::rigidBodySolver::~rigidBodySolver()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::RBD::rigidBodySolver::correctQuaternionJoints()
{
    if (model_.unitQuaternions())
    {
        forAll (model_.joints(), i)
        {
            const label qi = model_.joints()[i].qIndex();

            if (model_.joints()[i].unitQuaternion())
            {
                // Calculate the change in the unit quaternion
                vector dv((q().block<vector>(qi) - q0().block<vector>(qi)));
                scalar magDv = mag(dv);

                if (magDv > SMALL)
                {
                    // Calculate the unit quaternion corresponding to the change
                    quaternion dQuat(dv/magDv, cos(magDv), true);

                    // Transform the previous time unit quaternion
                    quaternion quat
                    (
                        normalize(model_.joints()[i].unitQuaternion(q0())*dQuat)
                    );

                    // Update the joint unit quaternion
                    model_.joints()[i].unitQuaternion(quat, q());
                }
            }
        }
    }
}


// ************************************************************************* //
