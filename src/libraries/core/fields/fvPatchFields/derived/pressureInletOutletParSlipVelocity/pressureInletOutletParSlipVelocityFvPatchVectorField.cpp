/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "pressureInletOutletParSlipVelocityFvPatchVectorField.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvPatchFieldMapper.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::pressureInletOutletParSlipVelocityFvPatchVectorField::
pressureInletOutletParSlipVelocityFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF
)
:
    mixedFvPatchVectorField(p, iF),
    phiName_("phi"),
    rhoName_("rho")
{
    refValue() = *this;
    refGrad() = vector::zero;
    valueFraction() = 0.0;
}


CML::pressureInletOutletParSlipVelocityFvPatchVectorField::
pressureInletOutletParSlipVelocityFvPatchVectorField
(
    const pressureInletOutletParSlipVelocityFvPatchVectorField& ptf,
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchVectorField(ptf, p, iF, mapper),
    phiName_(ptf.phiName_),
    rhoName_(ptf.rhoName_)
{}


CML::pressureInletOutletParSlipVelocityFvPatchVectorField::
pressureInletOutletParSlipVelocityFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchVectorField(p, iF),
    phiName_(dict.lookupOrDefault<word>("phi", "phi")),
    rhoName_(dict.lookupOrDefault<word>("rho", "rho"))
{
    fvPatchVectorField::operator=(vectorField("value", dict, p.size()));
    refValue() = *this;
    refGrad() = vector::zero;
    valueFraction() = 0.0;
}


CML::pressureInletOutletParSlipVelocityFvPatchVectorField::
pressureInletOutletParSlipVelocityFvPatchVectorField
(
    const pressureInletOutletParSlipVelocityFvPatchVectorField& pivpvf
)
:
    mixedFvPatchVectorField(pivpvf),
    phiName_(pivpvf.phiName_),
    rhoName_(pivpvf.rhoName_)
{}


CML::pressureInletOutletParSlipVelocityFvPatchVectorField::
pressureInletOutletParSlipVelocityFvPatchVectorField
(
    const pressureInletOutletParSlipVelocityFvPatchVectorField& pivpvf,
    const DimensionedField<vector, volMesh>& iF
)
:
    mixedFvPatchVectorField(pivpvf, iF),
    phiName_(pivpvf.phiName_),
    rhoName_(pivpvf.rhoName_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::pressureInletOutletParSlipVelocityFvPatchVectorField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    const surfaceScalarField& phi =
        db().lookupObject<surfaceScalarField>(phiName_);

    const fvsPatchField<scalar>& phip =
        patch().patchField<surfaceScalarField, scalar>(phi);

    tmp<vectorField> n = patch().nf();
    const Field<scalar>& magSf = patch().magSf();

    // Get the tangential component from the internalField (zero-gradient)
    vectorField Ut(patchInternalField());
    Ut -= n()*(Ut & n());

    if (phi.dimensions() == dimVelocity*dimArea)
    {
        refValue() = Ut + n*phip/magSf;
    }
    else if (phi.dimensions() == dimDensity*dimVelocity*dimArea)
    {
        const fvPatchField<scalar>& rhop =
            patch().lookupPatchField<volScalarField, scalar>(rhoName_);

        refValue() = Ut + n*phip/(rhop*magSf);
    }
    else
    {
        FatalErrorInFunction
            << "dimensions of phi are not correct" << nl
            << "    on patch " << this->patch().name()
            << " of field " << this->dimensionedInternalField().name()
            << " in file " << this->dimensionedInternalField().objectPath()
            << exit(FatalError);
    }

    valueFraction() = 1.0 - pos(phip);

    mixedFvPatchVectorField::updateCoeffs();
}


void CML::pressureInletOutletParSlipVelocityFvPatchVectorField::write
(
    Ostream& os
) const
{
    fvPatchVectorField::write(os);
    os.writeKeyword("phi") << phiName_ << token::END_STATEMENT << nl;
    os.writeKeyword("rho") << rhoName_ << token::END_STATEMENT << nl;
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void CML::pressureInletOutletParSlipVelocityFvPatchVectorField::operator=
(
    const fvPatchField<vector>& pvf
)
{
    fvPatchField<vector>::operator=(pvf);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    makePatchTypeField
    (
        fvPatchVectorField,
        pressureInletOutletParSlipVelocityFvPatchVectorField
    );
}

// ************************************************************************* //
