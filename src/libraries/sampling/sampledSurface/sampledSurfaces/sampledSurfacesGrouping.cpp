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

#include "sampledSurfaces.hpp"
#include "volFields.hpp"
#include "IOobjectList.hpp"
#include "stringListOps.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

CML::label CML::sampledSurfaces::classifyFields()
{
    label nFields = 0;

    if (loadFromFiles_)
    {
        // Check files for a particular time
        IOobjectList objects(mesh_, mesh_.time().timeName());
        wordList allFields = objects.sortedNames();

        forAll(fieldSelection_, i)
        {
            labelList indices = findStrings(fieldSelection_[i], allFields);

            if (indices.size())
            {
                nFields += indices.size();
            }
            else
            {
                WarningInFunction
                    << "Cannot find field file matching "
                    << fieldSelection_[i] << endl;
            }
        }
    }
    else
    {
        // Check currently available fields
        wordList allFields = mesh_.sortedNames();
        labelList indices = findStrings(fieldSelection_, allFields);

        forAll(fieldSelection_, i)
        {
            labelList indices = findStrings(fieldSelection_[i], allFields);

            if (indices.size())
            {
                nFields += indices.size();
            }
            else
            {
                WarningInFunction
                    << "Cannot find registered field matching "
                    << fieldSelection_[i] << endl;
            }
        }
    }

    return nFields;
}


// ************************************************************************* //
