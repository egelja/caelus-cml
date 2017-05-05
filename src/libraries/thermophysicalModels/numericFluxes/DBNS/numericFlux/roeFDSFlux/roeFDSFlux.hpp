/*---------------------------------------------------------------------------*\
Copyright (C) 2015 - 2016 Applied CCM
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
    Riemann solver for the Flux Difference Splitting algorithm. Implementation
    is based on Roe-Pike algorithm.

Reference
    [1] "Computational Fluid Dynamics: Principles and Implementation",
        J. Blazek, Elsevier, 2001

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/
#ifndef roeFlux_HPP
#define roeFlux_HPP

#include "fvCFD.hpp"

namespace CML
{

class roeFlux
{

public:

    roeFlux() {}

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

