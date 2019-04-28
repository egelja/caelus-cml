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

#include "primitiveMesh.hpp"
#include "cellModeller.hpp"
#include "cellModel.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::Map<CML::label>& CML::cellMatcher::localPoint() const
{
    return localPoint_;
}


inline const CML::faceList& CML::cellMatcher::localFaces() const
{
    return localFaces_;
}


inline const CML::labelList& CML::cellMatcher::faceSize() const
{
    return faceSize_;
}


inline const CML::labelList& CML::cellMatcher::pointMap() const
{
    return pointMap_;
}


inline const CML::labelList& CML::cellMatcher::faceMap() const
{
    return faceMap_;
}


inline const CML::labelList& CML::cellMatcher::edgeFaces() const
{
    return edgeFaces_;
}


inline const CML::labelListList& CML::cellMatcher::pointFaceIndex() const
{
    return pointFaceIndex_;
}


inline const CML::labelList& CML::cellMatcher::vertLabels() const
{
    return vertLabels_;
}


inline const CML::labelList& CML::cellMatcher::faceLabels() const
{
    return faceLabels_;
}


inline const CML::cellModel& CML::cellMatcher::model() const
{
    if (cellModelPtr_ == nullptr)
    {
        cellModelPtr_ = cellModeller::lookup(cellModelName_);
    }
    return *cellModelPtr_;
}



// Key into edgeFaces_. key and key+1 are the entries for edge going from
// v0 to v1
inline CML::label CML::cellMatcher::edgeKey
(
    const label numVert,
    const label v0,
    const label v1
)
{
    return 2*(v0*numVert + v1);
}


// Walk along face consistent with face orientation
inline CML::label CML::cellMatcher::nextVert
(
    const label localVertI,
    const label size,
    const bool rightHand
)
{
    if (rightHand)
    {
        // face oriented acc. to righthand rule
        return (localVertI + 1) % size;
    }
    else
    {
        // face oriented acc. to lefthand rule
        return (size + localVertI - 1) % size;
    }
}


// ************************************************************************* //
