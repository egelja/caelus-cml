/*---------------------------------------------------------------------------*\
Copyright (C) 2016 Applied CCM
Copyright (C) 2016 OpenFOAM Foundation
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

#include "totalTemperatureFvPatchScalarField.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvPatchFieldMapper.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"

CML::totalTemperatureFvPatchScalarField::totalTemperatureFvPatchScalarField
(
    fvPatch const& p,
    DimensionedField<scalar, volMesh> const& iF
)
:
    fixedValueFvPatchScalarField(p, iF),
    UName_("U"),
    phiName_("phi"),
    psiName_("thermo:psi"),
    gamma_(0.0),
    T0_(p.size(), 0.0)
{}


CML::totalTemperatureFvPatchScalarField::totalTemperatureFvPatchScalarField
(
    totalTemperatureFvPatchScalarField const& ptf,
    fvPatch const& p,
    DimensionedField<scalar, volMesh> const& iF,
    fvPatchFieldMapper const& mapper
)
:
    fixedValueFvPatchScalarField(ptf, p, iF, mapper),
    UName_(ptf.UName_),
    phiName_(ptf.phiName_),
    psiName_(ptf.psiName_),
    gamma_(ptf.gamma_),
    T0_(ptf.T0_, mapper)
{}


CML::totalTemperatureFvPatchScalarField::totalTemperatureFvPatchScalarField
(
    fvPatch const& p,
    DimensionedField<scalar, volMesh> const& iF,
    dictionary const& dict
)
:
    fixedValueFvPatchScalarField(p, iF),
    UName_(dict.lookupOrDefault<word>("U", "U")),
    phiName_(dict.lookupOrDefault<word>("phi", "phi")),
    psiName_(dict.lookupOrDefault<word>("psi", "thermo:psi")),
    gamma_(readScalar(dict.lookup("gamma"))),
    T0_("T0", dict, p.size())
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


CML::totalTemperatureFvPatchScalarField::totalTemperatureFvPatchScalarField
(
    totalTemperatureFvPatchScalarField const& tppsf
)
:
    fixedValueFvPatchScalarField(tppsf),
    UName_(tppsf.UName_),
    phiName_(tppsf.phiName_),
    psiName_(tppsf.psiName_),
    gamma_(tppsf.gamma_),
    T0_(tppsf.T0_)
{}


CML::totalTemperatureFvPatchScalarField::totalTemperatureFvPatchScalarField
(
    totalTemperatureFvPatchScalarField const& tppsf,
    DimensionedField<scalar, volMesh> const& iF
)
:
    fixedValueFvPatchScalarField(tppsf, iF),
    UName_(tppsf.UName_),
    phiName_(tppsf.phiName_),
    psiName_(tppsf.psiName_),
    gamma_(tppsf.gamma_),
    T0_(tppsf.T0_)
{}


void CML::totalTemperatureFvPatchScalarField::autoMap
(
    fvPatchFieldMapper const& m
)
{
    fixedValueFvPatchScalarField::autoMap(m);
    T0_.autoMap(m);
}


void CML::totalTemperatureFvPatchScalarField::rmap
(
    fvPatchScalarField const& ptf,
    labelList const& addr
)
{
    fixedValueFvPatchScalarField::rmap(ptf, addr);

    totalTemperatureFvPatchScalarField const& tiptf =
        refCast<totalTemperatureFvPatchScalarField const>(ptf);

    T0_.rmap(tiptf.T0_, addr);
}


void CML::totalTemperatureFvPatchScalarField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    fvPatchVectorField const& Up =
        patch().lookupPatchField<volVectorField, vector>(UName_);

    fvsPatchField<scalar> const& phip =
        patch().lookupPatchField<surfaceScalarField, scalar>(phiName_);

    fvPatchField<scalar> const& psip =
        patch().lookupPatchField<volScalarField, scalar>(psiName_);

    scalar gM1ByG = (gamma_ - 1.0)/gamma_;

    operator==
    (
        T0_/(1.0 + 0.5*psip*gM1ByG*(1.0 - pos(phip))*magSqr(Up))
    );

    fixedValueFvPatchScalarField::updateCoeffs();
}


void CML::totalTemperatureFvPatchScalarField::write(Ostream& os) const
{
    fvPatchScalarField::write(os);
    writeEntryIfDifferent<word>(os, "U", "U", UName_);
    writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
    writeEntryIfDifferent<word>(os, "psi", "thermo:psi", psiName_);
    os.writeKeyword("gamma") << gamma_ << token::END_STATEMENT << nl;
    T0_.writeEntry("T0", os);
    writeEntry("value", os);
}


namespace CML
{
    makePatchTypeField
    (
        fvPatchScalarField,
        totalTemperatureFvPatchScalarField
    );
}


