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

#include "linearSpring.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "sixDoFRigidBodyMotion.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace sixDoFRigidBodyMotionRestraints
{
    defineTypeNameAndDebug(linearSpring, 0);

    addToRunTimeSelectionTable
    (
        sixDoFRigidBodyMotionRestraint,
        linearSpring,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::sixDoFRigidBodyMotionRestraints::linearSpring::linearSpring
(
    const dictionary& sDoFRBMRDict
)
:
    sixDoFRigidBodyMotionRestraint(sDoFRBMRDict),
    anchor_(),
    refAttachmentPt_(),
    stiffness_(),
    damping_(),
    restLength_()
{
    read(sDoFRBMRDict);
}


// * * * * * * * * * * * * * * * * Destructors * * * * * * * * * * * * * * * //

CML::sixDoFRigidBodyMotionRestraints::linearSpring::~linearSpring()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void CML::sixDoFRigidBodyMotionRestraints::linearSpring::restrain
(
    const sixDoFRigidBodyMotion& motion,
    vector& restraintPosition,
    vector& restraintForce,
    vector& restraintMoment
) const
{
    restraintPosition = motion.currentPosition(refAttachmentPt_);

    vector r = restraintPosition - anchor_;

    scalar magR = mag(r);

    // r is now the r unit vector
    r /= (magR + VSMALL);

    vector v = motion.currentVelocity(restraintPosition);

    restraintForce = -stiffness_*(magR - restLength_)*r - damping_*(r & v)*r;

    restraintMoment = vector::zero;

    if (motion.report())
    {
        Info<< " attachmentPt - anchor " << r*magR
            << " spring length " << magR
            << " force " << restraintForce
            << " moment " << restraintMoment
            << endl;
    }
}


bool CML::sixDoFRigidBodyMotionRestraints::linearSpring::read
(
    const dictionary& sDoFRBMRDict
)
{
    sixDoFRigidBodyMotionRestraint::read(sDoFRBMRDict);

    sDoFRBMRCoeffs_.lookup("anchor") >> anchor_;

    sDoFRBMRCoeffs_.lookup("refAttachmentPt") >> refAttachmentPt_;

    sDoFRBMRCoeffs_.lookup("stiffness") >> stiffness_;

    sDoFRBMRCoeffs_.lookup("damping") >> damping_;

    sDoFRBMRCoeffs_.lookup("restLength") >> restLength_;

    return true;
}


void CML::sixDoFRigidBodyMotionRestraints::linearSpring::write
(
    Ostream& os
) const
{
    os.writeKeyword("anchor")
        << anchor_ << token::END_STATEMENT << nl;

    os.writeKeyword("refAttachmentPt")
        << refAttachmentPt_ << token::END_STATEMENT << nl;

    os.writeKeyword("stiffness")
        << stiffness_ << token::END_STATEMENT << nl;

    os.writeKeyword("damping")
        << damping_ << token::END_STATEMENT << nl;

    os.writeKeyword("restLength")
        << restLength_ << token::END_STATEMENT << nl;
}

// ************************************************************************* //
