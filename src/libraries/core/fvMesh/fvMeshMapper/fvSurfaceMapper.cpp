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

Description
    FV surface mapper.

\*---------------------------------------------------------------------------*/

#include "fvSurfaceMapper.hpp"
#include "fvMesh.hpp"
#include "mapPolyMesh.hpp"
#include "faceMapper.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::fvSurfaceMapper::calcAddressing() const
{
    if
    (
        directAddrPtr_
     || interpolationAddrPtr_
     || weightsPtr_
     || insertedObjectLabelsPtr_
    )
    {
        FatalErrorInFunction
            << "Addressing already calculated"
            << abort(FatalError);
    }

    // Mapping

    const label oldNInternal = faceMap_.nOldInternalFaces();

    // Assemble the maps
    if (direct())
    {
        // Direct mapping - slice to size
        directAddrPtr_ =
            new labelList
            (
                labelList::subList(faceMap_.directAddressing(), size())
            );
        labelList& addr = *directAddrPtr_;

        // Adjust for creation of an internal face from a boundary face
        forAll(addr, faceI)
        {
            if (addr[faceI] > oldNInternal)
            {
                addr[faceI] = 0;
            }
        }
    }
    else
    {
        // Interpolative mapping - slice to size
        interpolationAddrPtr_ =
            new labelListList
            (
                labelListList::subList(faceMap_.addressing(), size())
            );
        labelListList& addr = *interpolationAddrPtr_;

        weightsPtr_ =
            new scalarListList
            (
                scalarListList::subList(faceMap_.weights(), size())
            );
        scalarListList& w = *weightsPtr_;

        // Adjust for creation of an internal face from a boundary face
        forAll(addr, faceI)
        {
            if (max(addr[faceI]) >= oldNInternal)
            {
                addr[faceI] = labelList(1, label(0));
                w[faceI] = scalarList(1, 1.0);
            }
        }
    }

    // Inserted objects

    // If there are, assemble the labels
    if (insertedObjects())
    {
        const labelList& insFaces = faceMap_.insertedObjectLabels();

        insertedObjectLabelsPtr_ = new labelList(insFaces.size());
        labelList& ins = *insertedObjectLabelsPtr_;

        label nIns = 0;

        forAll(insFaces, faceI)
        {
            // If the face is internal, keep it here
            if (insFaces[faceI] < size())
            {
                ins[nIns] = insFaces[faceI];
                nIns++;
            }
        }

        ins.setSize(nIns);
    }
    else
    {
        // No inserted objects
        insertedObjectLabelsPtr_ = new labelList(0);
    }
}


void CML::fvSurfaceMapper::clearOut()
{
    deleteDemandDrivenData(directAddrPtr_);
    deleteDemandDrivenData(interpolationAddrPtr_);
    deleteDemandDrivenData(weightsPtr_);

    deleteDemandDrivenData(insertedObjectLabelsPtr_);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::fvSurfaceMapper::fvSurfaceMapper
(
    const fvMesh& mesh,
    const faceMapper& fMapper
)
:
    mesh_(mesh),
    faceMap_(fMapper),
    directAddrPtr_(nullptr),
    interpolationAddrPtr_(nullptr),
    weightsPtr_(nullptr),
    insertedObjectLabelsPtr_(nullptr)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::fvSurfaceMapper::~fvSurfaceMapper()
{
    clearOut();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::labelUList& CML::fvSurfaceMapper::directAddressing() const
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


const CML::labelListList& CML::fvSurfaceMapper::addressing() const
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


const CML::scalarListList& CML::fvSurfaceMapper::weights() const
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


const CML::labelList& CML::fvSurfaceMapper::insertedObjectLabels() const
{
    if (!insertedObjectLabelsPtr_)
    {
        calcAddressing();
    }

    return *insertedObjectLabelsPtr_;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //


// ************************************************************************* //
