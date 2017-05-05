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

#include "pointBoundaryMesh.hpp"
#include "polyBoundaryMesh.hpp"
#include "facePointPatch.hpp"
#include "pointMesh.hpp"
#include "PstreamBuffers.hpp"
#include "lduSchedule.hpp"
#include "globalMeshData.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::pointBoundaryMesh::pointBoundaryMesh
(
    const pointMesh& m,
    const polyBoundaryMesh& basicBdry
)
:
    pointPatchList(basicBdry.size()),
    mesh_(m)
{
    // Set boundary patches
    pointPatchList& Patches = *this;

    forAll(Patches, patchI)
    {
        Patches.set
        (
            patchI,
            facePointPatch::New(basicBdry[patchI], *this).ptr()
        );
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::pointBoundaryMesh::calcGeometry()
{
    PstreamBuffers pBufs(Pstream::defaultCommsType);

    if
    (
        Pstream::defaultCommsType == Pstream::blocking
     || Pstream::defaultCommsType == Pstream::nonBlocking
    )
    {
        forAll(*this, patchi)
        {
            operator[](patchi).initGeometry(pBufs);
        }

        pBufs.finishedSends();

        forAll(*this, patchi)
        {
            operator[](patchi).calcGeometry(pBufs);
        }
    }
    else if (Pstream::defaultCommsType == Pstream::scheduled)
    {
        const lduSchedule& patchSchedule = mesh().globalData().patchSchedule();

        // Dummy.
        pBufs.finishedSends();

        forAll(patchSchedule, patchEvali)
        {
            label patchi = patchSchedule[patchEvali].patch;

            if (patchSchedule[patchEvali].init)
            {
                operator[](patchi).initGeometry(pBufs);
            }
            else
            {
                operator[](patchi).calcGeometry(pBufs);
            }
        }
    }
}


void CML::pointBoundaryMesh::movePoints(const pointField& p)
{
    PstreamBuffers pBufs(Pstream::defaultCommsType);

    if
    (
        Pstream::defaultCommsType == Pstream::blocking
     || Pstream::defaultCommsType == Pstream::nonBlocking
    )
    {
        forAll(*this, patchi)
        {
            operator[](patchi).initMovePoints(pBufs, p);
        }

        pBufs.finishedSends();

        forAll(*this, patchi)
        {
            operator[](patchi).movePoints(pBufs, p);
        }
    }
    else if (Pstream::defaultCommsType == Pstream::scheduled)
    {
        const lduSchedule& patchSchedule = mesh().globalData().patchSchedule();

        // Dummy.
        pBufs.finishedSends();

        forAll(patchSchedule, patchEvali)
        {
            label patchi = patchSchedule[patchEvali].patch;

            if (patchSchedule[patchEvali].init)
            {
                operator[](patchi).initMovePoints(pBufs, p);
            }
            else
            {
                operator[](patchi).movePoints(pBufs, p);
            }
        }
    }
}


void CML::pointBoundaryMesh::updateMesh()
{
    PstreamBuffers pBufs(Pstream::defaultCommsType);

    if
    (
        Pstream::defaultCommsType == Pstream::blocking
     || Pstream::defaultCommsType == Pstream::nonBlocking
    )
    {
        forAll(*this, patchi)
        {
            operator[](patchi).initUpdateMesh(pBufs);
        }

        pBufs.finishedSends();

        forAll(*this, patchi)
        {
            operator[](patchi).updateMesh(pBufs);
        }
    }
    else if (Pstream::defaultCommsType == Pstream::scheduled)
    {
        const lduSchedule& patchSchedule = mesh().globalData().patchSchedule();

        // Dummy.
        pBufs.finishedSends();

        forAll(patchSchedule, patchEvali)
        {
            label patchi = patchSchedule[patchEvali].patch;

            if (patchSchedule[patchEvali].init)
            {
                operator[](patchi).initUpdateMesh(pBufs);
            }
            else
            {
                operator[](patchi).updateMesh(pBufs);
            }
        }
    }
}


// ************************************************************************* //
