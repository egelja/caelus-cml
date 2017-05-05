/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2014 OpenFOAM Foundation
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

#include "meshToMesh.hpp"

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

inline const CML::polyMesh& CML::meshToMesh::srcRegion() const
{
    return srcRegion_;
}


inline const CML::polyMesh& CML::meshToMesh::tgtRegion() const
{
    return tgtRegion_;
}


inline const CML::labelListList&
CML::meshToMesh::srcToTgtCellAddr() const
{
    return srcToTgtCellAddr_;
}


inline const CML::labelListList&
CML::meshToMesh::tgtToSrcCellAddr() const
{
    return tgtToSrcCellAddr_;
}


inline const CML::scalarListList&
CML::meshToMesh::srcToTgtCellWght() const
{
    return srcToTgtCellWght_;
}


inline const CML::scalarListList&
CML::meshToMesh::tgtToSrcCellWght() const
{
    return tgtToSrcCellWght_;
}


inline CML::scalar CML::meshToMesh::V() const
{
    return V_;
}


inline const CML::PtrList<CML::AMIPatchToPatchInterpolation>&
CML::meshToMesh::patchAMIs() const
{
    return patchAMIs_;
}


// ************************************************************************* //
