/*---------------------------------------------------------------------------*\
Copyright (C) 2017-2018 Applied CCM
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
    CML::PDL

Description

    Linear scheme based on parametric differentiable limiter.

SourceFiles

    linearPDL.cpp

References

    [1] "Analysis of Slope Limiter on Irregular Grids", M. Berger, 
         M.J. Aftosmis and S.M. Murman, 43rd AIAA Aerospace Sciences Meeting, 
         Jan. 10-13, 2005, Reno, NV

Author

    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef linearPDL_HPP
#define linearPDL_HPP

#include "vector.hpp"

namespace CML
{

template<class LimiterFunc>
class PDL : public LimiterFunc
{

    scalar k_;

public:

    PDL(Istream& is)
    {
        if (is.eof())
        {
            k_ = scalar(1);
        }
        else
        {
            k_ = readScalar(is);
            if (k_ < 0 || k_ > 1)
            {
                FatalIOErrorInFunction(is)
                        << "coefficient = " << k_
                        << " should be >= 0 and <= 1"
                        << exit(FatalIOError);
            }
        }
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

        return (r >= k_) ? scalar(1.0) : max
        (
            0,
            CML::sin(scalar(1.0)/(k_+VSMALL)*constant::mathematical::pi/2.0*r)
        );
    }
};

}

#endif

