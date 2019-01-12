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

#include "totalFlowRateAdvectiveDiffusiveFvPatchScalarField.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvPatchFieldMapper.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "IOobjectList.hpp"
#include "compressibleTurbulenceModel.hpp"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //
CML::totalFlowRateAdvectiveDiffusiveFvPatchScalarField::
totalFlowRateAdvectiveDiffusiveFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchField<scalar>(p, iF),
    phiName_("phi"),
    rhoName_("none")
{
    refValue() = 0.0;
    refGrad() = 0.0;
    valueFraction() = 0.0;
}


CML::totalFlowRateAdvectiveDiffusiveFvPatchScalarField::
totalFlowRateAdvectiveDiffusiveFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchField<scalar>(p, iF),
    phiName_(dict.lookupOrDefault<word>("phi", "phi")),
    rhoName_(dict.lookupOrDefault<word>("rho", "none"))
{

    refValue() = 1.0;
    refGrad() = 0.0;
    valueFraction() = 0.0;

    if (dict.found("value"))
    {
        fvPatchField<scalar>::operator=
        (
            Field<scalar>("value", dict, p.size())
        );
    }
    else
    {
        fvPatchField<scalar>::operator=(refValue());
    }
}

CML::totalFlowRateAdvectiveDiffusiveFvPatchScalarField::
totalFlowRateAdvectiveDiffusiveFvPatchScalarField
(
    const totalFlowRateAdvectiveDiffusiveFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchField<scalar>(ptf, p, iF, mapper),
    phiName_(ptf.phiName_),
    rhoName_(ptf.rhoName_)
{}


CML::totalFlowRateAdvectiveDiffusiveFvPatchScalarField::
totalFlowRateAdvectiveDiffusiveFvPatchScalarField
(
    const totalFlowRateAdvectiveDiffusiveFvPatchScalarField& tppsf
)
:
    mixedFvPatchField<scalar>(tppsf),
    phiName_(tppsf.phiName_),
    rhoName_(tppsf.rhoName_)
{}

CML::totalFlowRateAdvectiveDiffusiveFvPatchScalarField::
totalFlowRateAdvectiveDiffusiveFvPatchScalarField
(
    const totalFlowRateAdvectiveDiffusiveFvPatchScalarField& tppsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchField<scalar>(tppsf, iF),
    phiName_(tppsf.phiName_),
    rhoName_(tppsf.rhoName_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::totalFlowRateAdvectiveDiffusiveFvPatchScalarField::autoMap
(
    const fvPatchFieldMapper& m
)
{
    scalarField::autoMap(m);
}


void CML::totalFlowRateAdvectiveDiffusiveFvPatchScalarField::rmap
(
    const fvPatchScalarField& ptf,
    const labelList& addr
)
{
    mixedFvPatchField<scalar>::rmap(ptf, addr);
}

void CML::totalFlowRateAdvectiveDiffusiveFvPatchScalarField::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    const label patchI = patch().index();

    const compressible::turbulenceModel& turbulence =
        db().lookupObject<compressible::turbulenceModel>
        (
            "turbulenceModel"
        );

    const fvsPatchField<scalar>& phip =
        patch().lookupPatchField<surfaceScalarField, scalar>(phiName_);

    const scalarField alphap(turbulence.alphaEff(patchI));

    refValue() = 1.0;
    refGrad() = 0.0;

    valueFraction() =
        1.0
        /
        (
            1.0 +
            alphap*patch().deltaCoeffs()*patch().magSf()/max(mag(phip), SMALL)
        );

    mixedFvPatchField<scalar>::updateCoeffs();

    if (debug)
    {
        scalar phi = gSum(-phip*(*this));

        Info<< patch().boundaryMesh().mesh().name() << ':'
            << patch().name() << ':'
            << this->dimensionedInternalField().name() << " :"
            << " mass flux[Kg/s]:" << phi
            << endl;
    }
}


void CML::totalFlowRateAdvectiveDiffusiveFvPatchScalarField::
write(Ostream& os) const
{
    fvPatchField<scalar>::write(os);
    os.writeKeyword("phi") << phiName_ << token::END_STATEMENT << nl;
    os.writeKeyword("rho") << rhoName_ << token::END_STATEMENT << nl;
    this->writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    makePatchTypeField
    (
        fvPatchScalarField,
        totalFlowRateAdvectiveDiffusiveFvPatchScalarField
    );

}

// ************************************************************************* //
