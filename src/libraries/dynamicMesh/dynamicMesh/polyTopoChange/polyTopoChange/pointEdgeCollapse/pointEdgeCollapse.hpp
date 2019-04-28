/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2016 OpenFOAM Foundation
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

Class
    CML::pointEdgeCollapse

Description
    Determines length of string of edges walked to point.

SourceFiles
    pointEdgeCollapse.cpp

\*---------------------------------------------------------------------------*/

#ifndef pointEdgeCollapse_H
#define pointEdgeCollapse_H

#include "point.hpp"
#include "tensor.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class polyPatch;
class polyMesh;

// Forward declaration of friend functions and operators

class pointEdgeCollapse;

Istream& operator>>(Istream&, pointEdgeCollapse&);
Ostream& operator<<(Ostream&, const pointEdgeCollapse&);


/*---------------------------------------------------------------------------*\
                           Class pointEdgeCollapse Declaration
\*---------------------------------------------------------------------------*/

class pointEdgeCollapse
{
    // Private data

        //- Collapse location
        point collapsePoint_;

        //- Collapse string index
        label collapseIndex_;

        //- Priority of the collapse
        label collapsePriority_;


    // Private Member Functions

        //- Evaluate distance to point.
        template<class TrackingData>
        inline bool update
        (
            const pointEdgeCollapse& w2,
            const scalar tol,
            TrackingData& td
        );


        //- Check for same coordinate
        inline bool samePoint(const point& pt) const;

public:

    // Constructors

        //- Construct null
        inline pointEdgeCollapse();

        //- Construct from components
        inline pointEdgeCollapse
        (
            const point& collapsePoint,
            const label collapseIndex,
            const label collapsePriority
        );


    // Member Functions

        // Access

            inline const point& collapsePoint() const;

            inline label collapseIndex() const;

            inline label collapsePriority() const;


        // Needed by meshWave

            //- Check whether origin has been changed at all or
            //  still contains original (invalid) value.
            template<class TrackingData>
            inline bool valid(TrackingData& td) const;

            //- Convert origin to relative vector to leaving point
            //  (= point coordinate)
            template<class TrackingData>
            inline void leaveDomain
            (
                const polyPatch& patch,
                const label patchPointI,
                const point& pos,
                TrackingData& td
            );

            //- Convert relative origin to absolute by adding entering point
            template<class TrackingData>
            inline void enterDomain
            (
                const polyPatch& patch,
                const label patchPointI,
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
                const label pointI,
                const label edgeI,
                const pointEdgeCollapse& edgeInfo,
                const scalar tol,
                TrackingData& td
            );

            //- Influence of different value on same point.
            //  Merge new and old info.
            template<class TrackingData>
            inline bool updatePoint
            (
                const polyMesh& mesh,
                const label pointI,
                const pointEdgeCollapse& newPointInfo,
                const scalar tol,
                TrackingData& td
            );

            //- Influence of different value on same point.
            //  No information about current position whatsoever.
            template<class TrackingData>
            inline bool updatePoint
            (
                const pointEdgeCollapse& newPointInfo,
                const scalar tol,
                TrackingData& td
            );

            //- Influence of point on edge.
            template<class TrackingData>
            inline bool updateEdge
            (
                const polyMesh& mesh,
                const label edgeI,
                const label pointI,
                const pointEdgeCollapse& pointInfo,
                const scalar tol,
                TrackingData& td
            );

            //- Same (like operator==)
            template<class TrackingData>
            inline bool equal(const pointEdgeCollapse&, TrackingData&)
            const;


    // Member Operators

        //Note: Used to determine whether to call update.
        inline bool operator==(const pointEdgeCollapse&) const;
        inline bool operator!=(const pointEdgeCollapse&) const;


    // IOstream Operators

        friend Ostream& operator<<(Ostream&, const pointEdgeCollapse&);
        friend Istream& operator>>(Istream&, pointEdgeCollapse&);
};


//- Data associated with pointEdgeCollapse type are contiguous
template<>
inline bool contiguous<pointEdgeCollapse>()
{
    return true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "polyMesh.hpp"
#include "transform.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// Update this with w2.
template<class TrackingData>
inline bool CML::pointEdgeCollapse::update
(
    const pointEdgeCollapse& w2,
    const scalar tol,
    TrackingData& td
)
{
    if (!w2.valid(td))
    {
        FatalErrorInFunction
            << "problem." << abort(FatalError);
    }

    if (!valid(td))
    {
        operator=(w2);
        return true;
    }

    if (w2.collapseIndex_ == -1 || collapseIndex_ == -1)
    {
        // Not marked for collapse; only happens on edges.
        return false;
    }

    if (w2.collapsePriority_ < collapsePriority_)
    {
        return false;
    }
    else if (w2.collapsePriority_ > collapsePriority_)
    {
        operator=(w2);
        return true;
    }

    // Get overwritten by w2 if it has a higher priority
    if (w2.collapseIndex_ < collapseIndex_)
    {
        operator=(w2);
        return true;
    }
    else if (w2.collapseIndex_ == collapseIndex_)
    {
        bool identicalPoint = samePoint(w2.collapsePoint_);

        bool nearer = (magSqr(w2.collapsePoint_) < magSqr(collapsePoint_));

        if (nearer)
        {
            operator=(w2);
        }

        if (identicalPoint)
        {
            return false;
        }
        else
        {
            return nearer;
        }
    }
    else
    {
        return false;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Null constructor
inline CML::pointEdgeCollapse::pointEdgeCollapse()
:
    collapsePoint_(GREAT, GREAT, GREAT),
    collapseIndex_(-2),
    collapsePriority_(-2)
{}


// Construct from origin, distance
inline CML::pointEdgeCollapse::pointEdgeCollapse
(
    const point& collapsePoint,
    const label collapseIndex,
    const label collapsePriority
)
:
    collapsePoint_(collapsePoint),
    collapseIndex_(collapseIndex),
    collapsePriority_(collapsePriority)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::point& CML::pointEdgeCollapse::collapsePoint() const
{
    return collapsePoint_;
}


inline CML::label CML::pointEdgeCollapse::collapseIndex() const
{
    return collapseIndex_;
}


inline CML::label CML::pointEdgeCollapse::collapsePriority() const
{
    return collapsePriority_;
}


inline bool CML::pointEdgeCollapse::samePoint(const point& pt) const
{
    bool isLegal1 = (cmptMin(collapsePoint_) < 0.5*GREAT);
    bool isLegal2 = (cmptMin(pt) < 0.5*GREAT);

    if (isLegal1 && isLegal2)
    {
        return mag(collapsePoint_ - pt) < 1e-9;//SMALL;
    }
    else
    {
        return isLegal1 == isLegal2;
    }
}


template<class TrackingData>
inline bool CML::pointEdgeCollapse::valid(TrackingData& td) const
{
    return collapseIndex_ != -2;
}


template<class TrackingData>
inline void CML::pointEdgeCollapse::leaveDomain
(
    const polyPatch& patch,
    const label patchPointI,
    const point& coord,
    TrackingData& td
)
{
    collapsePoint_ -= coord;
}


template<class TrackingData>
inline void CML::pointEdgeCollapse::transform
(
    const tensor& rotTensor,
    TrackingData& td
)
{
    collapsePoint_ = CML::transform(rotTensor, collapsePoint_);
}


// Update absolute geometric quantities. Note that distance (dist_)
// is not affected by leaving/entering domain.
template<class TrackingData>
inline void CML::pointEdgeCollapse::enterDomain
(
    const polyPatch& patch,
    const label patchPointI,
    const point& coord,
    TrackingData& td
)
{
    // back to absolute form
    collapsePoint_ += coord;
}


// Update this with information from connected edge
template<class TrackingData>
inline bool CML::pointEdgeCollapse::updatePoint
(
    const polyMesh& mesh,
    const label pointI,
    const label edgeI,
    const pointEdgeCollapse& edgeInfo,
    const scalar tol,
    TrackingData& td
)
{
    return update(edgeInfo, tol, td);
}


// Update this with new information on same point
template<class TrackingData>
inline bool CML::pointEdgeCollapse::updatePoint
(
    const polyMesh& mesh,
    const label pointI,
    const pointEdgeCollapse& newPointInfo,
    const scalar tol,
    TrackingData& td
)
{
    return update(newPointInfo, tol, td);
}


// Update this with new information on same point. No extra information.
template<class TrackingData>
inline bool CML::pointEdgeCollapse::updatePoint
(
    const pointEdgeCollapse& newPointInfo,
    const scalar tol,
    TrackingData& td
)
{
    return update(newPointInfo, tol, td);
}


// Update this with information from connected point
template<class TrackingData>
inline bool CML::pointEdgeCollapse::updateEdge
(
    const polyMesh& mesh,
    const label edgeI,
    const label pointI,
    const pointEdgeCollapse& pointInfo,
    const scalar tol,
    TrackingData& td
)
{
    return update(pointInfo, tol, td);
}


template<class TrackingData>
inline bool CML::pointEdgeCollapse::equal
(
    const pointEdgeCollapse& rhs,
    TrackingData& td
) const
{
    return operator==(rhs);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline bool CML::pointEdgeCollapse::operator==
(
    const CML::pointEdgeCollapse& rhs
) const
{
    return
        collapseIndex_ == rhs.collapseIndex_
     && collapsePriority_ == rhs.collapsePriority_
     && samePoint(rhs.collapsePoint_);
}


inline bool CML::pointEdgeCollapse::operator!=
(
    const CML::pointEdgeCollapse& rhs
) const
{
    return !(*this == rhs);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
