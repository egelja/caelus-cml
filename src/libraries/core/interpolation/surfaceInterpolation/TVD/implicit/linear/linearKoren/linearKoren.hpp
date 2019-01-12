/*---------------------------------------------------------------------------*\
Copyright (C) 2014-2018 Applied CCM
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
    CML::Koren

Description

    Third order limiter function. Formally third order accurate for
    sufficiently smooth data. Suitable for high order methods.

    Used in conjunction with the template class LimitedScheme.

SourceFiles
    LinearKoren.cpp

Author
    Aleksandar Jemcov

References

    [1] "A Robust upwind discretization method for advection, diffusion
        and source terms" ,Numerical Methods for Advection-Diffusion 
        Problems, C. B. Vreugdenhil and B. Koren, (Eds.), pp 117-138, 
        Braunschweig/Wiesbaden, 1993
    
    [2] "Upwind Discretization of the Steady Navier-Stokes Equations",
         B. Koren, International Journal for Numerical Methods in Fluids,
         Vol 11, pp 99-117, 1990

    [3] "Analysis of Slope Limiter on Irregular Grids", M. Berger, 
         M.J. Aftosmis and S.M. Murman, 43rd AIAA Aerospace Sciences Meeting, 
         Jan. 10-13, 2005, Reno, NV

\*---------------------------------------------------------------------------*/

#ifndef LINEARKOREN_HPP
#define LINEARKOREN_HPP

#include "vector.hpp"

namespace CML
{

template<class LimiterFunc>
class Koren : public LimiterFunc
{

    scalar order_;
  
public:

    Koren(Istream& is)
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

        return order_*max
        (
            min(min(4*r/(r+1+VSMALL),2*(2+r)/(3*(r+1+VSMALL))),4/(r+1+VSMALL)),
            0   
        );
    }
};

}

#endif
