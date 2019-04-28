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

#include "transform.hpp"
#include "polyMesh.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// Returns the wanted level
inline CML::label CML::refinementDistanceData::wantedLevel(const point& pt)
 const
{
    const scalar distSqr = magSqr(pt-origin_);

    // Get the size at the origin level
    scalar levelSize = level0Size_/(1<<originLevel_);

    scalar r = 0;

    for (label level = originLevel_; level >= 0; --level)
    {
        // Current range
        r += levelSize;

        // Check if our distance is within influence sphere
        if (sqr(r) > distSqr)
        {
            return level;
        }

        // Lower level will have double the size
        levelSize *= 2;
    }
    return 0;
}


template<class TrackingData>
inline bool CML::refinementDistanceData::update
(
    const point& pos,
    const refinementDistanceData& neighbourInfo,
    const scalar tol,
    TrackingData& td
)
{
    if (!valid(td))
    {
        if (!neighbourInfo.valid(td))
        {
            FatalErrorInFunction
                << "problem" << abort(FatalError);
        }
        operator=(neighbourInfo);
        return true;
    }

    // Determine wanted level at current position.
    label cellLevel = wantedLevel(pos);

    // Determine wanted level coming through the neighbour
    label nbrLevel = neighbourInfo.wantedLevel(pos);

    if (nbrLevel > cellLevel)
    {
        operator=(neighbourInfo);
        return true;
    }
    else if (nbrLevel == cellLevel)
    {
        scalar myDistSqr = magSqr(pos-origin_);
        scalar nbrDistSqr = magSqr(pos - neighbourInfo.origin());
        scalar diff = myDistSqr - nbrDistSqr;

        if (diff < 0)
        {
            // already nearest
            return false;
        }

        if ((diff < SMALL) || ((myDistSqr > SMALL) && (diff/myDistSqr < tol)))
        {
            // don't propagate small changes
            return false;
        }
        else
        {
            // update with new values
            operator=(neighbourInfo);
            return true;
        }
    }
    else
    {
        return false;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Null constructor
inline CML::refinementDistanceData::refinementDistanceData()
:
    level0Size_(-1)
{}


// Construct from components
inline CML::refinementDistanceData::refinementDistanceData
(
    const scalar level0Size,
    const point& origin,
    const label originLevel
)
:
    level0Size_(level0Size),
    origin_(origin),
    originLevel_(originLevel)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class TrackingData>
inline bool CML::refinementDistanceData::valid(TrackingData& td) const
{
    return level0Size_ != -1;
}


// No geometric data so never any problem on cyclics
template<class TrackingData>
inline bool CML::refinementDistanceData::sameGeometry
(
    const polyMesh&,
    const refinementDistanceData&,
    const scalar,
    TrackingData& td
) const
{
    return true;
}


template<class TrackingData>
inline void CML::refinementDistanceData::leaveDomain
(
    const polyMesh&,
    const polyPatch& patch,
    const label patchFaceI,
    const point& faceCentre,
    TrackingData& td
)
{
    origin_ -= faceCentre;
}


template<class TrackingData>
inline void CML::refinementDistanceData::transform
(
    const polyMesh&,
    const tensor& rotTensor,
    TrackingData& td
)
{
    origin_ = CML::transform(rotTensor, origin_);
}


// Update absolute geometric quantities.
template<class TrackingData>
inline void CML::refinementDistanceData::enterDomain
(
    const polyMesh&,
    const polyPatch& patch,
    const label patchFaceI,
    const point& faceCentre,
    TrackingData& td
)
{
    // back to absolute form
    origin_ += faceCentre;
}


// Update cell with neighbouring face information
template<class TrackingData>
inline bool CML::refinementDistanceData::updateCell
(
    const polyMesh& mesh,
    const label thisCellI,
    const label neighbourFaceI,
    const refinementDistanceData& neighbourInfo,
    const scalar tol,
    TrackingData& td
)
{
    const point& pos = mesh.cellCentres()[thisCellI];

    return update(pos, neighbourInfo, tol, td);
}


// Update face with neighbouring cell information
template<class TrackingData>
inline bool CML::refinementDistanceData::updateFace
(
    const polyMesh& mesh,
    const label thisFaceI,
    const label neighbourCellI,
    const refinementDistanceData& neighbourInfo,
    const scalar tol,
    TrackingData& td
)
{
    const point& pos = mesh.faceCentres()[thisFaceI];

    return update(pos, neighbourInfo, tol, td);
}


// Update face with coupled face information
template<class TrackingData>
inline bool CML::refinementDistanceData::updateFace
(
    const polyMesh& mesh,
    const label thisFaceI,
    const refinementDistanceData& neighbourInfo,
    const scalar tol,
    TrackingData& td
)
{
    const point& pos = mesh.faceCentres()[thisFaceI];

    return update(pos, neighbourInfo, tol, td);
}


template <class TrackingData>
inline bool CML::refinementDistanceData::equal
(
    const refinementDistanceData& rhs,
    TrackingData& td
) const
{
    if (!valid(td))
    {
        if (!rhs.valid(td))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return operator==(rhs);
    }
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline bool CML::refinementDistanceData::operator==
(
    const CML::refinementDistanceData& rhs
)
 const
{
    return
        level0Size_ == rhs.level0Size_
     && origin_ == rhs.origin_
     && originLevel_ == rhs.originLevel_;
}


inline bool CML::refinementDistanceData::operator!=
(
    const CML::refinementDistanceData& rhs
)
 const
{
    return !(*this == rhs);
}


// ************************************************************************* //
