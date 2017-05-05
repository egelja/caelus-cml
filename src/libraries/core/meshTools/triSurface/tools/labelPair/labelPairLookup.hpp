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

Typedef
    CML::labelPairLookup

Description
    A HashTable for two labels to another label.
    Used for e.g. for face1, face2 to shared edge.

Note
    The hash table is based on a FixedList and not edge, since an edge
    hashes commutatively!

\*---------------------------------------------------------------------------*/

#ifndef labelPairLookup_H
#define labelPairLookup_H

#include "FixedList.hpp"
#include "HashTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    typedef HashTable
    <
       label,
       FixedList<label, 2>,
       FixedList<label, 2>::Hash<>
    > labelPairLookup;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
