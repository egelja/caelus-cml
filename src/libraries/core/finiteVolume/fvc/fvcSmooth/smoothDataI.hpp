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

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class TrackingData>
inline bool CML::smoothData::update
(
    const smoothData& svf,
    const scalar scale,
    const scalar tol,
    TrackingData& td
)
{
    if (!valid(td) || (value_ < VSMALL))
    {
        // My value not set - take over neighbour
        value_ = svf.value()/scale;

        // Something changed - let caller know
        return true;
    }
    else if (svf.value() > (1 + tol)*scale*value_)
    {
        // Neighbour is too big for me - Up my value
        value_ = svf.value()/scale;

        // Something changed - let caller know
        return true;
    }
    else
    {
        // Neighbour is not too big for me or change is too small
        // Nothing changed
        return false;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::smoothData::smoothData()
:
    value_(-GREAT)
{}


inline CML::smoothData::smoothData(const scalar value)
:
    value_(value)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class TrackingData>
inline bool CML::smoothData::valid(TrackingData& td) const
{
    return value_ > -SMALL;
}


template<class TrackingData>
inline bool CML::smoothData::sameGeometry
(
    const polyMesh&,
    const smoothData&,
    const scalar,
    TrackingData& td
) const
{
    return true;
}


template<class TrackingData>
inline void CML::smoothData::leaveDomain
(
    const polyMesh&,
    const polyPatch&,
    const label,
    const point&,
    TrackingData& td
)
{}


template<class TrackingData>
inline void CML::smoothData::transform
(
    const polyMesh&,
    const tensor&,
    TrackingData& td
)
{}


template<class TrackingData>
inline void CML::smoothData::enterDomain
(
    const polyMesh&,
    const polyPatch&,
    const label,
    const point&,
    TrackingData& td
)
{}


template<class TrackingData>
inline bool CML::smoothData::updateCell
(
    const polyMesh&,
    const label,
    const label,
    const smoothData& svf,
    const scalar tol,
    TrackingData& td
)
{
    // Take over info from face if more than deltaRatio larger
    return update(svf, td.maxRatio, tol, td);
}


template<class TrackingData>
inline bool CML::smoothData::updateFace
(
    const polyMesh&,
    const label,
    const label,
    const smoothData& svf,
    const scalar tol,
    TrackingData& td
)
{
    // Take over information from cell without any scaling (scale = 1.0)
    return update(svf, 1.0, tol, td);
}


// Update this (face) with coupled face information.
template<class TrackingData>
inline bool CML::smoothData::updateFace
(
    const polyMesh&,
    const label,
    const smoothData& svf,
    const scalar tol,
    TrackingData& td
)
{
    // Take over information from coupled face without any scaling (scale = 1.0)
    return update(svf, 1.0, tol, td);
}


template <class TrackingData>
inline bool CML::smoothData::equal
(
    const smoothData& rhs,
    TrackingData& td
) const
{
    return operator==(rhs);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void CML::smoothData::operator=
(
    const scalar value
)
{
    value_ = value;
}


inline bool CML::smoothData::operator==
(
    const smoothData& rhs
) const
{
    return value_ == rhs.value();
}


inline bool CML::smoothData::operator!=
(
    const smoothData& rhs
) const
{
    return !(*this == rhs);
}


// ************************************************************************* //
