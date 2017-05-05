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

Description
    Checks stream state,ing error.

\*---------------------------------------------------------------------------*/

#include "error.hpp"
#include "token.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

bool CML::checkStream(istream& is, const string& fnName)
{
    if (!is.good())
    {
        state(is, fnName);
        Info<< "Bad object in istream?", fnName << endl;
    }

    return good();
}


bool CML::checkStream(ostream& os, const string& fnName)
{
    if (!os.good())
    {
        state(os, fnName);
        Info<< "Bad object in ostream?", fnName << endl;
    }

    return good();
}


// ************************************************************************* //
