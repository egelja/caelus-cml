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

#include "mutLowReWFFvPatchScalarField.hpp"
#include "compressible/turbulenceModel/compressibleTurbulenceModel.hpp"
#include "fvPatchFieldMapper.hpp"
#include "volFields.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace compressible
{

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

tmp<scalarField> mutLowReWallFunctionFvPatchScalarField::calcMut() const
{
    return tmp<scalarField>(new scalarField(patch().size(), 0.0));
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

mutLowReWallFunctionFvPatchScalarField::mutLowReWallFunctionFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mutkWallFunctionFvPatchScalarField(p, iF)
{}


mutLowReWallFunctionFvPatchScalarField::mutLowReWallFunctionFvPatchScalarField
(
    const mutLowReWallFunctionFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mutkWallFunctionFvPatchScalarField(ptf, p, iF, mapper)
{}


mutLowReWallFunctionFvPatchScalarField::mutLowReWallFunctionFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    mutkWallFunctionFvPatchScalarField(p, iF, dict)
{}


mutLowReWallFunctionFvPatchScalarField::mutLowReWallFunctionFvPatchScalarField
(
    const mutLowReWallFunctionFvPatchScalarField& mlrwfpsf
)
:
    mutkWallFunctionFvPatchScalarField(mlrwfpsf)
{}


mutLowReWallFunctionFvPatchScalarField::mutLowReWallFunctionFvPatchScalarField
(
    const mutLowReWallFunctionFvPatchScalarField& mlrwfpsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mutkWallFunctionFvPatchScalarField(mlrwfpsf, iF)
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField
(
    fvPatchScalarField,
    mutLowReWallFunctionFvPatchScalarField
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace compressible
} // End namespace CML

// ************************************************************************* //
