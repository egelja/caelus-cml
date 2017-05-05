/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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

Class
    CML::KorenLimiter

Description
    Third order limiter function. Formally third order accurate for
    sufficiently smooth data. Suitable for high order methods.

    Used in conjunction with the template class LimitedScheme.

SourceFiles
    Koren.cpp

Author
    Aleksandar Jemcov

Refrences

    [1] A Robust upwind discretization method for advection, diffusion
        and source terms, B. Koren, Numerical Methods for Advection-Diffusion 
        Problems, C. B. Vreugdenhil and B. Koren, (Eds.), pp 117-138, 
        Braunschweig/Wiesbaden, 1993

\*---------------------------------------------------------------------------*/

#ifndef KOREN_HPP
#define KOREN_HPP

#include "vector.hpp"

namespace CML
{

template<class LimiterFunc>
class KorenLimiter
:
    public LimiterFunc
{

public:

    KorenLimiter(Istream&)
    {}

    scalar limiter
    (
        const scalar cdWeight,
        const scalar faceFlux,
        const typename LimiterFunc::phiType& phiP,
        const typename LimiterFunc::phiType& phiN,
        const typename LimiterFunc::gradPhiType& gradcP,
        const typename LimiterFunc::gradPhiType& gradcN,
        const vector& d
    ) const
    {
        scalar r = LimiterFunc::r
        (
            faceFlux, phiP, phiN, gradcP, gradcN, d
        );

        return max
        (
            scalar(0),
            min(min(scalar(2)*r,(1+2*r)/scalar(3)),scalar(2))
        );
    }
};

}

#endif
