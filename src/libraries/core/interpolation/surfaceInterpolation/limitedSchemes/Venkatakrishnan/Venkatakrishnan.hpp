/*---------------------------------------------------------------------------*\
Copyright (C) 2105 Applied CCM
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
    CML::VenkatakrishnanLimiter

Description
    Symmetric Venkatakrishnan limiter.

    Used in conjunction with the template class LimitedScheme.

SourceFiles
    Venkatakrishnan.cpp

References
    [1] “On the Accuracy of Limiters and Convergence to Steady-State Solutions,”
         Venkatakrishnan, V., AIAA paper 93-0880, Jan. 1993.

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef Venkatakrishnan_H
#define Venkatakrishnan_H

#include "vector.hpp"

namespace CML
{

template<class LimiterFunc>
class VenkatakrishnanLimiter
:
    public LimiterFunc
{

public:

    VenkatakrishnanLimiter(Istream&)
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

        return max(r*(r+2)/(r*(r+1)+2+SMALL), 0);
    }
};

}

#endif

