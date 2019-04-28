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
    Lagrangian field decomposer.

\*---------------------------------------------------------------------------*/

#include "lagrangianFieldDecomposer.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::lagrangianFieldDecomposer::lagrangianFieldDecomposer
(
    const polyMesh& mesh,
    const polyMesh& procMesh,
    const labelList& faceProcAddressing,
    const labelList& cellProcAddressing,
    const word& cloudName,
    const Cloud<indexedParticle>& lagrangianPositions,
    const List<SLList<indexedParticle*>*>& cellParticles
)
:
    procMesh_(procMesh),
    positions_(procMesh, cloudName, IDLList<passiveParticle>()),
    particleIndices_(lagrangianPositions.size())
{
    label pi = 0;

    labelList decodedProcFaceAddressing(faceProcAddressing.size());

    forAll(faceProcAddressing, i)
    {
        decodedProcFaceAddressing[i] = mag(faceProcAddressing[i]) - 1;
    }

    forAll(cellProcAddressing, procCelli)
    {
        label celli = cellProcAddressing[procCelli];

        if (cellParticles[celli])
        {
            SLList<indexedParticle*>& particlePtrs = *cellParticles[celli];

            forAllConstIter(SLList<indexedParticle*>, particlePtrs, iter)
            {
                const indexedParticle& ppi = *iter();
                particleIndices_[pi++] = ppi.index();

                label mappedTetFace = findIndex
                (
                    decodedProcFaceAddressing,
                    ppi.tetFace()
                );

                if (mappedTetFace == -1)
                {
                    FatalErrorInFunction
                        << "Face lookup failure." << nl
                        << abort(FatalError);
                }

                positions_.append
                (
                    new passiveParticle
                    (
                        procMesh,
                        ppi.coordinates(),
                        procCelli,
                        mappedTetFace,
                        ppi.procTetPt(procMesh, procCelli, mappedTetFace)
                    )
                );
            }
        }
    }

    particleIndices_.setSize(pi);

    IOPosition<Cloud<passiveParticle> >(positions_).write();
}


// ************************************************************************* //
