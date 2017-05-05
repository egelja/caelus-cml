/*---------------------------------------------------------------------------*\
 Copyright (C) 2013-2016 OpenFOAM Foundation
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

Class
    CML::externalPointEdgePoint

Description
    Holds information regarding nearest wall point. Used in PointEdgeWave.
    (so not standard FaceCellWave)
    To be used in wall distance calculation.

SourceFiles
    externalPointEdgePointI.H
    externalPointEdgePoint.cpp

\*---------------------------------------------------------------------------*/

#ifndef externalPointEdgePoint_HPP
#define externalPointEdgePoint_HPP

#include "pointField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class polyPatch;
class polyMesh;


// Forward declaration of friend functions and operators

class externalPointEdgePoint;

Istream& operator>>(Istream&, externalPointEdgePoint&);
Ostream& operator<<(Ostream&, const externalPointEdgePoint&);


/*---------------------------------------------------------------------------*\
                           Class externalPointEdgePoint Declaration
\*---------------------------------------------------------------------------*/

class externalPointEdgePoint
{
    // Private data

        //- Position of nearest wall center
        point origin_;

        //- Normal distance (squared) from point to origin
        scalar distSqr_;


    // Private Member Functions

        //- Evaluate distance to point. Update distSqr, origin from whomever
        //  is nearer pt. Return true if w2 is closer to point,
        //  false otherwise.
        template<class TrackingData>
        inline bool update
        (
            const point&,
            const externalPointEdgePoint& w2,
            const scalar tol,
            TrackingData& td
        );

        //- Combine current with w2. Update distSqr, origin if w2 has smaller
        //  quantities and returns true.
        template<class TrackingData>
        inline bool update
        (
            const externalPointEdgePoint& w2,
            const scalar tol,
            TrackingData& td
        );


public:

    //- Class used to pass data into container
    class trackingData
    {
    public:
        const pointField& points_;

        trackingData(const pointField& points)
        :
            points_(points)
        {}
    };



    // Constructors

        //- Construct null
        inline externalPointEdgePoint();

        //- Construct from origin, distance
        inline externalPointEdgePoint(const point&, const scalar);

        //- Construct as copy
        inline externalPointEdgePoint(const externalPointEdgePoint&);


    // Member Functions

        // Access

            inline const point& origin() const;

            inline scalar distSqr() const;


        // Needed by PointEdgeWave

            //- Check whether origin has been changed at all or
            //  still contains original (invalid) value.
            template<class TrackingData>
            inline bool valid(TrackingData& td) const;

            //- Check for identical geometrical data. Used for cyclics checking.
            template<class TrackingData>
            inline bool sameGeometry
            (
                const externalPointEdgePoint&,
                const scalar tol,
                TrackingData& td
            ) const;

            //- Convert origin to relative vector to leaving point
            //  (= point coordinate)
            template<class TrackingData>
            inline void leaveDomain
            (
                const polyPatch& patch,
                const label patchPointi,
                const point& pos,
                TrackingData& td
            );

            //- Convert relative origin to absolute by adding entering point
            template<class TrackingData>
            inline void enterDomain
            (
                const polyPatch& patch,
                const label patchPointi,
                const point& pos,
                TrackingData& td
            );

            //- Apply rotation matrix to origin
            template<class TrackingData>
            inline void transform
            (
                const tensor& rotTensor,
                TrackingData& td
            );

            //- Influence of edge on point
            template<class TrackingData>
            inline bool updatePoint
            (
                const polyMesh& mesh,
                const label pointi,
                const label edgeI,
                const externalPointEdgePoint& edgeInfo,
                const scalar tol,
                TrackingData& td
            );

            //- Influence of different value on same point.
            //  Merge new and old info.
            template<class TrackingData>
            inline bool updatePoint
            (
                const polyMesh& mesh,
                const label pointi,
                const externalPointEdgePoint& newPointInfo,
                const scalar tol,
                TrackingData& td
            );

            //- Influence of different value on same point.
            //  No information about current position whatsoever.
            template<class TrackingData>
            inline bool updatePoint
            (
                const externalPointEdgePoint& newPointInfo,
                const scalar tol,
                TrackingData& td
            );

            //- Influence of point on edge.
            template<class TrackingData>
            inline bool updateEdge
            (
                const polyMesh& mesh,
                const label edgeI,
                const label pointi,
                const externalPointEdgePoint& pointInfo,
                const scalar tol,
                TrackingData& td
            );

            //- Equivalent to operator== with TrackingData
            template<class TrackingData>
            inline bool equal
            (
                const externalPointEdgePoint&,
                TrackingData& td
            ) const;


    // Member Operators

        // Needed for List IO
        inline bool operator==(const externalPointEdgePoint&) const;
        inline bool operator!=(const externalPointEdgePoint&) const;


    // IOstream Operators

        friend Ostream& operator<<(Ostream&, const externalPointEdgePoint&);
        friend Istream& operator>>(Istream&, externalPointEdgePoint&);
};


//- Data associated with externalPointEdgePoint type are contiguous
template<>
inline bool contiguous<externalPointEdgePoint>()
{
    return true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "polyMesh.hpp"
#include "transform.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class TrackingData>
inline bool CML::externalPointEdgePoint::update
(
    const point& pt,
    const externalPointEdgePoint& w2,
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


template<class TrackingData>
inline bool CML::externalPointEdgePoint::update
(
    const externalPointEdgePoint& w2,
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

inline CML::externalPointEdgePoint::externalPointEdgePoint()
:
    origin_(point::max),
    distSqr_(GREAT)
{}


inline CML::externalPointEdgePoint::externalPointEdgePoint
(
    const point& origin,
    const scalar distSqr
)
:
    origin_(origin),
    distSqr_(distSqr)
{}


inline CML::externalPointEdgePoint::externalPointEdgePoint
(
    const externalPointEdgePoint& wpt
)
:
    origin_(wpt.origin()),
    distSqr_(wpt.distSqr())
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::point& CML::externalPointEdgePoint::origin() const
{
    return origin_;
}


inline CML::scalar CML::externalPointEdgePoint::distSqr() const
{
    return distSqr_;
}


template<class TrackingData>
inline bool CML::externalPointEdgePoint::valid(TrackingData& td) const
{
    return origin_ != point::max;
}


// Checks for cyclic points
template<class TrackingData>
inline bool CML::externalPointEdgePoint::sameGeometry
(
    const externalPointEdgePoint& w2,
    const scalar tol,
    TrackingData& td
) const
{
    scalar diff = CML::mag(distSqr() - w2.distSqr());

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
inline void CML::externalPointEdgePoint::leaveDomain
(
    const polyPatch& patch,
    const label patchPointi,
    const point& coord,
    TrackingData& td
)
{
    origin_ -= coord;
}


template<class TrackingData>
inline void CML::externalPointEdgePoint::transform
(
    const tensor& rotTensor,
    TrackingData& td
)
{
    origin_ = CML::transform(rotTensor, origin_);
}


template<class TrackingData>
inline void CML::externalPointEdgePoint::enterDomain
(
    const polyPatch& patch,
    const label patchPointi,
    const point& coord,
    TrackingData& td
)
{
    // back to absolute form
    origin_ += coord;
}


template<class TrackingData>
inline bool CML::externalPointEdgePoint::updatePoint
(
    const polyMesh& mesh,
    const label pointi,
    const label edgeI,
    const externalPointEdgePoint& edgeInfo,
    const scalar tol,
    TrackingData& td
)
{
    return update(td.points_[pointi], edgeInfo, tol, td);
}


template<class TrackingData>
inline bool CML::externalPointEdgePoint::updatePoint
(
    const polyMesh& mesh,
    const label pointi,
    const externalPointEdgePoint& newPointInfo,
    const scalar tol,
    TrackingData& td
)
{
    return update(td.points_[pointi], newPointInfo, tol, td);
}


template<class TrackingData>
inline bool CML::externalPointEdgePoint::updatePoint
(
    const externalPointEdgePoint& newPointInfo,
    const scalar tol,
    TrackingData& td
)
{
    return update(newPointInfo, tol, td);
}


template<class TrackingData>
inline bool CML::externalPointEdgePoint::updateEdge
(
    const polyMesh& mesh,
    const label edgeI,
    const label pointi,
    const externalPointEdgePoint& pointInfo,
    const scalar tol,
    TrackingData& td
)
{
    const edge& e = mesh.edges()[edgeI];
    return update(e.centre(td.points_), pointInfo, tol, td);
}


template<class TrackingData>
inline bool CML::externalPointEdgePoint::equal
(
    const externalPointEdgePoint& rhs,
    TrackingData& td
) const
{
    return operator==(rhs);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline bool CML::externalPointEdgePoint::operator==
(
    const CML::externalPointEdgePoint& rhs
)
const
{
    return (origin() == rhs.origin()) && (distSqr() == rhs.distSqr());
}


inline bool CML::externalPointEdgePoint::operator!=
(
    const CML::externalPointEdgePoint& rhs
)
const
{
    return !(*this == rhs);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
