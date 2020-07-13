/*---------------------------------------------------------------------------*\
Copyright (C) 2016-2018 Applied CCM
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
    CML::VanLeer

Description

    Van Leer symmetric limiter function.

SourceFiles
    linearVanLeer.cpp

References

    [1] "Towards the ultimate conservative difference scheme II. Monotonicity 
         and Conservation Combined in Second Order Scheme", B. Van Leer,
         J. Comp. Phys. 14 (4), pp 361-370, 1974
        
    [2] "Analysis of Slope Limiter on Irregular Grids", M. Berger, 
         M.J. Aftosmis and S.M. Murman, 43rd AIAA Aerospace Sciences Meeting, 
         Jan. 10-13, 2005, Reno, NV

Author

    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef linearVanLeer_HPP
#define linearVanLeer_HPP

#include "vector.hpp"

namespace CML
{

template<class LimiterFunc>
class VanLeer : public LimiterFunc
{

    scalar order_;

public:

    VanLeer(Istream& is)
    {
        if (is.eof())
        {
            order_ = scalar(1);
        }
        else
        {
            order_ = readScalar(is);
            if (order_ < 0 || order_ > 1)
            {
                FatalIOErrorInFunction(is)
                        << "coefficient = " << order_
                        << " should be >= 0 and <= 1"
                        << exit(FatalIOError);
            }
        }
    }

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

        return order_*max(0,4*r/((r+1+VSMALL)*(r+1+VSMALL)));
    }
};

}

#endif

