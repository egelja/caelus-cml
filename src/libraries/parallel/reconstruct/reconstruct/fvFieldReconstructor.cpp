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

#include "fvFieldReconstructor.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::fvFieldReconstructor::fvFieldReconstructor
(
    fvMesh& mesh,
    const PtrList<fvMesh>& procMeshes,
    const PtrList<labelIOList>& faceProcAddressing,
    const PtrList<labelIOList>& cellProcAddressing,
    const PtrList<labelIOList>& boundaryProcAddressing
)
:
    mesh_(mesh),
    procMeshes_(procMeshes),
    faceProcAddressing_(faceProcAddressing),
    cellProcAddressing_(cellProcAddressing),
    boundaryProcAddressing_(boundaryProcAddressing),
    nReconstructed_(0)
{
    forAll(procMeshes_, procI)
    {
        const fvMesh& procMesh = procMeshes_[procI];
        if
        (
            faceProcAddressing[procI].size() != procMesh.nFaces()
         || cellProcAddressing[procI].size() != procMesh.nCells()
         || boundaryProcAddressing[procI].size() != procMesh.boundary().size()
        )
        {
            FatalErrorInFunction
                << "Size of maps does not correspond to size of mesh"
                << " for processor " << procI << endl
                << "faceProcAddressing : " << faceProcAddressing[procI].size()
                << " nFaces : " << procMesh.nFaces() << endl
                << "cellProcAddressing : " << cellProcAddressing[procI].size()
                << " nCell : " << procMesh.nCells() << endl
                << "boundaryProcAddressing : "
                << boundaryProcAddressing[procI].size()
                << " nFaces : " << procMesh.boundary().size()
                << exit(FatalError);
        }
    }
}


// ************************************************************************* //
