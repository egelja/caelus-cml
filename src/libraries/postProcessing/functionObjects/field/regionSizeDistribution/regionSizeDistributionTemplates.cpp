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

#include "regionSizeDistribution.hpp"
#include "regionSplit.hpp"
#include "volFields.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::Map<Type> CML::regionSizeDistribution::regionSum
(
    const regionSplit& regions,
    const Field<Type>& fld
) const
{
    // Per region the sum of fld
    Map<Type> regionToSum(regions.nRegions()/Pstream::nProcs());

    forAll(fld, cellI)
    {
        label regionI = regions[cellI];

        typename Map<Type>::iterator fnd = regionToSum.find(regionI);
        if (fnd == regionToSum.end())
        {
            regionToSum.insert(regionI, fld[cellI]);
        }
        else
        {
            fnd() += fld[cellI];
        }
    }
    Pstream::mapCombineGather(regionToSum, plusEqOp<Type>());
    Pstream::mapCombineScatter(regionToSum);

    return regionToSum;
}


// Get data in sortedToc order
template<class Type>
CML::List<Type> CML::regionSizeDistribution::extractData
(
    const UList<label>& keys,
    const Map<Type>& regionData
) const
{
    List<Type> sortedData(keys.size());

    forAll(keys, i)
    {
        sortedData[i] = regionData[keys[i]];
    }
    return sortedData;
}


// ************************************************************************* //
