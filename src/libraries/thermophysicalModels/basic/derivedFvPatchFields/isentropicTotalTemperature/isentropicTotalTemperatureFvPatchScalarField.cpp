/*---------------------------------------------------------------------------*\
Copyright (C) 2015 Applied CCM
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

#include "isentropicTotalTemperatureFvPatchScalarField.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvPatchFieldMapper.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"

#include "basicThermo.hpp"

namespace CML
{

isentropicTotalTemperatureFvPatchScalarField::isentropicTotalTemperatureFvPatchScalarField
(
    fvPatch const& p,
    DimensionedField<scalar, volMesh> const& iF
)
:
    fixedValueFvPatchScalarField(p, iF),
    pName_("Undefined"),
    T0_(p.size(), 0.0),
    p0_(p.size(), 0.0)
{}


isentropicTotalTemperatureFvPatchScalarField::isentropicTotalTemperatureFvPatchScalarField
(
    isentropicTotalTemperatureFvPatchScalarField const& ptf,
    fvPatch const& p,
    DimensionedField<scalar, volMesh> const& iF,
    fvPatchFieldMapper const& mapper
)
:
    fixedValueFvPatchScalarField(ptf, p, iF, mapper),
    pName_(ptf.pName_),
    T0_(ptf.T0_, mapper),
    p0_(ptf.p0_, mapper)
{}


isentropicTotalTemperatureFvPatchScalarField::isentropicTotalTemperatureFvPatchScalarField
(
    fvPatch const& p,
    DimensionedField<scalar, volMesh> const& iF,
    dictionary const& dict
)
:
    fixedValueFvPatchScalarField(p, iF),
    pName_(dict.lookup("p")),
    T0_("T0", dict, p.size()),
    p0_("p0", dict, p.size())
{
    if (dict.found("value"))
    {
        fvPatchField<scalar>::operator=
        (
            scalarField("value", dict, p.size())
        );
    }
    else
    {
        fvPatchField<scalar>::operator=(T0_);
    }
}


isentropicTotalTemperatureFvPatchScalarField::isentropicTotalTemperatureFvPatchScalarField
(
    isentropicTotalTemperatureFvPatchScalarField const& tppsf
)
:
    fixedValueFvPatchScalarField(tppsf),
    pName_(tppsf.pName_),
    T0_(tppsf.T0_),
    p0_(tppsf.p0_)
{}


isentropicTotalTemperatureFvPatchScalarField::isentropicTotalTemperatureFvPatchScalarField
(
    isentropicTotalTemperatureFvPatchScalarField const& tppsf,
    DimensionedField<scalar, volMesh> const& iF
)
:
    fixedValueFvPatchScalarField(tppsf, iF),
    pName_(tppsf.pName_),
    T0_(tppsf.T0_),
    p0_(tppsf.p0_)
{}


void isentropicTotalTemperatureFvPatchScalarField::autoMap
(
    fvPatchFieldMapper const& m
)
{
    fixedValueFvPatchScalarField::autoMap(m);
    T0_.autoMap(m);
}


void isentropicTotalTemperatureFvPatchScalarField::rmap
(
    fvPatchScalarField const& ptf,
    labelList const& addr
)
{
    fixedValueFvPatchScalarField::rmap(ptf, addr);

    isentropicTotalTemperatureFvPatchScalarField const& tiptf =
        refCast<const isentropicTotalTemperatureFvPatchScalarField>(ptf);

    T0_.rmap(tiptf.T0_, addr);
}


void isentropicTotalTemperatureFvPatchScalarField::updateCoeffs()
{
    if (updated())
    {
        return;
    }
    fvPatchField<scalar> const& p =
        patch().lookupPatchField<volScalarField, scalar>(pName_);

    basicThermo const& thermo =
        db().lookupObject<basicThermo>("thermophysicalProperties");

    tmp<volScalarField> gamma = thermo.Cp()/thermo.Cv();

    tmp<fvPatchField<scalar> > const& gammap =
      patch().patchField<volScalarField, scalar>(gamma());

    tmp<scalarField> gM1ByG = (gammap() - 1.0)/gammap();

    operator==(T0_*pow(p/p0_,gM1ByG()));

    fixedValueFvPatchScalarField::updateCoeffs();
}

void isentropicTotalTemperatureFvPatchScalarField::updateCoeffs
(
    vectorField const& Up
)
{
    updateCoeffs();
}


void isentropicTotalTemperatureFvPatchScalarField::write(Ostream& os) const
{
    fvPatchScalarField::write(os);
    os.writeKeyword("p") << pName_ << token::END_STATEMENT << nl;
    T0_.writeEntry("T0", os);
    p0_.writeEntry("p0", os);
    writeEntry("value", os);
}


makePatchTypeField(fvPatchScalarField, isentropicTotalTemperatureFvPatchScalarField);

}
