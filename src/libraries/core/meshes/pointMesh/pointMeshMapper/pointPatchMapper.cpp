/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "pointPatchMapper.hpp"
#include "pointPatch.hpp"
#include "mapPolyMesh.hpp"
#include "faceMapper.hpp"
#include "demandDrivenData.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::pointPatchMapper::calcAddressing() const
{
    if
    (
        directAddrPtr_
     || interpolationAddrPtr_
     || weightsPtr_
    )
    {
        FatalErrorInFunction
            << "Addressing already calculated"
            << abort(FatalError);
    }

    hasUnmapped_ = false;

    if (direct())
    {
        // Direct mapping.
        directAddrPtr_ = new labelList(mpm_.patchPointMap()[patch_.index()]);
        labelList& addr = *directAddrPtr_;

        forAll(addr, i)
        {
            if (addr[i] < 0)
            {
                hasUnmapped_ = true;
            }
        }
    }
    else
    {
        // Interpolative mapping.

        // NOTE: Incorrect:
        // FOR NOW only takes first patch point instead of averaging all
        // patch points. Problem is we don't know what points were in the patch
        // for points that were merged.

        interpolationAddrPtr_ = new labelListList(size());
        labelListList& addr = *interpolationAddrPtr_;

        weightsPtr_ = new scalarListList(addr.size());
        scalarListList& w = *weightsPtr_;

        const labelList& ppm = mpm_.patchPointMap()[patch_.index()];

        forAll(ppm, i)
        {
            if (ppm[i] >= 0)
            {
                addr[i] = labelList(1, ppm[i]);
                w[i] = scalarList(1, 1.0);
            }
            else
            {
                // Inserted point.
                ///// Map from point0 (arbitrary choice)
                //addr[i] = labelList(1, label(0));
                //w[i] = scalarList(1, 1.0);
                hasUnmapped_ = true;
            }
        }
    }
}


void CML::pointPatchMapper::clearOut()
{
    deleteDemandDrivenData(directAddrPtr_);
    deleteDemandDrivenData(interpolationAddrPtr_);
    deleteDemandDrivenData(weightsPtr_);
    hasUnmapped_ = false;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::pointPatchMapper::pointPatchMapper
(
    const pointPatch& patch,
    const pointMapper& pointMap,
    const mapPolyMesh& mpm
)
:
    pointPatchFieldMapper(),
    patch_(patch),
    pointMapper_(pointMap),
    mpm_(mpm),
    sizeBeforeMapping_
    (
        patch_.index() < mpm_.oldPatchNMeshPoints().size()
      ? mpm_.oldPatchNMeshPoints()[patch_.index()]
      : 0
    ),
    hasUnmapped_(false),
    directAddrPtr_(nullptr),
    interpolationAddrPtr_(nullptr),
    weightsPtr_(nullptr)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::pointPatchMapper::~pointPatchMapper()
{
    clearOut();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::labelUList& CML::pointPatchMapper::directAddressing() const
{
    if (!direct())
    {
        FatalErrorInFunction
            << "Requested direct addressing for an interpolative mapper."
            << abort(FatalError);
    }

    if (!directAddrPtr_)
    {
        calcAddressing();
    }

    return *directAddrPtr_;
}


const CML::labelListList& CML::pointPatchMapper::addressing() const
{
    if (direct())
    {
        FatalErrorInFunction
            << "Requested interpolative addressing for a direct mapper."
            << abort(FatalError);
    }

    if (!interpolationAddrPtr_)
    {
        calcAddressing();
    }

    return *interpolationAddrPtr_;
}


const CML::scalarListList& CML::pointPatchMapper::weights() const
{
    if (direct())
    {
        FatalErrorInFunction
            << "Requested interpolative weights for a direct mapper."
            << abort(FatalError);
    }

    if (!weightsPtr_)
    {
        calcAddressing();
    }

    return *weightsPtr_;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //


// ************************************************************************* //
