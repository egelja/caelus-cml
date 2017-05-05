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

#include "cyclicGAMGInterfaceField.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "lduMatrix.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(cyclicGAMGInterfaceField, 0);
    addToRunTimeSelectionTable
    (
        GAMGInterfaceField,
        cyclicGAMGInterfaceField,
        lduInterface
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::cyclicGAMGInterfaceField::cyclicGAMGInterfaceField
(
    const GAMGInterface& GAMGCp,
    const lduInterfaceField& fineInterface
)
:
    GAMGInterfaceField(GAMGCp, fineInterface),
    cyclicInterface_(refCast<const cyclicGAMGInterface>(GAMGCp)),
    doTransform_(false),
    rank_(0)
{
    const cyclicLduInterfaceField& p =
        refCast<const cyclicLduInterfaceField>(fineInterface);

    doTransform_ = p.doTransform();
    rank_ = p.rank();
}


// * * * * * * * * * * * * * * * * Desstructor * * * * * * * * * * * * * * * //

CML::cyclicGAMGInterfaceField::~cyclicGAMGInterfaceField()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::cyclicGAMGInterfaceField::updateInterfaceMatrix
(
    const scalarField& psiInternal,
    scalarField& result,
    const lduMatrix&,
    const scalarField& coeffs,
    const direction cmpt,
    const Pstream::commsTypes
) const
{
    // Get neighbouring field
    scalarField pnf
    (
        cyclicInterface_.neighbPatch().interfaceInternalField(psiInternal)
    );

    transformCoupleField(pnf, cmpt);

    const labelUList& faceCells = cyclicInterface_.faceCells();

    forAll(faceCells, elemI)
    {
        result[faceCells[elemI]] -= coeffs[elemI]*pnf[elemI];
    }
}


// ************************************************************************* //
