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

#include "fvPatchMapper.hpp"
#include "fvPatch.hpp"
#include "fvBoundaryMesh.hpp"
#include "fvMesh.hpp"
#include "mapPolyMesh.hpp"
#include "faceMapper.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::fvPatchMapper::calcAddressing() const
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

    // Mapping
    const label oldPatchStart =
        faceMap_.oldPatchStarts()[patch_.index()];

    const label oldPatchEnd =
        oldPatchStart + faceMap_.oldPatchSizes()[patch_.index()];

    hasUnmapped_ = false;

    // Assemble the maps: slice to patch
    if (direct())
    {
        // Direct mapping - slice to size
        directAddrPtr_ = new labelList
        (
            patch_.patchSlice
            (
                static_cast<const labelList&>(faceMap_.directAddressing())
            )
        );
        labelList& addr = *directAddrPtr_;

        // Adjust mapping to manage hits into other patches and into
        // internal
        forAll(addr, faceI)
        {
            if
            (
                addr[faceI] >= oldPatchStart
             && addr[faceI] < oldPatchEnd
            )
            {
                addr[faceI] -= oldPatchStart;
            }
            else
            {
                //addr[faceI] = 0;
                addr[faceI] = -1;
                hasUnmapped_ = true;
            }
        }

        if (fvMesh::debug)
        {
            if (min(addr) < 0)
            {
                WarningInFunction
                    << "Unmapped entry in patch mapping for patch "
                    << patch_.index() << " named " << patch_.name()
                    //<< abort(FatalError);
                    << endl;
            }
        }
    }
    else
    {
        // Interpolative mapping
        interpolationAddrPtr_ =
            new labelListList
            (
                patch_.patchSlice(faceMap_.addressing())
            );
        labelListList& addr = *interpolationAddrPtr_;

        weightsPtr_ =
            new scalarListList
            (
                patch_.patchSlice(faceMap_.weights())
            );
        scalarListList& w = *weightsPtr_;

        // Adjust mapping to manage hits into other patches and into
        // internal
        forAll(addr, faceI)
        {
            labelList& curAddr = addr[faceI];
            scalarList& curW = w[faceI];

            if
            (
                min(curAddr) >= oldPatchStart
             && max(curAddr) < oldPatchEnd
            )
            {
                // No adjustment of weights, just subtract patch start
                forAll(curAddr, i)
                {
                    curAddr[i] -= oldPatchStart;
                }
            }
            else
            {
                // Need to recalculate weights to exclude hits into internal
                labelList newAddr(curAddr.size(), false);
                scalarField newWeights(curAddr.size());
                label nActive = 0;

                forAll(curAddr, lfI)
                {
                    if
                    (
                        curAddr[lfI] >= oldPatchStart
                     && curAddr[lfI] < oldPatchEnd
                    )
                    {
                        newAddr[nActive] = curAddr[lfI] - oldPatchStart;
                        newWeights[nActive] = curW[lfI];
                        nActive++;
                    }
                }

                // Cater for bad mapping
                if (nActive == 0)
                {
                    newAddr[nActive] = 0;
                    newWeights[nActive] = 1;
                    nActive++;
                }

                newAddr.setSize(nActive);
                newWeights.setSize(nActive);

                // Re-scale the weights
                if (nActive > 0)
                {
                    newWeights /= sum(newWeights);
                }
                else
                {
                    hasUnmapped_ = true;
                }

                // Reset addressing and weights
                curAddr = newAddr;
                curW = newWeights;
            }
        }

        if (fvMesh::debug)
        {
            forAll(addr, i)
            {
                if (min(addr[i]) < 0)
                {
                    FatalErrorInFunction
                        << "Error in patch mapping for patch "
                        << patch_.index() << " named " << patch_.name()
                        << abort(FatalError);
                }
            }
        }
    }
}


void CML::fvPatchMapper::clearOut()
{
    deleteDemandDrivenData(directAddrPtr_);
    deleteDemandDrivenData(interpolationAddrPtr_);
    deleteDemandDrivenData(weightsPtr_);
    hasUnmapped_ = false;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::fvPatchMapper::fvPatchMapper
(
    const fvPatch& patch,
    const faceMapper& faceMap
)
:
    patch_(patch),
    faceMap_(faceMap),
    sizeBeforeMapping_(faceMap.oldPatchSizes()[patch_.index()]),
    hasUnmapped_(false),
    directAddrPtr_(nullptr),
    interpolationAddrPtr_(nullptr),
    weightsPtr_(nullptr)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::fvPatchMapper::~fvPatchMapper()
{
    clearOut();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::labelUList& CML::fvPatchMapper::directAddressing() const
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


const CML::labelListList& CML::fvPatchMapper::addressing() const
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


const CML::scalarListList& CML::fvPatchMapper::weights() const
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
