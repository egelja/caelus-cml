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

Description
    Riemann solver based on local Lax-Friedrichs central scheme

Reference
    [1] "Computational Gasdynamics", C. B. Laney, Cambridge University
         Press, 1998
Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/
#ifndef llfFlux_HPP
#define llfFlux_HPP

#include "fvCFD.hpp"

namespace CML
{

class llfFlux
{

public:

    llfFlux() {}

    void evaluateFlux
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
    ) const;
};

}

#endif

