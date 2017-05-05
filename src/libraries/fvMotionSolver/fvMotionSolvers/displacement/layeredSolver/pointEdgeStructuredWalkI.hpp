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

// Update this with w2.
template<class TrackingData>
inline bool CML::pointEdgeStructuredWalk::update
(
    const pointEdgeStructuredWalk& w2,
    const scalar tol,
    TrackingData& td
)
{
    if (!valid(td))
    {
        // current not yet set. Walked from w2 to here (=point0)
        dist_ = w2.dist_ + mag(point0_-w2.previousPoint_);
        previousPoint_ = point0_;
        data_ = w2.data_;

        return true;
    }
    else
    {
        return false;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Null constructor
inline CML::pointEdgeStructuredWalk::pointEdgeStructuredWalk()
:
    point0_(vector::max),
    previousPoint_(vector::max),
    dist_(0),
    data_(vector::zero)
{}


// Construct from origin, distance
inline CML::pointEdgeStructuredWalk::pointEdgeStructuredWalk
(
    const point& point0,
    const point& previousPoint,
    const scalar dist,
    const vector& data
)
:
    point0_(point0),
    previousPoint_(previousPoint),
    dist_(dist),
    data_(data)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline bool CML::pointEdgeStructuredWalk::inZone() const
{
    return point0_ != vector::max;
}


//inline const CML::point& CML::pointEdgeStructuredWalk::previousPoint() const
//{
//    return previousPoint_;
//}


inline CML::scalar CML::pointEdgeStructuredWalk::dist() const
{
    return dist_;
}


inline const CML::vector& CML::pointEdgeStructuredWalk::data() const
{
    return data_;
}


template<class TrackingData>
inline bool CML::pointEdgeStructuredWalk::valid(TrackingData& td) const
{
    return previousPoint_ != vector::max;
}


// Checks for cyclic points
template<class TrackingData>
inline bool CML::pointEdgeStructuredWalk::sameGeometry
(
    const pointEdgeStructuredWalk& w2,
    const scalar tol,
    TrackingData& td
) const
{
    scalar diff = CML::mag(dist() - w2.dist());

    if (diff < SMALL)
    {
        return true;
    }
    else
    {
        if ((dist() > SMALL) && ((diff/dist()) < tol))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}


template<class TrackingData>
inline void CML::pointEdgeStructuredWalk::leaveDomain
(
    const polyPatch& patch,
    const label patchPointI,
    const point& coord,
    TrackingData& td
)
{
    previousPoint_ -= coord;
}


template<class TrackingData>
inline void CML::pointEdgeStructuredWalk::transform
(
    const tensor& rotTensor,
    TrackingData& td
)
{
    previousPoint_ = CML::transform(rotTensor, previousPoint_);
}


// Update absolute geometric quantities. Note that distance (dist_)
// is not affected by leaving/entering domain.
template<class TrackingData>
inline void CML::pointEdgeStructuredWalk::enterDomain
(
    const polyPatch& patch,
    const label patchPointI,
    const point& coord,
    TrackingData& td
)
{
    // back to absolute form
    previousPoint_ += coord;
}


// Update this with information from connected edge
template<class TrackingData>
inline bool CML::pointEdgeStructuredWalk::updatePoint
(
    const polyMesh& mesh,
    const label pointI,
    const label edgeI,
    const pointEdgeStructuredWalk& edgeInfo,
    const scalar tol,
    TrackingData& td
)
{
    if (inZone())
    {
        return update(edgeInfo, tol, td);
    }
    else
    {
        return false;
    }
}


// Update this with new information on same point
template<class TrackingData>
inline bool CML::pointEdgeStructuredWalk::updatePoint
(
    const polyMesh& mesh,
    const label pointI,
    const pointEdgeStructuredWalk& newPointInfo,
    const scalar tol,
    TrackingData& td
)
{
    if (inZone())
    {
        return update(newPointInfo, tol, td);
    }
    else
    {
        return false;
    }
}


// Update this with new information on same point. No extra information.
template<class TrackingData>
inline bool CML::pointEdgeStructuredWalk::updatePoint
(
    const pointEdgeStructuredWalk& newPointInfo,
    const scalar tol,
    TrackingData& td
)
{
    return update(newPointInfo, tol, td);
}


// Update this with information from connected point
template<class TrackingData>
inline bool CML::pointEdgeStructuredWalk::updateEdge
(
    const polyMesh& mesh,
    const label edgeI,
    const label pointI,
    const pointEdgeStructuredWalk& pointInfo,
    const scalar tol,
    TrackingData& td
)
{
    if (inZone())
    {
        return update(pointInfo, tol, td);
    }
    else
    {
        return false;
    }
}


template <class TrackingData>
inline bool CML::pointEdgeStructuredWalk::equal
(
    const pointEdgeStructuredWalk& rhs,
    TrackingData& td
) const
{
    return operator==(rhs);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline bool CML::pointEdgeStructuredWalk::operator==
(
    const CML::pointEdgeStructuredWalk& rhs
) const
{
    return previousPoint_ == rhs.previousPoint_;
}


inline bool CML::pointEdgeStructuredWalk::operator!=
(
    const CML::pointEdgeStructuredWalk& rhs
) const
{
    return !(*this == rhs);
}


// ************************************************************************* //
