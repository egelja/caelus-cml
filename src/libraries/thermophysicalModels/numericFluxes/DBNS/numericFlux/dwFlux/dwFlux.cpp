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

\*---------------------------------------------------------------------------*/

#include "dwFlux.hpp"

void CML::dwFlux::evaluateFlux
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
    scalar const rhoLeft = pLeft/(RLeft*TLeft);
    scalar const rhoRight = pRight/(RRight*TRight);

    scalar const eLeft = CvLeft*TLeft+0.5*magSqr(ULeft);
    scalar const eRight = CvRight*TRight+0.5*magSqr(URight);

    scalar const kappaLeft = (CvLeft+RLeft)/CvLeft;
    scalar const kappaRight = (CvRight+RRight)/CvRight;

    vector const normalVector = Sf/magSf;

    scalar const contrVLeft  = (ULeft & normalVector);
    scalar const contrVRight = (URight & normalVector);

    scalar const hLeft = eLeft + pLeft/rhoLeft;
    scalar const hRight = eRight + pRight/rhoRight;

    scalar const rhoLeftSqrt = sqrt(max(rhoLeft,0.0));
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

    scalar const deltaRho = rhoRight - rhoLeft;

    scalar const lambda1 = mag(contrVTilda - cTilda);
    scalar const lambda3 = mag(contrVTilda + cTilda);

    scalar const fluxLeft11 = rhoLeft*contrVLeft;
    vector const fluxLeft124 = ULeft*fluxLeft11 + normalVector*pLeft;
    scalar const fluxLeft15 = hLeft*fluxLeft11;

    scalar const fluxRight11 = rhoRight*contrVRight;
    vector const fluxRight124 = URight*fluxRight11 + normalVector*pRight;
    scalar const fluxRight15 = hRight*fluxRight11;

    scalar flux1 =
        0.5*(fluxLeft11 + fluxRight11);
    vector flux24 =
        0.5*(fluxLeft124 + fluxRight124);
    scalar flux5 =
        0.5*(fluxLeft15 + fluxRight15);

    vector const deltaRhoU = rhoRight*URight - rhoLeft*ULeft;
    scalar const deltaRhoE = rhoRight*eRight - rhoLeft*eLeft;

    scalar const dF1  = fluxRight11  - fluxLeft11;
    vector const dF24 = fluxRight124 - fluxLeft124;
    scalar const dF5  = fluxRight15  - fluxLeft15;

    scalar lambdaRho  = deltaRho*dF1;
    scalar lambdaRhoU = (deltaRhoU & dF24);
    scalar lambdaRhoE = deltaRhoE*dF5;
    scalar const diffRho  = deltaRho*deltaRho;
    scalar const diffRhoU = (deltaRhoU & deltaRhoU);
    scalar const diffRhoE = deltaRhoE*deltaRhoE;
    lambdaRho  /= (diffRho + SMALL);
    lambdaRhoU /= (diffRhoU + SMALL);
    lambdaRhoE /= (diffRhoE + SMALL); 

    scalar lambdaMax = max(max(mag(lambdaRho),mag(lambdaRhoU)),mag(lambdaRhoE));

    if (lambdaMax <= mag(contrVTilda - cTilda))
    {
        lambdaMax = lambda1;
    }

    if (lambdaMax >= mag(contrVTilda + cTilda))
    {    
        lambdaMax = lambda3;
    }

    flux1  -= 0.5*mag(lambdaMax)*deltaRho;
    flux24 -= 0.5*mag(lambdaMax)*deltaRhoU;
    flux5  -= 0.5*mag(lambdaMax)*deltaRhoE;

    rhoFlux  = flux1*magSf;
    rhoUFlux = flux24*magSf;
    rhoEFlux = flux5*magSf;
}
