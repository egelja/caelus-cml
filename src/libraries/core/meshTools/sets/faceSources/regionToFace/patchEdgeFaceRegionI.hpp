/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2015 OpenFOAM Foundation
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

#include "polyMesh.hpp"
#include "transform.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool CML::patchEdgeFaceRegion::update
(
    const patchEdgeFaceRegion& w2,
    const scalar tol,
    TrackingData& td
)
{
    if (!w2.valid(td))
    {
        FatalErrorInFunction
            << "problem." << abort(FatalError);
    }

    if (w2.region_ == -2 || region_ == -2)
    {
        // Blocked edge/face
        return false;
    }

    if (!valid(td))
    {
        // current not yet set so use any value
        operator=(w2);
        return true;
    }
    else
    {
        if (w2.region_ < region_)
        {
            operator=(w2);
            return true;
        }
        else
        {
            return false;
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Null constructor
inline CML::patchEdgeFaceRegion::patchEdgeFaceRegion()
:
    region_(-1)
{}


// Construct from origin, distance
inline CML::patchEdgeFaceRegion::patchEdgeFaceRegion
(
    const label region
)
:
    region_(region)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::label CML::patchEdgeFaceRegion::region() const
{
    return region_;
}


template<class TrackingData>
inline bool CML::patchEdgeFaceRegion::valid(TrackingData& td) const
{
    return region_ != -1;
}


template<class TrackingData>
inline void CML::patchEdgeFaceRegion::transform
(
    const polyMesh& mesh,
    const indirectPrimitivePatch& patch,
    const tensor& rotTensor,
    const scalar tol,
    TrackingData& td
)
{}


template<class TrackingData>
inline bool CML::patchEdgeFaceRegion::updateEdge
(
    const polyMesh& mesh,
    const indirectPrimitivePatch& patch,
    const label edgeI,
    const label faceI,
    const patchEdgeFaceRegion& faceInfo,
    const scalar tol,
    TrackingData& td
)
{
    return update(faceInfo, tol, td);
}


template<class TrackingData>
inline bool CML::patchEdgeFaceRegion::updateEdge
(
    const polyMesh& mesh,
    const indirectPrimitivePatch& patch,
    const patchEdgeFaceRegion& edgeInfo,
    const bool sameOrientation,
    const scalar tol,
    TrackingData& td
)
{
    return update(edgeInfo, tol, td);
}


template<class TrackingData>
inline bool CML::patchEdgeFaceRegion::updateFace
(
    const polyMesh& mesh,
    const indirectPrimitivePatch& patch,
    const label faceI,
    const label edgeI,
    const patchEdgeFaceRegion& edgeInfo,
    const scalar tol,
    TrackingData& td
)
{
    return update(edgeInfo, tol, td);
}


template <class TrackingData>
inline bool CML::patchEdgeFaceRegion::equal
(
    const patchEdgeFaceRegion& rhs,
    TrackingData& td
) const
{
    return operator==(rhs);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline bool CML::patchEdgeFaceRegion::operator==
(
    const CML::patchEdgeFaceRegion& rhs
) const
{
    return region() == rhs.region();
}


inline bool CML::patchEdgeFaceRegion::operator!=
(
    const CML::patchEdgeFaceRegion& rhs
) const
{
    return !(*this == rhs);
}


// ************************************************************************* //
