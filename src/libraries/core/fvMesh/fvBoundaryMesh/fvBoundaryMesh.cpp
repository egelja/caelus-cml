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

#include "fvMesh.hpp"
#include "fvBoundaryMesh.hpp"


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::fvBoundaryMesh::addPatches(const polyBoundaryMesh& basicBdry)
{
    setSize(basicBdry.size());

    // Set boundary patches
    fvPatchList& Patches = *this;

    forAll(Patches, patchI)
    {
        Patches.set(patchI, fvPatch::New(basicBdry[patchI], *this));
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::fvBoundaryMesh::fvBoundaryMesh
(
    const fvMesh& m
)
:
    fvPatchList(0),
    mesh_(m)
{}


CML::fvBoundaryMesh::fvBoundaryMesh
(
    const fvMesh& m,
    const polyBoundaryMesh& basicBdry
)
:
    fvPatchList(basicBdry.size()),
    mesh_(m)
{
    addPatches(basicBdry);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::label CML::fvBoundaryMesh::findPatchID(const word& patchName) const
{
    const fvPatchList& patches = *this;

    forAll(patches, patchI)
    {
        if (patches[patchI].name() == patchName)
        {
            return patchI;
        }
    }

    // Not found, return -1
    return -1;
}


void CML::fvBoundaryMesh::movePoints()
{
    forAll(*this, patchI)
    {
        operator[](patchI).initMovePoints();
    }

    forAll(*this, patchI)
    {
        operator[](patchI).movePoints();
    }
}


CML::lduInterfacePtrsList CML::fvBoundaryMesh::interfaces() const
{
    lduInterfacePtrsList interfaces(size());

    forAll(interfaces, patchI)
    {
        if (isA<lduInterface>(this->operator[](patchI)))
        {
            interfaces.set
            (
                patchI,
               &refCast<const lduInterface>(this->operator[](patchI))
            );
        }
    }

    return interfaces;
}


void CML::fvBoundaryMesh::readUpdate(const polyBoundaryMesh& basicBdry)
{
    clear();
    addPatches(basicBdry);
}


// * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * * //

const CML::fvPatch& CML::fvBoundaryMesh::operator[]
(
    const word& patchName
) const
{
    const label patchI = findPatchID(patchName);

    if (patchI < 0)
    {
        FatalErrorInFunction
            << "Patch named " << patchName << " not found." << nl
            << abort(FatalError);
    }

    return operator[](patchI);
}


CML::fvPatch& CML::fvBoundaryMesh::operator[]
(
    const word& patchName
)
{
    const label patchI = findPatchID(patchName);

    if (patchI < 0)
    {
        FatalErrorInFunction
            << "Patch named " << patchName << " not found." << nl
            << abort(FatalError);
    }

    return operator[](patchI);
}


// ************************************************************************* //
