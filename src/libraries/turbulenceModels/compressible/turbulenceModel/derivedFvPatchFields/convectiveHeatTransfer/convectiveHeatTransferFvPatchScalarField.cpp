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

#include "convectiveHeatTransferFvPatchScalarField.hpp"
#include "compressible/turbulenceModel/turbulenceModel.hpp"
#include "fvPatchFieldMapper.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace compressible
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

convectiveHeatTransferFvPatchScalarField::convectiveHeatTransferFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedValueFvPatchScalarField(p, iF),
    L_(1.0)
{}


convectiveHeatTransferFvPatchScalarField::convectiveHeatTransferFvPatchScalarField
(
    const convectiveHeatTransferFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchScalarField(ptf, p, iF, mapper),
    L_(ptf.L_)
{}


convectiveHeatTransferFvPatchScalarField::convectiveHeatTransferFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchScalarField(p, iF, dict),
    L_(readScalar(dict.lookup("L")))
{}


convectiveHeatTransferFvPatchScalarField::convectiveHeatTransferFvPatchScalarField
(
    const convectiveHeatTransferFvPatchScalarField& htcpsf
)
:
    fixedValueFvPatchScalarField(htcpsf),
    L_(htcpsf.L_)
{}


convectiveHeatTransferFvPatchScalarField::convectiveHeatTransferFvPatchScalarField
(
    const convectiveHeatTransferFvPatchScalarField& htcpsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedValueFvPatchScalarField(htcpsf, iF),
    L_(htcpsf.L_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void convectiveHeatTransferFvPatchScalarField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    const label patchI = patch().index();

    const turbulenceModel& turbModel =
        db().lookupObject<turbulenceModel>("turbulenceModel");
    const scalarField alphaEffw(turbModel.alphaEff(patchI));
    const scalarField& muw = turbModel.mu().boundaryField()[patchI];
    const scalarField& rhow = turbModel.rho().boundaryField()[patchI];
    const vectorField& Uc = turbModel.U();
    const vectorField& Uw = turbModel.U().boundaryField()[patchI];
    const scalarField& Tw = turbModel.thermo().T().boundaryField()[patchI];
    const scalarField Cpw(turbModel.thermo().Cp(Tw, patchI));

    const scalarField kappaw(Cpw*alphaEffw);
    const scalarField Pr(muw*Cpw/kappaw);

    scalarField& htc = *this;
    forAll(htc, faceI)
    {
        label faceCellI = patch().faceCells()[faceI];

        scalar Re = rhow[faceI]*mag(Uc[faceCellI] - Uw[faceI])*L_/muw[faceI];

        if (Re < 5.0E+05)
        {
            htc[faceI] = 0.664*sqrt(Re)*cbrt(Pr[faceI])*kappaw[faceI]/L_;
        }
        else
        {
            htc[faceI] = 0.037*pow(Re, 0.8)*cbrt(Pr[faceI])*kappaw[faceI]/L_;
        }
    }
    fixedValueFvPatchScalarField::updateCoeffs();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void convectiveHeatTransferFvPatchScalarField::write(Ostream& os) const
{
    fvPatchField<scalar>::write(os);
    os.writeKeyword("L") << L_ << token::END_STATEMENT << nl;
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField
(
    fvPatchScalarField,
    convectiveHeatTransferFvPatchScalarField
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace compressible
} // End namespace CML

// ************************************************************************* //
