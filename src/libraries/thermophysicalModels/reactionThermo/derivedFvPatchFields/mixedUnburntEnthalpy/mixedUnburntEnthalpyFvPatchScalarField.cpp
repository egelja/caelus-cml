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

#include "mixedUnburntEnthalpyFvPatchScalarField.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvPatchFieldMapper.hpp"
#include "volFields.hpp"
#include "psiuReactionThermo.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::mixedUnburntEnthalpyFvPatchScalarField::
mixedUnburntEnthalpyFvPatchScalarField
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


CML::mixedUnburntEnthalpyFvPatchScalarField::
mixedUnburntEnthalpyFvPatchScalarField
(
    const mixedUnburntEnthalpyFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchScalarField(ptf, p, iF, mapper)
{}


CML::mixedUnburntEnthalpyFvPatchScalarField::
mixedUnburntEnthalpyFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchScalarField(p, iF, dict)
{}


CML::mixedUnburntEnthalpyFvPatchScalarField::
mixedUnburntEnthalpyFvPatchScalarField
(
    const mixedUnburntEnthalpyFvPatchScalarField& tppsf
)
:
    mixedFvPatchScalarField(tppsf)
{}


CML::mixedUnburntEnthalpyFvPatchScalarField::
mixedUnburntEnthalpyFvPatchScalarField
(
    const mixedUnburntEnthalpyFvPatchScalarField& tppsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchScalarField(tppsf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::mixedUnburntEnthalpyFvPatchScalarField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    const psiuReactionThermo& thermo = db().lookupObject<psiuReactionThermo>
    (
        basicThermo::dictName
    );

    const label patchi = patch().index();

    const scalarField& pw = thermo.p().boundaryField()[patchi];
    mixedFvPatchScalarField& Tw = refCast<mixedFvPatchScalarField>
    (
        const_cast<fvPatchScalarField&>(thermo.Tu().boundaryField()[patchi])
    );

    Tw.evaluate();

    valueFraction() = Tw.valueFraction();
    refValue() = thermo.heu(pw, Tw.refValue(), patchi);
    refGrad() = thermo.Cp(pw, Tw, patchi)*Tw.refGrad()
      + patch().deltaCoeffs()*
        (
            thermo.heu(pw, Tw, patchi)
          - thermo.heu(pw, Tw, patch().faceCells())
        );

    mixedFvPatchScalarField::updateCoeffs();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    makePatchTypeField
    (
        fvPatchScalarField,
        mixedUnburntEnthalpyFvPatchScalarField
    );
}
