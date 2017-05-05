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

#include "basicSource.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::word& CML::basicSource::name() const
{
    return name_;
}


inline const CML::fvMesh& CML::basicSource::mesh() const
{
    return mesh_;
}


inline const CML::dictionary& CML::basicSource::coeffs() const
{
    return coeffs_;
}


inline bool CML::basicSource::active() const
{
    return active_;
}


inline CML::scalar CML::basicSource::timeStart() const
{
    return timeStart_;
}


inline CML::scalar CML::basicSource::duration() const
{
    return duration_;
}


inline CML::scalar CML::basicSource::timeEnd() const
{
    return timeStart_ + duration_;
}


inline const CML::basicSource::selectionModeType&
CML::basicSource::selectionMode() const
{
    return selectionMode_;
}


inline const CML::word& CML::basicSource::cellSetName() const
{
    return cellSetName_;
}


inline CML::scalar CML::basicSource::V() const
{
    return V_;
}


inline const CML::labelList& CML::basicSource::cells() const
{
    return cells_;
}


inline void CML::basicSource::setApplied(const label fieldI)
{
    applied_[fieldI] = true;
}


inline bool& CML::basicSource::active()
{
    return active_;
}


inline CML::scalar& CML::basicSource::timeStart()
{
    return timeStart_;
}


inline CML::scalar& CML::basicSource::duration()
{
    return duration_;
}


// ************************************************************************* //
