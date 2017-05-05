/*---------------------------------------------------------------------------*\
Copyright (C) 2015 Applied CCM
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
    VLimiter

Description
     Venkatakrishnan multidimensional limiter

SourceFiles


References

    [1] “On the Accuracy of Limiters and Convergence to Steady-State Solutions,”
         Venkatakrishnan, V., AIAA paper 93-0880, Jan. 1993.

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef VLimiter_HPP_
#define VLimiter_HPP_

namespace CML
{

class VLimiter
{
    scalar k_;

public:

    VLimiter() :
        k_(5)
    {}

    inline scalar limiter
    (
        scalar const cellVolume,
        scalar const deltaOneMax,
        scalar const deltaOneMin,
        scalar deltaTwo
    )
    {
        scalar const epsilonSquare = pow3(k_)*cellVolume;
        if (deltaTwo > 0)
        {
            return max(min(((sqr(deltaOneMax)+epsilonSquare)*deltaTwo
                             +2.0*sqr(deltaTwo)*deltaOneMax)
			     /stabilise(deltaTwo*(sqr(deltaOneMax)
                             +2.0*sqr(deltaTwo)+deltaOneMax*deltaTwo
                             +epsilonSquare),VSMALL),1),0);
        }
        else if (deltaTwo < 0)
        {
            return max(min(((sqr(deltaOneMin)+epsilonSquare)*deltaTwo
                             +2.0*sqr(deltaTwo)*deltaOneMin)
			     /stabilise(deltaTwo*(sqr(deltaOneMin)
                             +2.0*sqr(deltaTwo)+deltaOneMin*deltaTwo
                             +epsilonSquare),VSMALL),1),0);
        }
        else
        {
            return 1.0;
        }
    }
	
    inline scalar limiter
    (
        scalar const cellVolume,
        scalar const deltaOneMax,
        scalar const deltaOneMin,
        vector const gradPhiP,
        vector const gradPhiN,
        vector const& d
    )
    {
        return limiter(cellVolume,deltaOneMax,deltaOneMin,d & gradPhiP);
    }
	
    inline vector limiter
    (
        const scalar cellVolume,
        const vector deltaOneMax,
        const vector deltaOneMin,
        const tensor gradPhiP,
        const tensor gradPhiN,
        const vector& d
    )
    {
        vector const deltaTwo = d & gradPhiP;
        return vector
        (
            limiter(cellVolume,deltaOneMax[0],deltaOneMin[0],deltaTwo[0]),
            limiter(cellVolume,deltaOneMax[1],deltaOneMin[1],deltaTwo[1]),
            limiter(cellVolume,deltaOneMax[2],deltaOneMin[2],deltaTwo[2])
        );
    }
};

}
#endif

