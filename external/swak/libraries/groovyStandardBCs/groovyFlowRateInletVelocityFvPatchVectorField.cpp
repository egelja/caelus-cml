/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 2006-2008 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

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

Contributors/Copyright:
    2011, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "groovyFlowRateInletVelocityFvPatchVectorField.hpp"
#include "volFields.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvPatchFieldMapper.hpp"
#include "surfaceFields.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::
groovyFlowRateInletVelocityFvPatchVectorField::
groovyFlowRateInletVelocityFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchField<vector>(p, iF),
    phiName_("phi"),
    rhoName_("rho"),
    flowRateExpression_("0"),
    driver_(this->patch())
{}


CML::
groovyFlowRateInletVelocityFvPatchVectorField::
groovyFlowRateInletVelocityFvPatchVectorField
(
    const groovyFlowRateInletVelocityFvPatchVectorField& ptf,
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchField<vector>(ptf, p, iF, mapper),
    phiName_(ptf.phiName_),
    rhoName_(ptf.rhoName_),
    flowRateExpression_(ptf.flowRateExpression_),
    driver_(this->patch(),ptf.driver_)
{}


CML::
groovyFlowRateInletVelocityFvPatchVectorField::
groovyFlowRateInletVelocityFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchField<vector>(p, iF, dict),
    phiName_(dict.lookupOrDefault<word>("phi", "phi")),
    rhoName_(dict.lookupOrDefault<word>("rho", "rho")),
    flowRateExpression_(
        dict.lookup("flowRateExpression"),
        dict
    ),
    driver_(dict,this->patch())
{
    WarningInFunction
        << "Starting with OF 2.1 this boundary condition is deprecated. Use the"
        << " regular flowRateInletVelocityFvPatch with the swak-DataEntry"
        << endl
        << endl;
}


CML::
groovyFlowRateInletVelocityFvPatchVectorField::
groovyFlowRateInletVelocityFvPatchVectorField
(
    const groovyFlowRateInletVelocityFvPatchVectorField& ptf
)
:
    fixedValueFvPatchField<vector>(ptf),
    phiName_(ptf.phiName_),
    rhoName_(ptf.rhoName_),
    flowRateExpression_(ptf.flowRateExpression_),
    driver_(this->patch(),ptf.driver_)
{}


CML::
groovyFlowRateInletVelocityFvPatchVectorField::
groovyFlowRateInletVelocityFvPatchVectorField
(
    const groovyFlowRateInletVelocityFvPatchVectorField& ptf,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchField<vector>(ptf, iF),
    phiName_(ptf.phiName_),
    rhoName_(ptf.rhoName_),
    flowRateExpression_(ptf.flowRateExpression_),
    driver_(this->patch(),ptf.driver_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::groovyFlowRateInletVelocityFvPatchVectorField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    driver_.clearVariables();

    const scalar flowRate=driver_.evaluateUniform<scalar>(this->flowRateExpression_);

    // Borrowed from the regular flowRateInletVelocityFvPatchVectorField
    const scalar avgU = -flowRate/gSum(patch().magSf());

    tmp<vectorField> n = patch().nf();

    const surfaceScalarField& phi =
        db().lookupObject<surfaceScalarField>(phiName_);

    if (phi.dimensions() == dimVelocity*dimArea)
    {
        // volumetric flow-rate
        operator==(n*avgU);
    }
    else if (phi.dimensions() == dimDensity*dimVelocity*dimArea)
    {
        const fvPatchField<scalar>& rhop =
            patch().lookupPatchField<volScalarField, scalar>(rhoName_);

        // mass flow-rate
        operator==(n*avgU/rhop);
    }
    else
    {
        FatalErrorInFunction
            << "dimensions of " << phiName_ << " are incorrect" << nl
            << "    on patch " << this->patch().name()
            << " of field " << this->dimensionedInternalField().name()
            << " in file " << this->dimensionedInternalField().objectPath()
            << nl << exit(FatalError);
    }

    fixedValueFvPatchField<vector>::updateCoeffs();
}


void CML::groovyFlowRateInletVelocityFvPatchVectorField::write(Ostream& os) const
{
    fixedValueFvPatchField<vector>::write(os);

    os.writeKeyword("flowRateExpression")
        << flowRateExpression_ << token::END_STATEMENT << nl;

    driver_.writeCommon(os,debug);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
   makePatchTypeField
   (
       fvPatchVectorField,
       groovyFlowRateInletVelocityFvPatchVectorField
   );
}


// ************************************************************************* //
