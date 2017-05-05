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

#include "buoyantPressureFvPatchScalarField.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvPatchFieldMapper.hpp"
#include "volFields.hpp"
#include "uniformDimensionedFields.hpp"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::buoyantPressureFvPatchScalarField::
buoyantPressureFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedGradientFvPatchScalarField(p, iF),
    rhoName_("rho")
{}


CML::buoyantPressureFvPatchScalarField::
buoyantPressureFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    fixedGradientFvPatchScalarField(p, iF),
    rhoName_(dict.lookupOrDefault<word>("rho", "rho"))
{
    fvPatchField<scalar>::operator=(patchInternalField());
    gradient() = 0.0;
}


CML::buoyantPressureFvPatchScalarField::
buoyantPressureFvPatchScalarField
(
    const buoyantPressureFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedGradientFvPatchScalarField(ptf, p, iF, mapper),
    rhoName_(ptf.rhoName_)
{}


CML::buoyantPressureFvPatchScalarField::
buoyantPressureFvPatchScalarField
(
    const buoyantPressureFvPatchScalarField& ptf
)
:
    fixedGradientFvPatchScalarField(ptf),
    rhoName_(ptf.rhoName_)
{}


CML::buoyantPressureFvPatchScalarField::
buoyantPressureFvPatchScalarField
(
    const buoyantPressureFvPatchScalarField& ptf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedGradientFvPatchScalarField(ptf, iF),
    rhoName_(ptf.rhoName_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::buoyantPressureFvPatchScalarField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    const uniformDimensionedVectorField& g =
        db().lookupObject<uniformDimensionedVectorField>("g");

    const fvPatchField<scalar>& rho =
        patch().lookupPatchField<volScalarField, scalar>(rhoName_);

    // If the variable name is "p_rgh", "ph_rgh" or "pd"
    // assume it is p? - rho*g.h and set the gradient appropriately.
    // Otherwise assume the variable is the static pressure.
    if
    (
        dimensionedInternalField().name() == "p_rgh"
     || dimensionedInternalField().name() == "ph_rgh"
     || dimensionedInternalField().name() == "pd"
    )
    {
        gradient() = -rho.snGrad()*(g.value() & patch().Cf());
    }
    else
    {
        gradient() = rho*(g.value() & patch().nf());
    }

    fixedGradientFvPatchScalarField::updateCoeffs();
}


void CML::buoyantPressureFvPatchScalarField::write(Ostream& os) const
{
    fixedGradientFvPatchScalarField::write(os);
    writeEntryIfDifferent<word>(os, "rho", "rho", rhoName_);
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    makePatchTypeField
    (
        fvPatchScalarField,
        buoyantPressureFvPatchScalarField
    );
}


// ************************************************************************* //
