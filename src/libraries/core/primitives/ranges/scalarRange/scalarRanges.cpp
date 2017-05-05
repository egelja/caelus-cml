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

#include "scalarRanges.hpp"
#include "DynamicList.hpp"
#include "ListOps.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::scalarRanges::scalarRanges()
:
    List<scalarRange>(0)
{}


CML::scalarRanges::scalarRanges(Istream& is)
:
    List<scalarRange>(0)
{
    DynamicList<scalarRange> lst;

    while (is.good())
    {
        scalarRange sr(is);
        if (sr.valid())
        {
            lst.append(sr);
        }
    }

    transfer(lst);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::scalarRanges::selected(const scalar value) const
{
    forAll(*this, i)
    {
        if (operator[](i).selected(value))
        {
            return true;
        }
    }

    return false;
}


CML::List<bool> CML::scalarRanges::selected
(
    const List<scalar>& values
) const
{
    List<bool> lst(values.size(), false);

    // check ranges
    forAll(values, i)
    {
        if (selected(values[i]))
        {
            lst[i] = true;
        }
    }

    // check specific values
    forAll(*this, rangeI)
    {
        if (operator[](rangeI).isExact())
        {
            scalar target = operator[](rangeI).value();

            int nearestIndex = -1;
            scalar nearestDiff = CML::GREAT;

            forAll(values, timeIndex)
            {
                scalar diff = fabs(values[timeIndex] - target);
                if (diff < nearestDiff)
                {
                    nearestDiff = diff;
                    nearestIndex = timeIndex;
                }
            }

            if (nearestIndex >= 0)
            {
                lst[nearestIndex] = true;
            }
        }
    }

    return lst;
}


CML::List<CML::scalar> CML::scalarRanges::select
(
    const List<scalar>& values
) const
{
    return subset(selected(values), values);
}


void CML::scalarRanges::inplaceSelect
(
    List<scalar>& values
) const
{
    inplaceSubset(selected(values), values);
}


// ************************************************************************* //
