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

#include "sixDoFRigidBodyMotionConstraint.hpp"

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::sixDoFRigidBodyMotionConstraint>
CML::sixDoFRigidBodyMotionConstraint::New(const dictionary& sDoFRBMCDict)
{
    const word constraintType
    (
        sDoFRBMCDict.lookup("sixDoFRigidBodyMotionConstraint")
    );

    // Info<< "Selecting sixDoFRigidBodyMotionConstraint function "
    //     << constraintType << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(constraintType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "sixDoFRigidBodyMotionConstraint::New"
            "("
                "const dictionary& sDoFRBMCDict"
            ")"
        )   << "Unknown sixDoFRigidBodyMotionConstraint type "
            << constraintType << nl << nl
            << "Valid sixDoFRigidBodyMotionConstraints are : " << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<sixDoFRigidBodyMotionConstraint>(cstrIter()(sDoFRBMCDict));
}


// ************************************************************************* //
