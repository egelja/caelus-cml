/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
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

#include "turbulentIntensityKineticEnergyInletFvPatchSymmTensorField.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "volFields.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::turbulentIntensityKineticEnergyInletFvPatchSymmTensorField::
turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
(
    const fvPatch& p,
    const DimensionedField<symmTensor, volMesh>& iF
)
:
    fixedValueFvPatchSymmTensorField(p, iF)
{}


CML::turbulentIntensityKineticEnergyInletFvPatchSymmTensorField::
turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
(
    const turbulentIntensityKineticEnergyInletFvPatchSymmTensorField& ptf,
    const fvPatch& p,
    const DimensionedField<symmTensor, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchSymmTensorField(ptf, p, iF, mapper)
{}


CML::turbulentIntensityKineticEnergyInletFvPatchSymmTensorField::
turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
(
    const fvPatch& p,
    const DimensionedField<symmTensor, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchSymmTensorField(p, iF)
{

    fvPatchSymmTensorField::operator=(symmTensorField("value", dict, p.size()));
}


CML::turbulentIntensityKineticEnergyInletFvPatchSymmTensorField::
turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
(
    const turbulentIntensityKineticEnergyInletFvPatchSymmTensorField& ptf
)
:
    fixedValueFvPatchSymmTensorField(ptf)
{}


CML::turbulentIntensityKineticEnergyInletFvPatchSymmTensorField::
turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
(
    const turbulentIntensityKineticEnergyInletFvPatchSymmTensorField& ptf,
    const DimensionedField<symmTensor, volMesh>& iF
)
:
    fixedValueFvPatchSymmTensorField(ptf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::turbulentIntensityKineticEnergyInletFvPatchSymmTensorField::write
(
    Ostream& os
) const
{
    fvPatchSymmTensorField::write(os);
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    makePatchTypeField
    (
        fvPatchSymmTensorField,
        turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
    );
}

// ************************************************************************* //
