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
    Oliver Borm
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/
#include "hllcFlux.hpp"

void CML::hllcFlux::evaluateFlux
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
    vector const normalVector = Sf/magSf;

    scalar const kappaLeft = (RLeft + CvLeft)/CvLeft;
    scalar const kappaRight = (RRight + CvRight)/CvRight;

    scalar const rhoLeft = pLeft / ( RLeft * TLeft);
    scalar const rhoRight = pRight / ( RLeft * TRight);

    vector const rhoULeft = rhoLeft*ULeft;
    vector const rhoURight = rhoRight*URight;

    scalar const rhoELeft = rhoLeft*(CvLeft*TLeft+0.5*magSqr(ULeft));
    scalar const rhoERight = rhoRight*(CvRight*TRight+0.5*magSqr(URight));

    scalar const HLeft = (rhoELeft + pLeft)/rhoLeft;
    scalar const HRight = (rhoERight + pRight)/rhoRight;

    scalar const qLeft = (ULeft & normalVector);
    scalar const qRight = (URight & normalVector);

    scalar const aLeft =
        sqrt(max(0.0,kappaLeft * pLeft/rhoLeft));

    scalar const aRight =
        sqrt(max(0.0,kappaRight * pRight/rhoRight));

    scalar const rhoLeftSqrt = sqrt(max(0.0,rhoLeft));
    scalar const rhoRightSqrt = sqrt(max(0.0,rhoRight));

    scalar const wLeft = rhoLeftSqrt
        /stabilise((rhoLeftSqrt + rhoRightSqrt),VSMALL);

    scalar const wRight = 1 - wLeft;

    vector const UTilde = wLeft*ULeft + wRight*URight;

    scalar const contrUTilde = (UTilde & normalVector);

    scalar const HTilde = wLeft*HLeft + wRight*HRight;

    scalar const kappaTilde = wLeft*kappaLeft + wRight*kappaRight;

    scalar const aTilde =
        sqrt(max(0.0,(kappaTilde-1.0)*(HTilde-0.5*sqr(contrUTilde))));

    scalar const SLeft  = min(qLeft-aLeft, contrUTilde-aTilde);
    scalar const SRight = max(contrUTilde+aTilde, qRight+aRight);

    scalar const SStar = (rhoRight*qRight*(SRight-qRight)
      - rhoLeft*qLeft*(SLeft - qLeft) + pLeft - pRight )/
        stabilise((rhoRight*(SRight-qRight)-rhoLeft*(SLeft-qLeft)),VSMALL);

    scalar const pStarRight =
        rhoRight*(qRight - SRight)*(qRight - SStar) + pRight;

    scalar const pStarLeft  =
        rhoLeft*(qLeft -  SLeft)*(qLeft - SStar) + pLeft;

    if (mag(pStarRight-pStarLeft) > 1e-6)
    {
        Info << "mag(pStarRight-pStarLeft) > VSMALL " << endl;
    }

    scalar const pStar = pStarRight;

    scalar convectionSpeed = 0.0;
    scalar rhoState = 0.0;
    vector rhoUState = vector::zero;
    scalar rhoEState = 0.0;
    scalar pState = 0.0;

    if (pos(SLeft))
    {
        convectionSpeed = qLeft;
        rhoState  = rhoLeft;
        rhoUState = rhoULeft;
        rhoEState = rhoELeft;
        pState = pLeft;
    }
    else if (pos(SStar))
    {
        scalar omegaLeft = scalar(1.0)/stabilise((SLeft - SStar),VSMALL);

        convectionSpeed = SStar;
        rhoState  = omegaLeft*(SLeft - qLeft)*rhoLeft;
        rhoUState = omegaLeft*((SLeft - qLeft)*rhoULeft
        + (pStar - pLeft)*normalVector);
        rhoEState = omegaLeft*((SLeft - qLeft)*rhoELeft
        - pLeft*qLeft + pStar*SStar);
        pState = pStar;
    }
    else if (pos(SRight))
    {
        scalar omegaRight = scalar(1.0)/stabilise((SRight - SStar),VSMALL);

        convectionSpeed = SStar;
        rhoState  = omegaRight*(SRight - qRight)*rhoRight;
        rhoUState = omegaRight*((SRight - qRight)*rhoURight
        + (pStar - pRight)*normalVector);
        rhoEState = omegaRight*((SRight - qRight)*rhoERight
        - pRight*qRight + pStar*SStar);
        pState = pStar;
    }
    else if (neg(SRight))
    {
        convectionSpeed = qRight;
        rhoState  = rhoRight;
        rhoUState = rhoURight;
        rhoEState = rhoERight;
        pState = pRight;
    }
    else
    {
        Info << "Error in HLLC Riemann solver" << endl;
    }

    rhoFlux  = (convectionSpeed*rhoState)*magSf;
    rhoUFlux = (convectionSpeed*rhoUState+pState*normalVector)*magSf;
    rhoEFlux = (convectionSpeed*(rhoEState+pState))*magSf;
}

