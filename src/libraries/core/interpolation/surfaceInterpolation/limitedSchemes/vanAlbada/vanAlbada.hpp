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
    CML::vanAlbadaLimiter

Description
    Symmetric vanAlbada limiter function.

    Used in conjunction with the template class LimitedScheme.

SourceFiles
    vanAlbada.cpp

References
    [1] "A comparative study of computational methods in cosmic gas dynamics",
         G.D., Van Albada, Astronomy and Astrophysics 108, pp. 76–84

\*---------------------------------------------------------------------------*/

#ifndef vanAlbada_H
#define vanAlbada_H

#include "vector.hpp"

namespace CML
{

template<class LimiterFunc>
class vanAlbadaLimiter
:
    public LimiterFunc
{

public:

    vanAlbadaLimiter(Istream&)
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

        return max(2*r/(r*r+1),0);
    }
};

} // End namespace CML

#endif

