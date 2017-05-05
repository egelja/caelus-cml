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

#include "HashTable.hpp"
#include "uLabel.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(CML::HashTableCore, 0);

const CML::label CML::HashTableCore::maxTableSize
(
    CML::HashTableCore::canonicalSize
    (
        CML::labelMax/2
    )
);

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

CML::label CML::HashTableCore::canonicalSize(const label size)
{
    if (size < 1)
    {
        return 0;
    }

    // enforce power of two
    uLabel goodSize = size;

    if (goodSize & (goodSize - 1))
    {
        // brute-force is fast enough
        goodSize = 1;
        while (goodSize < unsigned(size))
        {
            goodSize <<= 1;
        }
    }

    return goodSize;
}


// ************************************************************************* //
