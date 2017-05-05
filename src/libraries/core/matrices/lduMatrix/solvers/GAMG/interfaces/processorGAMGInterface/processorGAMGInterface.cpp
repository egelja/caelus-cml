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

#include "processorGAMGInterface.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "HashTable.hpp"
#include "labelPair.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(processorGAMGInterface, 0);
    addToRunTimeSelectionTable
    (
        GAMGInterface,
        processorGAMGInterface,
        lduInterface
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::processorGAMGInterface::processorGAMGInterface
(
    const label index,
    const lduInterfacePtrsList& coarseInterfaces,
    const lduInterface& fineInterface,
    const labelField& localRestrictAddressing,
    const labelField& neighbourRestrictAddressing
)
:
    GAMGInterface
    (
        index,
        coarseInterfaces,
        fineInterface,
        localRestrictAddressing,
        neighbourRestrictAddressing
    ),
    fineProcInterface_(refCast<const processorLduInterface>(fineInterface))
{
    // From coarse face to coarse cell
    DynamicList<label> dynFaceCells(localRestrictAddressing.size());
    // From fine face to coarse face
    DynamicList<label> dynFaceRestrictAddressing
    (
        localRestrictAddressing.size()
    );

    // From coarse cell pair to coarse face
    HashTable<label, labelPair, labelPair::Hash<> > cellsToCoarseFace
    (
        2*localRestrictAddressing.size()
    );

    forAll(localRestrictAddressing, ffi)
    {
        labelPair cellPair;

        // Do switching on master/slave indexes based on the owner/neighbour of
        // the processor index such that both sides get the same answer.
        if (myProcNo() < neighbProcNo())
        {
            // Master side
            cellPair = labelPair
            (
                localRestrictAddressing[ffi],
                neighbourRestrictAddressing[ffi]
            );
        }
        else
        {
            // Slave side
            cellPair = labelPair
            (
                neighbourRestrictAddressing[ffi],
                localRestrictAddressing[ffi]
            );
        }

        HashTable<label, labelPair, labelPair::Hash<> >::const_iterator fnd =
            cellsToCoarseFace.find(cellPair);

        if (fnd == cellsToCoarseFace.end())
        {
            // New coarse face
            label coarseI = dynFaceCells.size();
            dynFaceRestrictAddressing.append(coarseI);
            dynFaceCells.append(localRestrictAddressing[ffi]);
            cellsToCoarseFace.insert(cellPair, coarseI);
        }
        else
        {
            // Already have coarse face
            dynFaceRestrictAddressing.append(fnd());
        }
    }

    faceCells_.transfer(dynFaceCells);
    faceRestrictAddressing_.transfer(dynFaceRestrictAddressing);
}


// * * * * * * * * * * * * * * * * Desstructor * * * * * * * * * * * * * * * //

CML::processorGAMGInterface::~processorGAMGInterface()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::processorGAMGInterface::initInternalFieldTransfer
(
    const Pstream::commsTypes commsType,
    const labelUList& iF
) const
{
    send(commsType, interfaceInternalField(iF)());
}


CML::tmp<CML::labelField> CML::processorGAMGInterface::internalFieldTransfer
(
    const Pstream::commsTypes commsType,
    const labelUList& iF
) const
{
    return receive<label>(commsType, this->size());
}


// ************************************************************************* //
