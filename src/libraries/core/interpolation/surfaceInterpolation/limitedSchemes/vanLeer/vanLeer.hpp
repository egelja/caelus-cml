/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2014-2015 Applied CCM
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
    CML::vanLeerLimiter

Description
    Van Leer symmetric limiter function.

    Used in conjunction with the template class LimitedScheme.

SourceFiles
    vanLeer.cpp

References
    [1] "Towards the ulitmate conservative difference scheme II. Monotonicity 
         and Conservation Combined in Second Order Scheme", B. Van Leer,
         J. Comp. Phys. 14 (4), pp 361-370, 1974
        

\*---------------------------------------------------------------------------*/

#ifndef vanLeer_H
#define vanLeer_H

#include "vector.hpp"

namespace CML
{

template<class LimiterFunc>
class vanLeerLimiter
:
    public LimiterFunc
{

public:

    vanLeerLimiter(Istream&)
    {}

    scalar limiter
    (
        const scalar,
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

        return max(4*r/((r+1+VSMALL)*(r+1+VSMALL)),0);
    }
};

}

#endif

