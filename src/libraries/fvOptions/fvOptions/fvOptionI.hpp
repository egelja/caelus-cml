/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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

#include "fvMesh.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::word& CML::fv::option::name() const
{
    return name_;
}


inline const CML::fvMesh& CML::fv::option::mesh() const
{
    return mesh_;
}


inline const CML::dictionary& CML::fv::option::coeffs() const
{
    return coeffs_;
}


inline bool CML::fv::option::active() const
{
    return active_;
}


inline CML::scalar CML::fv::option::timeStart() const
{
    return timeStart_;
}


inline CML::scalar CML::fv::option::duration() const
{
    return duration_;
}


inline bool CML::fv::option::inTimeLimits(const scalar time) const
{
    return
    (
        (timeStart_ < 0)
     ||
        (
            (mesh_.time().value() >= timeStart_)
         && (mesh_.time().value() <= (timeStart_ + duration_))
        )
    );
}


inline const CML::fv::option::selectionModeType&
CML::fv::option::selectionMode() const
{
    return selectionMode_;
}


inline const CML::word& CML::fv::option::cellSetName() const
{
    return cellSetName_;
}


inline CML::scalar CML::fv::option::V() const
{
    return V_;
}


inline const CML::labelList& CML::fv::option::cells() const
{
    return cells_;
}


inline void CML::fv::option::setApplied(const label fieldI)
{
    applied_[fieldI] = true;
}


inline bool& CML::fv::option::active()
{
    return active_;
}


inline CML::scalar& CML::fv::option::timeStart()
{
    return timeStart_;
}


inline CML::scalar& CML::fv::option::duration()
{
    return duration_;
}


inline const CML::word& CML::fv::option::nbrRegionName() const
{
    return nbrRegionName_;
}


inline const CML::meshToMesh& CML::fv::option::meshInterp() const
{
    if (!meshInterpPtr_.valid())
    {
        FatalErrorInFunction
            << "Interpolation object not set"
            << abort(FatalError);
    }

    return meshInterpPtr_();
}



