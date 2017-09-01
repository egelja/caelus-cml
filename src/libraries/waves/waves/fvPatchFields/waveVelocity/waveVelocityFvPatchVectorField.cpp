/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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

#include "waveVelocityFvPatchVectorField.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "Time.hpp"
#include "dimensionedVector.hpp"
#include "volMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

waveVelocityFvPatchVectorField::waveVelocityFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF
)
:
    mixedFvPatchField<vector>(p, iF),
    convexPolyhedral(this->dimensionedInternalField().mesh(), true),
    waveProps_
    (
        waveTheories::waveTheory::New
        (
            this->patch().name(),
            this->dimensionedInternalField().mesh()
        )
    )
{
    this->refValue() = pTraits<vector>::zero;
    this->refGrad() = pTraits<vector>::zero;
    this->valueFraction() = 0.0;

}


waveVelocityFvPatchVectorField::waveVelocityFvPatchVectorField
(
    const waveVelocityFvPatchVectorField& ptf,
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchField<vector>(ptf, p, iF, mapper),
    convexPolyhedral(this->dimensionedInternalField().mesh(), true),
    waveProps_(ptf.waveProps_)
{
}


waveVelocityFvPatchVectorField::waveVelocityFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchField<vector>(p, iF),
    convexPolyhedral(this->dimensionedInternalField().mesh(), true),
    waveProps_
    (
        waveTheories::waveTheory::New
        (
            this->patch().name(),
            this->dimensionedInternalField().mesh()
        )
    )
{
    evaluate();
}


waveVelocityFvPatchVectorField::waveVelocityFvPatchVectorField
(
    const waveVelocityFvPatchVectorField& ptf,
    const DimensionedField<vector, volMesh>& iF
)
:
    mixedFvPatchField<vector>(ptf, iF),
    convexPolyhedral(this->dimensionedInternalField().mesh(), true),
    waveProps_
    (
        waveTheories::waveTheory::New
        (
            this->patch().name(),
            this->dimensionedInternalField().mesh()
        )
    )
{
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void waveVelocityFvPatchVectorField::signedPointToSurfaceDistance
(
    const pointField& pp,
    scalarField& sd
)
{
    forAll (pp, pointi)
    {
        sd[pointi] = signedPointToSurfaceDistance(pp[pointi]);
    }
}


scalar waveVelocityFvPatchVectorField::signedPointToSurfaceDistance
(
    const point& pp
) const
{
    scalar temp = waveProps_->eta(pp, db().time().value() );
    temp += ( waveProps_->returnDir() & pp );
    temp *= -1.0;

    return temp;
}


// Update the coefficients associated with the patch field
void waveVelocityFvPatchVectorField::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    const fvMesh& mesh = this->dimensionedInternalField().mesh();
    const word patchName = this->patch().name();
    const label patchID = mesh.boundaryMesh().findPatchID(patchName);
    const scalarField& magSf( mesh.magSf().boundaryField()[patchID] );

    const label start = patch().patch().start();

    forAll (magSf, facei)
    {
        localFace lf = this->divideFace(facei + start);

        if (lf.isNegFace())
        {
            this->refValue()[facei]
                = waveProps_->U(lf.negCentre(), db().time().value());
        }
        else
        {
            this->refValue()[facei]
                = waveProps_->windVelocity(db().time().value());
        }

        this->refGrad()[facei] = vector::zero;
        this->valueFraction()[facei] = 1.0;
    }

    mixedFvPatchField<vector>::updateCoeffs();
}


// Evaluate the field on the patch
void waveVelocityFvPatchVectorField::evaluate()
{
    if (!this->updated())
    {
        this->updateCoeffs();
    }

    Field<vector>::operator=
    (
        this->valueFraction()*this->refValue()
      +
        (1.0 - this->valueFraction())*
        (
            this->patchInternalField()
          + this->refGrad()/this->patch().deltaCoeffs()
        )
    );

    fvPatchField<vector>::evaluate();
}


// Write
void waveVelocityFvPatchVectorField::write(Ostream& os) const
{
    fvPatchField<vector>::write(os);
    this->refValue().writeEntry("refValue", os);
    this->refGrad().writeEntry("refGradient", os);
    this->valueFraction().writeEntry("valueFraction", os);
    this->writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField(fvPatchVectorField, waveVelocityFvPatchVectorField);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
