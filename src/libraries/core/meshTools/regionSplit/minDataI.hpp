/*---------------------------------------------------------------------------*\
Copyright (C) 2014 OpenFOAM Foundation
Copyright (C) 2015 OpenCFD Ltd.
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

#include "polyMesh.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::minData::minData()
:
    data_(labelMax)
{}


inline CML::minData::minData(const label data)
:
    data_(data)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class TrackingData>
inline bool CML::minData::valid(TrackingData& td) const
{
    return data_ != labelMax;
}


template<class TrackingData>
inline bool CML::minData::sameGeometry
(
    const polyMesh&,
    const minData&,
    const scalar,
    TrackingData&
) const
{
    return true;
}


template<class TrackingData>
inline void CML::minData::leaveDomain
(
    const polyMesh&,
    const polyPatch& patch,
    const label patchFaceI,
    const point& faceCentre,
    TrackingData&
)
{}


template<class TrackingData>
inline void CML::minData::transform
(
    const polyMesh&,
    const tensor& rotTensor,
    TrackingData&
)
{}


template<class TrackingData>
inline void CML::minData::enterDomain
(
    const polyMesh&,
    const polyPatch& patch,
    const label patchFaceI,
    const point& faceCentre,
    TrackingData&
)
{}


template<class TrackingData>
inline bool CML::minData::updateCell
(
    const polyMesh&,
    const label thisCellI,
    const label neighbourFaceI,
    const minData& neighbourInfo,
    const scalar tol,
    TrackingData&
)
{
    if (neighbourInfo.data_ < data_)
    {
        operator=(neighbourInfo);
        return true;
    }
    else
    {
        return false;
    }
}


template<class TrackingData>
inline bool CML::minData::updateFace
(
    const polyMesh& mesh,
    const label thisFaceI,
    const label neighbourCellI,
    const minData& neighbourInfo,
    const scalar tol,
    TrackingData&
)
{
    // From cell to its faces.

    if (neighbourInfo.data_ < data_)
    {
        operator=(neighbourInfo);
        return true;
    }
    else
    {
        return false;
    }
}


template<class TrackingData>
inline bool CML::minData::updateFace
(
    const polyMesh&,
    const label thisFaceI,
    const minData& neighbourInfo,
    const scalar tol,
    TrackingData&
)
{
    // From face to face (e.g. coupled faces)
    if (neighbourInfo.data_ < data_)
    {
        operator=(neighbourInfo);
        return true;
    }
    else
    {
        return false;
    }
}


template<class TrackingData>
inline bool CML::minData::equal
(
    const minData& rhs,
    TrackingData& td
) const
{
    return operator==(rhs);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline bool CML::minData::operator==
(
    const CML::minData& rhs
) const
{
    return data() == rhs.data();
}


inline bool CML::minData::operator!=
(
    const CML::minData& rhs
) const
{
    return !(*this == rhs);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline CML::Ostream& CML::operator<<
(
    CML::Ostream& os,
    const CML::minData& wDist
)
{
    return os << wDist.data_;
}


inline CML::Istream& CML::operator>>
(
    CML::Istream& is,
    CML::minData& wDist
)
{
    return is >> wDist.data_;
}


// ************************************************************************* //
