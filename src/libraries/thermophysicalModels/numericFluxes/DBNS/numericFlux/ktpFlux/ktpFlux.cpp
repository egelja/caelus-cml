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
#include "ktpFlux.hpp"

void CML::ktpFlux::evaluateFlux
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

    scalar const lambda1Left = contrVLeft - cLeft;
    scalar const lambda3Left = contrVLeft + cLeft;

    scalar const lambda1Right = contrVRight - cRight;
    scalar const lambda3Right = contrVRight + cRight;

    scalar const lambdaMax = max(max(lambda3Left,lambda3Right),0);
    scalar const lambdaMin = min(min(lambda1Left,lambda1Right),0);

    scalar const dissRho  = deltaRho*lambdaMax*lambdaMin/(lambdaMax-lambdaMin);
    vector const dissRhoU = deltaRhoU*lambdaMax*lambdaMin/(lambdaMax-lambdaMin);
    scalar const dissRhoE = deltaRhoE*lambdaMax*lambdaMin/(lambdaMax-lambdaMin);

    scalar const fluxLeft11 = rhoLeft*contrVLeft;
    vector const fluxLeft124 = ULeft*fluxLeft11 + normalVector*pLeft;
    scalar const fluxLeft15 = hLeft*fluxLeft11;

    scalar const fluxRight11 = rhoRight*contrVRight;
    vector const fluxRight124 = URight*fluxRight11 + normalVector*pRight;
    scalar const fluxRight15 = hRight*fluxRight11;

    scalar const flux1 =
        (lambdaMax*fluxLeft11 - lambdaMin*fluxRight11)/(lambdaMax-lambdaMin) 
      + dissRho;
    vector const flux24 =
        (lambdaMax*fluxLeft124 - lambdaMin*fluxRight124)/(lambdaMax-lambdaMin) 
      + dissRhoU;
    scalar const flux5 =
        (lambdaMax*fluxLeft15 - lambdaMin*fluxRight15)/(lambdaMax-lambdaMin) 
      + dissRhoE;

    rhoFlux  = flux1*magSf;
    rhoUFlux = flux24*magSf;
    rhoEFlux = flux5*magSf;
}


