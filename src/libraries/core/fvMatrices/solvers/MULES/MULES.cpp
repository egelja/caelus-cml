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

#include "MULES.hpp"
#include "profiling.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void CML::MULES::explicitSolve
(
    volScalarField& psi,
    const surfaceScalarField& phi,
    surfaceScalarField& phiPsi,
    const scalar psiMax,
    const scalar psiMin
)
{
    addProfiling(solve, "MULES::explicitSolve");

    explicitSolve
    (
        geometricOneField(),
        psi,
        phi,
        phiPsi,
        zeroField(), zeroField(),
        psiMax, psiMin
    );
}


void CML::MULES::limitSum(UPtrList<scalarField>& phiPsiCorrs)
{
    forAll(phiPsiCorrs[0], facei)
    {
        scalar sumPos = 0;
        scalar sumNeg = 0;

        for (int phasei=0; phasei<phiPsiCorrs.size(); phasei++)
        {
            if (phiPsiCorrs[phasei][facei] > 0)
            {
                sumPos += phiPsiCorrs[phasei][facei];
            }
            else
            {
                sumNeg += phiPsiCorrs[phasei][facei];
            }
        }

        scalar sum = sumPos + sumNeg;

        if (sum > 0 && sumPos > VSMALL)
        {
            scalar lambda = -sumNeg/sumPos;

            for (int phasei=0; phasei<phiPsiCorrs.size(); phasei++)
            {
                if (phiPsiCorrs[phasei][facei] > 0)
                {
                    phiPsiCorrs[phasei][facei] *= lambda;
                }
            }
        }
        else if (sum < 0 && sumNeg < -VSMALL)
        {
            scalar lambda = -sumPos/sumNeg;

            for (int phasei=0; phasei<phiPsiCorrs.size(); phasei++)
            {
                if (phiPsiCorrs[phasei][facei] < 0)
                {
                    phiPsiCorrs[phasei][facei] *= lambda;
                }
            }
        }
    }
}


// ************************************************************************* //
