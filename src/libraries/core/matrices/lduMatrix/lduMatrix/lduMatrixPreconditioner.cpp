/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "lduMatrix.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineRunTimeSelectionTable(lduMatrix::preconditioner, symMatrix);
    defineRunTimeSelectionTable(lduMatrix::preconditioner, asymMatrix);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::word CML::lduMatrix::preconditioner::getName
(
    const dictionary& solverControls
)
{
    word name;

    // handle primitive or dictionary entry
    const entry& e = solverControls.lookupEntry("preconditioner", false, false);
    if (e.isDict())
    {
        e.dict().lookup("preconditioner") >> name;
    }
    else
    {
        e.stream() >> name;
    }

    return name;
}


CML::autoPtr<CML::lduMatrix::preconditioner>
CML::lduMatrix::preconditioner::New
(
    const solver& sol,
    const dictionary& solverControls
)
{
    word name;

    // handle primitive or dictionary entry
    const entry& e = solverControls.lookupEntry("preconditioner", false, false);
    if (e.isDict())
    {
        e.dict().lookup("preconditioner") >> name;
    }
    else
    {
        e.stream() >> name;
    }

    const dictionary& controls = e.isDict() ? e.dict() : dictionary::null;

    if (sol.matrix().symmetric())
    {
        symMatrixConstructorTable::iterator constructorIter =
            symMatrixConstructorTablePtr_->find(name);

        if (constructorIter == symMatrixConstructorTablePtr_->end())
        {
            FatalIOErrorInFunction(controls)
                << "Unknown symmetric matrix preconditioner "
                << name << nl << nl
                << "Valid symmetric matrix preconditioners :" << endl
                << symMatrixConstructorTablePtr_->sortedToc()
                << exit(FatalIOError);
        }

        return autoPtr<lduMatrix::preconditioner>
        (
            constructorIter()
            (
                sol,
                controls
            )
        );
    }
    else if (sol.matrix().asymmetric())
    {
        asymMatrixConstructorTable::iterator constructorIter =
            asymMatrixConstructorTablePtr_->find(name);

        if (constructorIter == asymMatrixConstructorTablePtr_->end())
        {
            FatalIOErrorInFunction(controls)
                << "Unknown asymmetric matrix preconditioner "
                << name << nl << nl
                << "Valid asymmetric matrix preconditioners :" << endl
                << asymMatrixConstructorTablePtr_->sortedToc()
                << exit(FatalIOError);
        }

        return autoPtr<lduMatrix::preconditioner>
        (
            constructorIter()
            (
                sol,
                controls
            )
        );
    }
    else
    {
        FatalIOErrorInFunction(controls)
            << "cannot solve incomplete matrix, "
               "no diagonal or off-diagonal coefficient"
            << exit(FatalIOError);

        return autoPtr<lduMatrix::preconditioner>(nullptr);
    }
}


// ************************************************************************* //
