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

#include "polyMesh.hpp"
#include "transform.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool CML::patchEdgeFaceInfo::update
(
    const point& pt,
    const patchEdgeFaceInfo& w2,
    const scalar tol,
    TrackingData& td
)
{
    scalar dist2 = magSqr(pt - w2.origin());

    if (!valid(td))
    {
        // current not yet set so use any value
        distSqr_ = dist2;
        origin_ = w2.origin();

        return true;
    }

    scalar diff = distSqr_ - dist2;

    if (diff < 0)
    {
        // already nearer to pt
        return false;
    }

    if ((diff < SMALL) || ((distSqr_ > SMALL) && (diff/distSqr_ < tol)))
    {
        // don't propagate small changes
        return false;
    }
    else
    {
        // update with new values
        distSqr_ = dist2;
        origin_ = w2.origin();

        return true;
    }
}


// Update this with w2 (information on same edge)
template<class TrackingData>
inline bool CML::patchEdgeFaceInfo::update
(
    const patchEdgeFaceInfo& w2,
    const scalar tol,
    TrackingData& td
)
{
    if (!valid(td))
    {
        // current not yet set so use any value
        distSqr_ = w2.distSqr();
        origin_ = w2.origin();

        return true;
    }

    scalar diff = distSqr_ - w2.distSqr();

    if (diff < 0)
    {
        // already nearer to pt
        return false;
    }

    if ((diff < SMALL) || ((distSqr_ > SMALL) && (diff/distSqr_ < tol)))
    {
        // don't propagate small changes
        return false;
    }
    else
    {
        // update with new values
        distSqr_ =  w2.distSqr();
        origin_ = w2.origin();

        return true;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Null constructor
inline CML::patchEdgeFaceInfo::patchEdgeFaceInfo()
:
    origin_(point::max),
    distSqr_(sqr(GREAT))
{}


// Construct from origin, distance
inline CML::patchEdgeFaceInfo::patchEdgeFaceInfo
(
    const point& origin,
    const scalar distSqr
)
:
    origin_(origin),
    distSqr_(distSqr)
{}


// Construct as copy
inline CML::patchEdgeFaceInfo::patchEdgeFaceInfo(const patchEdgeFaceInfo& wpt)
:
    origin_(wpt.origin()),
    distSqr_(wpt.distSqr())
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::point& CML::patchEdgeFaceInfo::origin() const
{
    return origin_;
}


inline CML::scalar CML::patchEdgeFaceInfo::distSqr() const
{
    return distSqr_;
}


template<class TrackingData>
inline bool CML::patchEdgeFaceInfo::valid(TrackingData& td) const
{
    return origin_ != point::max;
}


template<class TrackingData>
inline void CML::patchEdgeFaceInfo::transform
(
    const polyMesh& mesh,
    const primitivePatch& patch,
    const tensor& rotTensor,
    const scalar tol,
    TrackingData& td
)
{
    origin_ = CML::transform(rotTensor, origin_);
}


template<class TrackingData>
inline bool CML::patchEdgeFaceInfo::updateEdge
(
    const polyMesh& mesh,
    const primitivePatch& patch,
    const label edgeI,
    const label faceI,
    const patchEdgeFaceInfo& faceInfo,
    const scalar tol,
    TrackingData& td
)
{
    const edge& e = patch.edges()[edgeI];
    point eMid =
        0.5
      * (
            patch.points()[patch.meshPoints()[e[0]]]
          + patch.points()[patch.meshPoints()[e[1]]]
        );
    return update(eMid, faceInfo, tol, td);
}


template<class TrackingData>
inline bool CML::patchEdgeFaceInfo::updateEdge
(
    const polyMesh& mesh,
    const primitivePatch& patch,
    const patchEdgeFaceInfo& edgeInfo,
    const bool sameOrientation,
    const scalar tol,
    TrackingData& td
)
{
    return update(edgeInfo, tol, td);
}


template<class TrackingData>
inline bool CML::patchEdgeFaceInfo::updateFace
(
    const polyMesh& mesh,
    const primitivePatch& patch,
    const label faceI,
    const label edgeI,
    const patchEdgeFaceInfo& edgeInfo,
    const scalar tol,
    TrackingData& td
)
{
    return update(patch.faceCentres()[faceI], edgeInfo, tol, td);
}


template <class TrackingData>
inline bool CML::patchEdgeFaceInfo::equal
(
    const patchEdgeFaceInfo& rhs,
    TrackingData& td
) const
{
    return operator==(rhs);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline bool CML::patchEdgeFaceInfo::operator==
(
    const CML::patchEdgeFaceInfo& rhs
) const
{
    return origin() == rhs.origin();
}


inline bool CML::patchEdgeFaceInfo::operator!=
(
    const CML::patchEdgeFaceInfo& rhs
) const
{
    return !(*this == rhs);
}


// ************************************************************************* //
