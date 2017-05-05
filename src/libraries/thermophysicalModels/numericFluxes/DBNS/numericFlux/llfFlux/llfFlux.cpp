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
#include "llfFlux.hpp"

void CML::llfFlux::evaluateFlux
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

    scalar const deltaRho = rhoRight - rhoLeft;
    vector const deltaRhoU = rhoRight*URight - rhoLeft*ULeft;
    scalar const deltaRhoE = rhoRight*eRight - rhoLeft*eLeft;

    scalar const cLeft = 
        sqrt(max((kappaLeft - 1)*(hLeft - 0.5*magSqr(ULeft)),SMALL));
    scalar const cRight = 
      sqrt(max((kappaRight - 1)*(hRight - 0.5*magSqr(URight)),SMALL));

    scalar const lambda1Left = mag(contrVLeft - cLeft);
    scalar const lambda2Left = mag(contrVLeft);
    scalar const lambda3Left = mag(contrVLeft + cLeft);

    scalar const lambdaLeftMax = 
        max(max(lambda1Left,lambda2Left),lambda3Left);

    scalar const lambda1Right = mag(contrVRight - cRight);
    scalar const lambda2Right = mag(contrVRight);
    scalar const lambda3Right = mag(contrVRight + cRight);

    scalar const lambdaRightMax = 
        max(max(lambda1Right,lambda2Right),lambda3Right);

    scalar const lambdaMax = max(lambdaLeftMax,lambdaRightMax);

    scalar const dissRho  = lambdaMax*deltaRho;
    vector const dissRhoU = lambdaMax*deltaRhoU;
    scalar const dissRhoE = lambdaMax*deltaRhoE;

    scalar const fluxLeft11 = rhoLeft*contrVLeft;
    vector const fluxLeft124 = ULeft*fluxLeft11 + normalVector*pLeft;
    scalar const fluxLeft15 = hLeft*fluxLeft11;

    scalar const fluxRight11 = rhoRight*contrVRight;
    vector const fluxRight124 = URight*fluxRight11 + normalVector*pRight;
    scalar const fluxRight15 = hRight*fluxRight11;

    scalar const flux1 =
        0.5*(fluxLeft11 + fluxRight11 - dissRho);
    vector const flux24 =
        0.5*(fluxLeft124 + fluxRight124 - dissRhoU);
    scalar const flux5 =
        0.5*(fluxLeft15 + fluxRight15 - dissRhoE);

    rhoFlux  = flux1*magSf;
    rhoUFlux = flux24*magSf;
    rhoEFlux = flux5*magSf;
}


