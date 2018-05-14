/*---------------------------------------------------------------------------*\
Copyright (C) 2016 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "fixedFluxExtrapolatedPressureFvPatchScalarField.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::fixedFluxExtrapolatedPressureFvPatchScalarField::
fixedFluxExtrapolatedPressureFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedFluxPressureFvPatchScalarField(p, iF)
{}


CML::fixedFluxExtrapolatedPressureFvPatchScalarField::
fixedFluxExtrapolatedPressureFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    fixedFluxPressureFvPatchScalarField(p, iF, dict)
{}


CML::fixedFluxExtrapolatedPressureFvPatchScalarField::
fixedFluxExtrapolatedPressureFvPatchScalarField
(
    const fixedFluxExtrapolatedPressureFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedFluxPressureFvPatchScalarField(ptf, p, iF, mapper)
{}


CML::fixedFluxExtrapolatedPressureFvPatchScalarField::
fixedFluxExtrapolatedPressureFvPatchScalarField
(
    const fixedFluxExtrapolatedPressureFvPatchScalarField& wbppsf
)
:
    fixedFluxPressureFvPatchScalarField(wbppsf)
{}


CML::fixedFluxExtrapolatedPressureFvPatchScalarField::
fixedFluxExtrapolatedPressureFvPatchScalarField
(
    const fixedFluxExtrapolatedPressureFvPatchScalarField& wbppsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedFluxPressureFvPatchScalarField(wbppsf, iF)
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    makePatchTypeField
    (
        fvPatchScalarField,
        fixedFluxExtrapolatedPressureFvPatchScalarField
    );
}
