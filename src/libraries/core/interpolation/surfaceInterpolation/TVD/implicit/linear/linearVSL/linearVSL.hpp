/*---------------------------------------------------------------------------*\
Copyright (C) 2011 - 2015 OpenFOAM Foundation
Copyright (C) 2016 - 2017 Applied CCM
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
    CML::VSL

Description

    Differencing scheme based on parameterised limiter function. 
    Scheme selects a parameter value between 0 and 2 where
    0 value represents unlimited linear scheme and values between 1 and 2
    representent a TVD satisfying scheme. A value between 0 and 2 must be
    specified in the dictionary entry. Thiis scheme is formerly known as
    limited linear and the name was changed to Variable Slope Limiter (VSL)
    to better represent the limiter function that is used.

SourceFiles

    VSL.cpp

References

    [1] "Analysis of Slope Limiter on Irregular Grids", M. Berger, 
         M.J. Aftosmis and S.M. Murman, 43rd AIAA Aerospace Sciences Meeting, 
         Jan. 10-13, 2005, Reno, NV

Author

    OpenFoam Foundation
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef VSL_HPP_
#define VSL_HPP_

#include "vector.hpp"

namespace CML
{

template<class LimiterFunc>
class VSL : public LimiterFunc
{
    scalar k_;
    scalar order_;
    scalar twoByk_;

public:

    VSL(Istream& is) : k_(readScalar(is))
    {
        if (k_ < 0 || k_ > 2)
        {
            FatalIOErrorInFunction(is)
                << "coefficient = " << k_
                << " should be >= 0 and <= 2"
                << exit(FatalIOError);
        }

        if (is.eof())
        {
            order_ = scalar(1);
        }
        else
        {
            order_ = readScalar(is);
            if (order_ < 0 || order_ > 2)
            {
                FatalIOErrorInFunction(is)
                        << "coefficient = " << order_
                        << " should be >= 0 and <= 2"
                        << exit(FatalIOError);
            }
        }

        // Avoid the /0 when k_ = 0
        twoByk_ = 2.0/max(k_, SMALL);
    }

    scalar limiter
    (
        scalar const cdWeight,
        scalar const faceFlux,
        typename LimiterFunc::phiType const& phiP,
        typename LimiterFunc::phiType const& phiN,
        typename LimiterFunc::gradPhiType const& gradcP,
        typename LimiterFunc::gradPhiType const& gradcN,
        vector const& d
    ) const
    {
        scalar r = LimiterFunc::r
        (
            faceFlux, phiP, phiN, gradcP, gradcN, d
        );

        return max(min(twoByk_*r, 1), 0);
    }
};


}

#endif

