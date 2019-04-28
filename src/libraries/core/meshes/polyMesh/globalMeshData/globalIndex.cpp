/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
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

#include "globalIndex.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::globalIndex::globalIndex(const label localSize, const int tag)
:
    offsets_(Pstream::nProcs()+1)
{
    labelList localSizes(Pstream::nProcs());
    localSizes[Pstream::myProcNo()] = localSize;
    Pstream::gatherList(localSizes, tag);
    Pstream::scatterList(localSizes, tag);

    label offset = 0;
    offsets_[0] = 0;
    for (label proci = 0; proci < Pstream::nProcs(); proci++)
    {
        label oldOffset = offset;
        offset += localSizes[proci];

        if (offset < oldOffset)
        {
            FatalErrorInFunction
                << "Overflow : sum of sizes " << localSizes
                << " exceeds capability of label (" << labelMax
                << "). Please recompile with larger datatype for label."
                << exit(FatalError);
        }
        offsets_[proci+1] = offset;
    }
}


CML::globalIndex::globalIndex(const labelList& offsets)
:
    offsets_(offsets)
{}


CML::globalIndex::globalIndex(Istream& is)
{
    is >> offsets_;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, globalIndex& gi)
{
    return is >> gi.offsets_;
}


CML::Ostream& CML::operator<<(Ostream& os, const globalIndex& gi)
{
    return os << gi.offsets_;
}


// ************************************************************************* //
