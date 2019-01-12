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

#include "gradientUnburntEnthalpyFvPatchScalarField.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvPatchFieldMapper.hpp"
#include "volFields.hpp"
#include "psiuReactionThermo.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::gradientUnburntEnthalpyFvPatchScalarField::
gradientUnburntEnthalpyFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedGradientFvPatchScalarField(p, iF)
{}


CML::gradientUnburntEnthalpyFvPatchScalarField::
gradientUnburntEnthalpyFvPatchScalarField
(
    const gradientUnburntEnthalpyFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedGradientFvPatchScalarField(ptf, p, iF, mapper)
{}


CML::gradientUnburntEnthalpyFvPatchScalarField::
gradientUnburntEnthalpyFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    fixedGradientFvPatchScalarField(p, iF, dict)
{}


CML::gradientUnburntEnthalpyFvPatchScalarField::
gradientUnburntEnthalpyFvPatchScalarField
(
    const gradientUnburntEnthalpyFvPatchScalarField& tppsf
)
:
    fixedGradientFvPatchScalarField(tppsf)
{}


CML::gradientUnburntEnthalpyFvPatchScalarField::
gradientUnburntEnthalpyFvPatchScalarField
(
    const gradientUnburntEnthalpyFvPatchScalarField& tppsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedGradientFvPatchScalarField(tppsf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::gradientUnburntEnthalpyFvPatchScalarField::updateCoeffs()
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
    fvPatchScalarField& Tw =
        const_cast<fvPatchScalarField&>(thermo.Tu().boundaryField()[patchi]);

    Tw.evaluate();

    gradient() = thermo.Cp(pw, Tw, patchi)*Tw.snGrad()
      + patch().deltaCoeffs()*
        (
            thermo.heu(pw, Tw, patchi)
          - thermo.heu(pw, Tw, patch().faceCells())
        );

    fixedGradientFvPatchScalarField::updateCoeffs();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    makePatchTypeField
    (
        fvPatchScalarField,
        gradientUnburntEnthalpyFvPatchScalarField
    );
}
