/*---------------------------------------------------------------------------* \
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

#include "sixDoFRigidBodyMotionRestraint.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(CML::sixDoFRigidBodyMotionRestraint, 0);

defineRunTimeSelectionTable(CML::sixDoFRigidBodyMotionRestraint, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::sixDoFRigidBodyMotionRestraint::sixDoFRigidBodyMotionRestraint
(
    const dictionary& sDoFRBMRDict
)
:
    sDoFRBMRCoeffs_
    (
        sDoFRBMRDict.subDict
        (
            word(sDoFRBMRDict.lookup("sixDoFRigidBodyMotionRestraint"))
          + "Coeffs"
        )
    )
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::sixDoFRigidBodyMotionRestraint::~sixDoFRigidBodyMotionRestraint()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool CML::sixDoFRigidBodyMotionRestraint::read
(
    const dictionary& sDoFRBMRDict
)
{
    sDoFRBMRCoeffs_ = sDoFRBMRDict.subDict(type() + "Coeffs");

    return true;
}


// ************************************************************************* //
