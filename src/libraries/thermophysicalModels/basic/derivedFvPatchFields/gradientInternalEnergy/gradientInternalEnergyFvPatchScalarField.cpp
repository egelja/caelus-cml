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

#include "gradientInternalEnergyFvPatchScalarField.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvPatchFieldMapper.hpp"
#include "volFields.hpp"
#include "basicThermo.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::gradientInternalEnergyFvPatchScalarField::
gradientInternalEnergyFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedGradientFvPatchScalarField(p, iF)
{}


CML::gradientInternalEnergyFvPatchScalarField::
gradientInternalEnergyFvPatchScalarField
(
    const gradientInternalEnergyFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedGradientFvPatchScalarField(ptf, p, iF, mapper)
{}


CML::gradientInternalEnergyFvPatchScalarField::
gradientInternalEnergyFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    fixedGradientFvPatchScalarField(p, iF, dict)
{}


CML::gradientInternalEnergyFvPatchScalarField::
gradientInternalEnergyFvPatchScalarField
(
    const gradientInternalEnergyFvPatchScalarField& tppsf
)
:
    fixedGradientFvPatchScalarField(tppsf)
{}


CML::gradientInternalEnergyFvPatchScalarField::
gradientInternalEnergyFvPatchScalarField
(
    const gradientInternalEnergyFvPatchScalarField& tppsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedGradientFvPatchScalarField(tppsf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::gradientInternalEnergyFvPatchScalarField::updateCoeffs()
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

    fvPatchScalarField& Tw =
        const_cast<fvPatchScalarField&>(thermo.T().boundaryField()[patchi]);

    Tw.evaluate();

    gradient() = thermo.Cv(Tw, patchi)*Tw.snGrad()
      + patch().deltaCoeffs()*
        (
            thermo.e(Tw, patchi)
          - thermo.e(Tw, patch().faceCells())
        );

    fixedGradientFvPatchScalarField::updateCoeffs();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    makePatchTypeField
    (
        fvPatchScalarField,
        gradientInternalEnergyFvPatchScalarField
    );
}

// ************************************************************************* //
