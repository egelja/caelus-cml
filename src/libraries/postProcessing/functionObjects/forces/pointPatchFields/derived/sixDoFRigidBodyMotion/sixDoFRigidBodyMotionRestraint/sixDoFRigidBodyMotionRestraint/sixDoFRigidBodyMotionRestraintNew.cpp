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

#include "sixDoFRigidBodyMotionRestraint.hpp"

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::sixDoFRigidBodyMotionRestraint>
CML::sixDoFRigidBodyMotionRestraint::New(const dictionary& sDoFRBMRDict)
{
    const word restraintType
    (
        sDoFRBMRDict.lookup("sixDoFRigidBodyMotionRestraint")
    );

    // Info<< "Selecting sixDoFRigidBodyMotionRestraint function "
    //     << restraintType << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(restraintType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "sixDoFRigidBodyMotionRestraint::New"
            "("
                "const dictionary& sDoFRBMRDict"
            ")"
        )   << "Unknown sixDoFRigidBodyMotionRestraint type "
            << restraintType << nl << nl
            << "Valid sixDoFRigidBodyMotionRestraint types are : " << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<sixDoFRigidBodyMotionRestraint>(cstrIter()(sDoFRBMRDict));
}


// ************************************************************************* //
