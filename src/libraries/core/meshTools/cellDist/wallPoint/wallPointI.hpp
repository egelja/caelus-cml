/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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
inline bool CML::wallPoint::update
(
    const point& pt,
    const wallPoint& w2,
    const scalar tol,
    TrackingData& td
)
{
    //Already done in calling algorithm
    //if (w2.origin() == origin_)
    //{
    //    // Shortcut. Same input so same distance.
    //    return false;
    //}

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


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::wallPoint::wallPoint()
:
    origin_(point::max),
    distSqr_(-GREAT)
{}


inline CML::wallPoint::wallPoint(const point& origin, const scalar distSqr)
:
    origin_(origin),
    distSqr_(distSqr)
{}


inline CML::wallPoint::wallPoint(const wallPoint& wpt)
:
    origin_(wpt.origin()),
    distSqr_(wpt.distSqr())
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::point& CML::wallPoint::origin() const
{
    return origin_;
}


inline CML::point& CML::wallPoint::origin()
{
    return origin_;
}


inline CML::scalar CML::wallPoint::distSqr() const
{
    return distSqr_;
}


inline CML::scalar& CML::wallPoint::distSqr()
{
    return distSqr_;
}


template<class TrackingData>
inline bool CML::wallPoint::valid(TrackingData& td) const
{
    return distSqr_ > -SMALL;
}


// Checks for cyclic faces
template<class TrackingData>
inline bool CML::wallPoint::sameGeometry
(
    const polyMesh&,
    const wallPoint& w2,
    const scalar tol,
    TrackingData& td
)
 const
{
    scalar diff = mag(distSqr() - w2.distSqr());

    if (diff < SMALL)
    {
        return true;
    }
    else
    {
        if ((distSqr() > SMALL) && ((diff/distSqr()) < tol))
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
inline void CML::wallPoint::leaveDomain
(
    const polyMesh&,
    const polyPatch&,
    const label,
    const point& faceCentre,
    TrackingData& td
)
{
    origin_ -= faceCentre;
}


template<class TrackingData>
inline void CML::wallPoint::transform
(
    const polyMesh&,
    const tensor& rotTensor,
    TrackingData& td
)
{
    origin_ = CML::transform(rotTensor, origin_);
}


// Update absolute geometric quantities. Note that distance (distSqr_)
// is not affected by leaving/entering domain.
template<class TrackingData>
inline void CML::wallPoint::enterDomain
(
    const polyMesh&,
    const polyPatch&,
    const label,
    const point& faceCentre,
    TrackingData& td
)
{
    // back to absolute form
    origin_ += faceCentre;
}


// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool CML::wallPoint::updateCell
(
    const polyMesh& mesh,
    const label thisCellI,
    const label neighbourFaceI,
    const wallPoint& neighbourWallInfo,
    const scalar tol,
    TrackingData& td
)
{
    return
        update
        (
            mesh.cellCentres()[thisCellI],
            neighbourWallInfo,
            tol,
            td
        );
    }


// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool CML::wallPoint::updateFace
(
    const polyMesh& mesh,
    const label thisFaceI,
    const label neighbourCellI,
    const wallPoint& neighbourWallInfo,
    const scalar tol,
    TrackingData& td
)
{
    return
        update
        (
            mesh.faceCentres()[thisFaceI],
            neighbourWallInfo,
            tol,
            td
        );
}

// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool CML::wallPoint::updateFace
(
    const polyMesh& mesh,
    const label thisFaceI,
    const wallPoint& neighbourWallInfo,
    const scalar tol,
    TrackingData& td
)
{
    return
        update
        (
            mesh.faceCentres()[thisFaceI],
            neighbourWallInfo,
            tol,
            td
        );
}


template <class TrackingData>
inline bool CML::wallPoint::equal
(
    const wallPoint& rhs,
    TrackingData& td
) const
{
    return operator==(rhs);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline bool CML::wallPoint::operator==(const CML::wallPoint& rhs) const
{
    return origin() == rhs.origin();
}


inline bool CML::wallPoint::operator!=(const CML::wallPoint& rhs) const
{
    return !(*this == rhs);
}


// ************************************************************************* //
