/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
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

#include "regionProperties.hpp"
#include "IOdictionary.hpp"
#include "argList.hpp"
#include "polyMesh.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::regionProperties::regionProperties(const Time& runTime)
:
    HashTable<wordList>
    (
        IOdictionary
        (
            IOobject
            (
                "regionProperties",
                runTime.time().constant(),
                runTime.db(),
                IOobject::MUST_READ_IF_MODIFIED,
                IOobject::NO_WRITE
            )
        ).lookup("regions")
    )
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::regionProperties::~regionProperties()
{}


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

const CML::word& CML::regionDir(const word& regionName)
{
    return
        regionName == polyMesh::defaultRegion
      ? word::null
      : regionName;
}


CML::wordList CML::selectRegionNames(const argList& args, const Time& runTime)
{
    const bool allRegions = args.optionFound("allRegions");

    wordList regionNames;

    if (allRegions)
    {
        const regionProperties rp(runTime);
        forAllConstIter(HashTable<wordList>, rp, iter)
        {
            const wordList& regions = iter();
            forAll(regions, i)
            {
                if (findIndex(regionNames, regions[i]) == -1)
                {
                    regionNames.append(regions[i]);
                }
            }
        }
    }
    else
    {
        word regionName;
        if (args.optionReadIfPresent("region", regionName))
        {
            regionNames = wordList(1, regionName);
        }
        else
        {
            regionNames = wordList(1, polyMesh::defaultRegion);
        }
    }

    return regionNames;
}


// ************************************************************************* //
