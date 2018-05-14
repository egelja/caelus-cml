/*---------------------------------------------------------------------------*\
Copyright (C) 2015-2016 OpenFOAM Foundation
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

#include "CorrectPhi_.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void CML::correctUphiBCs
(
    volVectorField& U,
    surfaceScalarField& phi
)
{
    const fvMesh& mesh = U.mesh();

    if (mesh.changing())
    {

        forAll(U.boundaryField(), patchi)
        {
            if (U.boundaryField()[patchi].fixesValue())
            {
                U.boundaryField()[patchi].initEvaluate();
            }
        }

        forAll(U.boundaryField(), patchi)
        {
            if (U.boundaryField()[patchi].fixesValue())
            {
                U.boundaryField()[patchi].evaluate();

                phi.boundaryField()[patchi] = 
                    U.boundaryField()[patchi] 
                  & mesh.Sf().boundaryField()[patchi];
            }
        }
    }
}


void CML::correctUphiBCs
(
    const volScalarField& rho,
    volVectorField& U,
    surfaceScalarField& phi
)
{
    const fvMesh& mesh = U.mesh();

    if (mesh.changing())
    {

        forAll(U.boundaryField(), patchi)
        {
            if (U.boundaryField()[patchi].fixesValue())
            {
                U.boundaryField()[patchi].initEvaluate();
            }
        }

        forAll(U.boundaryField(), patchi)
        {
            if (U.boundaryField()[patchi].fixesValue())
            {
                U.boundaryField()[patchi].evaluate();

                phi.boundaryField()[patchi] =
                    rho.boundaryField()[patchi]
                   *(
                        U.boundaryField()[patchi]
                      & mesh.Sf().boundaryField()[patchi]
                    );
            }
        }
    }
}


// ************************************************************************* //
