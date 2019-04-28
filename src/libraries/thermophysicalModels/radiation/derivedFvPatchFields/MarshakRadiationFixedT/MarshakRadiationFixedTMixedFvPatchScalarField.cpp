/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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

#include "MarshakRadiationFixedTMixedFvPatchScalarField.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvPatchFieldMapper.hpp"
#include "volFields.hpp"
#include "radiationModel.hpp"
#include "physicoChemicalConstants.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::MarshakRadiationFixedTemperatureFvPatchScalarField::
MarshakRadiationFixedTemperatureFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchScalarField(p, iF),
    radiationCoupledBase(p, "undefined", scalarField::null()),
    Trad_(p.size())
{
    refValue() = 0.0;
    refGrad() = 0.0;
    valueFraction() = 0.0;
}


CML::MarshakRadiationFixedTemperatureFvPatchScalarField::
MarshakRadiationFixedTemperatureFvPatchScalarField
(
    const MarshakRadiationFixedTemperatureFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchScalarField(ptf, p, iF, mapper),
    radiationCoupledBase
    (
        p,
        ptf.emissivityMethod(),
        ptf.emissivity_,
        mapper
    ),
    Trad_(ptf.Trad_, mapper)
{}


CML::MarshakRadiationFixedTemperatureFvPatchScalarField::
MarshakRadiationFixedTemperatureFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchScalarField(p, iF),
    radiationCoupledBase(p, dict),
    Trad_("Trad", dict, p.size())
{
    // refValue updated on each call to updateCoeffs()
    refValue() = 4.0*constant::physicoChemical::sigma.value()*pow4(Trad_);

    // zero gradient
    refGrad() = 0.0;

    valueFraction() = 1.0;

    fvPatchScalarField::operator=(refValue());
}


CML::MarshakRadiationFixedTemperatureFvPatchScalarField::
MarshakRadiationFixedTemperatureFvPatchScalarField
(
    const MarshakRadiationFixedTemperatureFvPatchScalarField& ptf
)
:
    mixedFvPatchScalarField(ptf),
    radiationCoupledBase
    (
        ptf.patch(),
        ptf.emissivityMethod(),
        ptf.emissivity_
    ),
    Trad_(ptf.Trad_)
{}


CML::MarshakRadiationFixedTemperatureFvPatchScalarField::
MarshakRadiationFixedTemperatureFvPatchScalarField
(
    const MarshakRadiationFixedTemperatureFvPatchScalarField& ptf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchScalarField(ptf, iF),
    radiationCoupledBase
    (
        ptf.patch(),
        ptf.emissivityMethod(),
        ptf.emissivity_
    ),
    Trad_(ptf.Trad_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::MarshakRadiationFixedTemperatureFvPatchScalarField::autoMap
(
    const fvPatchFieldMapper& m
)
{
    mixedFvPatchScalarField::autoMap(m);
    radiationCoupledBase::autoMap(m);
    Trad_.autoMap(m);
}


void CML::MarshakRadiationFixedTemperatureFvPatchScalarField::rmap
(
    const fvPatchScalarField& ptf,
    const labelList& addr
)
{
    mixedFvPatchScalarField::rmap(ptf, addr);
    radiationCoupledBase::rmap(ptf, addr);
    const MarshakRadiationFixedTemperatureFvPatchScalarField& mrptf =
        refCast<const MarshakRadiationFixedTemperatureFvPatchScalarField>(ptf);

    Trad_.rmap(mrptf.Trad_, addr);
}


void CML::MarshakRadiationFixedTemperatureFvPatchScalarField::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    // Since we're inside initEvaluate/evaluate there might be processor
    // comms underway. Change the tag we use.
    int oldTag = UPstream::msgType();
    UPstream::msgType() = oldTag+1;

    // Re-calc reference value
    refValue() = 4.0*constant::physicoChemical::sigma.value()*pow4(Trad_);

    // Diffusion coefficient - created by radiation model's ::updateCoeffs()
    const scalarField& gamma =
        patch().lookupPatchField<volScalarField, scalar>("gammaRad");

    const scalarField temissivity = emissivity();

    const scalarField Ep(temissivity/(2*(2 - temissivity)));

    // Set value fraction
    valueFraction() = 1.0/(1.0 + gamma*patch().deltaCoeffs()/Ep);

    // Restore tag
    UPstream::msgType() = oldTag;

    mixedFvPatchScalarField::updateCoeffs();
}


void CML::MarshakRadiationFixedTemperatureFvPatchScalarField::write
(
    Ostream& os
) const
{
    mixedFvPatchScalarField::write(os);
    radiationCoupledBase::write(os);
    Trad_.writeEntry("Trad", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    makePatchTypeField
    (
        fvPatchScalarField,
        MarshakRadiationFixedTemperatureFvPatchScalarField
    );
}
