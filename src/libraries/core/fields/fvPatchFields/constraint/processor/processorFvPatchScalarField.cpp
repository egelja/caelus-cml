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

#include "processorFvPatchScalarField.hpp"
#include "volMesh.hpp"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<>
void processorFvPatchField<scalar>::initInterfaceMatrixUpdate
(
    const scalarField& psiInternal,
    scalarField&,
    const lduMatrix&,
    const scalarField&,
    const direction,
    const Pstream::commsTypes commsType
) const
{
    procPatch_.compressedSend
    (
        commsType,
        patch().patchInternalField(psiInternal)()
    );
}


template<>
void processorFvPatchField<scalar>::updateInterfaceMatrix
(
    const scalarField&,
    scalarField& result,
    const lduMatrix&,
    const scalarField& coeffs,
    const direction,
    const Pstream::commsTypes commsType
) const
{
    scalarField pnf
    (
        procPatch_.compressedReceive<scalar>(commsType, this->size())()
    );

    const labelUList& faceCells = patch().faceCells();

    forAll(faceCells, facei)
    {
        result[faceCells[facei]] -= coeffs[facei]*pnf[facei];
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
