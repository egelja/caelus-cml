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

#include "calcType.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::calcType> CML::calcType::New
(
    const word& calcTypeName
)
{
    Info<< "Selecting calcType " << calcTypeName << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(calcTypeName);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        // special treatment for -help
        // exit without stack trace
        if (calcTypeName == "-help")
        {
            FatalErrorInFunction
                << "Valid calcType selections are:" << nl
                << dictionaryConstructorTablePtr_->sortedToc() << nl
                << exit(FatalError);
        }
        else
        {
            FatalErrorInFunction
                << "Unknown calcType type " << calcTypeName << nl
                << "Valid calcType selections are:" << nl
                << dictionaryConstructorTablePtr_->sortedToc() << nl
                << abort(FatalError);
        }
    }

    return autoPtr<calcType>(cstrIter()());
}


// ************************************************************************* //
