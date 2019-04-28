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

#include "ListOps.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::globalIndex::globalIndex()
{}


CML::globalIndex::globalIndex(const Xfer<labelList>& offsets)
:
    offsets_(offsets)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::labelList& CML::globalIndex::offsets()
{
    return offsets_;
}


inline CML::label CML::globalIndex::offset(const label proci) const
{
    return offsets_[proci];
}


inline CML::label CML::globalIndex::localSize(const label proci) const
{
    return offsets_[proci+1] - offsets_[proci];
}


inline CML::label CML::globalIndex::localSize() const
{
    return localSize(Pstream::myProcNo());
}


inline CML::label CML::globalIndex::size() const
{
    return offsets_[Pstream::nProcs()];
}


inline CML::label CML::globalIndex::toGlobal
(
    const label proci,
    const label i
) const
{
    return i + offsets_[proci];
}


inline CML::label CML::globalIndex::toGlobal(const label i) const
{
    return toGlobal(Pstream::myProcNo(), i);
}


//- Is on local processor
inline bool CML::globalIndex::isLocal(const label proci, const label i) const
{
    return i >= offsets_[proci] && i < offsets_[proci+1];
}


inline bool CML::globalIndex::isLocal(const label i) const
{
    return isLocal(Pstream::myProcNo(), i);
}


inline CML::label CML::globalIndex::toLocal(const label proci, const label i)
const
{
    label localI = i - offsets_[proci];

    if (localI < 0 || i >= offsets_[proci+1])
    {
        FatalErrorInFunction
            << "Global " << i << " does not belong on processor "
            << proci << endl << "Offsets:" << offsets_
            << abort(FatalError);
    }
    return localI;
}


inline CML::label CML::globalIndex::toLocal(const label i) const
{
    return toLocal(Pstream::myProcNo(), i);
}


inline CML::label CML::globalIndex::whichProcID(const label i) const
{
    if (i < 0 || i >= offsets_[Pstream::nProcs()])
    {
        FatalErrorInFunction
            << "Global " << i << " does not belong on any processor."
            << " Offsets:" << offsets_
            << abort(FatalError);
    }

    return findLower(offsets_, i+1);
}


// ************************************************************************* //
