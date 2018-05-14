/*---------------------------------------------------------------------------*\
Copyright (C) 2016 OpenFOAM Foundation
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

#include "constrainHbyA.hpp"
#include "volFields.hpp"
#include "fixedFluxExtrapolatedPressureFvPatchScalarField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::tmp<CML::volVectorField> CML::constrainHbyA
(
    const tmp<volVectorField>& tHbyA,
    const volVectorField& U,
    const volScalarField& p
)
{
    tmp<volVectorField> tHbyANew;

    if (tHbyA.isTmp())
    {
        tHbyANew = tHbyA;
        tHbyANew().rename("HbyA");
    }
    else
    {
        tHbyANew = new volVectorField("HbyA", tHbyA);
    }

    volVectorField& HbyA = tHbyANew();
//    volVectorField::boundaryField& HbyAbf = HbyA.boundaryField();

    forAll(U.boundaryField(), patchi)
    {
        if
        (
           !U.boundaryField()[patchi].assignable()
        && !isA<fixedFluxExtrapolatedPressureFvPatchScalarField>
            (
                p.boundaryField()[patchi]
            )
        )
        {
            HbyA.boundaryField()[patchi] = U.boundaryField()[patchi];
        }
    }

    return tHbyANew;
}


// ************************************************************************* //
