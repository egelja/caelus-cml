/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2014 Applied CCM
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

#include "alphaContactAngleFvPatchScalarField.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvPatchFieldMapper.hpp"
#include "volMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef windows
// Symbol to force loading at runtime
extern "C"
void twoPhaseInterfacePropertiesLoad()
{}
#endif

namespace CML
{
    defineTypeNameAndDebug(alphaContactAngleFvPatchScalarField, 0);

    template<>
    const char* CML::NamedEnum
    <
        CML::alphaContactAngleFvPatchScalarField::limitControls,
        4
    >::names[] =
    {
        "none",
        "gradient",
        "zeroGradient",
        "alpha"
    };
}


const CML::NamedEnum
<
    CML::alphaContactAngleFvPatchScalarField::limitControls,
    4
> CML::alphaContactAngleFvPatchScalarField::limitControlNames_;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::alphaContactAngleFvPatchScalarField::alphaContactAngleFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedGradientFvPatchScalarField(p, iF),
    limit_(lcZeroGradient)
{}


CML::alphaContactAngleFvPatchScalarField::alphaContactAngleFvPatchScalarField
(
    const alphaContactAngleFvPatchScalarField& acpsf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedGradientFvPatchScalarField(acpsf, p, iF, mapper),
    limit_(acpsf.limit_)
{}


CML::alphaContactAngleFvPatchScalarField::alphaContactAngleFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    fixedGradientFvPatchScalarField(p, iF),
    limit_(limitControlNames_.read(dict.lookup("limit")))
{
    if (dict.found("gradient"))
    {
        gradient() = scalarField("gradient", dict, p.size());
        fixedGradientFvPatchScalarField::updateCoeffs();
        fixedGradientFvPatchScalarField::evaluate();
    }
    else
    {
        fvPatchField<scalar>::operator=(patchInternalField());
        gradient() = 0.0;
    }
}


CML::alphaContactAngleFvPatchScalarField::alphaContactAngleFvPatchScalarField
(
    const alphaContactAngleFvPatchScalarField& acpsf
)
:
    fixedGradientFvPatchScalarField(acpsf),
    limit_(acpsf.limit_)
{}


CML::alphaContactAngleFvPatchScalarField::alphaContactAngleFvPatchScalarField
(
    const alphaContactAngleFvPatchScalarField& acpsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedGradientFvPatchScalarField(acpsf, iF),
    limit_(acpsf.limit_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::alphaContactAngleFvPatchScalarField::evaluate
(
    const Pstream::commsTypes
)
{
    if (limit_ == lcGradient)
    {
        gradient() =
        patch().deltaCoeffs()
       *(
           max(min
           (
               *this + gradient()/patch().deltaCoeffs(),
               scalar(1)), scalar(0)
           ) - *this
       );
    }
    else if (limit_ == lcZeroGradient)
    {
        gradient() = 0.0;
    }

    fixedGradientFvPatchScalarField::evaluate();

    if (limit_ == lcAlpha)
    {
        scalarField::operator=(max(min(*this, scalar(1)), scalar(0)));
    }
}


void CML::alphaContactAngleFvPatchScalarField::write
(
    Ostream& os
) const
{
    fixedGradientFvPatchScalarField::write(os);
    os.writeKeyword("limit")
        << limitControlNames_[limit_] << token::END_STATEMENT << nl;
}


// ************************************************************************* //
