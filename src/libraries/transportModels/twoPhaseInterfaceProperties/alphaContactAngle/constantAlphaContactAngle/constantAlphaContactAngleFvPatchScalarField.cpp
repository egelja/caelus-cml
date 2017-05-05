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

#include "constantAlphaContactAngleFvPatchScalarField.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "volMesh.hpp"
#include "fvPatchFieldMapper.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::constantAlphaContactAngleFvPatchScalarField::
constantAlphaContactAngleFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    alphaContactAngleFvPatchScalarField(p, iF),
    theta0_(0.0)
{}


CML::constantAlphaContactAngleFvPatchScalarField::
constantAlphaContactAngleFvPatchScalarField
(
    const constantAlphaContactAngleFvPatchScalarField& gcpsf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    alphaContactAngleFvPatchScalarField(gcpsf, p, iF, mapper),
    theta0_(gcpsf.theta0_)
{}


CML::constantAlphaContactAngleFvPatchScalarField::
constantAlphaContactAngleFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    alphaContactAngleFvPatchScalarField(p, iF, dict),
    theta0_(readScalar(dict.lookup("theta0")))
{
    evaluate();
}


CML::constantAlphaContactAngleFvPatchScalarField::
constantAlphaContactAngleFvPatchScalarField
(
    const constantAlphaContactAngleFvPatchScalarField& gcpsf
)
:
    alphaContactAngleFvPatchScalarField(gcpsf),
    theta0_(gcpsf.theta0_)
{}


CML::constantAlphaContactAngleFvPatchScalarField::
constantAlphaContactAngleFvPatchScalarField
(
    const constantAlphaContactAngleFvPatchScalarField& gcpsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    alphaContactAngleFvPatchScalarField(gcpsf, iF),
    theta0_(gcpsf.theta0_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::tmp<CML::scalarField>
CML::constantAlphaContactAngleFvPatchScalarField::theta
(
    const fvPatchVectorField&,
    const fvsPatchVectorField&
) const
{
    return tmp<scalarField>(new scalarField(size(), theta0_));
}


void CML::constantAlphaContactAngleFvPatchScalarField::write
(
    Ostream& os
) const
{
    alphaContactAngleFvPatchScalarField::write(os);
    os.writeKeyword("theta0") << theta0_ << token::END_STATEMENT << nl;
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    makePatchTypeField
    (
        fvPatchScalarField,
        constantAlphaContactAngleFvPatchScalarField
    );
}

// ************************************************************************* //
