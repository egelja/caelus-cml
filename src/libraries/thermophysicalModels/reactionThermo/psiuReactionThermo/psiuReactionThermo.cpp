/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "psiuReactionThermo.hpp"
#include "fvMesh.hpp"
#include "zeroGradientFvPatchFields.hpp"
#include "fixedUnburntEnthalpyFvPatchScalarField.hpp"
#include "gradientUnburntEnthalpyFvPatchScalarField.hpp"
#include "mixedUnburntEnthalpyFvPatchScalarField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */

defineTypeNameAndDebug(psiuReactionThermo, 0);
defineRunTimeSelectionTable(psiuReactionThermo, fvMesh);

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

wordList psiuReactionThermo::heuBoundaryTypes()
{
    const volScalarField::GeometricBoundaryField& tbf =
        this->Tu().boundaryField();

    wordList hbt = tbf.types();

    forAll(tbf, patchi)
    {
        if (isA<fixedValueFvPatchScalarField>(tbf[patchi]))
        {
            hbt[patchi] = fixedUnburntEnthalpyFvPatchScalarField::typeName;
        }
        else if
        (
            isA<zeroGradientFvPatchScalarField>(tbf[patchi])
         || isA<fixedGradientFvPatchScalarField>(tbf[patchi])
        )
        {
            hbt[patchi] = gradientUnburntEnthalpyFvPatchScalarField::typeName;
        }
        else if (isA<mixedFvPatchScalarField>(tbf[patchi]))
        {
            hbt[patchi] = mixedUnburntEnthalpyFvPatchScalarField::typeName;
        }
    }

    return hbt;
}

void psiuReactionThermo::heuBoundaryCorrection(volScalarField& heu)
{
    volScalarField::GeometricBoundaryField& hbf = heu.boundaryField();

    forAll(hbf, patchi)
    {
        if
        (
            isA<gradientUnburntEnthalpyFvPatchScalarField>(hbf[patchi])
        )
        {
            refCast<gradientUnburntEnthalpyFvPatchScalarField>(hbf[patchi])
                .gradient() = hbf[patchi].fvPatchField::snGrad();
        }
        else if
        (
            isA<mixedUnburntEnthalpyFvPatchScalarField>(hbf[patchi])
        )
        {
            refCast<mixedUnburntEnthalpyFvPatchScalarField>(hbf[patchi])
                .refGrad() = hbf[patchi].fvPatchField::snGrad();
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

psiuReactionThermo::psiuReactionThermo
(
    const fvMesh& mesh,
    const word& phaseName
)
:
    psiReactionThermo(mesh, phaseName)
{}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::psiuReactionThermo> CML::psiuReactionThermo::New
(
    const fvMesh& mesh,
    const word& phaseName
)
{
    return basicThermo::New<psiuReactionThermo>(mesh, phaseName);
}


} // End namespace CML
