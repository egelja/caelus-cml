/*---------------------------------------------------------------------------*\
Copyright (C) 2015 Applied CCM
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

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/
#include "roeFDSFlux.hpp"

void CML::roeFlux::evaluateFlux
(
    scalar& rhoFlux,
    vector& rhoUFlux,
    scalar& rhoEFlux,
    scalar const pLeft,
    scalar const pRight,
    vector const ULeft,
    vector const URight,
    scalar const TLeft,
    scalar const TRight,
    scalar const RLeft,
    scalar const RRight,
    scalar const CvLeft,
    scalar const CvRight,
    vector const Sf,
    scalar const magSf,
    vector const dotX
) const
{
    scalar const rhoLeft  = pLeft/(RLeft*TLeft);
    scalar const rhoRight = pRight/(RRight*TRight);

    scalar const eLeft  = CvLeft*TLeft+0.5*magSqr(ULeft);
    scalar const eRight = CvRight*TRight+0.5*magSqr(URight);

    scalar const kappaLeft  = (CvLeft+RLeft)/CvLeft;
    scalar const kappaRight = (CvRight+RRight)/CvRight;

    vector const normalVector = Sf/magSf;

    scalar const contrVLeft  = (ULeft  & normalVector);
    scalar const contrVRight = (URight & normalVector);

    scalar const hLeft  = eLeft + pLeft/rhoLeft;
    scalar const hRight = eRight + pRight/rhoRight;

    scalar const rhoTilda = sqrt(max(rhoLeft*rhoRight,0.0));

    scalar const rhoLeftSqrt  = sqrt(max(rhoLeft,0.0));
    scalar const rhoRightSqrt = sqrt(max(rhoRight,0.0));

    scalar const wLeft = rhoLeftSqrt/(rhoLeftSqrt + rhoRightSqrt);
    scalar const wRight = 1 - wLeft;

    vector const UTilda = ULeft*wLeft + URight*wRight;
    scalar const hTilda = hLeft*wLeft + hRight*wRight;
    scalar const qTildaSquare = magSqr(UTilda);
    scalar const kappaTilda = kappaLeft*wLeft + kappaRight*wRight;

    scalar const cTilda =
        sqrt(max((kappaTilda - 1)*(hTilda - 0.5*qTildaSquare),0.0));

    scalar const contrVTilda = (UTilda & normalVector) ;

    scalar const deltaP = pRight - pLeft;
    scalar const deltaRho = rhoRight - rhoLeft;
    vector const deltaU = URight - ULeft;
    scalar const deltaContrV = (deltaU & normalVector);

    scalar const r1 =
        (deltaP - rhoTilda*cTilda*deltaContrV)/(2.0*sqr(cTilda));
    scalar const r2 = deltaRho - deltaP/sqr(cTilda);
    scalar const r3 =
        (deltaP + rhoTilda*cTilda*deltaContrV)/(2.0*sqr(cTilda));

    scalar const l1rho = pTraits<scalar>::one;
    scalar const l2rho = pTraits<scalar>::one;
    scalar const l3rho = pTraits<scalar>::zero;
    scalar const l4rho = pTraits<scalar>::one;

    vector const l1U = UTilda - cTilda*normalVector;

    vector const l2U = UTilda;

    vector const l3U = deltaU - deltaContrV*normalVector;

    vector const l4U = UTilda + cTilda*normalVector;

    scalar const l1e = hTilda - cTilda*contrVTilda;
    scalar const l2e = 0.5*qTildaSquare;
    scalar const l3e = (UTilda & deltaU) - contrVTilda*deltaContrV;
    scalar const l4e = hTilda + cTilda*contrVTilda;

    scalar lambda1 = mag(contrVTilda - cTilda);
    scalar lambda2 = mag(contrVTilda);
    scalar lambda3 = mag(contrVTilda + cTilda);

    const scalar eps = 0.5*cTilda;

    if(lambda1 < eps || lambda2 < eps || lambda3 < eps)
    {
        scalar const lambdaMax = max(max(lambda1,lambda2),lambda3);
        lambda1 = lambda2 = lambda3 = lambdaMax;
    }

    scalar const diffF11 = lambda1*r1*l1rho;
    vector const diffF124 = lambda1*r1*l1U;
    scalar const diffF15 = lambda1*r1*l1e;

    scalar const diffF21 = lambda2*(r2*l2rho + rhoTilda*l3rho);
    vector const diffF224 = lambda2*(r2*l2U + rhoTilda*l3U);
    scalar const diffF25 = lambda2*(r2*l2e + rhoTilda*l3e);

    scalar const diffF31 = lambda3*r3*l4rho;
    vector const diffF324 = lambda3*r3*l4U;
    scalar const diffF35 = lambda3*r3*l4e;

    scalar const fluxLeft11 = rhoLeft*contrVLeft;
    vector const fluxLeft124 = ULeft*fluxLeft11 + normalVector*pLeft;
    scalar const fluxLeft15 = hLeft*fluxLeft11;

    scalar const fluxRight11 = rhoRight*contrVRight;
    vector const fluxRight124 = URight*fluxRight11 + normalVector*pRight;
    scalar const fluxRight15 = hRight*fluxRight11;

    scalar const flux1 =
        0.5*(fluxLeft11 + fluxRight11 - (diffF11 + diffF21 + diffF31));
    vector const flux24 =
        0.5*(fluxLeft124 + fluxRight124 - (diffF124 + diffF224 + diffF324));
    scalar const flux5 =
        0.5*(fluxLeft15 + fluxRight15 - (diffF15 + diffF25 + diffF35));

    rhoFlux  = flux1*magSf;
    rhoUFlux = flux24*magSf;
    rhoEFlux = flux5*magSf;
}

