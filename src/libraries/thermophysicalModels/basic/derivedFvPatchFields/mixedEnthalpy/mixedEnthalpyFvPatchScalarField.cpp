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

#include "mixedEnthalpyFvPatchScalarField.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvPatchFieldMapper.hpp"
#include "volFields.hpp"
#include "basicThermo.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::mixedEnthalpyFvPatchScalarField::mixedEnthalpyFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchScalarField(p, iF)
{
    valueFraction() = 0.0;
    refValue() = 0.0;
    refGrad() = 0.0;
}


CML::mixedEnthalpyFvPatchScalarField::mixedEnthalpyFvPatchScalarField
(
    const mixedEnthalpyFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchScalarField(ptf, p, iF, mapper)
{}


CML::mixedEnthalpyFvPatchScalarField::mixedEnthalpyFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchScalarField(p, iF, dict)
{}


CML::mixedEnthalpyFvPatchScalarField::mixedEnthalpyFvPatchScalarField
(
    const mixedEnthalpyFvPatchScalarField& tppsf
)
:
    mixedFvPatchScalarField(tppsf)
{}


CML::mixedEnthalpyFvPatchScalarField::mixedEnthalpyFvPatchScalarField
(
    const mixedEnthalpyFvPatchScalarField& tppsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchScalarField(tppsf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::mixedEnthalpyFvPatchScalarField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    const basicThermo& thermo = db().lookupObject<basicThermo>
    (
        "thermophysicalProperties"
    );

    const label patchi = patch().index();

    mixedFvPatchScalarField& Tw = refCast<mixedFvPatchScalarField>
    (
        const_cast<fvPatchScalarField&>(thermo.T().boundaryField()[patchi])
    );

    Tw.evaluate();

    valueFraction() = Tw.valueFraction();

    if (dimensionedInternalField().name() == "h")
    {
        refValue() = thermo.h(Tw.refValue(), patchi);
        refGrad() = thermo.Cp(Tw, patchi)*Tw.refGrad()
        + patch().deltaCoeffs()*
         (
            thermo.h(Tw, patchi)
          - thermo.h(Tw, patch().faceCells())
         );
    }
    else
    {
        refValue() = thermo.hs(Tw.refValue(), patchi);
        refGrad() = thermo.Cp(Tw, patchi)*Tw.refGrad()
        + patch().deltaCoeffs()*
         (
            thermo.hs(Tw, patchi)
          - thermo.hs(Tw, patch().faceCells())
         );
    }

    mixedFvPatchScalarField::updateCoeffs();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    makePatchTypeField
    (
        fvPatchScalarField,
        mixedEnthalpyFvPatchScalarField
    );
}


// ************************************************************************* //
