/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

\*---------------------------------------------------------------------------*/

#include "fixedShearStressFvPatchVectorField.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvPatchFieldMapper.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "incompressible/turbulenceModel/turbulenceModel.hpp"
#include "compressible/turbulenceModel/compressibleTurbulenceModel.hpp"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

fixedShearStressFvPatchVectorField::fixedShearStressFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchVectorField(p, iF),
    phiName_("phi"),
    rhoName_("rho"),
    tau0_(vector::zero)
{}


fixedShearStressFvPatchVectorField::fixedShearStressFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchVectorField(p, iF),
    phiName_(dict.lookupOrDefault<word>("phi", "phi")),
    rhoName_(dict.lookupOrDefault<word>("rho", "rho")),
    tau0_(dict.lookupOrDefault<vector>("tau", vector::zero))
{
    fvPatchField<vector>::operator=(patchInternalField());
}


fixedShearStressFvPatchVectorField::fixedShearStressFvPatchVectorField
(
    const fixedShearStressFvPatchVectorField& ptf,
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchVectorField(ptf, p, iF, mapper),
    phiName_(ptf.phiName_),
    rhoName_(ptf.rhoName_),
    tau0_(ptf.tau0_)
{}


fixedShearStressFvPatchVectorField::fixedShearStressFvPatchVectorField
(
    const fixedShearStressFvPatchVectorField& ptf
)
:
    fixedValueFvPatchVectorField(ptf),
    phiName_(ptf.phiName_),
    rhoName_(ptf.rhoName_),
    tau0_(ptf.tau0_)
{}


fixedShearStressFvPatchVectorField::fixedShearStressFvPatchVectorField
(
    const fixedShearStressFvPatchVectorField& ptf,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchVectorField(ptf, iF),
    phiName_(ptf.phiName_),
    rhoName_(ptf.rhoName_),
    tau0_(ptf.tau0_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void fixedShearStressFvPatchVectorField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    const label patchI = patch().index();

    const surfaceScalarField& phi =
        db().lookupObject<surfaceScalarField>(phiName_);

    scalarField nuEff;
    if (phi.dimensions() == dimVelocity*dimArea)
    {
        const incompressible::turbulenceModel& turbModel =
            db().lookupObject<incompressible::turbulenceModel>
            (
                "turbulenceModel"
            );

        nuEff = turbModel.nuEff()()[patchI];
    }
    else if (phi.dimensions() == dimDensity*dimVelocity*dimArea)
    {
        const compressible::turbulenceModel& turbModel =
            db().lookupObject<compressible::turbulenceModel>
            (
                "turbulenceModel"
            );

        const fvPatchField<scalar>& rhop =
            patch().lookupPatchField<volScalarField, scalar>(rhoName_);

        nuEff = turbModel.muEff()()[patchI]/rhop;
    }
    else
    {
        FatalErrorInFunction
            << "dimensions of phi are not correct"
            << "\n    on patch " << this->patch().name()
            << " of field " << this->dimensionedInternalField().name()
            << " in file " << this->dimensionedInternalField().objectPath()
            << exit(FatalError);
    }

    const vectorField Uc(patchInternalField());

    vector tauHat = tau0_/(mag(tau0_) + ROOTVSMALL);

    const scalarField& ry = patch().deltaCoeffs();

    operator==(tauHat*(tauHat & (tau0_*(1.0/(ry*nuEff)) + Uc)));

    fixedValueFvPatchVectorField::updateCoeffs();
}


void fixedShearStressFvPatchVectorField::write(Ostream& os) const
{
    fixedValueFvPatchVectorField::write(os);
    writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
    writeEntryIfDifferent<word>(os, "rho", "rho", rhoName_);
    os.writeKeyword("tau") << tau0_ << token::END_STATEMENT << nl;
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField
(
    fvPatchVectorField,
    fixedShearStressFvPatchVectorField
);

} // End namespace CML

// ************************************************************************* //
