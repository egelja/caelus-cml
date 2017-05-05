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

#include "movingWallVelocityFvPatchVectorField.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "fvcMeshPhi.hpp"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::movingWallVelocityFvPatchVectorField::
movingWallVelocityFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchVectorField(p, iF),
    UName_("U")
{}


CML::movingWallVelocityFvPatchVectorField::
movingWallVelocityFvPatchVectorField
(
    const movingWallVelocityFvPatchVectorField& ptf,
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchVectorField(ptf, p, iF, mapper),
    UName_(ptf.UName_)
{}


CML::movingWallVelocityFvPatchVectorField::
movingWallVelocityFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchVectorField(p, iF),
    UName_(dict.lookupOrDefault<word>("U", "U"))
{
    fvPatchVectorField::operator=(vectorField("value", dict, p.size()));
}


CML::movingWallVelocityFvPatchVectorField::
movingWallVelocityFvPatchVectorField
(
    const movingWallVelocityFvPatchVectorField& mwvpvf
)
:
    fixedValueFvPatchVectorField(mwvpvf),
    UName_(mwvpvf.UName_)
{}


CML::movingWallVelocityFvPatchVectorField::
movingWallVelocityFvPatchVectorField
(
    const movingWallVelocityFvPatchVectorField& mwvpvf,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchVectorField(mwvpvf, iF),
    UName_(mwvpvf.UName_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::movingWallVelocityFvPatchVectorField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    const fvPatch& p = patch();
    const polyPatch& pp = p.patch();
    const fvMesh& mesh = dimensionedInternalField().mesh();
    const pointField& oldPoints = mesh.oldPoints();

    vectorField oldFc(pp.size());

    forAll(oldFc, i)
    {
        oldFc[i] = pp[i].centre(oldPoints);
    }

    const vectorField Up((pp.faceCentres() - oldFc)/mesh.time().deltaTValue());

    const volVectorField& U = db().lookupObject<volVectorField>(UName_);
    scalarField phip
    (
        p.patchField<surfaceScalarField, scalar>(fvc::meshPhi(U))
    );

    const vectorField n(p.nf());
    const scalarField& magSf = p.magSf();
    tmp<scalarField> Un = phip/(magSf + VSMALL);


    vectorField::operator=(Up + n*(Un - (n & Up)));

    fixedValueFvPatchVectorField::updateCoeffs();
}


void CML::movingWallVelocityFvPatchVectorField::write(Ostream& os) const
{
    fvPatchVectorField::write(os);
    writeEntryIfDifferent<word>(os, "U", "U", UName_);
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    makePatchTypeField
    (
        fvPatchVectorField,
        movingWallVelocityFvPatchVectorField
    );
}

// ************************************************************************* //
