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

// * * * * * * * * * * * * * * * * Selector  * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::RBD::rigidBodySolver> CML::RBD::rigidBodySolver::New
(
    rigidBodyMotion& body,
    const dictionary& dict
)
{
    word rigidBodySolverType(dict.lookup("type"));

    Info<< "Selecting rigidBodySolver " << rigidBodySolverType << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(rigidBodySolverType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown rigidBodySolverType type "
            << rigidBodySolverType << endl << endl
            << "Valid rigidBodySolver types are : " << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return cstrIter()(body, dict);
}


// ************************************************************************* //
