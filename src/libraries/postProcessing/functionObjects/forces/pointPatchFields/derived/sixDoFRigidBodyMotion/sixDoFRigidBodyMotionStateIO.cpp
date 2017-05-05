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

#include "sixDoFRigidBodyMotionState.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void CML::sixDoFRigidBodyMotionState::write(Ostream& os) const
{
    os.writeKeyword("centreOfMass")
        << centreOfMass_ << token::END_STATEMENT << nl;
    os.writeKeyword("orientation")
        << Q_ << token::END_STATEMENT << nl;
    os.writeKeyword("velocity")
        << v_ << token::END_STATEMENT << nl;
    os.writeKeyword("acceleration")
        << a_ << token::END_STATEMENT << nl;
    os.writeKeyword("angularMomentum")
        << pi_ << token::END_STATEMENT << nl;
    os.writeKeyword("torque")
        << tau_ << token::END_STATEMENT << nl;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Istream& CML::operator>>
(
    Istream& is, sixDoFRigidBodyMotionState& sDoFRBMS
)
{
    is  >> sDoFRBMS.centreOfMass_
        >> sDoFRBMS.Q_
        >> sDoFRBMS.v_
        >> sDoFRBMS.a_
        >> sDoFRBMS.pi_
        >> sDoFRBMS.tau_;

    // Check state of Istream
    is.check
    (
        "CML::Istream& CML::operator>>"
        "(CML::Istream&, CML::sixDoFRigidBodyMotionState&)"
    );

    return is;
}


CML::Ostream& CML::operator<<
(
    Ostream& os,
    const sixDoFRigidBodyMotionState& sDoFRBMS
)
{
    os  << token::SPACE << sDoFRBMS.centreOfMass()
        << token::SPACE << sDoFRBMS.Q()
        << token::SPACE << sDoFRBMS.v()
        << token::SPACE << sDoFRBMS.a()
        << token::SPACE << sDoFRBMS.pi()
        << token::SPACE << sDoFRBMS.tau();

    // Check state of Ostream
    os.check
    (
        "CML::Ostream& CML::operator<<(CML::Ostream&, "
        "const CML::sixDoFRigidBodyMotionState&)"
    );

    return os;
}


// ************************************************************************* //
