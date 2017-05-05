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

#include "sampledSurfaces.hpp"
#include "volFields.hpp"
#include "IOobjectList.hpp"
#include "stringListOps.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

CML::label CML::sampledSurfaces::classifyFields()
{
    // check files for a particular time
    if (loadFromFiles_)
    {
        IOobjectList objects(mesh_, mesh_.time().timeName());
        wordList allFields = objects.sortedNames();

        labelList indices = findStrings(fieldSelection_, allFields);

        return indices.size();

    }

    else
    {
        wordList allFields = mesh_.sortedNames();
        labelList indices = findStrings(fieldSelection_, allFields);

        return indices.size();
    }
}


// ************************************************************************* //
