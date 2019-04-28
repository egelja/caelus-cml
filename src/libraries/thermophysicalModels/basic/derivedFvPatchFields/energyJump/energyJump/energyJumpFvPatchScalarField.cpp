/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2018 OpenFOAM Foundation
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

#include "addToRunTimeSelectionTable.hpp"
#include "energyJumpFvPatchScalarField.hpp"
#include "fixedJumpFvPatchFields.hpp"
#include "basicThermo.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::energyJumpFvPatchScalarField::energyJumpFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedJumpFvPatchField<scalar>(p, iF)
{}


CML::energyJumpFvPatchScalarField::energyJumpFvPatchScalarField
(
    const energyJumpFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedJumpFvPatchField<scalar>(ptf, p, iF, mapper)
{}


CML::energyJumpFvPatchScalarField::energyJumpFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    fixedJumpFvPatchField<scalar>(p, iF)
{
    if (dict.found("value"))
    {
        fvPatchScalarField::operator=
        (
            scalarField("value", dict, p.size())
        );
    }
    else
    {
        evaluate(Pstream::blocking);
    }
}


CML::energyJumpFvPatchScalarField::energyJumpFvPatchScalarField
(
    const energyJumpFvPatchScalarField& ptf
)
:
    fixedJumpFvPatchField<scalar>(ptf)
{}


CML::energyJumpFvPatchScalarField::energyJumpFvPatchScalarField
(
    const energyJumpFvPatchScalarField& ptf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedJumpFvPatchField<scalar>(ptf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::energyJumpFvPatchScalarField::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    if (this->cyclicPatch().owner())
    {
        const basicThermo& thermo = basicThermo::lookupThermo(*this);
        label patchID = patch().index();

        const scalarField& pp = thermo.p().boundaryField()[patchID];
        const fixedJumpFvPatchScalarField& TbPatch =
            refCast<const fixedJumpFvPatchScalarField>
            (
                thermo.T().boundaryField()[patchID]
            );

        fixedJumpFvPatchScalarField& Tbp =
            const_cast<fixedJumpFvPatchScalarField&>(TbPatch);

        // force update of jump
        Tbp.updateCoeffs();

        const labelUList& faceCells = this->patch().faceCells();

        jump_ = thermo.he(pp, Tbp.jump(), faceCells);
    }

    fixedJumpFvPatchField<scalar>::updateCoeffs();
}


void CML::energyJumpFvPatchScalarField::write(Ostream& os) const
{
    fixedJumpFvPatchField<scalar>::write(os);
    this->writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
   makePatchTypeField
   (
       fvPatchScalarField,
       energyJumpFvPatchScalarField
   );
}
