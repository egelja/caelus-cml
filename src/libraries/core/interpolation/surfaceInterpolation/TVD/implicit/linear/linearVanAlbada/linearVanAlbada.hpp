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
    CML::VanAlbada

Description

    Van Albada linear scheme.

SourceFiles
    linearVanAlbada.cpp

References

    [1] "A comparative study of computational methods in cosmic gas dynamics",
         G.D., Van Albada, Astronomy and Astrophysics 108, pp. 76–84
        
    [2] "Analysis of Slope Limiter on Irregular Grids", M. Berger, 
         M.J. Aftosmis and S.M. Murman, 43rd AIAA Aerospace Sciences Meeting, 
         Jan. 10-13, 2005, Reno, NV

Author

    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef linearVanAlbada_HPP
#define linearVanAlbada_HPP

#include "vector.hpp"

namespace CML
{

template<class LimiterFunc>
class VanAlbada : public LimiterFunc
{

    scalar order_;

public:

    VanAlbada(Istream& is)
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

        return order_*max(0,2/(r+stabilise(1/r,VSMALL)));
    }
};

}

#endif

