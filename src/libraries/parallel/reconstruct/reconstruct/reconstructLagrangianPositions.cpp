/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2017 OpenFOAM Foundation
Copyright (C) 2018 OpenCFD Ltd.
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

#include "reconstructLagrangian.hpp"
#include "labelIOList.hpp"
#include "passiveParticleCloud.hpp"

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

void CML::reconstructLagrangianPositions
(
    const polyMesh& mesh,
    const word& cloudName,
    const PtrList<fvMesh>& meshes,
    const PtrList<labelIOList>& faceProcAddressing,
    const PtrList<labelIOList>& cellProcAddressing
)
{
    passiveParticleCloud lagrangianPositions
    (
        mesh,
        cloudName,
        IDLList<passiveParticle>()
    );

    forAll(meshes, i)
    {
        const labelList& cellMap = cellProcAddressing[i];
        const labelList& faceMap = faceProcAddressing[i];

        Cloud<passiveParticle> lpi(meshes[i], cloudName, false);

        forAllConstIter(Cloud<passiveParticle>, lpi, iter)
        {
            const passiveParticle& ppi = iter();

            const label mappedCell = cellMap[ppi.cell()];

            // Inverting sign if necessary and subtracting 1 from
            // faceProcAddressing
            label mappedTetFace = mag(faceMap[ppi.tetFace()]) - 1;

            lagrangianPositions.append
            (
                new passiveParticle
                (
                    mesh,
                    ppi.coordinates(),
                    mappedCell,
                    mappedTetFace,
                    ppi.procTetPt(mesh, mappedCell, mappedTetFace)
                )
            );
        }
    }

    IOPosition<Cloud<passiveParticle> >(lagrangianPositions).write();
}


// ************************************************************************* //
