/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2010 OpenCFD Ltd.
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

#include "groovyTotalPressureFvPatchScalarField.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvPatchFieldMapper.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::groovyTotalPressureFvPatchScalarField::groovyTotalPressureFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    totalPressureFvPatchScalarField(p, iF),
    p0Expression_("0"),
    driver_(this->patch())
{}


CML::groovyTotalPressureFvPatchScalarField::groovyTotalPressureFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    totalPressureFvPatchScalarField(p, iF,dict),
    p0Expression_(
        dict.lookup("p0Expression"),
        dict
    ),
    driver_(dict,this->patch())
{
}


CML::groovyTotalPressureFvPatchScalarField::groovyTotalPressureFvPatchScalarField
(
    const groovyTotalPressureFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    totalPressureFvPatchScalarField(ptf, p, iF, mapper),
    p0Expression_(ptf.p0Expression_),
    driver_(this->patch(),ptf.driver_)
{}


CML::groovyTotalPressureFvPatchScalarField::groovyTotalPressureFvPatchScalarField
(
    const groovyTotalPressureFvPatchScalarField& tppsf
)
:
    totalPressureFvPatchScalarField(tppsf),
    p0Expression_(tppsf.p0Expression_),
    driver_(this->patch(),tppsf.driver_)
{}


CML::groovyTotalPressureFvPatchScalarField::groovyTotalPressureFvPatchScalarField
(
    const groovyTotalPressureFvPatchScalarField& tppsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    totalPressureFvPatchScalarField(tppsf, iF),
    p0Expression_(tppsf.p0Expression_),
    driver_(this->patch(),tppsf.driver_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::groovyTotalPressureFvPatchScalarField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    driver_.clearVariables();

    p0()=driver_.evaluate<scalar>(this->p0Expression_);

    totalPressureFvPatchScalarField::updateCoeffs();
}

void CML::groovyTotalPressureFvPatchScalarField::write(Ostream& os) const
{
    totalPressureFvPatchScalarField::write(os);

    os.writeKeyword("p0Expression")
        << p0Expression_ << token::END_STATEMENT << nl;

    driver_.writeCommon(os,debug);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    makePatchTypeField
    (
        fvPatchScalarField,
        groovyTotalPressureFvPatchScalarField
    );
}

// ************************************************************************* //
