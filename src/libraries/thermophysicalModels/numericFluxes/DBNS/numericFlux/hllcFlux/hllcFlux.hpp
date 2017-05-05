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
    HLLC approximate Riemann solver

Author
    Oliver Borm
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/
#ifndef hllcFlux_HPP
#define hllcFlux_HPP

#include "fvCFD.hpp"

namespace CML
{

class hllcFlux
{

public:

    hllcFlux() {}

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

