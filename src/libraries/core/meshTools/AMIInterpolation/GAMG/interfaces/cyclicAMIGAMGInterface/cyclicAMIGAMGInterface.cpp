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

#include "AMIInterpolation.hpp"
#include "cyclicAMIGAMGInterface.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "Map.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(cyclicAMIGAMGInterface, 0);
    addToRunTimeSelectionTable
    (
        GAMGInterface,
        cyclicAMIGAMGInterface,
        lduInterface
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::cyclicAMIGAMGInterface::cyclicAMIGAMGInterface
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
    fineCyclicAMIInterface_
    (
        refCast<const cyclicAMILduInterface>(fineInterface)
    )
{
    // Construct face agglomeration from cell agglomeration
    {
        // From coarse face to cell
        DynamicList<label> dynFaceCells(localRestrictAddressing.size());

        // From face to coarse face
        DynamicList<label> dynFaceRestrictAddressing
        (
            localRestrictAddressing.size()
        );

        Map<label> masterToCoarseFace(localRestrictAddressing.size());

        forAll(localRestrictAddressing, ffi)
        {
            label curMaster = localRestrictAddressing[ffi];

            Map<label>::const_iterator fnd = masterToCoarseFace.find
            (
                curMaster
            );

            if (fnd == masterToCoarseFace.end())
            {
                // New coarse face
                label coarseI = dynFaceCells.size();
                dynFaceRestrictAddressing.append(coarseI);
                dynFaceCells.append(curMaster);
                masterToCoarseFace.insert(curMaster, coarseI);
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


    // On the owner side construct the AMI

    if (fineCyclicAMIInterface_.owner())
    {
        // Construct the neighbour side agglomeration (as the neighbour would
        // do it so it the exact loop above using neighbourRestrictAddressing
        // instead of localRestrictAddressing)

        labelList nbrFaceRestrictAddressing;
        {
            // From face to coarse face
            DynamicList<label> dynNbrFaceRestrictAddressing
            (
                neighbourRestrictAddressing.size()
            );

            Map<label> masterToCoarseFace(neighbourRestrictAddressing.size());

            forAll(neighbourRestrictAddressing, ffi)
            {
                label curMaster = neighbourRestrictAddressing[ffi];

                Map<label>::const_iterator fnd = masterToCoarseFace.find
                (
                    curMaster
                );

                if (fnd == masterToCoarseFace.end())
                {
                    // New coarse face
                    label coarseI = masterToCoarseFace.size();
                    dynNbrFaceRestrictAddressing.append(coarseI);
                    masterToCoarseFace.insert(curMaster, coarseI);
                }
                else
                {
                    // Already have coarse face
                    dynNbrFaceRestrictAddressing.append(fnd());
                }
            }

            nbrFaceRestrictAddressing.transfer(dynNbrFaceRestrictAddressing);
        }

        amiPtr_.reset
        (
            new AMIPatchToPatchInterpolation
            (
                fineCyclicAMIInterface_.AMI(),
                faceRestrictAddressing_,
                nbrFaceRestrictAddressing
            )
        );
    }
}


// * * * * * * * * * * * * * * * * Desstructor * * * * * * * * * * * * * * * //

CML::cyclicAMIGAMGInterface::~cyclicAMIGAMGInterface()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::tmp<CML::labelField> CML::cyclicAMIGAMGInterface::internalFieldTransfer
(
    const Pstream::commsTypes,
    const labelUList& iF
) const
{
    const cyclicAMIGAMGInterface& nbr =
        dynamic_cast<const cyclicAMIGAMGInterface&>(neighbPatch());
    const labelUList& nbrFaceCells = nbr.faceCells();

    tmp<labelField> tpnf(new labelField(nbrFaceCells.size()));
    labelField& pnf = tpnf();

    forAll(pnf, facei)
    {
        pnf[facei] = iF[nbrFaceCells[facei]];
    }

    return tpnf;
}


// ************************************************************************* //
