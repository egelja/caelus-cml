/*---------------------------------------------------------------------------*\
 Copyright (C) 2017-2019 OpenFOAM Foundation
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

#include "flowRateOutletVelocityFvPatchVectorField.hpp"
#include "volFields.hpp"
#include "one.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvPatchFieldMapper.hpp"
#include "surfaceFields.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::flowRateOutletVelocityFvPatchVectorField::
flowRateOutletVelocityFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchField<vector>(p, iF),
    flowRate_(),
    volumetric_(false),
    rhoName_("rho"),
    rhoOutlet_(0.0)
{}


CML::flowRateOutletVelocityFvPatchVectorField::
flowRateOutletVelocityFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchField<vector>(p, iF),
    rhoOutlet_(dict.lookupOrDefault<scalar>("rhoOutlet", -VGREAT))
{
    if (dict.found("volumetricFlowRate"))
    {
        volumetric_ = true;
        flowRate_ = DataEntry<scalar>::New("volumetricFlowRate", dict);
        rhoName_ = "rho";
    }
    else if (dict.found("massFlowRate"))
    {
        volumetric_ = false;
        flowRate_ = DataEntry<scalar>::New("massFlowRate", dict);
        rhoName_ = word(dict.lookupOrDefault<word>("rho", "rho"));
    }
    else
    {
        FatalIOErrorInFunction
        (
            dict
        )   << "Please supply either 'volumetricFlowRate' or"
            << " 'massFlowRate' and 'rho'" << exit(FatalIOError);
    }

    // Value field require if mass based
    if (dict.found("value"))
    {
        fvPatchField<vector>::operator=
        (
            vectorField("value", dict, p.size())
        );
    }
    else
    {
        evaluate(Pstream::commsTypes::blocking);
    }
}


CML::flowRateOutletVelocityFvPatchVectorField::
flowRateOutletVelocityFvPatchVectorField
(
    const flowRateOutletVelocityFvPatchVectorField& ptf,
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchField<vector>(ptf, p, iF, mapper),
    flowRate_(ptf.flowRate_().clone().ptr()),
    volumetric_(ptf.volumetric_),
    rhoName_(ptf.rhoName_),
    rhoOutlet_(ptf.rhoOutlet_)
{}


CML::flowRateOutletVelocityFvPatchVectorField::
flowRateOutletVelocityFvPatchVectorField
(
    const flowRateOutletVelocityFvPatchVectorField& ptf
)
:
    fixedValueFvPatchField<vector>(ptf),
    flowRate_(ptf.flowRate_().clone().ptr()),
    volumetric_(ptf.volumetric_),
    rhoName_(ptf.rhoName_),
    rhoOutlet_(ptf.rhoOutlet_)
{}


CML::flowRateOutletVelocityFvPatchVectorField::
flowRateOutletVelocityFvPatchVectorField
(
    const flowRateOutletVelocityFvPatchVectorField& ptf,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchField<vector>(ptf, iF),
    flowRate_(ptf.flowRate_().clone().ptr()),
    volumetric_(ptf.volumetric_),
    rhoName_(ptf.rhoName_),
    rhoOutlet_(ptf.rhoOutlet_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class RhoType>
void CML::flowRateOutletVelocityFvPatchVectorField::updateValues
(
    const RhoType& rho
)
{
    const scalar t = db().time().timeOutputValue();

    const vectorField n(patch().nf());

    // Extrapolate patch velocity
    vectorField Up(this->patchInternalField());

    // Patch normal extrapolated velocity
    scalarField nUp(n & Up);

    // Remove the normal component of the extrapolate patch velocity
    Up -= nUp*n;

    // Remove any reverse flow
    nUp = max(nUp, 0.0);

    const scalar flowRate = flowRate_->value(t);
    const scalar estimatedFlowRate = gSum(rho*(this->patch().magSf()*nUp));

    if (estimatedFlowRate/flowRate > 0.5)
    {
        nUp *= (mag(flowRate)/mag(estimatedFlowRate));
    }
    else
    {
        nUp += ((flowRate - estimatedFlowRate)/gSum(rho*patch().magSf()));
    }

    // Add the corrected normal component of velocity to the patch velocity
    Up += nUp*n;

    // Correct the patch velocity
    this->operator==(Up);
}


void CML::flowRateOutletVelocityFvPatchVectorField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    if (volumetric_ || rhoName_ == "none")
    {
        updateValues(one());
    }
    else
    {
        // Mass flow-rate
        if (db().foundObject<volScalarField>(rhoName_))
        {
            const fvPatchField<scalar>& rhop =
                patch().lookupPatchField<volScalarField, scalar>(rhoName_);

            updateValues(rhop);
        }
        else
        {
            // Use constant density
            if (rhoOutlet_ < 0)
            {
                FatalErrorInFunction
                    << "Did not find registered density field " << rhoName_
                    << " and no constant density 'rhoOutlet' specified"
                    << exit(FatalError);
            }

            updateValues(rhoOutlet_);
        }
    }

    fixedValueFvPatchVectorField::updateCoeffs();
}


void CML::flowRateOutletVelocityFvPatchVectorField::write(Ostream& os) const
{
    fvPatchField<vector>::write(os);
    flowRate_->writeData(os);
    if (!volumetric_)
    {
        writeEntryIfDifferent<word>(os, "rho", "rho", rhoName_);
        writeEntryIfDifferent<scalar>(os, "rhoOutlet", -VGREAT, rhoOutlet_);
    }
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
   makePatchTypeField
   (
       fvPatchVectorField,
       flowRateOutletVelocityFvPatchVectorField
   );
}


// ************************************************************************* //
