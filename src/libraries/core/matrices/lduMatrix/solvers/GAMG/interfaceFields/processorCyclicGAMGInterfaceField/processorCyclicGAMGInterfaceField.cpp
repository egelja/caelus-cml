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

#include "processorCyclicGAMGInterfaceField.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "lduMatrix.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(processorCyclicGAMGInterfaceField, 0);
    addToRunTimeSelectionTable
    (
        GAMGInterfaceField,
        processorCyclicGAMGInterfaceField,
        lduInterface
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::processorCyclicGAMGInterfaceField::processorCyclicGAMGInterfaceField
(
    const GAMGInterface& GAMGCp,
    const lduInterfaceField& fineInterface
)
:
    processorGAMGInterfaceField(GAMGCp, fineInterface)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::processorCyclicGAMGInterfaceField::~processorCyclicGAMGInterfaceField()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//void CML::processorCyclicGAMGInterfaceField::initInterfaceMatrixUpdate
//(
//    const scalarField& psiInternal,
//    scalarField&,
//    const lduMatrix&,
//    const scalarField&,
//    const direction,
//    const Pstream::commsTypes commsType
//) const
//{
//    procInterface_.compressedSend
//    (
//        commsType,
//        procInterface_.interfaceInternalField(psiInternal)()
//    );
//}
//
//
//void CML::processorCyclicGAMGInterfaceField::updateInterfaceMatrix
//(
//    const scalarField&,
//    scalarField& result,
//    const lduMatrix&,
//    const scalarField& coeffs,
//    const direction cmpt,
//    const Pstream::commsTypes commsType
//) const
//{
//    scalarField pnf
//    (
//        procInterface_.compressedReceive<scalar>(commsType, coeffs.size())
//    );
//    transformCoupleField(pnf, cmpt);
//
//    const labelUList& faceCells = procInterface_.faceCells();
//
//    forAll(faceCells, elemI)
//    {
//        result[faceCells[elemI]] -= coeffs[elemI]*pnf[elemI];
//    }
//}


// ************************************************************************* //
